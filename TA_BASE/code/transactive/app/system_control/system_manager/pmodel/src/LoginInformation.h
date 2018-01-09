/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/TransActiveLoginDlg.h $
* @author:  Karen Graham
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
*
* This is the dialog used to allow the user to enter their login details. IT will then attempt to authenticate
*/
#pragma once
#include <vector>
#include <map>
#include <string>

namespace TA_Base_Core
{
    class IOperator;
    class ILocation;
    class IProfile;
    class IConsole;
}

namespace TA_Base_App
{
    struct LoginInformation
    {
    public:

        std::vector<std::string> getAllOperatorNames();
        std::vector<std::string> getProfileNamesForOperator(const std::string& operatorName);
        std::vector<std::string> getLocationDescriptionsForProfile(const std::string& profileName);
        size_t& getCurrentConsoleKey();
        std::string& getCurrentLocationDescription();
        std::vector<unsigned long>& getOperatorAssociatedProfileKeys(const std::string& operatorName);
        bool requiresLocationToBeSelected(const std::string& profileName);

    public:

        TA_Base_Core::IConsole* getCurrentConsole();
        TA_Base_Core::ILocation* getCurrentLocation();

        TA_Base_Core::IOperator* getOperator(const std::string& operatorName);
        std::vector<TA_Base_Core::IOperator*>& getAllOperators();
        std::map<size_t, TA_Base_Core::IOperator*>& getAllOperatorsKeyMap();
        std::map<std::string, TA_Base_Core::IOperator*>& getAllOperatorsNameMap();

        TA_Base_Core::IProfile* getProfile(const std::string& profileName);
        std::vector<TA_Base_Core::IProfile*>& getAllProfiles();
        std::map<size_t, TA_Base_Core::IProfile*>& getAllProfilesKeyMap();
        std::map<std::string, TA_Base_Core::IProfile*>& getAllProfilesNameMap();

        std::vector<TA_Base_Core::ILocation*>& getAllLocations();
        std::map<size_t, TA_Base_Core::ILocation*>& getAllLocationsKeyMap();
        std::map<std::string, TA_Base_Core::ILocation*>& getAllLocationsNameMap();
    };
}
