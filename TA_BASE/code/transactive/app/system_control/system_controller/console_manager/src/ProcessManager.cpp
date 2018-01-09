/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_controller/console_manager/src/ProcessManager.cpp $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  */
// ProcessManager.cpp
//
// Demonstration implementation of the ProcessManagerDmIf interface
//
// Author:      Bruce Fountain 08-Feb-2001
// Modified by: $Author: haijun.li $
// Modified on: $DateTime: 2012/02/06 16:15:14 $
// Version:     $Revision: #2 $
//
#include "StdAfx.h"
#include "ProcessManager.h"
#include "SystemControllerAdmin.h"
#include "core/corba/src/CorbaUtil.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include "core/process_management/IDL/src/IManagedAgentCorbaDef.h"
#include "core/process_management/IDL/src/IManagedGUICorbaDef.h"

using namespace TA_Base_Core;

struct ProcessIdNotFoundException : std::exception
{
    ProcessIdNotFoundException(unsigned long id)
    {
        m_what = str(boost::format("No matching id %d") % id);
    }

    virtual const char* what() const
    {
        return m_what.c_str();
    }

    std::string m_what;
};

ProcessManager::ProcessManager()
{
    // Build up the set of runtime parameters that will be passed to registering
    // processes

    m_defaultParams = RunParams::getInstance().getAll();
    std::cout << "\nRunParams:\n";

    for (RunParams::ParamNameValue& param : m_defaultParams)
    {
        RunParams::getInstance().registerRunParamUser(this, param.name.c_str());

        if (param.value.size())
        {
            std::cout << "\t" << boost::format("%s=%s") % param.name % param.value << std::endl;
        }
        else
        {
            std::cout << "\t" << boost::format("%s") % param.name << std::endl;
        }
    }

    std::cout << "\n";

    // Give this object a user-friendly key, so that the managed process can
    // access it using a URL-style IOR
    // (eg "corbaloc::1.2@localhost:6666/ProcessManager")
    // See section 6.4.1 "corbaloc: URLs" in the ORBacus manual
    CorbaUtil::getInstance().setServantKey(this, "ProcessManager");

    activateServantWithName("ProcessManager");
}

// Methods to meet the ProcessManagerCorbaDef interface requirements.

TA_Base_Core::RunParamSeq* ProcessManager::getParams(TA_Base_Core::ProcessId id, const char* hostname)
{
    TA_Base_Core::RunParamSeq* result = new TA_Base_Core::RunParamSeq;
    ThreadGuard guard(m_mapLock);
    std::stringstream log;
    log << boost::format("GetParams (id=%d, hostname=%s)") % id % hostname << std::endl;
    std::cout << log.rdbuf() << std::endl;

    tryCatchAny([&]
    {
        Client& client = getClient(id);
        RunParams::ParamVector params = RunParams::getInstance().getAll();

        for (unsigned int i = 0; i < params.size(); i++)
        {
            // If this is the debug file path parameter then we want to change this to a DebugFile parameter
            if (0 == params[i].name.compare("DebugFilePath"))
            {
                if (!RunParams::getInstance().isSet(RPARAM_DEBUGFILE))
                {
                    boost::filesystem::path logDir = boost::filesystem::system_complete(params[i].value) / "log";
                    boost::filesystem::path debugFile = str(boost::format("%s_%d.log") % client.entity % id);
                    result->length(i + 1);
                    (*result)[i].name = static_cast<const char*>(RPARAM_DEBUGFILE);
                    (*result)[i].value = (logDir / debugFile).string().c_str();
                }
            }
            else
            {
                result->length(i + 1);
                (*result)[i].name = params[i].name.c_str();
                (*result)[i].value = params[i].value.c_str();
            }

            log << boost::format("argv[%d]: %s=%s\n") % i % (*result)[i].name.in() % (*result)[i].value.in();
        }
    });

    LOG_DEBUG("%s", log.str().c_str());
    return result;
}

void ProcessManager::terminating(TA_Base_Core::ProcessId p_processID, TA_Base_Core::EManagedProcessTerminateCode p_code)
{
    tryCatchAny([&]
    {
        ThreadGuard guard(m_mapLock);
        std::cout << boost::format("Terminating %s (reason=%s)") % getDescription(p_processID) % toString(p_code) << std::endl;
        Client& client = getClient(p_processID);
        m_clients.erase(p_processID);
        SystemControllerAdmin::getInstance().terminating(p_processID, p_code);
    });
}

void ProcessManager::registerManagedProcess(TA_Base_Core::IManagedProcessCorbaDef_ptr p_managedProcess,
                                            TA_Base_Core::ProcessId p_processID,
                                            CORBA::ULong applicationType,
                                            const char* entity)
{
    ThreadGuard guard(m_mapLock);

    if (CORBA::is_nil(p_managedProcess))
    {
        std::cout << "Received registration from a nil managed process" << std::endl;
        return;
    }

    m_clients[p_processID] = { entity, "", applicationType,  TA_Base_Core::IManagedProcessCorbaDef::_duplicate(p_managedProcess)};
    std::cout << boost::format("Register %s") % getDescription(p_processID) << std::endl;
    SystemControllerAdmin::getInstance().registerManagedProcess(p_managedProcess, p_processID, applicationType, entity);
}

