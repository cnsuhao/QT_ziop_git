/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/SessionManager.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This class manages all session related tasks such as logging in, logging out
  * and communicating with the System Controller.
  */

#pragma warning(disable:4786)
#include "Utilities.h"
#include "app/system_control/control_station/src/StdAfx.h"
#include "app/system_control/control_station/src/SessionManager.h"
#include "app/system_control/control_station/src/GraphWorxManager.h"
#include "app/system_control/control_station/src/ControlStationCorbaImpl.h"
#include "app/system_control/control_station/src/IGraphWorxManager.h"
#include "app/system_control/control_station/src/ControlStationRootDialog.h"
#include "app/system_control/control_station/src/ProcessManager.h"
#include "app/system_control/control_station/src/WinkeysMgr.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/generic_gui_pmod/src/AppLauncher.h"
#include "bus/application_types/src/apptypes.h"
#include "bus/database_status_monitor/DsmCorbaDef/IDL/src/DatabaseStatusCorbaDef.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"
#include "bus/security/authentication_library/src/AuthenticationLibrary.h"
#include "bus/security/rights_library/src/RightsLibrary.h"
#include "core/alarm/src/AlarmHelperManager.h"
#include "core/configuration_updates/src/OnlineUpdateListener.h"
#include "core/corba/src/CorbaUtil.h"
#include "core/data_access_interface/entity_access/src/ControlStation.h"
#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/data_access_interface/src/ILocation.h"
#include "core/data_access_interface/src/ISession.h"
#include "core/data_access_interface/src/IProfile.h"
#include "core/data_access_interface/src/IResource.h"
#include "core/data_access_interface/src/OperatorAccessFactory.h"
#include "core/data_access_interface/src/ProfileAccessFactory.h"
#include "core/data_access_interface/src/SessionAccessFactory.h"
#include "core/data_access_interface/src/LocationAccessFactory.h"
#include "core/data_access_interface/src/ResourceAccessFactory.h"
#include "core/data_access_interface/src/SimpleDbDatabase.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/EntityTypeException.h"
#include "core/exceptions/src/SessionException.h"
#include "core/exceptions/src/ApplicationException.h"
#include "core/exceptions/src/GraphWorxException.h"
#include "core/exceptions/src/AuthenticationSecurityException.h"
#include "core/exceptions/src/AuthenticationAgentException.h"
#include "core/exceptions/src/RightsException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/Hostname.h"
#include "core/message/types/SystemControllerComms_MessageTypes.h"
#include "core/message/types/ControlStationAudit_MessageTypes.h"
#include "core/message/types/ControlStationComms_MessageTypes.h"
#include "core/message/src/MessageSubscriptionManager.h"
#include "core/message/src/AuditMessageSender.h"
#include "core/message/src/MessagePublicationManager.h"
#include "core/message/src/TransactiveMessageSubscription.h"
#include "core/message/src/PublicationManager.h"
#include "core/message/src/ConnectionChecker.h"
#include "core/process_management/IDL/src/IControlStationCorbaDef.h"
#include "core/process_management/IDL/src/IManagedApplicationCorbaDef.h"
#include "core/database/src/DBException.h"
#include "core/data_access_interface/src/OperatorPasswordAccessFactory.h"
#include "core/data_access_interface/src/IOperatorPassword.h"
#include "core/data_access_interface/src/SecuritySettingAccessFactory.h"
#include "core/data_access_interface/alarm_rule/src/AlarmRuleAccessFactory.h"
#include <windows.h>
#include <winbase.h>
#include <algorithm>
#include <cctype>
#include <sstream>
#include <winsock2.h>

using namespace TA_Base_App::TA_ControlStation;
using TA_Base_Bus::AuthenticationLibrary;
using TA_Base_Core::DebugUtil;
using TA_Base_Core::OnlineUpdateListener;
using TA_Base_Core::RunParams;
using TA_Base_Core::ThreadGuard;
using TA_Base_Core::MessageSubscriptionManager;
using TA_Base_Core::EntityAccessFactory;
using TA_Base_Core::DatabaseException;
using TA_Base_Core::DataException;
using TA_Base_Core::EntityTypeException;
using TA_Base_Core::GraphWorxException;
using TA_Base_Core::SessionException;
using TA_Base_Core::ApplicationException;
using TA_Base_Core::RightsException;
using TA_Base_Core::AuthenticationSecurityException;
using TA_Base_Core::AuthenticationAgentException;
using TA_Base_Core::TransactiveException;
using TA_Base_Bus::TransActiveMessage;
using TA_Base_Core::DescriptionParametersWrap;

SessionManager* SessionManager::s_instance = NULL;
bool SessionManager::m_dbUserPwdValid = true;

TA_Base_Core::NonReEntrantThreadLockable SessionManager::m_singletonLock;
const std::string SessionManager::CONSOLE_NAME("ConsoleName");
const std::string SessionManager::PROFILE_NAME("ProfileName");
const std::string SessionManager::USER_NAME("Username");
const std::string SessionManager::GROUP_ONLINE = "GroupOnline";
const std::string SessionManager::LOGIC_LOCATION("LogicLoc");
bool SessionManager::s_exitNeedsRestart = false;

SessionManager::SessionManager()
    : m_isLoggedIn(false),
      m_resourceKey(-1),
      m_sCGroupOnline(true),
      m_hostname(""),
      m_systemControllerName(""),
      m_graphWorxHelp(""),
      m_controlStationEntity(NULL),
      m_systemControllerEntity(NULL),
      m_controlStationImpl(NULL),
      m_processManager(NULL),
      m_graphWorxManager(NULL),
      m_authenticationMgr(NULL),
      m_rightsMgr(NULL),
      m_dutyManager(NULL),
      m_dllHostEntity(NULL),
      m_rootDialog(ControlStationRootDialog::getInstance())
{
    LOG(SourceInfo, DebugUtil::FunctionEntry, "run");

    m_hostname = TA_Base_Core::Hostname::getHostname();
    LOG(DEBUG_INFO, "This console is: %s", m_hostname.c_str());

    locateSystemController();
    retrieveRunParams(m_hostname);
    readSecuritySetting();

    // Load the Control Station entity and retrieve all the data we need
    std::string entityName = RunParams::getInstance().get(RPARAM_ENTITYNAME);

    try
    {
        m_controlStationEntity = dynamic_cast<TA_Base_Core::ControlStation*>(EntityAccessFactory::getInstance().getEntity(entityName));
    }
    catch (const DataException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "DataException", "Converting into a SessionException");
        TA_THROW(SessionException("Could not load the ControlStation entity. Unable to continue"));
    }
    catch (const DatabaseException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "DatabaseException", "Converting into a SessionException");
        TA_THROW(SessionException("Could not load the ControlStation entity. Unable to continue"));
    }
    catch (const DBException& e)
    {
        if (e.getNativeCode() == TA_Base_Core::INVALID_USERNAME_PASSWORD)
        {
            m_dbUserPwdValid = false; //TD17205,jianghp
        }
    }

    m_auditMessageSender.reset(TA_Base_Core::MessagePublicationManager::getInstance().getAuditMessageSender(TA_Base_Core::ControlStationAudit::Context));
}

SessionManager& SessionManager::getInstance()
{
    // protect the instance check / creation
    ThreadGuard guard(m_singletonLock);

    if (s_instance == NULL)
    {
        s_instance = new SessionManager();
    }

    return *s_instance;
}

SessionManager::~SessionManager()
{
    FUNCTION_ENTRY("~SessionManager");

    try
    {
        // This takes a long time to shutdown, so do this first, hopefully all of them
        // would have shutdown by the time process manager shuts down its external processes
        LOG(DEBUG_DEBUG, "Going to delete GraphWorx Manager");

        if (m_graphWorxManager !=  NULL)
        {
            delete m_graphWorxManager;
            m_graphWorxManager = NULL;
            LOG(DEBUG_INFO, "Successfully deleted GraphWorx Manager");
        }

        LOG(DEBUG_DEBUG, "Going to delete banner entities");

        for (unsigned int i = 0; i < m_bannerEntities.size(); i++)
        {
            delete m_bannerEntities[i];
            m_bannerEntities[i] = NULL;
            LOG(DEBUG_INFO, "Successfully deleted banner entities");
        }

        LOG(DEBUG_DEBUG, "Going to delete dllhost entity");
        delete m_dllHostEntity;
        m_dllHostEntity = NULL;
        LOG(DEBUG_INFO, "Successfully deleted dllhost entity");

        LOG(DEBUG_DEBUG, "Going to delete control station entity");

        if (m_controlStationEntity != NULL)
        {
            delete m_controlStationEntity;
            m_controlStationEntity = NULL;
            LOG(DEBUG_INFO, "Successfully deleted control station entity");
        }

        LOG(DEBUG_DEBUG, "Going to delete system controller entity");

        if (m_systemControllerEntity != NULL)
        {
            delete m_systemControllerEntity;
            m_systemControllerEntity = NULL;
            LOG(DEBUG_INFO, "Successfully deleted system controller entity");
        }

        LOG(DEBUG_DEBUG, "Going to delete authentication manager");

        if (m_authenticationMgr != NULL)
        {
            std::string id = m_sessionDetails.getSessionId();

            if (!id.empty())
            {
                try
                {
                    LOG(DEBUG_WARN, "Still got a session, going to terminate the session first");
                    // make sure we end the session
                    m_authenticationMgr->endSession(id);
                }
                catch (...)
                {
                    // catch all exceptions since we want to continue deleting the rest of the stuff
                    // of the stuff even if some exception occurred
                }
            }

            delete m_authenticationMgr;
            m_authenticationMgr = NULL;
            LOG(DEBUG_INFO, "Successfully deleted authentication Manager");
        }

        LOG(DEBUG_DEBUG, "Going to delete rights manager");

        if (m_rightsMgr != NULL)
        {
            delete m_rightsMgr;
            m_rightsMgr = NULL;
            LOG(DEBUG_INFO, "Successfully deleted rights Manager");
        }

        LOG(DEBUG_DEBUG, "Going to delete duty manager");

        if (m_dutyManager != NULL)
        {
            delete m_dutyManager; // TD11312, TD11486 - Clean up duty manager registration
            m_dutyManager = NULL;
            LOG(DEBUG_INFO, "Successfully deleted duty Manager");
        }

        LOG(DEBUG_DEBUG, "Going to delete control station impl");
        bool isCorbaShutdownRequired = false;

        try
        {
            if (m_controlStationImpl != NULL)
            {
                m_controlStationImpl->deactivateServant();
                isCorbaShutdownRequired = true;
                LOG(DEBUG_INFO, "Successfully deactivated control station servant");
            }
        }
        catch (...)
        {
            LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "Failed to deactivate control station");
        }

        LOG(DEBUG_DEBUG, "Going to delete process manager");

        try
        {
            if (m_processManager != NULL)
            {
                m_processManager->terminateAllApplications(TA_Base_App::TA_ControlStation::EXIT);
                m_processManager->deactivateServant();
                isCorbaShutdownRequired = true;
                LOG(DEBUG_INFO, "Successfully deleted process Manager");
            }
        }
        catch (std::exception& ex)
        {
            LOG_EXCEPTION_CATCH(SourceInfo, "Error terminating/deactivating process manager", ex.what());
        }
        catch (...)
        {
            LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "Failed to deactivate process manager");
        }

        LOG(DEBUG_DEBUG, "Going to clean up online update and message registrations");
        OnlineUpdateListener::cleanUp(); // TD1399 - Clean up all registrations to online updates so that we can shutdown properly
        // Unsubscribe the subscriber
        MessageSubscriptionManager::getInstance().unsubscribeToMessages(this);

        LOG(DEBUG_DEBUG, "Going to delete process manager");

        if (isCorbaShutdownRequired)
        {
            // Clean up the alarm subsystem
            TA_Base_Core::AlarmHelperManager::getInstance().cleanUp();

            // Clean up named objects
            TA_Base_Core::Naming::cleanUp();

            TA_Base_Core::gDestroySubscriptionManager();
            TA_Base_Core::gTerminateAndDeleteStructuredEventSuppliers();
            TA_Base_Core::ChannelLocatorConnectionMgr::removeInstance();

            // Shutdown corba
            TA_Base_Core::CorbaUtil::getInstance().shutdown();
            TA_Base_Core::CorbaUtil::cleanup();
            LOG(DEBUG_INFO, "Successfully shutdown corba");
        }
    }
    catch (...)
    {
        // Catch all exceptions so we don't get errors filtering out
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "Unknown", "Caught in destructor of SessionManager");
    }

    FUNCTION_EXIT;
}

