/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/ProcessManager.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This receives callbacks from the ManagedProcesses and will also maintain the running
  * of applications. This implements the ProcessManager corba definition which means this
  * object is what the ManagedProcess's get when they resolve the initial process monitor.
  */
#pragma comment( lib, "winmm.lib" )
#include "StdAfx.h"
#include "ProcessManager.h"
#include "ApplicationSignal.h"
#include "SessionSignal.h"
#include "RunningApplication.h"
#include "RunningApplicationManager.h"
#include "ControlStationConstants.h"
#include "RunParamSeqWrap.h"
#include "ControlStationConstants.h"
#include "core/exceptions/src/ApplicationException.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include "core/corba/src/CorbaUtil.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"

using namespace TA_Base_Core;
using namespace TA_Base_App;

const std::string USE_DEBUG_PID_FILE_NAMES = "1";

ProcessManager::ProcessManager()
{
    setupSignalConnections();
}

void ProcessManager::setupSignalConnections()
{
    SessionSignal::logined.connect(boost::bind(&ProcessManager::activate, this));
    SessionSignal::logouted.connect(boost::bind(&ProcessManager::deactivate, this));
}

void ProcessManager::activate()
{
    TA_Base_Core::CorbaUtil::getInstance().setServantKey(this, "ProcessManager");
    activateServant();
}

void ProcessManager::deactivate()
{
    deactivateServant();
}

void ProcessManager::launchApplication(unsigned long appType,
                                       const std::string& additionalCommands,
                                       unsigned long posFlag,
                                       unsigned long alignFlag,
                                       const RECT& objectDim,
                                       const RECT& boundaryDim,
                                       bool isVisible /*= true*/)
{
    LOG_INFO("Launching the application %lu with the command line: %s", appType, additionalCommands.c_str());
    ApplicationSignal::launchApplication(appType, additionalCommands, posFlag, alignFlag, objectDim, boundaryDim, isVisible);
}

void ProcessManager::terminating(TA_Base_Core::ProcessId p_processID, TA_Base_Core::EManagedProcessTerminateCode p_code)
{
    ApplicationSignal::processTerminating[p_processID](p_code);
}

void ProcessManager::registerManagedProcess(TA_Base_Core::IManagedProcessCorbaDef_ptr managedProcess,
                                            TA_Base_Core::ProcessId processId,
                                            CORBA::ULong applicationType,
                                            const char* entity)
{
    LOG_INFO("Managed Process Registered (pid=%lu, appType=%lu, entity=%s)", processId, applicationType, entity);
    TA_Base_Core::IManagedProcessCorbaDef_var process = TA_Base_Core::IManagedProcessCorbaDef::_duplicate(managedProcess);
    ApplicationSignal::registerManagedProcess(process, processId, applicationType, entity);
}

void ProcessManager::registerManagedApplication(TA_Base_Core::IManagedApplicationCorbaDef_ptr managedApp, TA_Base_Core::ProcessId processId)
{
    LOG_INFO("Managed Application Registered (pid=%lu)", processId);
    TA_Base_Core::IManagedGUICorbaDef_var app = IManagedGUICorbaDef::_narrow(managedApp);
    ApplicationSignal::registerManagedApplication[processId](app);
}

void ProcessManager::poll()
{
}

TA_Base_Core::RunParamSeq* ProcessManager::getParams(TA_Base_Core::ProcessId processId, const char* /* hostname */)
{
    RunParamSeqWrap params;
    RunParams::ParamVector paramVector;
    RunParams::getInstance().getAll(paramVector);
    std::stringstream log;
    log << boost::format("Parameters passed to application with pid %lu: \n") % processId;

    static std::set<std::string> ignores =
    {
        TA_ControlStation::HOST_NAMES, TA_ControlStation::PORT_NUMBER, TA_ControlStation::DEBUG_LOG_DIR,
        RPARAM_ENTITYNAME, RPARAM_DEBUGFILE, RPARAM_MGRPORT, RPARAM_PROGNAME, RPARAM_HELPFILE
    };

    for (unsigned int i = 0; i < paramVector.size(); ++i)
    {
        if (!boost::algorithm::one_of_equal(ignores, paramVector[i].name))
        {
            log << boost::format("%s=%s") % paramVector[i].name % paramVector[i].value;
            params.push_name_value(paramVector[i]);
        }
    }

    if (processId == 0)
    {
        log << boost::format("Anonymous process fetching RunParams. Returning the common run params: %s=%s\n") %
            TA_ControlStation::DEBUG_LOG_DIR % RunParams::getInstance().get(TA_ControlStation::DEBUG_LOG_DIR);
        params.push_back(TA_ControlStation::DEBUG_LOG_DIR, RunParams::getInstance().get(TA_ControlStation::DEBUG_LOG_DIR));
        LOG_INFO("%s", log.str().c_str());
        return params._return();
    }

    // If we know about this application then populate the list with some parameters
    RunningApplication* runningApplication = RunningApplicationManager::getInstance().getRunningApplication(processId);

    if (runningApplication)
    {
        // Set up the debug file for the application from the DebugPath we have
        boost::filesystem::path debugPath = boost::filesystem::system_complete(RunParams::getInstance().get(TA_ControlStation::DEBUG_LOG_DIR));
        std::string entityName = runningApplication->getEntityName();

        if (debugPath.size() && entityName.size())
        {
            // set the RPARAM_DEBUGPIDFILENAMES so that it doesn't try to name the files without the PID
            if (!runningApplication->areInstancesLimited())
            {
                params.push_back(RPARAM_DEBUGPIDFILENAMES, USE_DEBUG_PID_FILE_NAMES);
                LOGMORE(SourceInfo, "%s=%s", RPARAM_DEBUGPIDFILENAMES, USE_DEBUG_PID_FILE_NAMES.c_str());
            }

            debugPath /= TA_ControlStation::LOG_PRE_STRING + entityName + TA_ControlStation::LOG_POST_STRING;
            params.push_back(RPARAM_DEBUGFILE, debugPath.string());
            log << boost::format("%s=%s") % RPARAM_DEBUGFILE % debugPath.string();
        }

        std::string appName = runningApplication->getApplicationFullName();

        if (appName.size())
        {
            params.push_back(RPARAM_APPNAME, appName);
            log << params.last_str();
        }

        // if help file is set and not empty, then use it
        std::string helpFile = runningApplication->getHelpFile();

        if (helpFile.size())
        {
            params.push_back(RPARAM_HELPFILE, helpFile);
            log << boost::format("%s=%s") % RPARAM_HELPFILE % helpFile;
        }
    }

    LOG_INFO("%s", log.str().c_str());
    return params._return();
}

void ProcessManager::giveApplicationFocus(unsigned long pid)
{
    ApplicationSignal::giveApplicationFocus[pid](TA_Base_Core::GetFocus);
}