void ProcessManager::registerManagedApplication(TA_Base_Core::IManagedApplicationCorbaDef_ptr p_managedApp, TA_Base_Core::ProcessId p_processId)
{
    tryCatchAny([&]
    {
        ThreadGuard guard(m_mapLock);

        if (CORBA::is_nil(p_managedApp))
        {
            std::cout << "Received registration from a nil managed application, pid=" << p_processId << std::endl;
            return;
        }

        m_apps[p_processId] = TA_Base_Core::IManagedApplicationCorbaDef::_duplicate(p_managedApp);
        Client& client = getClient(p_processId);
        TA_Base_Core::IManagedAgentCorbaDef_var agent = TA_Base_Core::IManagedAgentCorbaDef::_narrow(p_managedApp);

        if (!CORBA::is_nil(agent))
        {
            client.type = "Agent";
            SystemControllerAdmin::getInstance().registerManagedApplication(p_managedApp, p_processId);
        }
        else
        {
            TA_Base_Core::IManagedGUICorbaDef_var managedProcess = TA_Base_Core::IManagedGUICorbaDef::_narrow(p_managedApp);

            if (!CORBA::is_nil(managedProcess))
            {
                client.type = "GUI";
            }
        }

        std::cout << boost::format("Register Managed App %s") % getDescription(p_processId) << std::endl;
    });
}

void ProcessManager::poll()
{
}

void ProcessManager::statusChanged(TA_Base_Core::EProcessStatus status, TA_Base_Core::ProcessId p_processId)
{
    static const char* str[] =
    {
        "Unstarted",
        "Startup",
        "Going to Control",
        "Going to Monitor",
        "Running Control",
        "Running Monitor",
        "Terminating",
        "Not Running",
        "Stopped",
        "RunningStandby",
        "GoingToStandby"
    };

    tryCatchAny([&]
    {
        getClient(p_processId);
        std::cout << boost::format("Status update received for %s, Changed to state '%s'") % getDescription(p_processId) % str[status] << std::endl;
        SystemControllerAdmin::getInstance().statusChanged(status, p_processId);
    });
}

void ProcessManager::onRunParamChange(const std::string& paramName, const std::string& paramValue)
{
    if (m_clients.size())
    {
        // Reset default parameters for future clients.
        m_defaultParams = RunParams::getInstance().getAll();

        // Notify existing clients.
        for (ClientMap::const_iterator iter = m_clients.begin(); iter != m_clients.end(); ++iter)
        {
            setClientRunParam(iter->first, paramName, paramValue);
        }

        std::cout << "   " << paramName << "=" << paramValue << std::endl;
    }
}

std::string ProcessManager::getRunningApps()
{
    ThreadGuard guard(m_mapLock);
    std::stringstream listOfApps;
    listOfApps << "----------------------------------------------------\n";
    listOfApps << "Id\tAppType\tType\tEntity\n";
    listOfApps << "----------------------------------------------------\n";

    for (ClientMap::value_type& v : m_clients)
    {
        listOfApps << boost::format("%lu\t%d\t%s\t%s\n") % v.first % v.second.applicationType % v.second.type % v.second.entity;
    }

    listOfApps << "----------------------------------------------------\n";
    return listOfApps.str();
}

void ProcessManager::terminateApp(unsigned long id)
{
    tryCatchAny([&]
    {
        ThreadGuard guard(m_mapLock);
        std::cout << "Attempting to terminate " << id;
        Client& client = getClient(id);
        std::cout << " " << getDescription(id);

        if (!CORBA::is_nil(client.managedProcess))
        {
            TA_Base_Core::IManagedProcessCorbaDef_ptr app = TA_Base_Core::IManagedProcessCorbaDef::_narrow(client.managedProcess);

            if (CORBA::is_nil(app))
            {
                std::cout << " - Failed. Could not narrow\n";
                return;
            }

            app->terminate();
            std::cout << " - Success\n";
            SystemControllerAdmin::getInstance().terminateApp(id);
        }
    }, " -Failed. ");
}

void ProcessManager::changeMode(bool isControl, unsigned long id)
{
    tryCatchAny([&]
    {
        ThreadGuard guard(m_mapLock);
        std::cout << isControl ? "Attempting to change to control mode " : "Attempting to change to monitor mode ";

        TA_Base_Core::IManagedApplicationCorbaDef_var application = getApplication(id);
        std::cout << " " << getDescription(id);

        if (!CORBA::is_nil(application))
        {
            TA_Base_Core::IManagedAgentCorbaDef_var agent = TA_Base_Core::IManagedAgentCorbaDef::_narrow(application);

            if (CORBA::is_nil(agent))
            {
                std::cout << " - Failed. App is not an agent.\n";
                return;
            }

            if (isControl)
            {
                agent->setOperationMode(TA_Base_Core::Control);
            }
            else
            {
                agent->setOperationMode(TA_Base_Core::Monitor);
            }

            std::cout << " - Success\n";
            SystemControllerAdmin::getInstance().changeMode(isControl, id);
        }
    }, " -Failed. ");
}