void SessionManager::cleanUp()
{
    LOG(SourceInfo, DebugUtil::FunctionEntry, "cleanUp");

    // protect the instance check / creation
    ThreadGuard guard(m_singletonLock);
    //
    // stop the background task if the flag indicates that the thread has been
    // started
    //

    if (s_instance != NULL)
    {
        if (THREAD_STATE_RUNNING == s_instance->getCurrentState())
        {
            s_instance->terminateAndWait();
        }

        delete s_instance;
        s_instance = NULL;
    }

    LOG(SourceInfo, DebugUtil::FunctionExit, "cleanUp");
}

void SessionManager::run()
{
    using TA_Base_Core::IResource;
    using TA_Base_Core::ResourceAccessFactory;
    bool wasSuccessfulInitialisation = false;

    try
    {
        /////////////////////////////////////////
        // Initialise all the necessary run params
        /////////////////////////////////////////

        RunParams::getInstance().set(TA_Base_App::TA_ControlStation::DEBUG_LOG_DIR.c_str(), m_controlStationEntity->getDebugLogDir().c_str());

        if (RunParams::getInstance().get(RPARAM_DEBUGFILE).empty())
        {
            std::string debugFile = RunParams::getInstance().get(TA_Base_App::TA_ControlStation::DEBUG_LOG_DIR.c_str());

            if (!debugFile.empty())
            {
                if ((debugFile[debugFile.length() - 1] != '/') && (debugFile[ debugFile.length() - 1] != '\\'))
                {
                    debugFile += "\\";
                }

                debugFile += TA_Base_App::TA_ControlStation::LOG_PRE_STRING;
                debugFile += m_controlStationEntity->getName();
                debugFile += TA_Base_App::TA_ControlStation::LOG_POST_STRING;
                DebugUtil::getInstance().setFile(debugFile.c_str());
            }
        }

        if (true == m_controlStationEntity->getUserPreferences())
        {
            RunParams::getInstance().set(RPARAM_USERPREFERENCES, "on");
        }

        RunParams::getInstance().set(ACCESS_CONTROL_POPUP_TIMEOUT.c_str(), boost::lexical_cast<std::string>(m_controlStationEntity->getAccessControlPopupTimeout()).c_str());

        //TD16984 - the runparam string should not be sent if value is empty
        if (!(m_controlStationEntity->getMmsSubmitterType().empty()))
        {
            //TD16491++
            RunParams::getInstance().set(TA_Base_Core::AlarmRuleAccessFactory::RPARAM_MMS_SUBMITTER_TYPE.c_str(), m_controlStationEntity->getMmsSubmitterType().c_str());
            //++TD16491
        }

        /////////////////////////////////////////
        // Initialise member variables
        /////////////////////////////////////////
        m_sessionDetails.initialise(m_hostname);

        m_graphWorxHelp = m_controlStationEntity->getGraphWorxHelpfile();

        try
        {
            // load banners that are child entities of this control station entity
            m_bannerEntities = EntityAccessFactory::getInstance().getChildEntitiesOfEntityOfType(m_controlStationEntity->getName(), TA_Base_App::TA_ControlStation::BANNER_ENTITY_TYPE);
            m_dllHostEntity = EntityAccessFactory::getInstance().getEntity(TA_Base_App::TA_ControlStation::DLLHOST_ENTITY_NAME);
        }
        catch (const TA_Base_Core::DatabaseException& ex)
        {
            LOG_EXCEPTION_CATCH(SourceInfo, "DatabaseException", ex.what());
        }
        catch (const TA_Base_Core::DataException& ex)
        {
            LOG_EXCEPTION_CATCH(SourceInfo, "DataException", ex.what());
        }
        catch (...)
        {
            LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "Unknown");
        }

        // We can now initialise the Screen positioning object since we have a ControlStation object.
        m_screenPositions.loadScreenLayout(*m_controlStationEntity, m_bannerEntities);

        // Start the process manager
        m_processManager = new ProcessManager;

        TA_Base_Core::CorbaUtil::getInstance().setServantKey(m_processManager, "ProcessManager");
        m_processManager->activateServant();

        // Set the list of applications that are to be terminated on exit
        m_processManager->setApplicationsToTerminate(m_controlStationEntity->getApplicationsToTerminate());
        m_processManager->setApplicationsToTerminateOnLogout(m_controlStationEntity->getApplicationsToTerminateOnLogout());
        // get rid of all the zombie external processes first before we proceed.  Need to
        // do this before we start the graphworx processes so the the new processes that
        // we start off won't be killed
        m_processManager->terminateExternalApplications();

        // Start GraphWorx loading as soon as possible as this takes a long time to load
        m_graphWorxManager = new GraphWorxManager(m_controlStationEntity->getBackgroundDisplay(), m_controlStationEntity->getStartupDisplay(), m_screenPositions);

        TA_Base_Core::IResourcePtr resource(TA_Base_Core::ResourceAccessFactory::getInstance().getResourceFromEntity(m_controlStationEntity->getKey()));

        if (resource)
        {
            m_resourceKey = resource->getKey();
            LOG(DEBUG_DEBUG, "IResource key = %d", m_resourceKey);
        }
        else
        {
            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugError, "Cannot find the resource key for control station");
        }

        // This member has to be created after we've set up the messaging stuff
        m_dutyManager = new DutyManager;

        // Initialising authentication library
        m_authenticationMgr = new AuthenticationLibrary;

        m_rightsMgr = TA_Base_Bus::RightsLibraryFactory().buildRightsLibrary();

        m_controlStationImpl = new ControlStationCorbaImpl(m_sessionDetails.getConsoleName());

        TA_Base_Core::IEntityDataPtr systemControllerEntity(EntityAccessFactory::getInstance().getEntity(m_systemControllerName));
        // now subscribe to the system controller online and offline comms messages
        MessageSubscriptionManager::getInstance().subscribeToCommsMessage(
            TA_Base_Core::SystemControllerComms::SystemControllerGroupOnline, this, systemControllerEntity->getKey(), systemControllerEntity->getSubsystem(), systemControllerEntity->getLocation(), NULL);
        MessageSubscriptionManager::getInstance().subscribeToCommsMessage(
            TA_Base_Core::SystemControllerComms::SystemControllerGroupOffline, this, systemControllerEntity->getKey(), systemControllerEntity->getSubsystem(), systemControllerEntity->getLocation(), NULL);
        MessageSubscriptionManager::getInstance().subscribeToCommsMessage(
            TA_Base_Core::ControlStationComms::TAToGXMessageNotification, this, m_sessionDetails.getConsoleId(), 0, atoi((TA_Base_Core::RunParams::getInstance().get(RPARAM_LOCATIONKEY)).c_str()), NULL);
        //lizettejl++ (DP-changes)
        MessageSubscriptionManager::getInstance().subscribeToCommsMessage(
            TA_Base_Core::ControlStationComms::GXToTAMessageNotification, this,  0, 0, atoi((TA_Base_Core::RunParams::getInstance().get(RPARAM_LOCATIONKEY)).c_str()), NULL);
        //++lizettejl

        // Register interest for online updates
        OnlineUpdateListener::getInstance().registerInterest(TA_Base_Core::ENTITY_OWNER, *this, m_controlStationEntity->getKey());
        OnlineUpdateListener::getInstance().registerAllInterests(TA_Base_Core::SECURITY, *this);
        OnlineUpdateListener::getInstance().registerAllInterests(TA_Base_Core::ACTION_GROUP, *this);

        wasSuccessfulInitialisation = true;
    }
    catch (const EntityTypeException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "EntityTypeException", "Converting into a SessionException");
    }
    catch (const GraphWorxException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "GraphWorxException", "Converting into a SessionException");
    }
    catch (const DataException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "DataException", "Converting into a SessionException");
    }
    catch (const DatabaseException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "DatabaseException", "Converting into a SessionException");
    }
    catch (const RightsException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "RightsException", "Converting into a SessionException");
    }
    catch (...)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "Unknown exception", "Converting into a SessionException");
    }

    if (wasSuccessfulInitialisation)
    {
        // Submit an audit with the name of the console on which this application is running
        sendControlStationAuditMessage(TA_Base_Core::ControlStationAudit::ControlStationStart);

        while (!m_rootDialog.hasInitDialog())
        {
            TRACE("sleep\n");
            sleep(2000);
        }

        // if the entity contains login information, then automatically login
        if (m_controlStationEntity->canLoginAutomatically())
        {
            try
            {
                // TD14005
                m_graphWorxManager->waitUntilReady();
                login(m_controlStationEntity->getLoginOperator(),
                      m_controlStationEntity->getLoginProfile(),
                      m_controlStationEntity->getLoginLocation(),
                      CString(m_controlStationEntity->getLoginPassword().c_str()));
            }
            catch (...)
            {
                wasSuccessfulInitialisation = false;
            }
        }
    }

    // Tell root dialog whether we managed to initialise properly
    m_rootDialog.initialisationComplete(wasSuccessfulInitialisation);

    if (wasSuccessfulInitialisation)
    {
        // TD6972 if the group offline run param is set send an update message. The group is online we
        // don't need to send anything
        std::string param = TA_Base_Core::RunParams::getInstance().get(RPARAM_GROUPSOFFLINE);

        if ((!param.empty()) && (param.compare(GROUP_ONLINE) != 0))
        {
            m_sCGroupOnline = false;
            m_rootDialog.PostMessage(WM_SYSTEMCONTROLLER_STATE_CHANGE, (LPARAM)false);
        }
    }

    LOG(SourceInfo, DebugUtil::FunctionExit, "run");
}

