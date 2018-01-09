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
#include "StdAfx.h"
#include "SessionManager.h"
#include "Utilities.h"
#include "DataCache.h"
#include "ControlStationAuditMessageSender.h"
#include "Signals.h"
#include "SessionSignal.h"
#include "CommonSignal.h"
#include "SessionDetails.h"
#include "ControlStationConstants.h"
#include "bus/security/authentication_library/src/AuthenticationLibrary.h"
#include "core/data_access_interface/src/ILocation.h"
#include "core/data_access_interface/src/IProfile.h"
#include "core/data_access_interface/src/IOperator.h"
#include "core/data_access_interface/entity_access/src/IConsole.h"
#include "core/data_access_interface/entity_access/src/ControlStation.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/exceptions/src/SessionException.h"
#include "core/exceptions/src/ApplicationException.h"
#include "core/exceptions/src/AuthenticationSecurityException.h"
#include "core/exceptions/src/AuthenticationAgentException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/message/types/ControlStationAudit_MessageTypes.h"

using namespace TA_Base_App::TA_ControlStation;
using namespace TA_Base_Core;

typedef std::list<unsigned long> ResourceList;

const std::string SessionManager::CONSOLE_NAME      = "ConsoleName";
const std::string SessionManager::PROFILE_NAME      = "ProfileName";
const std::string SessionManager::USER_NAME         = "Username";
const std::string SessionManager::GROUP_ONLINE      = "GroupOnline";
const std::string SessionManager::LOGIC_LOCATION    = "LogicLoc";

SessionManager::SessionManager()
    : m_sCGroupOnline(true)
{
    setupSignalConnections();
}

SessionManager::~SessionManager()
{
    FUNCTION_ENTRY("~SessionManager");

    try
    {
        LOG_DEBUG("Going to delete authentication manager");

        std::string id = SessionDetails::getInstance().getSessionId();

        if (id.size())
        {
            LOG(DEBUG_WARN, "Still got a session, going to terminate the session first");
            // make sure we end the session
            m_authenticationLibrary->endSession(id);
        }

        LOG_INFO("Successfully deleted authentication Manager");
    }
    catch (...)
    {
        // Catch all exceptions so we don't get errors filtering out
        LOG_EXCEPTION("Unknown", "Caught in destructor of SessionManager");
    }

    FUNCTION_EXIT;
}

void SessionManager::setupSignalConnections()
{
    SessionSignal::requestSessionId.connect(boost::bind(&SessionManager::handleRequestSessionIdSignal, this, _1, _2, _3, _4, _5, _6));
    SessionSignal::requestChangeProfile.connect(boost::bind(&SessionManager::handleRequestChangeProfileSignal, this, _1, _2, _3, _4, _5, _6));
    SessionSignal::requestChangeOperator.connect(boost::bind(&SessionManager::handleRequestChangeOperatorSignal, this, _1, _2, _3, _4, _5, _6));
    SessionSignal::requestOperatorOverride.connect(boost::bind(&SessionManager::handleRequestOperatorOverrideSignal, this, _1, _2, _3, _4, _5, _6));
    SessionSignal::requestRemoveOperatorOverride.connect(boost::bind(&SessionManager::handleRequestRemoveOperatorOverrideSignal, this, _1));
    SessionSignal::requestEndSession.connect(boost::bind(&SessionManager::handleRequestEndSessionSignal, this, _1));
    CommonSignal::serverStateChanged.connect(boost::bind(&SessionManager::handleServerStateChangedSignal, this, _1));
    CommonSignal::systemControllerStateChanged.connect(boost::bind(&SessionManager::handleSystemControllerStateChangedSignal, this, _1, _2));
}

void SessionManager::asyncInitialize()
{
    m_controlStationEntity = DataCache::getInstance().getControlStationEntity();
    m_console = DataCache::getInstance().getConsole();
    m_authenticationLibrary.setCreator();
}

