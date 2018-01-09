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
#include "StdAfx.h"
#include "SecurityChecker.h"
#include "core/data_access_interface/src/SecuritySettingAccessFactory.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/AuthenticationSecurityException.h"
#include "core/exceptions/src/AuthenticationAgentException.h"
#include "core/exceptions/src/RightsException.h"
#include "core/database/src/DBException.h"
#include "core/exceptions/src/DataException.h"

namespace TA_Base_App
{
    SecurityChecker::SecurityChecker()
    {
        readSecuritySetting();
    }

    void SecurityChecker::readSecuritySetting()
    {
        try
        {
            TA_Base_Core::SecuritySettingAccessFactory& factory = TA_Base_Core::SecuritySettingAccessFactory::getInstance();
            automatic_logout_time = factory.getAutomaticLogoutTime();
            failed_login_attempts = factory.getFailedLoginAttemtps();
            password_reuse_max = factory.getPasswordReuseMax();
            password_warn_time = factory.getPasswordWarnTime();
            password_reuse_time = factory.getPasswordReuseTime();
            password_life_time = factory.getPasswordLifeTime();
            password_lock_time = factory.getPasswordLockTime();
            LOG_INFO("The security settings are:\nautomatic_logout_time:%d\nfailed_login_attempts:%d\npassword_reuse_max:%d\npassword_warn_time:%d\npassword_reuse_time:%d\npassword_life_time:%d\npassword_lock_time:%d\n",
                automatic_logout_time,
                failed_login_attempts,
                password_reuse_max,
                password_warn_time,
                password_reuse_time,
                password_life_time,
                password_lock_time);
        }
        catch (TA_Base_Core::DataException&)
        {
            TA_THROW(TA_Base_Core::AuthenticationAgentException("A database error occured while attempting to get security Setting values"));
        }
        catch (TA_Base_Core::DatabaseException&)
        {
            TA_THROW(TA_Base_Core::AuthenticationAgentException("A database error occured while attempting to get security Setting values"));
        }
        catch (...)
        {
            TA_THROW(TA_Base_Core::AuthenticationAgentException("An unknown error occured while attempting to get security Setting values"));
        }
    }
}