void SessionManager::terminate()
{
    // Don't need to do anything here as the run method will always finish.
}

void SessionManager::login(const ProfileLoginDlg& loginDlg)
{
    LOG(SourceInfo, DebugUtil::FunctionEntry, "login");

    TA_Base_Core::IOperator* theOperator = NULL;
    TA_Base_Core::IProfile*  theProfile  = NULL;
    TA_Base_Core::ILocation*  theLocation  = NULL;
    CString password;
    loginDlg.getSelectedDetails(theOperator, theProfile, theLocation, password);

    bool displayOnly = loginDlg.getIsDisplayOnly();

    // if locked out, throw out
    LOG(DEBUG_INFO, "SessionManager::login");
    TA_Base_Core::IOperatorPasswordPtr operatorPassword(TA_Base_Core::OperatorPasswordAccessFactory::getInstance().getOperatorPassword(theOperator->getKey(), false, false));
    int isLockedOut = operatorPassword->getLockedTimes();
    time_t lockedTime = operatorPassword->getLatestLockedTime();
    CTime cLockedTime(lockedTime);
    LOG(DEBUG_INFO, "SessionManager::getLatestLockedTime done %d, %s", isLockedOut, cLockedTime.Format("%Y%m%d %H:%M:%S"));
    CTimeSpan timeSpan = CTime::GetCurrentTime() - cLockedTime;

    if (isLockedOut >= failed_login_attempts && timeSpan.GetTotalMinutes() <= password_lock_time)
    {
        LOG(DEBUG_INFO, "SessionManager::first time login");
        TA_THROW(SessionException(SessionException::USER_LOCKED, "User is locked for failed to login several times"));
    }

    login(theOperator, theProfile, theLocation, password, displayOnly);
    LOG(DEBUG_INFO, "SessionManager::login done");
    LOG(SourceInfo, DebugUtil::FunctionExit, "login");
}

void SessionManager::login(TA_Base_Core::IOperator* theOperator,
                           TA_Base_Core::IProfile* theProfile,
                           TA_Base_Core::ILocation* theLocation,
                           CString& password,
                           bool displayOnly)
{
    LOG(SourceInfo, DebugUtil::FunctionEntry, "login");

    // Try to request for a session
    TA_Base_Core::IOperatorPasswordPtr operatorPassword(TA_Base_Core::OperatorPasswordAccessFactory::getInstance().getOperatorPassword(theOperator->getKey(), false, false));
    time_t latestLoginDate = operatorPassword->getLatestLoginDate();
    time_t latestModifiedDate = operatorPassword->getLatestModifiedDate();

    requestSession(theOperator, theProfile, theLocation, password, displayOnly);

    CTime loginDate(latestLoginDate);
    CTime modifiedDate(latestModifiedDate);
    CTimeSpan modifiedTimeSpan = CTime::GetCurrentTime() - modifiedDate;
    LOG(DEBUG_INFO, "The loginDate is %d ; the modifiedTimespan is :%d", loginDate.GetYear(), modifiedTimeSpan.GetDays() >= password_life_time);

    boolean turnPasswordExpireNotificationOn = true;

    if (TA_Base_Core::RunParams::getInstance().isSet("PasswordExpireNotificationSwitch"))
    {
        std::string passwordEditNotification = TA_Base_Core::RunParams::getInstance().get("PasswordExpireNotificationSwitch");
        LOG(DEBUG_INFO, "The passwordEditNotification status is %s", passwordEditNotification.c_str());

        if (passwordEditNotification.compare("OFF") == 0 || passwordEditNotification.compare("off") == 0 || passwordEditNotification.compare("Off") == 0)
            turnPasswordExpireNotificationOn = false;
    }

    if (turnPasswordExpireNotificationOn)
    {
        if ((loginDate.GetYear() == 1970) || (modifiedTimeSpan.GetDays() >= password_life_time))
        {
            if (loginDate.GetYear() == 1970)
            {
                showMsgBox(IDS_UE_620024, theOperator->getName());
            }
            else if (modifiedTimeSpan.GetDays() >= password_life_time)
            {
                showMsgBox(IDS_UE_620025);
            }

            LOG(DEBUG_INFO, "PopUpChangePasswordDialog");

            bool isSuccessfulChange;

            do
            {
                LOG(DEBUG_INFO, "PopUpChangePasswordDialog => inWhileLoop");
                isSuccessfulChange = PopUpChangePasswordDialog();
            }
            while (!isSuccessfulChange);

            LOG(DEBUG_INFO, "PopUpChangePasswordDialog Done");
        }
        else if ((modifiedTimeSpan.GetDays() < password_life_time) && (modifiedTimeSpan.GetDays() >= (password_life_time - password_warn_time)))
        {
            std::ostringstream outputMessage;
            outputMessage << ((LONGLONG)password_life_time - modifiedTimeSpan.GetDays());
            int resp = showMsgBox(IDS_UE_620026, outputMessage.str());

            if (resp == IDOK)
            {
                LOG(DEBUG_INFO, "PopUpChangePasswordDialog => inWhileLoop");
                PopUpChangePasswordDialog();
            }
        }
    }

    LOG(DEBUG_INFO, "Operator logged in");

    // Indicate that we have logged in so when GraphWorx starts up and calls back we are ready
    // to answer it.
    m_isLoggedIn = true;
    m_rootDialog.PostMessage(WM_LOGIN_DETAILS_CHANGED);

    try
    {
        LOG(DEBUG_INFO, "m_graphWorxManager->launchGraphWorx()");
        m_graphWorxManager->launchGraphWorx();
        LOG(DEBUG_INFO, "m_graphWorxManager->launchGraphWorx() Done");
    }
    catch (const GraphWorxException&)
    {
        // Login has failed so undo everything we've done so far
        m_isLoggedIn = false;
        m_sessionDetails.sessionLogout();
        m_graphWorxManager->shutdownGraphWorx();
        m_processManager->terminateAllApplications();
        m_processManager->terminateExternalApplications();
        throw;
    }

    // TD10353: Load the banner AFTER GraphWorx has been successfully launched.
    //          Otherwise it just gets killed straight away.
    LOG(DEBUG_INFO, "Load Banner()");
    loadBanners();
    LOG(DEBUG_INFO, "Load Banner() Done");
    LOG(DEBUG_INFO, "Load TADllHost()");
    loadDllHost();
    LOG(DEBUG_INFO, "Load TADllHost() Done");
    // Submit an audit with the name of the console on which this application is running
    sendControlStationAuditMessage(TA_Base_Core::ControlStationAudit::ControlStationLogin);
    LOG(SourceInfo, DebugUtil::FunctionExit, "login");
}

