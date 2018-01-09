/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/ControlStationAppController.cpp $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  */
#include "StdAfx.h"
#include "ControlStationAppController.h"
#include "ArchiveLogs.h"
#include "ControlStationConstants.h"
#include "DataCache.h"
#include "RunAnys.h"
#include "ServerMonitor.h"
#include "DatabaseStatusMonitor.h"
#include "SessionDetails.h"
#include "DutyManager.h"
#include "ScreenPositioning.h"
#include "GraphWorxManager.h"
#include "ControlStationCorbaImpl.h"
#include "SystemControllerMessageHandler.h"
#include "GraphworxMessageHandler.h"
#include "Signals.h"
#include "SessionManager.h"
#include "SoundManager.h"
#include "LoginLocationSwitcher.h"
#include "ControlStationOnlineUpdateHandler.h"
#include "ProfileLoginDlg.h"
#include "LogoutDlg.h"
#include "LoginTask.h"
#include "RightsManager.h"
#include "ApplicationManager.h"
#include "RunningApplicationManager.h"
#include "ControlStationAuditMessageSender.h"
#include "ControlStation2.h"
#include "ControlStation2Impl.h"
#include "ControlStationSignal.h"
#include "ControlStationRootDialog.h"
#include "core/corba/src/CorbaUtil.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/Hostname.h"
#include "core/utilities/src/RunParamsAny.h"
#include "core/exceptions/src/SessionException.h"
#include "core/exceptions/src/InvalidCommandLineException.h"
#include "core/exceptions/src/UtilitiesException.h"
#include "core/message/src/TransactiveMessageSubscription.h"
#include "core/message/src/PublicationManager.h"
#include "core/message/src/ConnectionChecker.h"
#include "core/configuration_updates/src/OnlineUpdateListener.h"
#include "ace/Init_ACE.h"

using namespace TA_Base_Core;

ControlStationAppController::ControlStationAppController(TA_Base_Bus::ITransActiveApp* pApp, const char* appName)
    : TA_Base_Bus::AbstractGUIAppController(pApp, appName)
{
    setupSignalConnections();
}

ControlStationAppController::~ControlStationAppController()
{
}

void ControlStationAppController::setupSignalConnections()
{
    ControlStationSignal::waitInitializationComplete.connect(boost::bind(&ControlStationAppController::waitInitializationComplete, this));

    //TODO
    //Signals::systemControllerStateChanged.connect(boost::bind(&ControlStationAppController::handleSystemControllerStateChangedSignal, this, _1, _2));
}

void ControlStationAppController::initAppController(const std::string& commandLine, bool)
{
    ACE::init();

    boost::async(boost::bind(&ControlStationAppController::initializeRunParams, this, commandLine));
    archiveLogFiles(RunParams::getInstance().promiseGet(RPARAM_DEBUGFILE));
    initializeDebugUtility();

    boost::async(boost::bind(&ControlStationAppController::initializeCorba, this));
    boost::async(boost::bind(&ControlStationAppController::locateSystemController, this));
    boost::async(boost::bind(&ControlStationAppController::retrieveRunParamsFromSystemController, this));

    DataCache::getInstanceNoInit();
    DutyManager::getInstanceNoInit();
    ScreenPositioning::getInstanceNoInit();
    GraphWorxManager::getInstanceNoInit();
    ControlStationCorbaImpl::getInstanceNoInit();
    SystemControllerMessageHandler::getInstanceNoInit();
    GraphworxMessageHandler::getInstanceNoInit();
    ServerMonitor::getInstanceNoInit();
    ControlStationOnlineUpdateHandler::getInstanceNoInit();
    SessionManager::getInstanceNoInit();
    LoginLocationSwitcher::getInstanceNoInit();
    SoundManager::getInstanceNoInit();
    DatabaseStatusMonitor::getInstanceNoInit();
    SessionDetails::getInstanceNoInit();
    RightsManager::getInstanceNoInit();
    ProfileLoginDlg::getInstanceNoInit();
    LogoutDlg::getInstanceNoInit();
    ApplicationManager::getInstanceNoInit();
    RunningApplicationManager::getInstanceNoInit();
    ControlStationAuditMessageSender::getInstanceNoInit();
    ControlStation2Impl::getInstanceNoInit();

    boost::async(LoginTask());
}