void SessionManager::handleRequestSessionIdSignal(StringPromisePtr sessionIdPromise,
                                                  IOperator* theOperator,
                                                  IProfile* theProfile,
                                                  ILocation* theLocation,
                                                  const std::string& password,
                                                  bool displayOnly)
{
    try
    {
        Keys keys = getKeys(theOperator, theProfile, theLocation);

        LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo,
                    "Attempting to request a session with OperatorKey = %d, ProfileKey = %d, LocationKey = %d, ConsoleKey = %d",
                    keys.operatorId, keys.profileId, keys.locationKey, keys.consoleId);

        std::string sessionId = m_authenticationLibrary->requestSession(keys.operatorId, keys.profileId, keys.locationKey, keys.consoleId, password, displayOnly);

        sessionIdPromise->set_value(sessionId);
    }
    catch (const AuthenticationSecurityException& ex)
    {
        sessionIdPromise->set_exception(boost::copy_exception(ex));
    }
    catch (const AuthenticationAgentException& ex)
    {
        sessionIdPromise->set_exception(boost::copy_exception(ex));
    }
    catch (TA_Base_Core::ObjectResolutionException& ex)
    {
        sessionIdPromise->set_exception(boost::copy_exception(ex));
    }
    catch (const SessionException& ex)
    {
        sessionIdPromise->set_exception(boost::copy_exception(ex));
    }
    catch (...)
    {
        sessionIdPromise->set_exception(boost::current_exception());
    }
}

void SessionManager::handleRequestChangeProfileSignal(StringPromisePtr sessionIdPromise,
                                                      IOperator* theOperator,
                                                      IProfile* theProfile,
                                                      ILocation* theLocation,
                                                      const std::string& password,
                                                      bool displayOnly)
{
    try
    {
        if (!SessionDetails::getInstance().isSessionValid())
        {
            sessionIdPromise->set_exception(boost::copy_exception(SessionException(SessionException::INVALID_SESSION_CONFIGURATION, "Trying to change profile when the session configuration has become invalid")));
        }

        if (theOperator->getName() != SessionDetails::getInstance().getOperatorName())
        {
            sessionIdPromise->set_exception(boost::copy_exception(SessionException("Attempting to change the profile of operator who is not logged in")));
        }

        Keys keys = getKeys(theOperator, theProfile, theLocation);

        LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo,
                    "Attempting to change profile with OperatorKey = %d, ProfileKey = %d, LocationKey = %d, ConsoleKey = %d",
                    keys.operatorId,  keys.profileId, keys.locationKey, keys.consoleId);

        std::string sessionId = m_authenticationLibrary->changeProfile(SessionDetails::getInstance().getSessionId(),
                                                                       keys.profileId,
                                                                       keys.locationKey,
                                                                       password,
                                                                       displayOnly);

        sessionIdPromise->set_value(sessionId);
    }
    catch (const AuthenticationSecurityException& ex)
    {
        sessionIdPromise->set_exception(boost::copy_exception(ex));
    }
    catch (const AuthenticationAgentException& ex)
    {
        sessionIdPromise->set_exception(boost::copy_exception(ex));
    }
    catch (TA_Base_Core::ObjectResolutionException& ex)
    {
        sessionIdPromise->set_exception(boost::copy_exception(ex));
    }
    catch (const SessionException& ex)
    {
        sessionIdPromise->set_exception(boost::copy_exception(ex));
    }
    catch (const TransactiveException& ex)
    {
        sessionIdPromise->set_exception(boost::copy_exception(ex));
    }
    catch (...)
    {
        sessionIdPromise->set_exception(boost::current_exception());
    }
}

