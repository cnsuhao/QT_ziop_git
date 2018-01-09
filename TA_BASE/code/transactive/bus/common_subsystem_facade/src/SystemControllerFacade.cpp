#include "SystemControllerFacade.h"
#include "core/corba/src/CorbaUtil.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/exceptions/src/CannotUseSysControllerException.h"
#include <sstream>

namespace TA_Base_Bus
{
    namespace
    {
        const size_t DEFAULT_CALL_TIMEOUT = 1000;
    }

    SystemControllerAccessor::SystemControllerAccessor()
        : m_systemControllerAdminCorbaDef(TA_Base_Core::ISystemControllerAdminCorbaDef::_nil()),
          m_processManagerCorbaDef(TA_Base_Core::IProcessManagerCorbaDef::_nil()),
          m_adminCallTimeout(DEFAULT_CALL_TIMEOUT),
          m_processManagerCallTimeout(DEFAULT_CALL_TIMEOUT)
    {
    }

    void SystemControllerAccessor::setEndPointInfo(const std::string& hostName, unsigned long portNumber)
    {
        m_hostName = hostName;

        if (portNumber == 0)
        {
            portNumber = TA_Base_Core::DEFAULT_MGR_PORT;
        }

        {
            std::stringstream str;
            str << "corbaloc::1.2@" << hostName.c_str() << ":" << portNumber << "/ProcessManager";
            m_processMgrCorbalocStr = str.str();
        }

        {
            std::stringstream str;
            str << "corbaloc::1.2@" << hostName.c_str() << ":" << portNumber << "/SystemControllerAdmin";
            m_systemControllerAdminCorbalocStr = str.str();
        }

        m_processManagerCorbaDef = TA_Base_Core::IProcessManagerCorbaDef::_nil();
        m_systemControllerAdminCorbaDef = TA_Base_Core::ISystemControllerAdminCorbaDef::_nil();
    }

    void SystemControllerAccessor::resetEndPointInfo()
    {
        m_hostName.clear();
        m_processMgrCorbalocStr.clear();
        m_systemControllerAdminCorbalocStr.clear();
        m_processManagerCorbaDef = TA_Base_Core::IProcessManagerCorbaDef::_nil();
        m_systemControllerAdminCorbaDef = TA_Base_Core::ISystemControllerAdminCorbaDef::_nil();
    }

    bool SystemControllerAccessor::isValidConfig()
    {
        return !m_processMgrCorbalocStr.empty();
    }

    SystemControllerAccessor::~SystemControllerAccessor()
    {
    }

    TA_Base_Core::ProcessDataSeq* SystemControllerAccessor::getProcessData()
    {
        return getSystemControllerAdmin()->getProcessData();
    }

    void SystemControllerAccessor::getProcessData(std::vector<ManagedProcessData>& processData)
    {
        processData.clear();
        TA_Base_Core::ProcessDataSeq_var processDataCorbaSeq = getSystemControllerAdmin()->getProcessData();
        ManagedProcessDataConverter::convertProcessDataSeq(processDataCorbaSeq, processData);
    }

    void SystemControllerAccessor::shutdown(const char* hostname)
    {
        return getSystemControllerAdmin()->shutdown(hostname);
    }

    void SystemControllerAccessor::startProcess(const char* entityName)
    {
        return getSystemControllerAdmin()->startProcess(entityName);
    }

    void SystemControllerAccessor::changeAgentOperationMode(const char* entityName, TA_Base_Core::EOperationMode operationMode)
    {
        return getSystemControllerAdmin()->changeAgentOperationMode(entityName, operationMode);
    }

    void SystemControllerAccessor::stopProcess(const char* entityName)
    {
        return getSystemControllerAdmin()->stopProcess(entityName);
    }

    void SystemControllerAccessor::setRunParam(const char* entityName, const TA_Base_Core::RunParam& param)
    {
        getSystemControllerAdmin()->setRunParam(entityName, param);
    }

    void SystemControllerAccessor::setRunParam(const char* entityName, const TA_Base_Core::RunParams::ParamNameValue& param)
    {
        TA_Base_Core::RunParam runParamCorba;
        runParamCorba.name = CORBA::string_dup(param.name.c_str());
        runParamCorba.value = CORBA::string_dup(param.value.c_str());
        return getSystemControllerAdmin()->setRunParam(entityName, runParamCorba);
    }

    TA_Base_Core::AgentStatusEnum SystemControllerAccessor::getAgentsStatus()
    {
        return getSystemControllerAdmin()->getAgentsStatus();
    }

    void SystemControllerAccessor::setAdminCallTimeout(size_t timeout)
    {
        m_adminCallTimeout = timeout;
    }