void SessionManager::changeProfile(const ProfileLoginDlg& loginDlg)
{
    LOG(SourceInfo, DebugUtil::FunctionEntry, "changeProfile");

    // if the session is no longer valid then just throw an exception to prevent user
    // from continuing on

    if (!m_sessionDetails.isSessionValid())
    {
        TA_THROW(SessionException(SessionException::INVALID_SESSION_CONFIGURATION, "Trying to change profile when the session configuration has become invalid"));
    }

    TA_Base_Core::IOperator* theOperator = NULL;
    TA_Base_Core::IProfile*  theProfile  = NULL;
    TA_Base_Core::ILocation*  theLocation  = NULL;
    CString password;

    loginDlg.getSelectedDetails(theOperator, theProfile, theLocation, password);

    try
    {
        if (theProfile == NULL)
        {
            TA_THROW(SessionException("Profile retrieved from the login dialogue was NULL. Cannot continue to log the user in"));
        }

        // the operator should still be same one
        if (theOperator->getName() != m_sessionDetails.getOperatorName())
        {
            TA_THROW(SessionException("Attempting to change the profile of operator who is not logged in"));
        }

        unsigned int locationKey = 0;

        // if we're offline, we'll always just use the current location
        if (!m_sCGroupOnline)
        {
            locationKey = getSessionDetails().getServerLocation();
            TA_Base_Core::ILocationPtr location(TA_Base_Core::LocationAccessFactory::getInstance().getLocationByKey(locationKey));
        }
        else if (theLocation != NULL)
        {
            locationKey = theLocation->getKey();
        }

        bool displayOnly = loginDlg.getIsDisplayOnly();

        // we get the new session id when we change profile
        std::string sessionId = m_authenticationMgr->changeProfile(
                                    m_sessionDetails.getSessionId(),
                                    theProfile->getKey(),
                                    locationKey,
                                    std::string(password),
                                    displayOnly);
        m_sessionDetails.profileChanged(sessionId, theProfile, theLocation);
        m_sessionDetails.setDisplayMode(displayOnly);
        // Tell all running applications that the session Id has changed
        m_processManager->updateSessionId(sessionId);

        // Submit an audit message for the new profile
        sendControlStationAuditMessage(TA_Base_Core::ControlStationAudit::ControlStationChangeProfile);
    }
    catch (const AuthenticationSecurityException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "AuthenticationSecurityException", "Converting into a SessionException");
        TA_THROW(SessionException(SessionException::AUTHENTICATION_FAILED, "Failed to authenticate to change profile"));
    }
    catch (const AuthenticationAgentException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "AuthenticationAgentException", "Converting into a SessionException");
        TA_THROW(SessionException(SessionException::GENERAL_ERROR, "Authentication Agent failed when trying to change profile"));
    }
    catch (const SessionException&)
    {
        throw;
    }
    // TD14617
    catch (const TransactiveException&)
    {
        CString actionName;
        TA_VERIFY(actionName.LoadString(IDS_CHANGE_PROFILE) != 0, "Unable to load IDS_CHANGE_PROFILE");
        showMsgBox(IDS_UE_010018, actionName);
        // TransactiveException need to log operator off cos we don't have a valid session.
        m_rootDialog.PostMessage(WM_FORCE_LOGOUT);
    }
    catch (...) // whatever exception that we got, just make it SessionException
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "Unknown Exception", "An unknown exception is caught during login authentication.  Converting into a SessionException");
        TA_THROW(SessionException(SessionException::GENERAL_ERROR, "Unknown error when attempting to login"));
    }

    LOG(SourceInfo, DebugUtil::FunctionExit, "changeProfile");
}

void SessionManager::changeOperator(const ProfileLoginDlg& loginDlg)
{
    LOG(SourceInfo, DebugUtil::FunctionEntry, "changeOperator");

    // if the session is no longer valid then just throw an exception to prevent user
    // from continuing on
    if (!m_sessionDetails.isSessionValid())
    {
        TA_THROW(SessionException(SessionException::INVALID_SESSION_CONFIGURATION, "Trying to change operator when the session configuration has become invalid"));
    }

    TA_Base_Core::IOperator* theOperator = NULL;
    TA_Base_Core::IProfile*  theProfile  = NULL;
    TA_Base_Core::ILocation*  theLocation  = NULL;
    CString password;

    loginDlg.getSelectedDetails(theOperator, theProfile, theLocation, password);

    try
    {
        if ((theOperator == NULL) || (theProfile == NULL))
        {
            TA_THROW(SessionException("Operator or profile retrieved from the login dialogue was NULL. Cannot continue to log the user in"));
        }

        // the operator shouldn't be same one
        if (theOperator->getName() == m_sessionDetails.getOperatorName())
        {
            TA_THROW(SessionException("Attempting to change into the same operator"));
        }

        unsigned int locationKey = 0;

        // if we're offline, we'll always just use the current location
        if (!m_sCGroupOnline)
        {
            locationKey = getSessionDetails().getConsoleId();
        }
        else if (theLocation != NULL)
        {
            locationKey = theLocation->getKey();
        }

        std::string oldSessionId = m_sessionDetails.getSessionId();

        // And an audit message for the logout
        sendControlStationAuditMessage(TA_Base_Core::ControlStationAudit::ControlStationLogout);

        // Try to request for a new session
        requestSession(theOperator, theProfile, theLocation, password);

        // Submit an audit message for the new login
        sendControlStationAuditMessage(TA_Base_Core::ControlStationAudit::ControlStationLogin);

        // logout the old session
        try
        {
            // make sure we remove the operator override before logging off
            if (m_sessionDetails.isOperatorOverridden())
            {
                m_authenticationMgr->endOverride(oldSessionId);
            }

            m_authenticationMgr->endSession(oldSessionId);
        }
        catch (...)
        {
            // If the session manager somehow couldn't logout, just ignore
            // we already got the new session
        }
    }
    catch (const AuthenticationSecurityException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "AuthenticationSecurityException", "Converting into a SessionException");
        TA_THROW(SessionException(SessionException::AUTHENTICATION_FAILED, "Failed to authenticate to change operator"));
    }
    catch (const AuthenticationAgentException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "AuthenticationAgentException", "Converting into a SessionException");
        TA_THROW(SessionException(SessionException::GENERAL_ERROR, "Authentication Agent failed when trying to change operator"));
    }
    catch (const SessionException&)
    {
        throw;
    }
    catch (...) // whatever exception that we got, just make it SessionException
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "Unknown Exception", "An unknown exception is caught during login authentication.  Converting into a SessionException");
        TA_THROW(SessionException(SessionException::GENERAL_ERROR, "Unknown error when attempting to login"));
    }

    LOG(SourceInfo, DebugUtil::FunctionExit, "changeOperator");
}

void SessionManager::operatorOverride(const ProfileLoginDlg& loginDlg)
{
    LOG(SourceInfo, DebugUtil::FunctionEntry, "operatorOverride");

    // if the session is no longer valid then just throw an exception to prevent user
    // from continuing on
    if (!m_sessionDetails.isSessionValid())
    {
        TA_THROW(SessionException(SessionException::INVALID_SESSION_CONFIGURATION, "Trying to override when the session configuration has become invalid"));
    }

    TA_Base_Core::IOperator* theOperator = NULL;
    TA_Base_Core::IProfile*  theProfile  = NULL;
    TA_Base_Core::ILocation*  theLocation  = NULL;
    CString password;

    loginDlg.getSelectedDetails(theOperator, theProfile, theLocation, password);

    try
    {
        if ((theOperator == NULL) || (theProfile == NULL))
        {
            TA_THROW(SessionException("Operator or profile retrieved from the login dialogue was NULL. Cannot continue to log the user in"));
        }

        std::string locationName = "";
        unsigned int locationKey = 0;

        try
        {
            // if we're offline, we'll always just use the current location
            if (!m_sCGroupOnline)
            {
                locationName = getSessionDetails().getConsoleName();
                locationKey = getSessionDetails().getConsoleId();
            }
            else if (theLocation != NULL)
            {
                locationName = theLocation->getDisplayName();
                locationKey = theLocation->getKey();
            }
        }
        catch (const DataException&)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "DataException", "Error occurred while attempting to retrieve profile, console or operator name for audit message");
            return;
        }
        catch (const DatabaseException&)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "DatabaseException", "Error occurred while attempting to retrieve profile, console or operator name for audit message");
            return;
        }

        // Check if the two profiles are the same AND
        // The location names are the same
        if ((m_sessionDetails.getProfileName() == theProfile->getName()) &&
                (m_sessionDetails.getLocationDisplayName() == locationName)
           )
        {
            LOG(DEBUG_INFO, "The Details selected for override are the same as the current details. Ignoring.");
            LOG(SourceInfo, DebugUtil::FunctionExit, "operatorOverride");
            return;
        }

        m_authenticationMgr->beginOverride(m_sessionDetails.getSessionId(), theOperator->getKey(), theProfile->getKey(), locationKey, std::string(password));
        m_sessionDetails.operatorOverride(theOperator, theProfile, theLocation);

        // yanrong: reset rights cache, rights library will call rights agent to check permission.
        m_rightsMgr->resetCache();

        // Tell all running applications that the session Id has changed
        m_processManager->updateSessionId(m_sessionDetails.getSessionId());
    }
    catch (const AuthenticationSecurityException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "AuthenticationSecurityException", "Converting into a SessionException");
        TA_THROW(SessionException(SessionException::AUTHENTICATION_FAILED, "Failed to authenticate for operator override"));
    }
    catch (const AuthenticationAgentException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "AuthenticationAgentException", "Converting into a SessionException");
        TA_THROW(SessionException(SessionException::GENERAL_ERROR, "Authentication Agent failed when trying to do operator override"));
    }
    catch (const SessionException&)
    {
        throw;
    }
    catch (...) // whatever exception that we got, just make it SessionException
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "Unknown Exception", "An unknown exception is caught during login authentication.  Converting into a SessionException");
        TA_THROW(SessionException(SessionException::GENERAL_ERROR, "Unknown error when attempting to do operator override"));
    }

    LOG(SourceInfo, DebugUtil::FunctionExit, "operatorOverride");
}

