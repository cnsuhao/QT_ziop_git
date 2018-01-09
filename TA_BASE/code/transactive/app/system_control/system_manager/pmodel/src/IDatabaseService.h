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

namespace TA_Base_App
{
    struct ProcessInfo
    {
        ProcessInfo() : isManagedProcess(true), isServiceProcess(false), isAutoFailBack(false) {}
        std::string name;
        std::string description;
        bool isManagedProcess;
        bool isServiceProcess;
        bool isAutoFailBack;
    };

    struct IDatabaseService
    {
        virtual ProcessInfo& getProcessInfo(const std::string& name) = 0;
        virtual std::vector<std::string>& getHostnames() = 0;
        virtual std::vector<std::string> getAllOperatorNames() = 0;
        virtual std::vector<std::string> getProfileNamesForOperator(const std::string& operatorName) = 0;
        virtual std::vector<std::string> getLocationDescriptionsForProfile(const std::string& profileName) = 0;
        virtual size_t& getCurrentConsoleKey() = 0;
        virtual std::string& getCurrentLocationDescription() = 0;
        virtual std::vector<unsigned long>& getOperatorAssociatedProfileKeys(const std::string& operatorName) = 0;
        virtual bool requiresLocationToBeSelected(const std::string& profileName) = 0;
        virtual ~IDatabaseService() {}
    };
}