    void SystemControllerAccessor::locateSystemControllerAdmin()
    {
        if (m_systemControllerAdminCorbalocStr.empty())
        {
            TA_THROW(TA_Base_Core::CannotUseSysControllerException("Missing endpoint information, System Controller process information could not be retrieved"));
        }

        try
        {
            CORBA::Object_var obj = TA_Base_Core::CorbaUtil::getInstance().stringToObject(m_systemControllerAdminCorbalocStr.c_str());

            if (CORBA::is_nil(obj))
            {
                TA_THROW(TA_Base_Core::CannotUseSysControllerException("System Controller Admin information could not be retrieved."));
            }

            omniORB::setClientCallTimeout(obj, m_adminCallTimeout);
            m_systemControllerAdminCorbaDef = TA_Base_Core::ISystemControllerAdminCorbaDef::_narrow(obj);
            omniORB::setClientCallTimeout(m_systemControllerAdminCorbaDef, m_adminCallTimeout);

            if (CORBA::is_nil(m_systemControllerAdminCorbaDef))
            {
                TA_THROW(TA_Base_Core::CannotUseSysControllerException("System Controller Admin information could not be retrieved."));
            }
        }
        catch (const CORBA::Exception& ex)
        {
            // We are not expecting to catch anything other than ApplicationExceptions
            // If any unexpected exceptions are raised (eg CORBA::Exception), we re-throw
            // them as ApplicationExceptions.
            LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch,
                TA_Base_Core::CorbaUtil::getInstance().exceptionToString(ex).c_str(),
                "Caught a CORBA exception, could not contact SystemController");
            TA_THROW(TA_Base_Core::CannotUseSysControllerException("System Controller Admin information could not be retrieved."));
        }
        catch (...)
        {
            LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "Cannot find system controller. Converting into a SystemController exception");
            TA_THROW(TA_Base_Core::CannotUseSysControllerException("System Controller process information could not be retrieved."));
        }
    }

    void SystemControllerAccessor::locateProcessManager()
    {
        if (m_processMgrCorbalocStr.empty())
        {
            TA_THROW(TA_Base_Core::CannotUseSysControllerException("Missing endpoint information, System Controller process information could not be retrieved"));
        }

        try
        {
            // Attempt to get Process Manager
            CORBA::Object_var obj = TA_Base_Core::CorbaUtil::getInstance().stringToObject(m_processMgrCorbalocStr.c_str());

            if (CORBA::is_nil(obj))
            {
                TA_THROW(TA_Base_Core::CannotUseSysControllerException("System Controller process information could not be retrieved."));
            }

            omniORB::setClientCallTimeout(obj, m_processManagerCallTimeout);
            m_processManagerCorbaDef = TA_Base_Core::IProcessManagerCorbaDef::_narrow(obj);
            omniORB::setClientCallTimeout(m_processManagerCorbaDef, m_processManagerCallTimeout);

            if (CORBA::is_nil(m_processManagerCorbaDef))
            {
                TA_THROW(TA_Base_Core::CannotUseSysControllerException("System Controller process information could not be retrieved."));
            }
        }
        catch (const CORBA::Exception& ex)
        {
            // We are not expecting to catch anything other than ApplicationExceptions
            // If any unexpected exceptions are raised (eg CORBA::Exception), we re-throw
            // them as ApplicationExceptions.
            LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch,
                TA_Base_Core::CorbaUtil::getInstance().exceptionToString(ex).c_str(),
                "Caught a CORBA exception, could not contact SystemController");
            TA_THROW(TA_Base_Core::CannotUseSysControllerException("System Controller process information could not be retrieved."));
        }
        catch (...)
        {
            LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "Cannot find system controller. Converting into a SystemController exception");
            TA_THROW(TA_Base_Core::CannotUseSysControllerException("System Controller process information could not be retrieved."));
        }
    }

    TA_Base_Core::ISystemControllerAdminCorbaDef_ptr SystemControllerAccessor::getSystemControllerAdmin()
    {
        if (CORBA::is_nil(m_systemControllerAdminCorbaDef.in()))
        {
            locateSystemControllerAdmin();
        }

        return m_systemControllerAdminCorbaDef.in();
    }

    TA_Base_Core::IProcessManagerCorbaDef_ptr SystemControllerAccessor::getProcessManager()
    {
        if (CORBA::is_nil(m_processManagerCorbaDef.in()))
        {
            locateProcessManager();
        }

        return m_processManagerCorbaDef.in();
    }

    void SystemControllerAccessor::terminating(TA_Base_Core::ProcessId p_processID, TA_Base_Core::EManagedProcessTerminateCode p_code)
    {
        getProcessManager()->terminating(p_processID, p_code);
    }

    void SystemControllerAccessor::registerManagedProcess(TA_Base_Core::IManagedProcessCorbaDef_ptr p_managedProcess, TA_Base_Core::ProcessId p_processId, ::CORBA::ULong applicationType, const char* entity)
    {
        getProcessManager()->registerManagedProcess(p_managedProcess, p_processId, applicationType, entity);
    }

    void SystemControllerAccessor::registerManagedApplication(TA_Base_Core::IManagedApplicationCorbaDef_ptr p_managedApp, TA_Base_Core::ProcessId p_processId)
    {
        getProcessManager()->registerManagedApplication(p_managedApp, p_processId);
    }

    TA_Base_Core::RunParamSeq* SystemControllerAccessor::getParams(TA_Base_Core::ProcessId p_processId, const char* hostname)
    {
        return getProcessManager()->getParams(p_processId, hostname);
    }

    void SystemControllerAccessor::getParams(TA_Base_Core::ProcessId p_processId, const char* hostname, TA_Base_Core::RunParams::ParamVector& vtRunParamInfo)
    {
        TA_Base_Core::RunParamSeq_var runParamSeq = getProcessManager()->getParams(p_processId, hostname);
        vtRunParamInfo.clear();
        ManagedProcessDataConverter::convertRunParamSeq(runParamSeq, vtRunParamInfo);
    }

    void SystemControllerAccessor::poll()
    {
        getProcessManager()->poll();
    }

    void SystemControllerAccessor::statusChanged(TA_Base_Core::EProcessStatus status, TA_Base_Core::ProcessId p_processId)
    {
        getProcessManager()->statusChanged(status, p_processId);
    }

    void SystemControllerAccessor::setProcessManagerCallTimeout(size_t timeout)
    {
        m_processManagerCallTimeout = timeout;
    }

    bool SystemControllerAccessor::ping()
    {
        try
        {
            locateProcessManager();
            locateSystemControllerAdmin();
            return true;
        }
        catch (...)
        {
        }

        return false;
    }
}
