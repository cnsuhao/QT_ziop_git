/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/SystemControllerService.h $
* @author:  Dominique Beauregard
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
*
* AdminManager is a singleton class that is the central class of the
* System Manager. It retreives data from a System Controller and
* sends the GUI's orders to that System Controller.
*/
#pragma once
#include "core/process_management/IDL/src/ISystemControllerAdminCorbaDef.h"
#include <string>
#include <vector>

namespace TA_Base_App
{
    struct IProcessDataObserver;

    struct ISystemControllerService
    {
        virtual void refresh() = 0;
        virtual void startPullProcessData() = 0;
        virtual void stopPullProcessData() = 0;
        virtual void setCurrentSystemController(const std::string& hostname, unsigned long port) = 0;
        virtual void registerProcessDataObserver(const std::string& hostname, IProcessDataObserver* observer) = 0;
        virtual void unregisterProcessDataObserver(const std::string& hostname, IProcessDataObserver* observer) = 0;
        virtual void setRefreshRate(unsigned int newRate) = 0;
        virtual void startProcess(const std::string& processName) = 0;
        virtual void startProcess(const std::vector<std::string>& processNames) = 0;
        virtual void changeProcessOperationMode(const std::string& processName, TA_Base_Core::EOperationMode opMode) = 0;
        virtual void changeProcessOperationMode(const std::vector<std::string>& processNames, TA_Base_Core::EOperationMode opMode) = 0;
        virtual void stopProcess(const std::string& processName) = 0;
        virtual void stopProcess(const std::vector<std::string>& processNames) = 0;
        virtual void shutdownSystemController(const std::string& hostname) = 0;
        virtual void setProcessRunParam(const std::string& processName, const TA_Base_Core::RunParam& param) = 0;
        virtual void setProcessRunParam(const std::vector<std::string>& processNames, const TA_Base_Core::RunParam& param) = 0;
    };
}