void SessionManager::changePassword(const CPasswordDlg& passwordDlg)
{
    LOG(SourceInfo, DebugUtil::FunctionEntry, "changePassword");

    TA_Base_Core::IOperator* theOperator = NULL;
    CString oldPassword, newPassword1, newPassword2;

    passwordDlg.getSelectedDetails(theOperator, oldPassword, newPassword1, newPassword2);

    // if the session is no longer valid then just throw an exception to prevent user
    // from continuing on
    if (!m_sessionDetails.isSessionValid())
    {
        TA_THROW(SessionException(SessionException::INVALID_SESSION_CONFIGURATION, "Trying to change password when the session configuration has become invalid"));
    }

    try
    {
        // if both new passwords are not empty and the same
        if ((newPassword1 != "") && (newPassword1 == newPassword2))
        {
            // if new passwords are shorter than what's allowed
            if ((newPassword1.GetLength() < MIN_PASSWORD_LENGTH) || (newPassword1.GetLength() > MAX_PASSWORD_LENGTH))
            {
                TA_THROW(SessionException(SessionException::INVALID_PASSWORD_LENGTH, "Password is shorter than minimum length"));
            }

            m_authenticationMgr->changePassword(m_sessionDetails.getSessionId(), std::string(oldPassword), std::string(newPassword1));
        }
        else  // new passwords are different
        {
            TA_THROW(SessionException(SessionException::PASSWORD_MISMATCH, "New passwords are different"));
        }
    }
    catch (const SessionException&)
    {
        throw;
    }
    catch (const AuthenticationSecurityException& ex)
    {
        if (ex.getReason() == AuthenticationSecurityException::INVALID_PASSWORD_LENGTH)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "AuthenticationSecurityException::INVALID_PASSWORD", "Converting into a SessionException::CHANGE_PASSWORD_FAILED");
            // We shouldn't really get this since we've checked the length of the password in CPasswordDlg, but
            // if for some reason it wasn't caught there,
            TA_THROW(SessionException(SessionException::INVALID_PASSWORD_LENGTH, "Failed to change password"));
        }
        else if (ex.getReason() == AuthenticationSecurityException::PASSWORD_USEDBEFORE)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "AuthenticationSecurityException::PASSWORD_USEDBEFORE", "Converting into a SessionException::PASSWORD_USEDBEFORE");
            // We shouldn't really get this since we've checked the length of the password in CPasswordDlg, but
            // if for some reason it wasn't caught there,
            TA_THROW(SessionException(SessionException::PASSWORD_USEDBEFORE, "Password has been used before"));
        }
        else  // Any other exceptions, just throw it as a general AUTHENTICATION_FAILED exception
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "AuthenticationSecurityException", "Converting into a SessionException");
            TA_THROW(SessionException(SessionException::AUTHENTICATION_FAILED, "Failed to authenticate to change password"));
        }
    }
    catch (const AuthenticationAgentException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "AuthenticationAgentException", "Converting into a SessionException");
        TA_THROW(SessionException(SessionException::GENERAL_ERROR, "Authentication Agent failed when trying to change password"));
    }
    catch (...) // whatever exception that we got, just make it SessionException
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "Unknown Exception", "Some unknown exception has occurred while attempting to change password. Converting into a SessionException");
        TA_THROW(SessionException(SessionException::GENERAL_ERROR, "Unknown error encountered when attempting to change password"));
    }

    LOG(SourceInfo, DebugUtil::FunctionExit, "changePassword");
}

void SessionManager::removeOperatorOverride()
{
    LOG(SourceInfo, DebugUtil::FunctionEntry, "removeOperatorOverride");

    try
    {
        m_authenticationMgr->endOverride(m_sessionDetails.getSessionId());

        // yanrong: reset rights cache, rights library will call rights agent to check permission.
        m_rightsMgr->resetCache();

        // Tell all running applications that the session Id has changed
        m_processManager->updateSessionId(m_sessionDetails.getSessionId());
    }
    catch (...)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "Unknown exception", "Some unknown exception has occurred while attempting to remove operator override");
    }

    m_sessionDetails.removeOperatorOverride();

    LOG(SourceInfo, DebugUtil::FunctionExit, "removeOperatorOverride");
}

void SessionManager::logout()
{
    LOG(SourceInfo, DebugUtil::FunctionEntry, "logout");

    TA_ASSERT(m_isLoggedIn, "Trying to log out though not logged in");

    // TD9195 GT: Moved these above the sessionLogout call so that the details passed
    //         into the audit message are valid.
    // Submit an audit with the name of the console on which this application is running
    sendControlStationAuditMessage(TA_Base_Core::ControlStationAudit::ControlStationLogout);

    try
    {
        // make sure we remove the operator override before logging off
        if (m_sessionDetails.isOperatorOverridden())
        {
            m_authenticationMgr->endOverride(m_sessionDetails.getSessionId());
        }

        m_authenticationMgr->endSession(m_sessionDetails.getSessionId());
    }
    catch (...)
    {
        // If the session manager somehow couldn't logout, we still want to
        // continue with the rest of the logout process
    }

    m_isLoggedIn = false;

    m_sessionDetails.sessionLogout();

    // Shutdown GraphWorx if it is currently running.
    m_graphWorxManager->shutdownGraphWorx();

    m_rootDialog.SendMessage(WM_HIDE_GRAPHWORX);

    // Tell the processManager to terminate all running applications.
    if (m_processManager != NULL)
    {
        m_processManager->terminateAllApplications();
    }

    LOG(SourceInfo, DebugUtil::FunctionExit, "logout");
}

EPermission SessionManager::isShutdownPermitted(const ProfileLoginDlg& loginDlg)
{
    bool isExit = (loginDlg.getTerminateCode() == TA_Base_App::TA_ControlStation::EXIT);

    // if it is not access controlled, then it'll always be permitted to do this
    if ((isExit && !isExitAccessControlled()) || (!isExit && !isRestartAccessControlled()))
    {
        return TA_Base_App::TA_ControlStation::PERMITTED;
    }

    TA_Base_Core::IOperator* theOperator = NULL;
    TA_Base_Core::IProfile*  theProfile  = NULL;
    TA_Base_Core::ILocation*  theLocation  = NULL;
    CString password;

    loginDlg.getSelectedDetails(theOperator, theProfile, theLocation, password);
    requestSession(theOperator, theProfile, theLocation, password);

    unsigned int action = isExit ? TA_Base_Bus::aca_CONTROL_STATION_EXIT : TA_Base_Bus::aca_CONTROL_STATION_RESTART;

    // login details are correct, so see if operator has rights to restart
    return isControlPermitted(action);
}

IProcessManager* SessionManager::getProcessManager()
{
    if (!m_isLoggedIn)
    {
        return NULL;
    }

    return m_processManager;
}

IGraphWorxManager* SessionManager::getGraphWorxManager()
{
    return m_graphWorxManager;
}

void SessionManager::locateSystemController()
{
    using TA_Base_Core::RunParams;
    using TA_Base_Core::CorbaUtil;

    LOG(SourceInfo, DebugUtil::FunctionEntry, "locateSystemController");

    std::string hosts = RunParams::getInstance().get(TA_Base_App::TA_ControlStation::HOST_NAMES.c_str());

    //
    // Convert the portnumber into an unsigned long
    //
    unsigned long portNumber = TA_Base_Core::DEFAULT_MGR_PORT;

    std::string portNumberString = RunParams::getInstance().get(TA_Base_App::TA_ControlStation::PORT_NUMBER.c_str()).c_str();

    if (!portNumberString.empty())
    {
        if (-1 == portNumberString.find_first_not_of("0123456789"))
        {
            //all valid characters
            portNumber = atol(portNumberString.c_str());

            if (0 == portNumber)
            {
                portNumber = TA_Base_Core::DEFAULT_MGR_PORT;
            }
        }
    }

    // Generate a list of hostnames from the command line argument
    std::vector<std::string> hostNames;
    int position = hosts.find_first_of(",", 0);

    while (position != -1)
    {
        hostNames.push_back(hosts.substr(0, position));
        position = hosts.find_first_of(",", position + 1);
    }

    // Get the last name that is not followed by a comma
    position = hosts.find_last_of(",");
    hostNames.push_back(hosts.substr(position + 1, hosts.length() - (position + 1)));

    bool haveFoundSystemController = false;
    std::vector<std::string>::iterator iter = hostNames.begin();

    while ((iter != hostNames.end()) && (!haveFoundSystemController))
    {
        CString procManString;
        procManString.Format("corbaloc::1.2@%s:%lu/ProcessManager", iter->c_str(), portNumber);

        try
        {
            // Attempt to get Process Manager
            CORBA::Object_var obj = CorbaUtil::getInstance().stringToObject(procManString.GetBuffer(1));

            if (!CORBA::is_nil(obj))
            {
                m_systemController = TA_Base_Core::IProcessManagerCorbaDef::_narrow(obj);

                if (!CORBA::is_nil(m_systemController))
                {
                    haveFoundSystemController = true;
                    LOG(DEBUG_INFO, "Found system controller, %s", iter->c_str());
                    m_systemControllerName = *iter;

                    // TD 6950: Set the system controller name as a run param so it can be retrieved by
                    // applications that require subscribing to System Controller messages to be notified
                    // when the system is in degraded mode or recovered from degraded mode.
                    RunParams::getInstance().set(RPARAM_SYSTEMCONTROLLERNAME, m_systemControllerName.c_str());
                }
            }
        }
        catch (...)
        {
            // Nothing to do as we just move onto the next host
        }

        ++iter;
    }

    if (!haveFoundSystemController)
    {
        std::string errorMsg = "No system controller was located on any of the hosts specifed: ";
        errorMsg += hosts;
        TA_THROW(SessionException(errorMsg.c_str()));
    }

    LOG(SourceInfo, DebugUtil::FunctionExit, "locateSystemController");
}

void SessionManager::retrieveRunParams(const std::string& hostName)
{
    using TA_Base_Core::RunParams;

    LOG(SourceInfo, DebugUtil::FunctionEntry, "retrieveRunParams");

    try
    {
        TA_Base_Core::RunParamSeq_var tmpParams = m_systemController->getParams(0, hostName.c_str());

        // For each RunParam we have retrieved. If it is not already set then
        // set it.
        for (unsigned long i = 0; i < tmpParams->length(); ++i)
        {
            if (!RunParams::getInstance().isSet(tmpParams[i].name))
            {
                RunParams::getInstance().set(tmpParams[i].name, tmpParams[i].value);
            }
        }
    }
    catch (const CORBA::Exception& ex)
    {
        LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, TA_Base_Core::CorbaUtil::getInstance().exceptionToString(ex).c_str(), "");
        TA_THROW(SessionException("Could not retrieve a list of RunParams from the System Controller. Since we won't have most of the information we need then there is no point continuing"));
    }
    catch (...)
    {
        LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "Unknown", "Caught some unknown exception when trying to retrive run params");
        TA_THROW(SessionException("Could not retrieve a list of RunParams from the System Controller. Since we won't have most of the information we need then there is no point continuing"));
    }

    LOG(SourceInfo, DebugUtil::FunctionExit, "retrieveRunParams");
}

