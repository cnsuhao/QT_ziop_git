/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/TransActiveLoginDlg.cpp $
* @author:  Karen Graham
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
*
* This is the dialog used to allow the user to enter their login details. IT will then attempt to authenticate
*/

#include "LoginInformation.h"
#include "core/utilities/src/Hostname.h"
#include "core/data_access_interface/src/IOperator.h"
#include "core/data_access_interface/src/OperatorAccessFactory.h"
#include "core/data_access_interface/src/ILocation.h"
#include "core/data_access_interface/src/LocationAccessFactory.h"
#include "core/data_access_interface/src/IProfile.h"
#include "core/data_access_interface/src/ProfileAccessFactory.h"
#include "core/data_access_interface/entity_access/src/IConsole.h"
#include "core/data_access_interface/entity_access/src/ConsoleAccessFactory.h"
#include <boost/foreach.hpp>


namespace TA_Base_App
{
    std::vector<std::string> LoginInformation::getAllOperatorNames()
    {
        std::vector<std::string> names;

        std::map<std::string, TA_Base_Core::IOperator*>& operators = getAllOperatorsNameMap();

        for (std::map<std::string, TA_Base_Core::IOperator*>::iterator it = operators.begin(); it != operators.end(); ++it)
        {
            names.push_back(it->first);
        }

        return names;
    }


    std::vector<std::string> LoginInformation::getProfileNamesForOperator(const std::string& operatorName)
    {
        std::vector<std::string> names;
        TA_Base_Core::IOperator* opr = getOperator(operatorName);

        if (NULL == opr)
        {
            return names;
        }

        std::vector<unsigned long>& keys = getOperatorAssociatedProfileKeys(operatorName);
        std::map<size_t, TA_Base_Core::IProfile*>& profiles = getAllProfilesKeyMap();

        BOOST_FOREACH(unsigned long key, keys)
        {
            std::map<size_t, TA_Base_Core::IProfile*>::iterator it = profiles.find(key);

            if (it != profiles.end())
            {
                names.push_back(it->second->getName());
            }
        }

        return names;
    }


    std::vector<std::string> LoginInformation::getLocationDescriptionsForProfile(const std::string& profileName)
    {
        std::vector<std::string> descriptions;
        TA_Base_Core::IProfile* profile = getProfile(profileName);

        if (NULL == profile)
        {
            return descriptions;
        }

        std::vector<unsigned long> keys = profile->getAssociatedLocationKeys();
        std::map<size_t, TA_Base_Core::ILocation*>& locations = getAllLocationsKeyMap();

        BOOST_FOREACH(unsigned long key, keys)
        {
            std::map<size_t, TA_Base_Core::ILocation*>::iterator it = locations.find(key);

            if (it != locations.end())
            {
                descriptions.push_back(it->second->getDescription());
            }
        }

        return descriptions;
    }


    TA_Base_Core::IOperator* LoginInformation::getOperator(const std::string& operatorName)
    {
        std::map<std::string, TA_Base_Core::IOperator*>& operators = getAllOperatorsNameMap();
        std::map<std::string, TA_Base_Core::IOperator*>::iterator it = operators.find(operatorName);

        if (it != operators.end())
        {
            return it->second;
        }

        return NULL;
    }


    std::vector<TA_Base_Core::IOperator*>& LoginInformation::getAllOperators()
    {
        static std::vector<TA_Base_Core::IOperator*> operators;

        if (operators.empty())
        {
            try
            {
                operators = TA_Base_Core::OperatorAccessFactory::getInstance().getAllNonSystemOperators(false, false);
            }
            catch (...)
            {
            }
        }

        return operators;
    }


    std::map<std::string, TA_Base_Core::IOperator*>& LoginInformation::getAllOperatorsNameMap()
    {
        static std::map<std::string, TA_Base_Core::IOperator*> operatorsNameMap;

        if (operatorsNameMap.empty())
        {
            std::vector<TA_Base_Core::IOperator*>& operators = getAllOperators();

            BOOST_FOREACH(TA_Base_Core::IOperator * opr, operators)
            {
                operatorsNameMap[opr->getName()] = opr;
            }
        }

        return operatorsNameMap;
    }


    std::map<size_t, TA_Base_Core::IOperator*>& LoginInformation::getAllOperatorsKeyMap()
    {
        static std::map<size_t, TA_Base_Core::IOperator*> operatorsKeyMap;

        if (operatorsKeyMap.empty())
        {
            std::vector<TA_Base_Core::IOperator*>& operators = getAllOperators();

            BOOST_FOREACH(TA_Base_Core::IOperator * opr, operators)
            {
                operatorsKeyMap[opr->getKey()] = opr;
            }
        }

        return operatorsKeyMap;
    }

    std::vector<TA_Base_Core::ILocation*>& LoginInformation::getAllLocations()
    {
        static std::vector<TA_Base_Core::ILocation*> locations;

        if (locations.empty())
        {
            try
            {
                locations = TA_Base_Core::LocationAccessFactory::getInstance().getAllLocations(false);
            }
            catch (...)
            {
            }
        }

        return locations;
    }