void SessionManager::handleRequestChangeOperatorSignal(StringPromisePtr sessionIdPromise,
                                                       IOperator* theOperator,
                                                       IProfile* theProfile,
                                                       ILocation* theLocation,
                                                       const std::string& password,
                                                       bool displayOnly)
{
    try
    {
        if (!SessionDetails::getInstance().isSessionValid())
        {
            sessionIdPromise->set_exception(boost::copy_exception(SessionException(SessionException::INVALID_SESSION_CONFIGURATION, "Trying to change operator when the session configuration has become invalid")));
        }

        if (theOperator->getName() == SessionDetails::getInstance().getOperatorName())
        {
            sessionIdPromise->set_exception(boost::copy_exception(SessionException("Attempting to change into the same operator")));
        }

        std::string oldSessionId = SessionDetails::getInstance().getSessionId();
        Keys keys = getKeys(theOperator, theProfile, theLocation);

        LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo,
                    "Attempting to request a session with OperatorKey = %d, ProfileKey = %d, LocationKey = %d, ConsoleKey = %d",
                    keys.operatorId, keys.profileId, keys.locationKey, keys.consoleId);

        // And an audit message for the logout
        ControlStationAuditMessageSender::getInstance().sendControlStationAuditMessage(TA_Base_Core::ControlStationAudit::ControlStationLogout);

        std::string sessionId = m_authenticationLibrary->requestSession(keys.operatorId, keys.profileId, keys.locationKey, keys.consoleId, password, displayOnly);

        // Submit an audit message for the new login
        ControlStationAuditMessageSender::getInstance().sendControlStationAuditMessage(TA_Base_Core::ControlStationAudit::ControlStationLogin);

        sessionIdPromise->set_value(sessionId);

        {
            // TODO: thread
            // logout the old session
            try
            {
                // make sure we remove the operator override before logging off
                if (SessionDetails::getInstance().isOperatorOverridden())
                {
                    m_authenticationLibrary->endOverride(oldSessionId);
                }

                m_authenticationLibrary->endSession(oldSessionId);
            }
            catch (...)
            {
                // If the session manager somehow couldn't logout, just ignore
                // we already got the new session
            }
        }
    }
    catch (const AuthenticationSecurityException& ex)
    {
        sessionIdPromise->set_exception(boost::copy_exception(ex));
    }
    catch (const AuthenticationAgentException& ex)
    {
        sessionIdPromise->set_exception(boost::copy_exception(ex));
    }
    catch (TA_Base_Core::ObjectResolutionException& ex)
    {
        sessionIdPromise->set_exception(boost::copy_exception(ex));
    }
    catch (const SessionException& ex)
    {
        sessionIdPromise->set_exception(boost::copy_exception(ex));
    }
    catch (const TransactiveException& ex)
    {
        sessionIdPromise->set_exception(boost::copy_exception(ex));
    }
    catch (...)
    {
        sessionIdPromise->set_exception(boost::current_exception());
    }
}

void SessionManager::handleRequestOperatorOverrideSignal(PromisePtr promise,
                                                         IOperator* theOperator,
                                                         IProfile* theProfile,
                                                         ILocation* theLocation,
                                                         const std::string& password,
                                                         bool displayOnly)
{
    try
    {
        if (!SessionDetails::getInstance().isSessionValid())
        {
            promise->set_exception(boost::copy_exception(SessionException(SessionException::INVALID_SESSION_CONFIGURATION, "Trying to override when the session configuration has become invalid")));
        }

        if (theOperator->getName() == SessionDetails::getInstance().getOperatorName())
        {
            promise->set_exception(boost::copy_exception(SessionException("Attempting to change into the same operator")));
        }

        // Check if the two profiles are the same AND
        // The location names are the same
        if ((SessionDetails::getInstance().getProfileName() == theProfile->getName()) &&
                (SessionDetails::getInstance().getLocationDisplayName() == getLocationName(theLocation)))
        {
            LOG_INFO("The Details selected for override are the same as the current details. Ignoring.");
            promise->set_value();
            return;
        }

        Keys keys = getKeys(theOperator, theProfile, theLocation);

        LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo,
                    "Attempting to begin override with OperatorKey = %d, ProfileKey = %d, LocationKey = %d, ConsoleKey = %d",
                    keys.operatorId, keys.profileId, keys.locationKey, keys.consoleId);

        m_authenticationLibrary->beginOverride(SessionDetails::getInstance().getSessionId(), keys.operatorId, keys.profileId, keys.locationKey, password);
        promise->set_value();
    }
    catch (const AuthenticationSecurityException& ex)
    {
        promise->set_exception(boost::copy_exception(ex));
    }
    catch (const AuthenticationAgentException& ex)
    {
        promise->set_exception(boost::copy_exception(ex));
    }
    catch (TA_Base_Core::ObjectResolutionException& ex)
    {
        promise->set_exception(boost::copy_exception(ex));
    }
    catch (const SessionException& ex)
    {
        promise->set_exception(boost::copy_exception(ex));
    }
    catch (const TransactiveException& ex)
    {
        promise->set_exception(boost::copy_exception(ex));
    }
    catch (...)
    {
        promise->set_exception(boost::current_exception());
    }
}

void SessionManager::handleRequestRemoveOperatorOverrideSignal(PromisePtr promise)
{
    try
    {
        m_authenticationLibrary->endOverride(SessionDetails::getInstance().getSessionId());

        // Tell all running applications that the session Id has changed
        //Signals::sessionIdChanged(SessionDetails::getInstance().getSessionId());
    }
    catch (...)
    {
        LOG_EXCEPTION("Unknown exception", "Some unknown exception has occurred while attempting to remove operator override");
    }

    promise->set_value();
}

