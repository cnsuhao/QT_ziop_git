/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/SessionDetails.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This class holds all session details about the current login as well as the lists of
  * operators, profiles etc.
  */
#pragma warning(disable:4786)
#include "StdAfx.h"
#include "DataCache.h"
#include "Signals.h"
#include "app/system_control/control_station/src/ControlStationRootDialog.h"
#include "app/system_control/control_station/src/SessionDetails.h"
#include "bus/security/authentication_library/src/AuthenticationLibrary.h"
#include "bus/security/authentication_agent/idl/src/IAuthenticationAgentCorbaDef.h"
#include "core/configuration_updates/src/OnlineUpdateListener.h"
#include "core/data_access_interface/entity_access/src/IConsole.h"
#include "core/data_access_interface/src/IOperator.h"
#include "core/data_access_interface/src/ILocation.h"
#include "core/data_access_interface/src/IProfile.h"
#include "core/data_access_interface/src/OperatorAccessFactory.h"
#include "core/data_access_interface/src/ProfileAccessFactory.h"
#include "core/data_access_interface/src/LocationAccessFactory.h"
#include "core/data_access_interface/entity_access/src/ConsoleAccessFactory.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/TAAssert.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/EntityTypeException.h"
#include "core/exceptions/src/SessionException.h"

using namespace TA_Base_Core;