void SessionManager::loadBanners()
{
    LOG(SourceInfo, DebugUtil::FunctionEntry, "loadBanners");
    LOG(DEBUG_INFO, "loadBanners()");

    try
    {
        // Get a debug path we can use to create a log file for each banner
        std::string debugFilePath = TA_Base_Core::RunParams::getInstance().get(TA_Base_App::TA_ControlStation::DEBUG_LOG_DIR.c_str());

        if (!debugFilePath.empty())
        {
            if ((debugFilePath[debugFilePath.length() - 1] != '/') &&
                    (debugFilePath[ debugFilePath.length() - 1] != '\\'))
            {
                debugFilePath += "\\";
            }
        }

        try
        {
            unsigned int numBanners = m_bannerEntities.size();
            std::vector<bool> bannerVisibility = getScreenPositioning().getBannerVisibility();

            for (unsigned int i = 0; i < numBanners; i++)
            {
                if (bannerVisibility[i] == false)
                {
                    continue;
                }

                OnlineUpdateListener::getInstance().registerInterest(TA_Base_Core::ENTITY_OWNER, *this, m_bannerEntities[i]->getKey());
                std::ostringstream commandLine;
                commandLine << "--entity-name=";
                commandLine << m_bannerEntities[i]->getName();

                RECT boundaryRect = getScreenPositioning().getRect(TA_Base_Bus::TA_GenericGui::SCREEN_SPECIFIC, TA_Base_Bus::TA_GenericGui::AREA_BANNER, i + 1);

                m_processManager->launchApplication(
                    BANNER_GUI_APPTYPE,
                    commandLine.str(),
                    TA_Base_Bus::TA_GenericGui::POS_BOUNDED,
                    TA_Base_Bus::TA_GenericGui::ALIGN_FIT,
                    TA_Base_Bus::TA_GenericGui::DEFAULT_RECT.toRect<RECT>(),
                    boundaryRect,
                    false);  // false indicates this application is not available as a running application
            }
        }
        catch (const DataException&)
        {
        }
        catch (const DatabaseException&)
        {
        }
    }
    catch (ApplicationException&)
    {
        // TD1484, TD2207 - display error message when it couldn't launch one or more banners
        CString banner;
        TA_VERIFY(banner.LoadString(IDS_BANNER) != 0, "Unable to load IDS_DISPLAY");
        UINT selectedButton = showMsgBox(IDS_UE_010002, banner);
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "ApplicationException", "Could not launch one or more of the banners");
    }

    LOG(DEBUG_INFO, "loadBanners() leaving...");
    LOG(SourceInfo, DebugUtil::FunctionExit, "loadBanners");
}

void SessionManager::loadDllHost()
{
    FUNCTION_ENTRY("loadDllHost");

    if (NULL == m_dllHostEntity)
    {
        FUNCTION_EXIT;
        return;
    }

    try
    {
        try
        {
            std::ostringstream commandLine;
            commandLine << " --" << RPARAM_CORBA_NAMING_PORT << "=" << m_dllHostEntity->getAddress();

            m_processManager->launchApplication(
                DLL_HOST_GUI_APPTYPE,
                commandLine.str(),
                TA_Base_Bus::TA_GenericGui::POS_BOUNDED,
                TA_Base_Bus::TA_GenericGui::ALIGN_FIT,
                TA_Base_Bus::TA_GenericGui::DEFAULT_RECT.toRect<RECT>(),
                TA_Base_Bus::TA_GenericGui::DEFAULT_RECT.toRect<RECT>(),
                false);  // false indicates this application is not available as a running application
        }
        catch (const DataException&)
        {
        }
        catch (const DatabaseException&)
        {
        }
    }
    catch (ApplicationException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "ApplicationException", "Could not launch DllHost");
    }

    FUNCTION_EXIT;
}

void SessionManager::requestSession(TA_Base_Core::IOperator*& theOperator,
                                    TA_Base_Core::IProfile*& theProfile,
                                    TA_Base_Core::ILocation*& theLocation,
                                    CString& password,
                                    bool displayOnly)
{
    LOG(SourceInfo, DebugUtil::FunctionEntry, "requestSession");

    try
    {
        if ((theOperator == NULL) || (theProfile == NULL))
        {
            TA_THROW(SessionException("Operator or profile retrieved from the login dialogue was NULL. Cannot continue to log the user in"));
        }

        unsigned int locationKey = 0;

        // if we're offline, we'll always just use the current location
        if (!m_sCGroupOnline)
        {
            // TD10804: Use the key for the consoles location, not the key for the console itself.
            locationKey = m_sessionDetails.getConsoleLocationId();
            TA_Base_Core::ILocationPtr location(TA_Base_Core::LocationAccessFactory::getInstance().getLocationByKey(locationKey));
        }
        else if (theLocation != NULL)
        {
            locationKey = theLocation->getKey();
        }

        LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugNorm,
            "Attempting to request a session with OperatorKey = %d, ProfileKey = %d, LocationKey = %d, ConsoleKey = %d",
            theOperator->getKey(),
            theProfile->getKey(),
            locationKey,
            m_sessionDetails.getConsoleId());

        std::string sessionId = m_authenticationMgr->requestSession(
                                    theOperator->getKey(),
                                    theProfile->getKey(),
                                    locationKey,
                                    m_sessionDetails.getConsoleId(),
                                    std::string(password),
                                    displayOnly);

        m_sessionDetails.sessionLogin(sessionId, theOperator, theProfile, theLocation);
        m_sessionDetails.setDisplayMode(displayOnly);
        // Tell all running applications that the session Id has changed
        m_processManager->updateSessionId(sessionId);
    }
    catch (const AuthenticationSecurityException&)
    {
        TA_THROW(SessionException(SessionException::AUTHENTICATION_FAILED, "Fail to request a session"));
    }
    catch (const AuthenticationAgentException&)
    {
        TA_THROW(SessionException(SessionException::GENERAL_ERROR, "Authentication agent throws an exception when requesting for a session"));
    }
    catch (TA_Base_Core::ObjectResolutionException& ex)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "ObjectResolutionException", ex.what());
        TA_THROW(SessionException(SessionException::GENERAL_ERROR, "Unknown login authentication error"));
    }
    catch (const SessionException&)
    {
        throw;
    }
    catch (...) // whatever exception that we got, just make it SessionException
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "Unknown Exception", "An unknown exception is caught during login authentication");
        TA_THROW(SessionException(SessionException::GENERAL_ERROR, "Unknown login authentication error"));
    }

    LOG(SourceInfo, DebugUtil::FunctionExit, "requestSession");
}

EPermission SessionManager::isActionPermitted(unsigned long action, unsigned long resourceKey)
{
    EPermission permission;

    try
    {
        TA_Base_Bus::EDecisionModule decisionModule = TA_Base_Bus::UNDEFINED_DM;
        std::string reason;

        bool allowed = m_rightsMgr->isActionPermittedOnResource(m_sessionDetails.getSessionId(), resourceKey, action, reason, decisionModule);

        if (allowed)
        {
            permission = TA_Base_App::TA_ControlStation::PERMITTED;
        }
        else
        {
            permission = TA_Base_App::TA_ControlStation::DENIED;
            LOG(DEBUG_INFO, "Not permitted to perform action=%d on control station as %s", action, reason.c_str());
        }
    }
    catch (const RightsException&)
    {
        // The library throws this exception when the right is either
        // indeterminate or not applicable
        permission = TA_Base_App::TA_ControlStation::UNKNOWN;
    }
    catch (...)
    {
        // The library does not catch any exception, so any exception
        // other than the RightsException is error
        permission = TA_Base_App::TA_ControlStation::GENERAL_ERROR;
    }

    return permission;
}

EPermission SessionManager::isControlPermitted(unsigned long action)
{
    TA_ASSERT(m_resourceKey != -1, "Resource key is not set");
    // adding some retry in case of corba failures.
    // set a runparam to set retry.
    EPermission isPermitted = isActionPermitted(action, m_resourceKey);

    if ((isPermitted != TA_Base_App::TA_ControlStation::PERMITTED) && (isPermitted != TA_Base_App::TA_ControlStation::DENIED))
    {
        // retry to check if it's due to croba timeout or agent temporarily not available
        unsigned int retries = m_controlStationEntity->getCorbaRetries();

        for (unsigned int i = 0; i < retries; i++)
        {
            LOG(DEBUG_ERROR, "could not determine operator rights with sessionId=%s. retrying %d times", m_sessionDetails.getSessionId().c_str(), i + 1);
            isPermitted = isActionPermitted(action, m_resourceKey);

            if ((isPermitted != TA_Base_App::TA_ControlStation::PERMITTED) && (isPermitted != TA_Base_App::TA_ControlStation::DENIED))
            {
                // Give some sleep. But how long should we wait for retry?
                Thread::sleep(1000);
            }
            else
            {
                break;
            }
        }
    }

    return isPermitted;
}