void ControlStationAppController::waitInitializationComplete()
{
    DataCache::getInstance();
    DutyManager::getInstance();
    ScreenPositioning::getInstance();
    GraphWorxManager::getInstance();
    ControlStationCorbaImpl::getInstance();
    SystemControllerMessageHandler::getInstance();
    GraphworxMessageHandler::getInstance();
    ServerMonitor::getInstance();
    ControlStationOnlineUpdateHandler::getInstance();
    SessionManager::getInstance();
    LoginLocationSwitcher::getInstance();
    SoundManager::getInstance();
    DatabaseStatusMonitor::getInstance();
    SessionDetails::getInstance();
    RightsManager::getInstance();
    ProfileLoginDlg::getInstance();
    LogoutDlg::getInstance();
    ApplicationManager::getInstance();
    RunningApplicationManager::getInstance();
    ControlStationAuditMessageSender::getInstance();
    ControlStation2Impl::getInstance();
}

void ControlStationAppController::archiveLogFiles(const std::string& debugFileName)
{
    new ArchiveLogs(debugFileName);
}

void ControlStationAppController::initializeRunParams(const std::string& commandLine)
{
    if (false == RunParams::getInstance().parseCmdLine(commandLine.c_str()))
    {
        TA_ASSERT(false, "Command line arguments are in invalid format");
    }

    TA_Base_Core::parseLocalConfigurationFile();

    {
        TA_Assert(RunParams::getInstance().isSet(TA_ControlStation::HOST_NAMES));
        std::vector<std::string> hostnames;
        boost::split(hostnames, RunParams::getInstance().get(TA_ControlStation::HOST_NAMES), boost::is_any_of(",;"));
        RunParamsAny::getInstance().set(TA_ControlStation::HOST_NAMES, hostnames);
    }

    RunParams::getInstance().set("Localhost", TA_Base_Core::Hostname::getHostname());
    RunParams::getInstance().set(RPARAM_INITIALIZE_COMPLETE);
}

void ControlStationAppController::initializeCorba()
{
    TA_Base_Core::RunParams::getInstance().promiseGet(RPARAM_INITIALIZE_COMPLETE);

    unsigned long mgrPort = TA_Base_Core::getRunParamValue(RPARAM_MGRPORT, TA_Base_Core::DEFAULT_MGR_PORT);

    if (false == TA_Base_Core::CorbaUtil::getInstance().initialise(mgrPort))
    {
        TA_ASSERT(false, "Failed to initialise corba");
    }

    try
    {
        TA_Base_Core::CorbaUtil::getInstance().activate();
    }
    catch (...)
    {
        TA_ASSERT(false, "Failed to activate corba");
    }
}

void ControlStationAppController::initializeDebugUtility()
{
    RunParams::getInstance().promiseIsSet(RPARAM_INITIALIZE_COMPLETE);

    // Set the debug file name
    std::string param = RunParams::getInstance().get(RPARAM_DEBUGFILE);

    if (!param.empty())
    {
        DebugUtil::getInstance().setFile(param, getRunParamValue(RPARAM_DEBUGFILEMAXSIZE, 0));
    }

    // Set the debug level
    param = RunParams::getInstance().get(RPARAM_DEBUGLEVEL);

    if (!param.empty())
    {
        DebugUtil::getInstance().setLevel(DebugUtil::getDebugLevelFromString(param.c_str()));
    }

    // Set the maximum size of a debug file in bytes
    param = RunParams::getInstance().get(RPARAM_DEBUGFILEMAXSIZE);

    if (!param.empty())
    {
        DebugUtil::getInstance().setMaxSize(atoi(param.c_str()));
    }

    // Set the maximum number of log files
    param = RunParams::getInstance().get(RPARAM_DEBUGMAXFILES);

    if (!param.empty())
    {
        DebugUtil::getInstance().setMaxFiles(atoi(param.c_str()));
    }

    // Use Pid encoding in filenames
    param = TA_Base_Core::RunParams::getInstance().get(RPARAM_DEBUGPIDFILENAMES);

    if (!param.empty())
    {
        TA_Base_Core::DebugUtil::getInstance().encryptPidInFilenames(param);
    }

    // Use debug level specific log files
    param = TA_Base_Core::RunParams::getInstance().get(RPARAM_DEBUGLEVELFILE);

    if (!param.empty())
    {
        TA_Base_Core::DebugUtil::getInstance().decodeLevelFilenames(param);
    }

    // Turn off logging for debug levels
    param = TA_Base_Core::RunParams::getInstance().get(RPARAM_DEBUGLEVELOFF);

    if (!param.empty())
    {
        TA_Base_Core::DebugUtil::getInstance().decodeDisabledLevels(param);
    }
}

