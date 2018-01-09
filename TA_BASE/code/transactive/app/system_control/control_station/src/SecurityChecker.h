/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/ControlStationAppController.h $
* @author:  Justin Ebedes
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
*
* This is the System Manager implementation of the
* GenericGUI IApplicationController interfaces.
*
*/
#pragma once
#include <string>

namespace TA_Base_App
{
    struct SecurityChecker
    {
    public:

        SecurityChecker();

    private:

        void readSecuritySetting();

    private:

        int automatic_logout_time;
        int failed_login_attempts;
        int password_reuse_max;
        int password_warn_time;
        int password_reuse_time;
        int password_life_time;
        int password_lock_time;
    };
}