void ProcessManager::changeGUI(TA_Base_Core::EFocusType focus, unsigned long id)
{
    tryCatchAny([&]
    {
        ThreadGuard guard(m_mapLock);
        std::cout << "Attempting to change to focus type ";
        TA_Base_Core::IManagedApplicationCorbaDef_var application = getApplication(id);
        std::cout << getDescription(id);

        if (!CORBA::is_nil(application))
        {
            TA_Base_Core::IManagedGUICorbaDef_ptr gui = TA_Base_Core::IManagedGUICorbaDef::_narrow(application);

            if (CORBA::is_nil(gui))
            {
                std::cout << " - Failed. App is not a GUI.\n";
                return;
            }

            gui->changeFocus(focus);
            std::cout << " - Success\n";
        }
    }, " -Failed. ");
}

void ProcessManager::serverChange(bool isUp, unsigned long id)
{
    tryCatchAny([&]
    {
        ThreadGuard guard(m_mapLock);
        std::cout << "Attempting to alert GUI of server change ";
        TA_Base_Core::IManagedApplicationCorbaDef_var application = getApplication(id);
        std::cout << getDescription(id);

        if (!CORBA::is_nil(application))
        {
            TA_Base_Core::IManagedGUICorbaDef_ptr gui = TA_Base_Core::IManagedGUICorbaDef::_narrow(application);

            if (CORBA::is_nil(gui))
            {
                std::cout << " - Failed. App is not a GUI.\n";
                return;
            }

            gui->serverStateChange(isUp);
            std::cout << " - Success\n";
        }
    }, " -Failed. ");
}

const char* ProcessManager::toString(TA_Base_Core::EManagedProcessTerminateCode p_code)
{
    static const char* s[] =
    {
        "Terminate requested",
        "Initialisation error",
        "Communication error",
        "User exit",
        "Dependency Failure",
        "Timeout Error",
        "Fatal Error",
        "Access Denied",
        "NoActive Session",
        "Rights Error",
        "Config Error",
        "NotKnown"
    };

    return s[p_code];
}

bool ProcessManager::setClientRunParam(int id, const std::string& paramName, const std::string& paramValue)
{
    tryCatchAny([&]
    {
        ThreadGuard guard(m_mapLock);
        std::cout << "Attempting to set run-param for specified process " << id;
        Client& client = getClient(id);
        std::cout << " " << getDescription(id);

        if (!CORBA::is_nil(client.managedProcess))
        {
            omniORB::setClientCallTimeout(client.managedProcess, 100);

            TA_Base_Core::IManagedProcessCorbaDef_var app = TA_Base_Core::IManagedProcessCorbaDef::_narrow(client.managedProcess);

            if (CORBA::is_nil(app))
            {
                std::cout << " - Failed. Could not narrow\n";
                return;
            }

            omniORB::setClientCallTimeout(app, 100);
            TA_Base_Core::RunParam param = { paramName.c_str(), paramValue.c_str() };
            app->setParam(param);
            std::cout << " - Param has been set\n";
            SystemControllerAdmin::getInstance().setClientRunParam(id, paramName, paramValue);
            return;
        }
    }, " -Failed. ");

    return true;
}

std::string ProcessManager::getDescription(unsigned long id)
{
    try
    {
        Client& client = getClient(id);
        return str(boost::format("(entity=%s, type=%d, pid=%d)") % client.entity.c_str() % client.applicationType % id);
    }
    catch (...)
    {
    }

    return str(boost::format("(pid=%d)") % id);
}

ProcessManager::Client& ProcessManager::getClient(unsigned long id)
{
    ClientMap::iterator it = m_clients.find(id);

    if (it == m_clients.end())
    {
        throw ProcessIdNotFoundException(id);
    }

    return it->second;
}

TA_Base_Core::IManagedApplicationCorbaDef_var ProcessManager::getApplication(unsigned id)
{
    AppMap::iterator it = m_apps.find(id);

    if (it == m_apps.end())
    {
        throw ProcessIdNotFoundException(id);
    }

    return it->second;
}

void ProcessManager::terminateAndWaitForAll()
{
    for (ClientMap::value_type& v : m_clients)
    {
        terminateApp(v.first);
    }

    while (SystemControllerAdmin::getInstance().isAnyRunning())
    {
        boost::this_thread::sleep_for(boost::chrono::milliseconds(20));
    }
}

void ProcessManager::tryCatchAny(boost::function<void()> f, const std::string& msg)
{
    try
    {
        f();
    }
    catch (const std::exception& ex)
    {
        std::cout << msg << ex.what() << std::endl;
    }
    catch (...)
    {
        std::cout << msg << "unknown exception" << std::endl;
    }
}