void ControlStationAppController::locateSystemController()
{
    RunParams::getInstance().promiseIsSet(RPARAM_INITIALIZE_COMPLETE);
    unsigned long portNumber = TA_Base_Core::getRunParamValue(TA_ControlStation::PORT_NUMBER, TA_Base_Core::DEFAULT_MGR_PORT);
    const std::vector<std::string>& hostnames = TA_Base_Core::RunParamsAny::getInstance().promiseGet<std::vector<std::string> >(TA_ControlStation::HOST_NAMES);
    typedef std::map<std::string, boost::shared_future<TA_Base_Core::IProcessManagerCorbaDef_var> > TheMap;
    TheMap futureMap;

    for (const std::string& hostname : hostnames)
    {
        futureMap[hostname] = boost::async(boost::bind(&ControlStationAppController::locateSpecificSystemController, hostname, portNumber)).share();
    }

    for (TheMap::value_type& v : futureMap)
    {
        const std::string& hostname = v.first;
        const TA_Base_Core::IProcessManagerCorbaDef_var& systemController = v.second.get();

        if (!CORBA::is_nil(systemController))
        {
            m_systemController = systemController;
            LOG_INFO("Found system controller, %s", hostname.c_str());
            omniORB::setClientCallTimeout(m_systemController, 2000);

            // TD 6950: Set the system controller name as a run param so it can be retrieved by
            // applications that require subscribing to System Controller messages to be notified
            // when the system is in degraded mode or recovered from degraded mode.
            RunParams::getInstance().set(RPARAM_SYSTEMCONTROLLERNAME, hostname);
            break;
        }
    }

    if (CORBA::is_nil(m_systemController))
    {
        TA_ASSERT(false, "Cannot locate SystemController");
    }
}

TA_Base_Core::IProcessManagerCorbaDef_var ControlStationAppController::locateSpecificSystemController(const std::string& hostname, unsigned long portNumber)
{
    std::string corbaloc = str(boost::format("corbaloc::1.2@%s:%d/ProcessManager") % hostname % portNumber);
    LOG_INFO("Locating system controller %s - %s", hostname.c_str(), corbaloc.c_str());

    try
    {
        CORBA::Object_var obj = CorbaUtil::getInstance().stringToObject(corbaloc);

        if (!CORBA::is_nil(obj))
        {
            omniORB::setClientCallTimeout(obj, 2000);
            return TA_Base_Core::IProcessManagerCorbaDef::_narrow(obj);
        }
    }
    catch (...)
    {
    }

    return TA_Base_Core::IProcessManagerCorbaDef::_nil();
}

void ControlStationAppController::retrieveRunParamsFromSystemController()
{
    try
    {
        std::string server = RunParams::getInstance().promiseGet(RPARAM_SYSTEMCONTROLLERNAME);
        std::string localhost = RunParams::getInstance().promiseGet("Localhost");
        TA_Base_Core::RunParamSeq_var params = m_systemController->getParams(0, localhost.c_str());

        // For each RunParam we have retrieved. If it is not already set then set it
        for (unsigned long i = 0; i < params->length(); ++i)
        {
            if (!RunParams::getInstance().isSet(params[i].name))
            {
                LOG_INFO("Set new run param %s to %s from system controller %s", params[i].name, params[i].value, server.c_str());
                RunParams::getInstance().set(params[i].name, params[i].value);
            }
            else
            {
                std::string value = RunParams::getInstance().get(params[i].name);

                if (value.empty())
                {
                    LOG_INFO("Set run param %s to %s from system controller %s", params[i].name, params[i].value, server.c_str());
                    RunParams::getInstance().set(params[i].name, params[i].value);
                }
                else
                {
                    LOG_INFO("Ignore run param %s=%s (local=%s) from system controller %s", params[i].name, params[i].value, value.c_str(), server.c_str());
                }
            }
        }
    }
    catch (const CORBA::Exception& ex)
    {
        LOG_EXCEPTION(TA_Base_Core::CorbaUtil::getInstance().exceptionToString(ex), "");
        TA_THROW(SessionException("Could not retrieve a list of RunParams from the System Controller. Since we won't have most of the information we need then there is no point continuing"));
    }
    catch (...)
    {
        LOG_EXCEPTION("Unknown", "Caught some unknown exception when trying to retrive run params");
        TA_THROW(SessionException("Could not retrieve a list of RunParams from the System Controller. Since we won't have most of the information we need then there is no point continuing"));
    }
}

void ControlStationAppController::cleanup()
{
    ACE::fini();
    OnlineUpdateListener::cleanUp();
    Naming::cleanUp();
    gDestroySubscriptionManager();
    gTerminateAndDeleteStructuredEventSuppliers();
    ChannelLocatorConnectionMgr::removeInstance();
    CorbaUtil::getInstance().shutdown();
    CorbaUtil::cleanup();
}

void ControlStationAppController::handleSystemControllerStateChangedSignal(bool isGroupOnline, const std::string& groupName)
{
    locateSystemController();
}