void SessionManager::processUpdate(const TA_Base_Core::ConfigUpdateDetails& updateEvent)
{
    LOG(SourceInfo, DebugUtil::FunctionEntry, "processUpdate");

    switch (updateEvent.getType())
    {
    case TA_Base_Core::SECURITY:
    case TA_Base_Core::ACTION_GROUP:
        m_rightsMgr->resetCache();
        break;

    case TA_Base_Core::ENTITY_OWNER:
    {
        if (updateEvent.getKey() != m_controlStationEntity->getKey())
        {
            LOG(DEBUG_INFO, "This is not control station update, ignore");
            LOG(SourceInfo, DebugUtil::FunctionExit, "processUpdate");
            return;
        }

        if (updateEvent.getModifications() == TA_Base_Core::Create)
        {
            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugWarn, "Got 'Create' update, ignore");
            LOG(SourceInfo, DebugUtil::FunctionExit, "processUpdate");
            return;
        }

        if (updateEvent.getModifications() == TA_Base_Core::Delete)
        {
            LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugWarn, "Received a 'Delete' update the control station");
            LOG(SourceInfo, DebugUtil::FunctionExit, "processUpdate");
            UINT selectedButton = showMsgBox(IDS_UW_020003);
            LOG(SourceInfo, DebugUtil::FunctionExit, "processUpdate");
            return;
        }

        const std::vector< std::string> changes = updateEvent.getChangedParams();

        if (changes.empty())
        {
            LOG(DEBUG_INFO, "Got no update changes, ignore");
            LOG(SourceInfo, DebugUtil::FunctionExit, "processUpdate");
            return;
        }

        // update our control station database entity
        m_controlStationEntity->invalidate();

        using TA_Base_App::TA_ControlStation::DEBUG_LOG_DIR;
        using TA_Base_App::TA_ControlStation::USER_PREFERENCES;

        for (unsigned int i = 0; i < changes.size(); i++)
        {
            if (DEBUG_LOG_DIR.compare(changes[i]) == STRINGS_EQUIVALENT)
            {
                RunParams::getInstance().set(DEBUG_LOG_DIR.c_str(), m_controlStationEntity->getDebugLogDir().c_str());
                LOG(DEBUG_INFO, "DEBUG_LOG_DIR has been changed to %s", m_controlStationEntity->getDebugLogDir().c_str());
            }
            else if (USER_PREFERENCES.compare(changes[i]) == STRINGS_EQUIVALENT)
            {
                if (true == m_controlStationEntity->getUserPreferences())
                {
                    LOG(DEBUG_INFO, "USER_PREFERENCES has been changed to on");
                    RunParams::getInstance().set(RPARAM_USERPREFERENCES, "on");
                }
                else
                {
                    LOG(DEBUG_INFO, "RPARAM_USERPREFERENCES has been changed to off");
                    RunParams::getInstance().set(RPARAM_USERPREFERENCES, "");
                }
            }
            else if (ACCESS_CONTROL_POPUP_TIMEOUT.compare(changes[i]) == STRINGS_EQUIVALENT)
            {
                RunParams::getInstance().set(ACCESS_CONTROL_POPUP_TIMEOUT.c_str(), boost::lexical_cast<std::string>(m_controlStationEntity->getAccessControlPopupTimeout()).c_str());
                LOG(DEBUG_INFO, "AccessControlPopupTimeout has been changed to %d", m_controlStationEntity->getAccessControlPopupTimeout());
            }
        }
    }
    break;
    }

    // Make the control station update its displays to reflect any changes.
    m_rootDialog.PostMessage(WM_LOGIN_DETAILS_CHANGED);

    //Haipeng Jiang added the codes below to update the current CAD keys status
    WinkeysMgr::instance()->onRuntimeParams();

    LOG(SourceInfo, DebugUtil::FunctionExit, "processUpdate");
}

bool SessionManager::isRestartAccessControlled() const
{
    return m_controlStationEntity->isRestartAccessControlled();
}

bool SessionManager::isExitAccessControlled() const
{
    return m_controlStationEntity->isExitAccessControlled();
}

void SessionManager::receiveSpecialisedMessage(const TA_Base_Core::CommsMessageCorbaDef& message)
{
    try
    {
        //
        // messages from the system controller
        //
        std::string mesgTypeKey = message.messageTypeKey;

        // if we got an online message and we're already know that, then just ignore, otherwise notify
        if (0 == mesgTypeKey.compare(TA_Base_Core::SystemControllerComms::SystemControllerGroupOnline.getTypeKey()))
        {
            if (!m_sCGroupOnline)
            {
                char const* groupString = "";

                if ((message.messageState >>= groupString) == 0)
                {
                    LOG(DEBUG_ERROR, "Could not retrieve the group string for SystemControllerGroupOnline message");
                    groupString = "Unknown";
                }

                m_sCGroupOnline = true;
                LOG(DEBUG_INFO, "Received a SystemControllerGroupOnline message");
                // tell the control station root dialog to display notification
                // update the session id of all
                std::string* group = new std::string(groupString);
                m_rootDialog.PostMessage(WM_SYSTEMCONTROLLER_STATE_CHANGE, (WPARAM)true, (LPARAM)group);
            }
        }

        // if we got an offline message and we're already know that, then just ignore, otherwise notify
        if (0 == mesgTypeKey.compare(TA_Base_Core::SystemControllerComms::SystemControllerGroupOffline.getTypeKey()))
        {
            if (m_sCGroupOnline)
            {
                char const* groupString = "";

                if ((message.messageState >>= groupString) == 0)
                {
                    LOG(DEBUG_ERROR, "Could not retrieve the group string for SystemControllerGroupOffline message");
                    groupString = "Unknown";
                }

                m_sCGroupOnline = false;
                LOG(DEBUG_INFO, "Received a SystemControllerGroupOffline message");
                // tell the control station root dialog to display notification
                // update the session id of all
                std::string* group = new std::string(groupString);
                m_rootDialog.PostMessage(WM_SYSTEMCONTROLLER_STATE_CHANGE, (WPARAM)false, (LPARAM)group);
            }
        }

        // GX targeted message
        if (0 == mesgTypeKey.compare(TA_Base_Core::ControlStationComms::TAToGXMessageNotification.getTypeKey()))
        {
            processTAToGxMessage(message);
        }

        //lizettejl++ (DP-changes)
        // TA targeted message
        if (0 == mesgTypeKey.compare(TA_Base_Core::ControlStationComms::GXToTAMessageNotification.getTypeKey()))
        {
            processGxToTAMessage(message);
        }

        //++lizettejl

        // Added for TD 6950: Some GUIs need to know when the system is in degraded mode.
        // Refresh the run params as the group online value would have changed
        retrieveRunParams(m_hostname);
    }
    catch (...)
    {
        LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "Caught some error when processing specialised message");
    }
}

void SessionManager::processTAToGxMessage(const TA_Base_Core::CommsMessageCorbaDef& message)
{
    TA_Base_Core::IControlStationCorbaDef::TAToGXMessage* data;

    // can't extract to TAToGXMessage, ignore
    if ((message.messageState >>= data) == 0)
    {
        LOG(DEBUG_DEBUG, "Received a TAToGXMessageNotification but couldn't extract TAToGXMessage.  Ignore");
        return;
    }

    LOG(DEBUG_INFO, "Received a TAToGXMessageNotification message");
    // Tell the control station root dialog to fire the message to Graphworx
    TAToGxMessageInformation* pGxMsg = new TAToGxMessageInformation();
    pGxMsg->targetSessionID = data->targetSessionID;
    pGxMsg->targetSchematicID = data->targetSchematicID;
    pGxMsg->sourceEntityName = data->sourceEntityName;
    pGxMsg->message = data->message;
    m_rootDialog.PostMessage(WM_TA_TO_GX_MESSAGE, (WPARAM)pGxMsg);
}

void SessionManager::processGxToTAMessage(const TA_Base_Core::CommsMessageCorbaDef& message)
{
    try
    {
        LOG(DEBUG_INFO, "Received a GXToTAMessageNotification message");
        TA_Base_Core::IManagedApplicationCorbaDef::GXToTAMessage* event;

        if ((message.messageState >>= event) == 0)
        {
            LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog,  TA_Base_Core::DebugUtil::DebugError, "Failed to interpret message");
            return;
        }
        else
        {
            if (event->entityType == TA_Base_Core::IManagedApplicationCorbaDef::ControlStation)
            {
                // Create message copy on the heap to make sure
                // the handler for WM_GX_TO_TA_MESSAGE has a valid
                // object to work with.
                TA_Base_Core::IManagedApplicationCorbaDef::GXToTAMessage* copyTAMessage =
                    new struct TA_Base_Core::IManagedApplicationCorbaDef::GXToTAMessage();

                copyTAMessage->sessionId = event->sessionId;
                copyTAMessage->sourceSchematicID = event->sourceSchematicID;
                copyTAMessage->entityType = event->entityType;
                copyTAMessage->message = event->message;
                m_rootDialog.PostMessage(WM_GX_TO_TA_MESSAGE, reinterpret_cast<WPARAM>(copyTAMessage));
            }
        }
    }
    catch (...)
    {
        LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "Caught some error when processing specialized message");
    }
}

void SessionManager::exit(const bool isRestart /*=false*/)
{
    FUNCTION_ENTRY("exit");

    s_exitNeedsRestart = isRestart;

    // Submit an audit with the name of the console on which this application is running
    if (RunParams::getInstance().isSet(RPARAM_LOCATIONKEY) &&
            !RunParams::getInstance().get(RPARAM_NOTIFYHOSTS).empty() &&
            RunParams::getInstance().get(RPARAM_LOCATIONKEY) != "0")
    {
        // TODO: This is just a dodgy hack at the moment so that I don't have to update the message corba
        // register to use ConsoleName or update the audit message sender to use case insensitive comparison
        // when comparing the placeholder names.
        sendControlStationAuditMessage(isRestart ? TA_Base_Core::ControlStationAudit::MFTRestart : TA_Base_Core::ControlStationAudit::ControlStationExit);
    }

    FUNCTION_EXIT;
}

void SessionManager::renewSession()
{
    FUNCTION_ENTRY("renewSession");

    if (RunParams::getInstance().get(RPARAM_SESSIONID).empty())
    {
        LOG(DEBUG_INFO, "No operator is logged in, so no need to renew session");
        return;
    }

    LOG(DEBUG_INFO, "An operator is logged in, so need to renew session");

    try
    {
        SessionDetails::SessionInfo sessionInfo = m_sessionDetails.getSessionInfo();
        std::string sessionId = m_authenticationMgr->requestSession(sessionInfo.operatorId, sessionInfo.profileId, sessionInfo.locationId, sessionInfo.consoleId, sessionInfo.password);

        // if the session Id returned by the Authentication Agent is still the same as the existing one,
        // it means the Authentication Agent still regard the session as active.  So don't need to
        // perform override and update the session ID then.
        if (sessionId == m_sessionDetails.getSessionId())
        {
            LOG(DEBUG_INFO, "Session remains unchanged.  No need to check for override and update session details");
            return;
        }

        LOG(DEBUG_INFO, "Requested a new session from the Authentication Agent.");

        // if the session is overriden, we need to perform an override to make sure the session has the same privilege
        // as before the control station lost connection to the network
        if (sessionInfo.isOverriden)
        {
            LOG(DEBUG_INFO, "Session is overriden, so need to override this new session");
            m_authenticationMgr->beginOverride(sessionId, sessionInfo.overrideOperatorId, sessionInfo.overrideProfileId, sessionInfo.locationId, sessionInfo.overridePassword);
        }

        // update the session details
        m_sessionDetails.renewSession(sessionId);

        // Tell all running applications that the session Id has changed
        m_processManager->updateSessionId(sessionId);
    }
    catch (const AuthenticationSecurityException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "AuthenticationSecurityException", "re-authentication failed.  No need to retry");
    }
    catch (const AuthenticationAgentException&)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "AuthenticationAgentException", "re-authentication failed.  No need to retry");
    }
    catch (...) // whatever exception that we got, just make it SessionException
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "Unknown Exception", "An unknown exception is caught during re-authentication.  No need to retry");
    }

    FUNCTION_EXIT;
}