void SessionManager::handleRequestEndSessionSignal(PromisePtr promise)
{
    try
    {
        // make sure we remove the operator override before logging off
        if (SessionDetails::getInstance().isOperatorOverridden())
        {
            m_authenticationLibrary->endOverride(SessionDetails::getInstance().getSessionId());
        }

        m_authenticationLibrary->endSession(SessionDetails::getInstance().getSessionId());
    }
    catch (...)
    {
        // If the session manager somehow couldn't logout, we still want to
        // continue with the rest of the logout process
    }

    promise->set_value();
}

unsigned int SessionManager::getLocationKey(TA_Base_Core::ILocation* theLocation)
{
    // if we're offline, we'll always just use the current location
    if (!m_sCGroupOnline)
    {
        return SessionDetails::getInstance().getConsoleId();
    }
    else if (theLocation != NULL)
    {
        return theLocation->getKey();
    }

    return 0;
}

std::string SessionManager::getLocationName(TA_Base_Core::ILocation* theLocation)
{
    // if we're offline, we'll always just use the current location
    if (!m_sCGroupOnline)
    {
        return SessionDetails::getInstance().getConsoleName();
    }
    else if (theLocation != NULL)
    {
        return theLocation->getDisplayName();
    }

    return "";
}

void SessionManager::renewSession()
{
    if (RunParams::getInstance().get(RPARAM_SESSIONID).empty())
    {
        return;
    }

    LOG_INFO("An operator is logged in, so need to renew session");

    try
    {
        SessionDetails::SessionInfo sessionInfo = SessionDetails::getInstance().getSessionInfo();
        std::string sessionId = m_authenticationLibrary->requestSession(sessionInfo.operatorId, sessionInfo.profileId, sessionInfo.locationId, sessionInfo.consoleId, sessionInfo.password);

        // if the session Id returned by the Authentication Agent is still the same as the existing one,
        // it means the Authentication Agent still regard the session as active.  So don't need to
        // perform override and update the session ID then.
        if (sessionId == SessionDetails::getInstance().getSessionId())
        {
            LOG_INFO("Session remains unchanged.  No need to check for override and update session details");
            return;
        }

        LOG_INFO("Requested a new session from the Authentication Agent.");

        // if the session is overriden, we need to perform an override to make sure the session has the same privilege
        // as before the control station lost connection to the network
        if (sessionInfo.isOverriden)
        {
            LOG_INFO("Session is overriden, so need to override this new session");
            m_authenticationLibrary->beginOverride(sessionId, sessionInfo.overrideOperatorId, sessionInfo.overrideProfileId, sessionInfo.locationId, sessionInfo.overridePassword);
        }

        // update the session details
        Signals::renewSession(sessionId);
    }
    catch (const AuthenticationSecurityException&)
    {
        LOG_EXCEPTION("AuthenticationSecurityException", "re-authentication failed.  No need to retry");
    }
    catch (const AuthenticationAgentException&)
    {
        LOG_EXCEPTION("AuthenticationAgentException", "re-authentication failed.  No need to retry");
    }
    catch (...) // whatever exception that we got, just make it SessionException
    {
        LOG_EXCEPTION("Unknown Exception", "An unknown exception is caught during re-authentication.  No need to retry");
    }
}

TA_Base_Bus::SessionInfo SessionManager::getSessionInfo(const std::string& sessionId)
{
    return m_authenticationLibrary->getSessionInfo(sessionId, "");
}

void SessionManager::handleSystemControllerStateChangedSignal(bool isGroupOnline, const std::string& groupName)
{
    m_sCGroupOnline = isGroupOnline;
}

void SessionManager::handleServerStateChangedSignal(bool isServerUp)
{
    if (isServerUp)
    {
        renewSession();
    }
}

SessionManager::Keys SessionManager::getKeys(IOperator* theOperator, IProfile* theProfile, ILocation* theLocation)
{
    if (!theOperator || !theProfile)
    {
        throw SessionException(SessionException::INVALID_SESSION_CONFIGURATION, "Trying to request session id when the session configuration is invalid");
    }

    return Keys(theOperator->getKey(), theProfile->getKey(), getLocationKey(theLocation), m_console->getKey());
}