    std::map<std::string, TA_Base_Core::ILocation*>& LoginInformation::getAllLocationsNameMap()
    {
        static std::map<std::string, TA_Base_Core::ILocation*> locationsNameMap;

        if (locationsNameMap.empty())
        {
            std::vector<TA_Base_Core::ILocation*>& locations = getAllLocations();

            BOOST_FOREACH(TA_Base_Core::ILocation * location, locations)
            {
                locationsNameMap[location->getName()] = location;
            }
        }

        return locationsNameMap;
    }


    std::map<size_t, TA_Base_Core::ILocation*>& LoginInformation::getAllLocationsKeyMap()
    {
        static std::map<size_t, TA_Base_Core::ILocation*> locationsKeyMap;

        if (locationsKeyMap.empty())
        {
            std::vector<TA_Base_Core::ILocation*>& locations = getAllLocations();

            BOOST_FOREACH(TA_Base_Core::ILocation * location, locations)
            {
                locationsKeyMap[location->getKey()] = location;
            }
        }

        return locationsKeyMap;
    }


    TA_Base_Core::IProfile* LoginInformation::getProfile(const std::string& profileName)
    {
        std::map<std::string, TA_Base_Core::IProfile*>& profiles = getAllProfilesNameMap();
        std::map<std::string, TA_Base_Core::IProfile*>::iterator it = profiles.find(profileName);

        if (it != profiles.end())
        {
            return it->second;
        }

        return NULL;
    }


    std::vector<TA_Base_Core::IProfile*>& LoginInformation::getAllProfiles()
    {
        static std::vector<TA_Base_Core::IProfile*> profiles;

        if (profiles.empty())
        {
            try
            {
                profiles = TA_Base_Core::ProfileAccessFactory::getInstance().getAllNonSystemProfiles(false);
            }
            catch (...)
            {
            }
        }

        return profiles;
    }


    std::map<size_t, TA_Base_Core::IProfile*>& LoginInformation::getAllProfilesKeyMap()
    {
        static std::map<size_t, TA_Base_Core::IProfile*> profilesKeyMap;

        if (profilesKeyMap.empty())
        {
            std::vector<TA_Base_Core::IProfile*>& profiles = getAllProfiles();

            BOOST_FOREACH(TA_Base_Core::IProfile * profile, profiles)
            {
                profilesKeyMap[profile->getKey()] = profile;
            }
        }

        return profilesKeyMap;
    }


    std::map<std::string, TA_Base_Core::IProfile*>& LoginInformation::getAllProfilesNameMap()
    {
        static std::map<std::string, TA_Base_Core::IProfile*> profilesNameMap;

        if (profilesNameMap.empty())
        {
            std::vector<TA_Base_Core::IProfile*>& profiles = getAllProfiles();

            BOOST_FOREACH(TA_Base_Core::IProfile * profile, profiles)
            {
                profilesNameMap[profile->getName()] = profile;
            }
        }

        return profilesNameMap;
    }


    std::vector<unsigned long>& LoginInformation::getOperatorAssociatedProfileKeys(const std::string& operatorName)
    {
        static std::map< std::string, std::vector<unsigned long> > associatedProfileKeys;
        std::map< std::string, std::vector<unsigned long> >::iterator it = associatedProfileKeys.find(operatorName);

        if (it == associatedProfileKeys.end())
        {
            TA_Base_Core::IOperator* oper = getOperator(operatorName);

            if (oper)
            {
                associatedProfileKeys[operatorName] = oper->getAssociatedProfileKeys();
                return associatedProfileKeys[operatorName];
            }

            static std::vector<unsigned long> empty;
            return empty;
        }

        return it->second;
    }


    bool LoginInformation::requiresLocationToBeSelected(const std::string& profileName)
    {
        TA_Base_Core::IProfile* profile = getProfile(profileName);

        if (profile)
        {
            return profile->requiresLocationToBeSelected();
        }

        return false;
    }


    size_t& LoginInformation::getCurrentConsoleKey()
    {
        static size_t consoleKey = 0;

        if (0 == consoleKey)
        {
            TA_Base_Core::IConsole* console = getCurrentConsole();

            if (console)
            {
                consoleKey = console->getKey();
            }
        }

        return consoleKey;
    }


    std::string& LoginInformation::getCurrentLocationDescription()
    {
        static std::string locationDescription;

        if (locationDescription.empty())
        {
            TA_Base_Core::ILocation* location = getCurrentLocation();

            if (location)
            {
                locationDescription = location->getDescription();
            }
        }

        return locationDescription;
    }


    TA_Base_Core::IConsole* LoginInformation::getCurrentConsole()
    {
        static TA_Base_Core::IConsole* console = NULL;

        if (NULL == console)
        {
            try
            {
                console = TA_Base_Core::ConsoleAccessFactory::getInstance().getConsoleFromAddress(TA_Base_Core::Hostname::getHostname());
            }
            catch (...)
            {
            }
        }

        return console;
    }


    TA_Base_Core::ILocation* LoginInformation::getCurrentLocation()
    {
        static TA_Base_Core::ILocation* location = NULL;

        if (NULL == location)
        {
            TA_Base_Core::IConsole* console = getCurrentConsole();

            if (console)
            {
                try
                {
                    location = TA_Base_Core::LocationAccessFactory::getInstance().getLocationByKey(console->getLocation());
                }
                catch (...)
                {
                }
            }
        }

        return location;
    }
}