#define TRY_CATCH(statement)                                                            \
    try                                                                                 \
    {                                                                                   \
        statement;                                                                      \
    }                                                                                   \
    catch (DataException&)                                                              \
    {                                                                                   \
        LOG_EXCEPTION("DataException", "Error occurred while doing " #statement);       \
    }                                                                                   \
    catch (DatabaseException&)                                                          \
    {                                                                                   \
        LOG_EXCEPTION("DatabaseException", "Error occurred while doing " #statement);   \
    }

#undef TRY
#undef CATCH
#define TRY try
#define CATCH(message)                                                                                                          \
    catch (DataException&)                                                                                                      \
    {                                                                                                                           \
        LOG_EXCEPTION("DataException", "Error occurred while attempting to retrieve the " message  " from the database.");      \
    }                                                                                                                           \
    catch (DatabaseException&)                                                                                                  \
    {                                                                                                                           \
        LOG_EXCEPTION("DatabaseException", "Error occurred while attempting to retrieve the " message  " from the database.");  \
    }

SessionDetails::SessionDetails()
    : m_console(NULL),
      m_operator(NULL),
      m_profile(NULL),
      m_location(NULL),
      m_operatorOverride(NULL),
      m_profileOverride(NULL),
      m_hasOperatorUpdated(false),
      m_hasLocationUpdated(false),
      m_hasProfileUpdated(false),
      m_isSessionValid(false),
      m_isOverrideValid(false),
      m_shouldChangeDisplay(false),
      m_displayOnly(false)
{
    setupSignalConnections();
}

void SessionDetails::setupSignalConnections()
{
    Signals::sessionLoginInfo.connect(boost::bind(&SessionDetails::sessionLogin, this, _1, _2, _3, _4, _5));
    Signals::renewSession.connect(boost::bind(&SessionDetails::renewSession, this, _1));
    Signals::profileChangedInfo.connect(boost::bind(&SessionDetails::profileChanged, this, _1, _2, _3));
    Signals::operatorOverrideInfo.connect(boost::bind(&SessionDetails::operatorOverride, this, _1, _2, _3));
    Signals::removeOperatorOverride.connect(boost::bind(&SessionDetails::removeOperatorOverride, this));
    //TODO: SessionSignal::logouted.connect(boost::bind(&SessionDetails::sessionLogout, this));
}

SessionDetails::~SessionDetails()
{
    FUNCTION_ENTRY("Destructor");

    try
    {
        //
        // Delete all the operators
        //
        OperatorVector::iterator operatorIter = m_allOperators.begin();

        while (operatorIter != m_allOperators.end())
        {
            if (*operatorIter != NULL)
            {
                delete *operatorIter;
                *operatorIter = NULL;
            }

            ++operatorIter;
        }

        m_allOperators.clear();

        //
        // Delete all the profiles
        //
        for (ProfileMap::iterator prof = m_profileMap.begin(); prof != m_profileMap.end(); prof++)
        {
            delete prof->second;
            prof->second = NULL;
        }

        //
        // Delete all the locationss
        //
        for (LocationMap::iterator loc = m_locationMap.begin(); loc != m_locationMap.end(); loc++)
        {
            delete loc->second;
            loc->second = NULL;
        }
    }
    catch (...)
    {
        // Catch all exceptions so we don't get errors filtering out
        LOG_EXCEPTION("Unknown", "Caught in destructor of SessionDetails");
    }

    FUNCTION_EXIT;
}

void SessionDetails::asyncInitialize()
{
    FUNCTION_ENTRY("initialise");

    //
    // Retrieve the console name and check it is in the database
    //
    m_console = DataCache::getInstance().getConsole();
    m_consoleLocation = DataCache::getInstance().getConsoleLocation();

    if (!m_console || !m_consoleLocation)
    {
        TA_THROW(SessionException("Console does not exist or has errors in the database"));
    }

    // TD1362 - Need to set the location key so that applications get the right location key
    RunParams::getInstance().set(RPARAM_LOCATIONKEY, boost::lexical_cast<std::string>(m_console->getLocation()).c_str());

    //
    // Load all the operators and locations.
    //
    try
    {
        // listen to all location and operator changes
        OnlineUpdateListener::getInstance().registerAllInterests(TA_Base_Core::LOCATION, *this);
        OnlineUpdateListener::getInstance().registerAllInterests(TA_Base_Core::PROFILE, *this);
        OnlineUpdateListener::getInstance().registerAllInterests(TA_Base_Core::OPERATOR, *this);
        // only get the non-system operators PW2480
        m_allOperators = DataCache::getInstance().getAllOperators();

        for (TA_Base_Core::IProfile* profile : DataCache::getInstance().getAllProfiles())
        {
            m_profileMap.insert(std::make_pair(profile->getKey(), profile));
        }

        for (TA_Base_Core::ILocation* location : DataCache::getInstance().getAllLocations())
        {
            m_locationMap.insert(std::make_pair(location->getKey(), location));
        }
    }
    catch (DataException&)
    {
        LOG_EXCEPTION("DataException", "Could not load the list of available operators or locations.");
    }
    catch (DatabaseException&)
    {
        LOG_EXCEPTION("DatabaseException", "Could not load the list of available operators or locations.");
    }

    // We cannot load all the profiles as they are based on the selected operator.

    FUNCTION_EXIT;
}

void SessionDetails::sessionLogin(const std::string& sessionId,
                                  TA_Base_Core::IOperator* theOperator,
                                  TA_Base_Core::IProfile*  theProfile,
                                  TA_Base_Core::ILocation*  theLocation,
                                  bool displayOnly)
{
    FUNCTION_ENTRY("sessionLogin");

    // We must have a session ID that is not ""
    TA_ASSERT(sessionId.compare("") != 0, "The sessionId is empty");

    // We must have operator, profile and sessionId
    TA_ASSERT((theOperator != NULL) && (theProfile != NULL), "The operator or profile is NULL");

    m_sessionId = sessionId;
    // Set up the RunParams so that everyone knows about the current session ID
    RunParams::getInstance().set(RPARAM_SESSIONID, m_sessionId.c_str());
    Signals::sessionIdChanged(m_sessionId);
    RunParams::getInstance().set("OperatorName", theOperator->getName().c_str());
    m_operator = theOperator;

    // Set up profile.
    m_profile = theProfile; // reference the profile, just make sure we don't delete it when we repopulate

    // TD1362, TD2405 - reset the location key to the one specified
    // If not location specific (i.e. NULL), use the console location, otherwise use the one specified
    m_location = (theLocation == NULL) ? m_consoleLocation.get() : theLocation;
    RunParams::getInstance().set(RPARAM_LOCATIONKEY, boost::lexical_cast<std::string>(m_location->getKey()).c_str());

    m_isSessionValid = true;
    m_shouldChangeDisplay = true;
    m_displayOnly = displayOnly;
}

void SessionDetails::renewSession(const std::string& sessionId)
{
    FUNCTION_ENTRY("renewSession");
    TA_ASSERT(sessionId.compare("") != 0, "The sessionId is empty");

    m_sessionId = sessionId;
    // Set up the RunParams so that everyone knows about the current session ID
    RunParams::getInstance().set(RPARAM_SESSIONID, sessionId);
    Signals::sessionIdChanged(m_sessionId);
    FUNCTION_EXIT;
}

void SessionDetails::profileChanged(const std::string& sessionId, TA_Base_Core::IProfile* theProfile, TA_Base_Core::ILocation* theLocation)
{
    FUNCTION_ENTRY("profileChanged");

    // We must have a session ID that is not ""
    TA_ASSERT(sessionId.compare("") != 0, "The sessionId is empty");
    TA_ASSERT(theProfile != NULL, "The profile passed was equal to NULL");

    m_sessionId = sessionId;
    // Set up the RunParams so that everyone knows about the new session ID
    RunParams::getInstance().set(RPARAM_SESSIONID, m_sessionId);
    Signals::sessionIdChanged(m_sessionId);
    RunParams::getInstance().set("OperatorName", m_operator->getName());
    m_profile = theProfile;

    // TD1362, TD2405 - reset the location key to the one specified
    m_location = (theLocation == NULL) ? m_consoleLocation.get() : theLocation;
    RunParams::getInstance().set(RPARAM_LOCATIONKEY, boost::lexical_cast<std::string>(m_location->getKey()));

    // since we've changed the profile, we need to remove operator override
    m_profileOverride = NULL;
    m_operatorOverride = NULL;
    m_isOverrideValid = false;
    m_shouldChangeDisplay = true;
}

void SessionDetails::operatorOverride(TA_Base_Core::IOperator* theOperator,
                                      TA_Base_Core::IProfile*  theProfile,
                                      TA_Base_Core::ILocation*  theLocation)
{
    FUNCTION_ENTRY("operatorOverride");

    // We must have operator and profile
    TA_ASSERT((theOperator != NULL) && (theProfile != NULL), "The operator or profile is NULL");

    // Set the RunParams so that the applications will recheck their security
    RunParams::getInstance().set(RPARAM_SESSIONID, m_sessionId.c_str());
    Signals::sessionIdChanged(m_sessionId);
    RunParams::getInstance().set("OperatorName", theOperator->getName().c_str());
    m_operatorOverride = theOperator;
    m_profileOverride = theProfile;
    m_isOverrideValid = true;
    m_shouldChangeDisplay = false;
}

void SessionDetails::sessionLogout()
{
    FUNCTION_ENTRY("sessionLogout");

    // set the session id to "" so that we know that someone is not logged in
    m_sessionId = "";
    RunParams::getInstance().set(RPARAM_SESSIONID, "");
    Signals::sessionIdChanged(m_sessionId);
    RunParams::getInstance().set("OperatorName", "");
    removeOperatorOverride();

    // reset all login session details
    m_operator = NULL;

    if (m_location != m_consoleLocation.get())
    {
        m_location = m_consoleLocation.get();
    }

    // TD1362 - use the console location
    RunParams::getInstance().set(RPARAM_LOCATIONKEY, boost::lexical_cast<std::string>(m_location->getKey()).c_str());

    m_profile = NULL;

    m_isSessionValid = false;

    // don't bother updating the displays since we're going to hide the display anyway
    m_shouldChangeDisplay = false;
    m_displayOnly = false;
}

void SessionDetails::removeOperatorOverride()
{
    FUNCTION_ENTRY("removeOperatorOverride");

    // reset all login session details
    m_operatorOverride = NULL;
    m_profileOverride = NULL;
    m_isOverrideValid = false;

    // if there is no longer a valid session to return to (the configuration
    // settings that the login session used have been deleted)
    if (m_isSessionValid)
    {
        // Set the RunParams so that the applications will recheck their security
        TA_Base_Core::RunParams::getInstance().set(RPARAM_SESSIONID, m_sessionId.c_str());
        Signals::sessionIdChanged(m_sessionId);
    }
    else
    {
        ControlStationRootDialog::getInstance().sendMessage(WM_FORCE_LOGOUT);
    }

    m_shouldChangeDisplay = false;
}

SessionDetails::ProfileVector SessionDetails::getListOfProfiles(TA_Base_Core::IOperator* theOperator, bool onlyOverridable /*=false*/)
{
    FUNCTION_ENTRY("getListOfProfiles");

    TA_ASSERT(theOperator != NULL, "The operator passed was equal to NULL");

    ProfileVector associatedProfiles;

    // Load the new profiles
    TRY
    {
        // TD2844 - The operator override is only allowed to operator as follows:
        //              - central profile cannot be overriden by local profile
        //              - local profile can be overriden by central profile
        //              - local profile can override another profile on the same location
        //              - a central profile can override another central profile
        //
        // So, see if the current profile is central,
        //     if it is, only return central profiles
        // Otherwise, return all other profiles
        bool onlyGetCentralProfiles = onlyOverridable ;//&& (m_profile != NULL) && (!m_profile->requiresLocationToBeSelected());
        std::vector<unsigned long> profiles = theOperator->getAssociatedProfileKeys();

        for (unsigned int i = 0 ; i < profiles.size(); i++)
        {
            ProfileMap::iterator prfIter = m_profileMap.find(profiles[i]);

            if (prfIter == m_profileMap.end())
            {
                LOG(DEBUG_WARN, "Unable to find the profile, %d, that is associated with operator, %d.  Could be system profile. Ignoring", profiles[i], theOperator->getKey());
            }
            else
            {
                if (!(onlyGetCentralProfiles && prfIter->second->requiresLocationToBeSelected()))
                {
                    associatedProfiles.push_back(prfIter->second);
                }
            }
        }
    }
    CATCH("a list of profiles")
    {
    }

    // returning an array which points to our internal copies
    return associatedProfiles;
}

SessionDetails::ProfileVector SessionDetails::getListOfProfiles(TA_Base_Core::IOperator* theOperator, bool isCentral, bool onlyOverridable /*=false*/)
{
    FUNCTION_ENTRY("getListOfProfiles");

    TA_ASSERT(theOperator != NULL, "The operator passed was equal to NULL");

    ProfileVector associatedProfiles;

    // Load the new profiles
    TRY
    {
        // TD2844 - The operator override is only allowed to operator as follows:
        //              - central profile cannot be overriden by local profile
        //              - local profile can be overriden by central profile
        //              - local profile can override another profile on the same location
        //              - a central profile can override another central profile
        //
        // So, see if the current profile is central,
        //     if it is, only return central profiles
        // Otherwise, return all other profiles
        bool onlyGetCentralProfiles = onlyOverridable ;//&& (m_profile != NULL) && (!m_profile->requiresLocationToBeSelected());
        std::vector<unsigned long> profiles = theOperator->getAssociatedProfileKeys();

        for (unsigned int i = 0 ; i < profiles.size(); i++)
        {
            ProfileMap::iterator prfIter = m_profileMap.find(profiles[i]);

            if (prfIter == m_profileMap.end())
            {
                LOG(DEBUG_WARN, "Unable to find the profile, %d, that is associated with operator, %d.  Could be system profile. Ignoring", profiles[i], theOperator->getKey());
            }
            else
            {
                if (onlyGetCentralProfiles && isCentral && (!prfIter->second->requiresLocationToBeSelected()))
                {
                    associatedProfiles.push_back(prfIter->second);
                }

                if (onlyGetCentralProfiles && (!isCentral) && prfIter->second->requiresLocationToBeSelected())
                {
                    associatedProfiles.push_back(prfIter->second);
                }

                if ((!onlyGetCentralProfiles) && isCentral && (!prfIter->second->requiresLocationToBeSelected()))
                {
                    associatedProfiles.push_back(prfIter->second);
                }

                if ((!onlyGetCentralProfiles) && (!isCentral) && prfIter->second->requiresLocationToBeSelected())
                {
                    associatedProfiles.push_back(prfIter->second);
                }
            }
        }
    }
    CATCH("a list of profiles")
    {
    }

    // returning an array which points to our internal copies
    return associatedProfiles;
}

ILocationList SessionDetails::getListOfLocations(TA_Base_Core::IProfile* theProfile)
{
    FUNCTION_ENTRY("getListOfLocations");

    TA_ASSERT(theProfile != NULL, "The profile passed was equal to NULL");

    ILocationList associatedLocations;

    TRY
    {
        std::vector<unsigned long> locations = theProfile->getAssociatedLocationKeys();

        for (unsigned long locationKey : locations)
        {
            LocationMap::iterator it = m_locationMap.find(locationKey);

            if (it == m_locationMap.end())
            {
                LOG(DEBUG_WARN, "Unable to find the location, %d, that is associated with profile, %d.  Ignoring", locationKey, theProfile->getKey());
            }
            else
            {
                associatedLocations.push_back(it->second);
            }
        }
    }
    CATCH("a list of locations")
    {
    }

    // returning an array which points to our internal copies
    return associatedLocations;
}

std::string SessionDetails::getOperatorName()
{
    if (m_operator == NULL)
    {
        return "";
    }

    std::string name = "Unknown";
    TRY_CATCH(name = m_operator->getName());
    return name;
}

std::string SessionDetails::getOperatorDescription()
{
    if (m_operator == NULL)
    {
        return "";
    }

    std::string description = "Unknown";
    TRY_CATCH(description = m_operator->getDescription());
    return description;
}

std::string SessionDetails::getOverrideOperatorName()
{
    if (m_operatorOverride == NULL)
    {
        return "";
    }

    std::string name = "Unknown";
    TRY_CATCH(name = m_operatorOverride->getName());
    return name;
}

std::string SessionDetails::getOverrideOperatorDescription()
{
    if (m_operatorOverride == NULL)
    {
        return "";
    }

    std::string description = "Unknown";
    TRY_CATCH(description = m_operatorOverride->getDescription());
    return description;
}

std::string SessionDetails::getProfileName()
{
    if (m_profile == NULL)
    {
        return "";
    }

    std::string name = "Unknown";
    TRY_CATCH(name = m_profile->getName());
    return name;
}

std::string SessionDetails::getOverrideProfileName()
{
    if (m_profileOverride == NULL)
    {
        return "";
    }

    std::string name = "Unknown";
    TRY_CATCH(name = m_profileOverride->getName());
    return name;
}

std::string SessionDetails::getProfileAndLocationDisplayString()
{
    if (m_profile == NULL)
    {
        return "";
    }

    std::string displayString = "Unknown";

    TRY
    {
        displayString = m_profile->getName();

        if (m_profile->requiresLocationToBeSelected() && (m_location != NULL))
        {
            displayString += TA_Base_App::TA_ControlStation::LOCATION_PROFILE_SEPARATOR;
            displayString += m_location->getName();
        }
    }
    CATCH("the profile and location names")
    {
    }

    LOG_INFO("Operator profile and location display string is %s", displayString.c_str());
    return displayString;
}

std::string SessionDetails::getOverrideProfileAndLocationDisplayString()
{
    if (m_profileOverride == NULL)
    {
        return "";
    }

    std::string displayString = "Unknown";

    TRY
    {
        displayString = m_profileOverride->getName();

        if (m_profileOverride->requiresLocationToBeSelected() && (m_location != NULL))
        {
            displayString += TA_Base_App::TA_ControlStation::LOCATION_PROFILE_SEPARATOR;
            displayString += m_location->getName();
        }
    }
    CATCH("the profile and location override names")
    {
    }

    LOG_INFO("Operator profile and location override display string is %s", displayString.c_str());

    return displayString;
}

std::string SessionDetails::getProfileDisplay(unsigned long screenNumber)
{
    // TD341 - should only change display when there is a change in primary operator or profile
    // i.e. new login, profile or operator change.  Should not change display when the override
    // is applied or removed
    if (!m_shouldChangeDisplay || (m_profile == NULL))
    {
        return "";
    }

    TRY
    {
        std::string display(m_profile->getDefaultDisplay(m_location->getKey(), screenNumber));
        LOG_INFO("Profile display on screen %d is %s", screenNumber, display.c_str());
        return display;
    }
    CATCH("profile or location display details")
    {
    }

    return "Unknown";
}

std::string SessionDetails::getProfileDisplayAtLocation(unsigned long screenNumber, const std::string& locationName)
{
    if (m_profile == NULL)
    {
        return "";
    }

    TRY
    {
        unsigned long locationKey = TA_Base_Core::LocationAccessFactory::getInstance().getLocationKeyFromName(locationName);
        std::string display(m_profile->getDefaultDisplay(locationKey, screenNumber));
        LOG_INFO("Profile display on screen %d at location %d is %s", screenNumber, locationKey, display.c_str());
        return display;
    }
    CATCH("profile or location display details")
    {
    }

    return "Unknown";
}

std::string SessionDetails::getLocationName()
{
    if (m_location == NULL)
    {
        return "";
    }

    TRY_CATCH(return m_location->getName());
    return "Unknown";
}

std::string SessionDetails::getLocationDisplayName()
{
    if (m_location == NULL)
    {
        m_location = m_consoleLocation.get();
    }

    TRY_CATCH(return m_location->getDisplayName());
    return "Unknown";
}

unsigned long SessionDetails::getLocationKey()
{
    FUNCTION_ENTRY("getLocationKey");
    FUNCTION_EXIT;
    return m_location->getKey();
}

std::string SessionDetails::getConsoleName()
{
    if (m_console == NULL)
    {
        return "";
    }

    TRY_CATCH(return m_console->getName());
    return "Unknown";
}

std::string SessionDetails::getConsoleLocationName()
{
    if (m_console == NULL)
    {
        return "";
    }

    std::string name = "Unknown";
    TRY_CATCH(name = m_console->getLocationName());
    return name;
}

std::string SessionDetails::getSessionId()
{
    return m_sessionId;
}

unsigned long SessionDetails::getConsoleId()
{
    TA_ASSERT(m_console != NULL, "The console is NULL");
    return m_console->getKey();
}

unsigned long SessionDetails::getServerLocation()
{
    TA_ASSERT(m_console != NULL, "The console is NULL");
    return m_console->getLocation();
}

unsigned long SessionDetails::getConsoleLocationId()
{
    TA_ASSERT(m_consoleLocation, "The console location is NULL");
    return m_consoleLocation->getKey();
}

void SessionDetails::processUpdate(const TA_Base_Core::ConfigUpdateDetails& updateEvent)
{
    FUNCTION_ENTRY("processUpdate");

    switch (updateEvent.getType())
    {
    case TA_Base_Core::LOCATION:
        processLocationUpdate(updateEvent);
        break;

    case TA_Base_Core::OPERATOR:
        processOperatorUpdate(updateEvent);
        break;

    case TA_Base_Core::PROFILE:
        processProfileUpdate(updateEvent);
        break;

    default:
        LOG_INFO("We're not registering for updates of this object type. Ignore");
        break;
    }

    FUNCTION_EXIT;
}

void SessionDetails::processOperatorUpdate(const TA_Base_Core::ConfigUpdateDetails& updateEvent)
{
    unsigned long key = updateEvent.getKey();

    try
    {
        switch (updateEvent.getModifications())
        {
        case TA_Base_Core::Create:
        case TA_Base_Core::Update:
        {
            bool found = false;

            // see if it is already there first
            for (OperatorVector::iterator iter = m_allOperators.begin(); iter != m_allOperators.end(); iter++)
            {
                if ((*iter)->getKey() == key)
                {
                    // update the operator object in our cache
                    (*iter)->invalidate();
                    found = true;
                    break;
                }
            }

            if (!found)
            {
                // if it's not there, just add it and just out of the case block
                LOG_INFO("Adding Operator %d", key);
                m_allOperators.push_back(TA_Base_Core::OperatorAccessFactory::getInstance().getOperator(key, false));
                break;
            }

            const std::vector<std::string> changes = updateEvent.getChangedParams();
            bool hasNameUpdated = false;

            for (unsigned int i = 0; i < changes.size(); i++)
            {
                // see if we got profile association changes
                if (changes[i].compare("Profiles") == 0)
                {
                    m_hasProfileUpdated = true;

                    // we can break if we've found a name change and a profile change.
                    // otherwise the search must go on..
                    if (hasNameUpdated)
                    {
                        break;
                    }
                }
                else if (changes[i].compare("Name") == 0)
                {
                    hasNameUpdated = true;

                    if (m_hasProfileUpdated)
                    {
                        break;
                    }
                }
            }

            // TD #11810
            // has the name updated for the current operator?
            // if so, we need to refresh the transactive taskbar to display the new name
            // TD 15092
            // if(hasNameUpdated && (m_operator->getKey() == key))
            if (hasNameUpdated && (m_operator != NULL) && (m_operator->getKey() == key))
            {
                //ControlStationRootDialog::getInstance().postMessage(WM_LOGIN_DETAILS_CHANGED);
                Signals::loginDetailChanged();
            }

            if (!m_hasProfileUpdated)
            {
                // if the profile has not been updated, we don't need to continue with the rest
                break;
            }

            // check to see if the login session or overriding session is using a profile that is no longer associated with the operator
            if ((m_operator != NULL) && (m_operator->getKey() == key))
            {
                std::vector<unsigned long> profileKeys = m_operator->getAssociatedProfileKeys();
                unsigned long currentProfile = m_profile->getKey();

                // If we can't find the current profile in the list of profile associated with the operator,
                // then the session is no longer valid
                if (boost::algorithm::none_of_equal(profileKeys, currentProfile))
                {
                    ControlStationRootDialog::getInstance().postMessage(WM_FORCE_LOGOUT);
                    LOG_INFO("The current profile of the login operator has been disassociated from that operator");
                    m_isSessionValid = false;
                }
            }
            else if ((m_operatorOverride != NULL) && (m_operatorOverride->getKey() == key))
            {
                std::vector<unsigned long> profileKeys = m_operatorOverride->getAssociatedProfileKeys();
                unsigned long overrideProfile = m_profile->getKey();

                // If we can't find the current profile in the list of profile associated with the operator,
                // then the session is no longer valid
                if (boost::algorithm::none_of_equal(profileKeys, overrideProfile))
                {
                    ControlStationRootDialog::getInstance().postMessage(WM_FORCE_END_OVERRIDE);
                    LOG_INFO("The current profile of the overriding operator has been disassociated from that operator");
                    m_isOverrideValid = false;
                }
            }
        }
        break;

        case TA_Base_Core::Delete:
        {
            // PW2743 - terminate the active session when the current operator is deleted
            // check if the operator is used by the current sessions first before deleting them
            if ((m_operatorOverride != NULL) && (key == m_operatorOverride->getKey()))
            {
                ControlStationRootDialog::getInstance().postMessage(WM_FORCE_END_OVERRIDE);
                LOG_INFO("Configuration of the operator that has overriden the current operator %d has been deleted", key);
                m_isOverrideValid = false;
            }
            else if ((m_operator != NULL) && (key == m_operator->getKey()))
            {
                ControlStationRootDialog::getInstance().postMessage(WM_FORCE_LOGOUT);
                LOG_INFO("Current operator %d has been deleted", key);
                m_isSessionValid = false;
            }

            for (OperatorVector::iterator iter = m_allOperators.begin(); iter != m_allOperators.end(); iter++)
            {
                if ((*iter)->getKey() == key)
                {
                    LOG_INFO("Deleting Operator %d from the Operator list", key);
                    delete (*iter);
                    m_allOperators.erase(iter);
                    break;
                }
            }
        }
        break;

        default:
            break;
        }

        m_hasOperatorUpdated = true;
    }
    catch (...)
    {
        LOG_EXCEPTION("DataException", str(boost::format("Could not access operator, %d") % key));
    }
}

void SessionDetails::processLocationUpdate(const TA_Base_Core::ConfigUpdateDetails& updateEvent)
{
    unsigned long key = updateEvent.getKey();
    LocationMap::iterator loc = m_locationMap.find(key);

    try
    {
        switch (updateEvent.getModifications())
        {
        case TA_Base_Core::Update:
            if (m_consoleLocation->getKey() == key)
            {
                m_consoleLocation->invalidate();
            }

        // just continue on to the create switch to update the location map
        case TA_Base_Core::Create:
            if (loc == m_locationMap.end())
            {
                // we haven't got it yet, so just reload
                LOG_INFO("Got online update for Location.  Adding new location, %d, into map", key);
                ILocation* newLoc = LocationAccessFactory::getInstance().getLocationByKey(key);
                m_locationMap.insert(LocationMap::value_type(key, newLoc));
            }
            else
            {
                // already got it so just update
                LOG_INFO("Got online update for Location.  Updating location, %d", key);
                loc->second->invalidate();
            }

            break;

        case TA_Base_Core::Delete:

            // PW2743 - terminate the active session when the current location is deleted
            // check if the location is used by the current sessions first before deleting them
            if ((m_location != NULL) && (key == m_location->getKey()))
            {
                ControlStationRootDialog::getInstance().postMessage(WM_FORCE_LOGOUT);
                LOG_INFO("Current location %d has been deleted", key);
                m_isSessionValid = false;
            }

            //TD17832 - marc_bugfix
            //Check first if a valid location was found
            if (loc != m_locationMap.end())
            {
                LOG_INFO("Got online update for Location.  Removing location, %d", key);

                delete loc->second;
                loc->second = NULL;
                m_locationMap.erase(key);
            }

            break;

        default:
            break;
        }

        m_hasLocationUpdated = true;
    }
    catch (...)
    {
        LOG_EXCEPTION("DataException", str(boost::format("Could not access location, %d") % key));
    }
}

void SessionDetails::processProfileUpdate(const TA_Base_Core::ConfigUpdateDetails& updateEvent)
{
    unsigned long key = updateEvent.getKey();
    ProfileMap::iterator prof = m_profileMap.find(key);

    try
    {
        switch (updateEvent.getModifications())
        {
        case TA_Base_Core::Create:
        case TA_Base_Core::Update:
        {
            if (prof == m_profileMap.end())
            {
                // we haven't got it yet, so just reload
                LOG_INFO("Got online update for Profile.  Adding new profile, %d, into map", key);
                IProfile* newProf = ProfileAccessFactory::getInstance().getProfile(key);
                m_profileMap.insert(ProfileMap::value_type(key, newProf));
                break;
            }

            // already got it so just update
            LOG_INFO("Got online update for Profile.  Updating profile, %d", key);
            prof->second->invalidate();

            const std::vector<std::string> changes = updateEvent.getChangedParams();

            for (unsigned int i = 0; i < changes.size(); i++)
            {
                // see if we got location association changes
                if (changes[i].find("Location") != -1)
                {
                    m_hasLocationUpdated = true;
                    break;
                }
            }

            if (!m_hasLocationUpdated)
            {
                // if the location has not been updated, we don't need to continue with the rest
                break;
            }

            // check to see if the login session or overriding session is using a location that is no longer associated with the profile
            if ((m_profile != NULL) && (m_profile->getKey() == key) && (m_profile->requiresLocationToBeSelected())) // sant TD14528
            {
                std::vector<unsigned long> locationKeys = m_profile->getAssociatedLocationKeys();
                unsigned long currentLocation = m_location->getKey();

                // If we can't find the current location in the list of location associated with the profile,
                // then the session is no longer valid
                if (boost::algorithm::none_of_equal(locationKeys, currentLocation))
                {
                    ControlStationRootDialog::getInstance().postMessage(WM_FORCE_LOGOUT);
                    LOG_INFO("The current location of the login profile has been disassociated from that profile");
                    m_isSessionValid = false;
                }
            }
            else if ((m_profileOverride != NULL) && (m_profileOverride->getKey() == key) && (m_profileOverride->requiresLocationToBeSelected())) //sant TD14528
            {
                std::vector<unsigned long> locationKeys = m_profileOverride->getAssociatedLocationKeys();
                unsigned long overrideLocation = m_location->getKey();

                // If we can't find the current location in the list of location associated with the profile,
                // then the session is no longer valid
                if (boost::algorithm::none_of_equal(locationKeys, overrideLocation))
                {
                    ControlStationRootDialog::getInstance().postMessage(WM_FORCE_END_OVERRIDE);
                    LOG_INFO("The current location of the overriding profile has been disassociated from that profile");
                    m_isOverrideValid = false;
                }
            }
        }
        break;

        case TA_Base_Core::Delete:

            // PW2743 - terminate the active session when the current profile is deleted
            // check if the profile is used by the current sessions first before deleting them
            if ((m_profileOverride != NULL) && (key == m_profileOverride->getKey()))
            {
                ControlStationRootDialog::getInstance().postMessage(WM_FORCE_END_OVERRIDE);
                LOG_INFO("Configuration of the profile that has overriden the current profile %d has been deleted", key);
                m_isOverrideValid = false;
            }
            else if ((m_profile != NULL) && (key == m_profile->getKey()))
            {
                ControlStationRootDialog::getInstance().postMessage(WM_FORCE_LOGOUT);
                LOG_INFO("Current profile %d has been deleted", key);
                m_isSessionValid = false;
            }

            // TD 12050
            if (prof != m_profileMap.end())
            {
                LOG_INFO("Got online update for Profile.  Removing profile, %d", key);
                delete prof->second;
                prof->second = NULL;
                m_profileMap.erase(key);
            }
            else
            {
                LOG(DEBUG_WARN, "Got online update (type=delete) for unknown profile: %d", key);
            }

            break;

        default:
            break;
        }

        // TD 12050
        m_hasProfileUpdated = true;
    }
    catch (...)
    {
        LOG_EXCEPTION("DataException", str(boost::format("Could not access profile, %d") % key));
    }
}

SessionDetails::SessionInfo SessionDetails::getSessionInfo()
{
    TA_ASSERT(m_sessionId.compare("") != 0, "The sessionId is empty");

    SessionInfo info;

    LOG_INFO("Got a session i.e. someone is logged in");

    TA_ASSERT(m_operator != NULL, "Operator shouldn't be NULL");
    TA_ASSERT(m_profile != NULL, "Profile shouldn't be NULL");
    TA_ASSERT(m_console != NULL, "Console shouldn't be NULL");

    info.operatorId = m_operator->getKey();
    info.profileId = m_profile->getKey();
    info.consoleId = m_console->getKey();

    if (m_profile->requiresLocationToBeSelected())
    {
        TA_ASSERT(m_location != NULL, "Location shouldn't be NULL");
        info.locationId = m_location->getKey();
    }

    info.password = m_operator->getPassword();

    if (m_operatorOverride == NULL)
    {
        LOG_INFO("No operator override, so just returning the login info");
        return info;
    }

    info.isOverriden = true;
    info.overrideOperatorId = m_operatorOverride->getKey();
    info.overridePassword = m_operatorOverride->getPassword();
    info.overrideProfileId = m_profileOverride->getKey();

    LOG_INFO("Got operator override so returning both login and override info");
    return info;
}

TA_Base_Bus::IAuthenticationAgentCorbaDef::SessionInfoCorbaDef SessionDetails::toCorbaSessionInfo()
{
    TA_ASSERT(m_sessionId.compare("") != 0, "The sessionId is empty");
    TA_ASSERT(m_operator != NULL, "Operator shouldn't be NULL");
    TA_ASSERT(m_profile != NULL, "Profile shouldn't be NULL");

    TA_Base_Bus::SessionInfo result;
    result.SessionId = m_sessionId;
    result.LocationId = m_location->getKey();
    result.UserId = m_operator->getKey();
    result.WorkstationId = m_console->getKey();
    result.OperatorId.push_back(m_operator->getKey());
    result.ProfileId.push_back(m_profile->getKey());

    if (m_operatorOverride != NULL)
    {
        result.OperatorId.push_back(m_operatorOverride->getKey());
        result.ProfileId.push_back(m_profileOverride->getKey());
    }

    return TA_Base_Bus::AuthenticationLibrary::convertCSessionToCORBASession(result);
}

void SessionDetails::setDisplayMode(bool displayMode)
{
    m_displayOnly = displayMode;
}