void SessionManager::readSecuritySetting()
{
    try
    {
        TA_Base_Core::SecuritySettingAccessFactory& factory = TA_Base_Core::SecuritySettingAccessFactory::getInstance();
        automatic_logout_time = factory.getAutomaticLogoutTime();
        failed_login_attempts = factory.getFailedLoginAttemtps() ;
        password_reuse_max = factory.getPasswordReuseMax() ;
        password_warn_time = factory.getPasswordWarnTime() ;
        password_reuse_time = factory.getPasswordReuseTime();
        password_life_time = factory.getPasswordLifeTime();
        password_lock_time = factory.getPasswordLockTime();
        LOG(DEBUG_INFO, "The security settings are:\nautomatic_logout_time:%d\nfailed_login_attempts:%d\npassword_reuse_max:%d\npassword_warn_time:%d\npassword_reuse_time:%d\npassword_life_time:%d\npassword_lock_time:%d\n",
            automatic_logout_time,
            failed_login_attempts,
            password_reuse_max,
            password_warn_time,
            password_reuse_time,
            password_life_time,
            password_lock_time);
    }
    catch (DataException&)
    {
        TA_THROW(AuthenticationAgentException("A database error occured while attempting to get security Setting values"));
    }
    catch (DatabaseException&)
    {
        TA_THROW(AuthenticationAgentException("A database error occured while attempting to get security Setting values"));
    }
    catch (...)
    {
        TA_THROW(AuthenticationAgentException("An unknown error occured while attempting to get security Setting values"));
    }
}

bool SessionManager::PopUpChangePasswordDialog()
{
    bool isSuccessfulChange = false;

    //TD641 - for reason similar to TD653 see the comment in onLogout
    if (m_passwordDialog.m_hWnd != NULL)
    {
        LOG(DEBUG_INFO, "Already have 1 instance of change password dialog opened");
        return false;
    }

    // Check if current operator is allowed to change password
    EPermission permission = isControlPermitted(TA_Base_Bus::aca_CONTROL_STATION_CHANGE_PASSWORD);

    CString actionName;
    TA_VERIFY(actionName.LoadString(IDS_CHANGE_PASSWORD) != 0, "Unable to load IDS_CHANGE_PASSWORD");

    if (permission == TA_Base_App::TA_ControlStation::DENIED)
    {
        LOG(DEBUG_INFO, "Operator tries to change password without the right permission");
        showMsgBox(IDS_UE_010021, Utilities::getOperatorNameFromSession(), actionName);
        LOG(SourceInfo, DebugUtil::FunctionExit, "onDisplayChangePasswordDialogue");
        //return S_OK;
        return false;
    }
    else if (permission == TA_Base_App::TA_ControlStation::UNKNOWN)
    {
        LOG(DEBUG_INFO, "The system could not determine the rights of the current operator to change password");
        showMsgBox(IDS_UE_020056, actionName);
        LOG(SourceInfo, DebugUtil::FunctionExit, "onDisplayChangePasswordDialogue");
        //return S_OK;
        return false;
    }
    else if (permission == TA_Base_App::TA_ControlStation::GENERAL_ERROR)
    {
        LOG(DEBUG_INFO, "Some kind of error has occurred while checking for rights to change password");
        showMsgBox(IDS_UE_010018, actionName);
        LOG(SourceInfo, DebugUtil::FunctionExit, "onDisplayChangePasswordDialogue");
        //return S_OK;
        return false;
    }

    // fire off MENU_SUSPEND_MESG message to GraphWorx so that the menu won't be active
    // when the logout dialog is showing.  This prevents a selected menu from
    // remaining visible when graphworx is asked to hide (i.e. user logout).
    /*if (m_comControlStation != NULL)
    {
    m_comControlStation->Fire_shortMessage(MENU_SUSPEND_MESG.AllocSysString());
    }
    */
    // We got rights to do this, so continue
    //BOOL bEnable = TRUE;     // To enable
    m_passwordDialog.setDialogType(TA_Base_App::TA_ControlStation::FORCEDTOCHANGEPASSWORD);

    do
    {
        // Call DoModal on the dialog box. This will display and wait for the run()
        // method to complete before allowing the user to do anything.
        int response = m_passwordDialog.DoModal();

        if (response == IDCANCEL)
        {
            LOG(DEBUG_INFO, "User has cancelled the change password");
            break;
        }

        try
        {
            //BeginWaitCursor(); // display the hourglass cursor
            LOG(DEBUG_INFO, "SessionManager::PopUpChangePasswordDialog():changepassword");

            changePassword(m_passwordDialog);
            LOG(DEBUG_INFO, "SessionManager::PopUpChangePasswordDialog():changepassword successfully");

            isSuccessfulChange = true;
        }
        catch (const SessionException& ex)
        {
            CString agentName, password;
            TA_VERIFY(agentName.LoadString(IDS_AUTHENTICATION_AGENT) != 0, "Unable to load IDS_AUTHENTICATION_AGENT");
            TA_VERIFY(password.LoadString(IDS_PASSWORD) != 0, "Unable to load IDS_PASSWORD");

            switch (ex.getFailType())
            {
            case SessionException::AUTHENTICATION_FAILED:
                showMsgBox(IDS_UE_020046, actionName);
                break;

            case SessionException::AUTHENTICATION_AGENT_FAILED:
                showMsgBox(IDS_UE_030064, actionName, agentName);
                break;

            case SessionException::INVALID_PASSWORD_LENGTH:
                showMsgBox(IDS_UE_020055, password, MIN_PASSWORD_LENGTH);
                m_passwordDialog.useExistingDataOnNextDoModal();
                break;

            case SessionException::PASSWORD_MISMATCH:
                showMsgBox(IDS_UE_020045);
                m_passwordDialog.useExistingDataOnNextDoModal();
                break;

            case SessionException::PASSWORD_USEDBEFORE:
                showMsgBox(IDS_UE_620023);
                break;

            default:
                showMsgBox(IDS_UE_010018, actionName);
                break;
            }
        }
    }
    while (!isSuccessfulChange);

    LOG(SourceInfo, DebugUtil::FunctionExit, "onDisplayChangePasswordDialogue");
    return isSuccessfulChange;
}

TA_Base_Bus::SessionInfo SessionManager::getSessionInfo(const std::string& sessionId)
{
    return m_authenticationMgr->getSessionInfo(sessionId, "");
}

void SessionManager::areActionsPermitted(unsigned long action, ResourceList& resourceKeyList, ResourceList& permittedResourceKeyList)
{
    permittedResourceKeyList.clear();

    try
    {
        TA_Base_Bus::EDecisionModule decisionModule = TA_Base_Bus::UNDEFINED_DM;
        std::string reason;
        m_rightsMgr->isActionPermittedOnResources(m_sessionDetails.getSessionId(), resourceKeyList, permittedResourceKeyList, action, reason, decisionModule);
    }
    catch (const RightsException&)
    {
        // The library throws this exception when the right is either
        // indeterminate or not applicable
        LOG(DEBUG_INFO, "Failed to get the permitted resource list!");
    }
    catch (...)
    {
        // The library does not catch any exception, so any exception
        // other than the RightsException is error
        LOG(DEBUG_INFO, "Failed to get the permitted resource list!");
    }
}

void SessionManager::sendControlStationAuditMessage(const TA_Base_Core::MessageType& messageType)
{
    unsigned long entityKey = 0;

    try
    {
        if (m_controlStationEntity != NULL)
        {
            entityKey = m_controlStationEntity->getKey();
        }
    }
    catch (...)
    {
        // If there is any type of exception thrown, then go with the default value. Log the failure
        LOG_EXCEPTION_CATCH(SourceInfo, "General exception", "An exception was caught while attempting to retrieve the Control Station entity key. Proceeding with default value");
    }

    std::string consoleName = m_sessionDetails.getConsoleName();
    std::string userName = m_sessionDetails.getOperatorName();
    std::string profileName = m_sessionDetails.getProfileName();
    std::string logicLocation = m_sessionDetails.getLocationDisplayName();
    std::string sessionID = m_sessionDetails.getSessionId();
    DescriptionParametersWrap description;

    if (messageType == TA_Base_Core::ControlStationAudit::ControlStationStart ||
            messageType == TA_Base_Core::ControlStationAudit::MFTRestart ||
            messageType == TA_Base_Core::ControlStationAudit::ControlStationExit)
    {
        description = tuple_list_of(CONSOLE_NAME, consoleName);
    }
    else if (messageType == TA_Base_Core::ControlStationAudit::ControlStationLogout ||
             messageType == TA_Base_Core::ControlStationAudit::ControlStationChangeProfile)
    {
        description = tuple_list_of(CONSOLE_NAME, consoleName)(PROFILE_NAME, profileName)(USER_NAME, userName);
    }
    else if (messageType == TA_Base_Core::ControlStationAudit::ControlStationLogin ||
             messageType == TA_Base_Core::ControlStationAudit::ControlStationChangeProfile)
    {
        description = tuple_list_of(CONSOLE_NAME, consoleName)(PROFILE_NAME, profileName)(LOGIC_LOCATION, logicLocation)(USER_NAME, userName);
    }
    else
    {
        description = tuple_list_of(CONSOLE_NAME, consoleName)(PROFILE_NAME, profileName)(LOGIC_LOCATION, logicLocation)(USER_NAME, userName);
    }

    m_auditMessageSender->sendAuditMessage(messageType, entityKey, description, "N/A", sessionID, "", "", "");
}
