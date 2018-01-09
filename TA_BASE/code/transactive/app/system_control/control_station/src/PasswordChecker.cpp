#include "StdAfx.h"
#include "PasswordChecker.h"
#include "ControlStationConstants.h"
#include "RightsManager.h"
#include "resourceQt.h"
#include "Utilities.h"
#include "SessionDetails.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"
#include "core/data_access_interface/src/OperatorAccessFactory.h"
#include "core/exceptions/src/SessionException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/data_access_interface/src/IOperatorPassword.h"
#include "core/data_access_interface/src/OperatorPasswordAccessFactory.h"
#include "core/data_access_interface/src/SecuritySettingAccessFactory.h"
#include "core\exceptions\src\SessionException.h"
#include "core/exceptions/src/AuthenticationSecurityException.h"
#include "core/exceptions/src/AuthenticationAgentException.h"

using namespace TA_Base_Core;
using namespace TA_Base_App;

void PasswordChecker::login(const ProfileLoginDlg& loginDlg)
{
    TA_Base_Core::IOperator* theOperator = NULL;
    TA_Base_Core::IProfile*  theProfile = NULL;
    TA_Base_Core::ILocation*  theLocation = NULL;
    CString password;

    // if locked out, throw out
    LOG_INFO("SessionManager::login");
    TA_Base_Core::IOperatorPasswordPtr operatorPassword(TA_Base_Core::OperatorPasswordAccessFactory::getInstance().getOperatorPassword(theOperator->getKey(), false, false));
    int isLockedOut = operatorPassword->getLockedTimes();
    time_t lockedTime = operatorPassword->getLatestLockedTime();
    CTime cLockedTime(lockedTime);
    LOG_INFO("SessionManager::getLatestLockedTime done %d, %s", isLockedOut, cLockedTime.Format("%Y%m%d %H:%M:%S"));
    CTimeSpan timeSpan = CTime::GetCurrentTime() - cLockedTime;

    if (isLockedOut >= m_securitySetting->m_failed_login_attempts && timeSpan.GetTotalMinutes() <= m_securitySetting->m_password_lock_time)
    {
        LOG_INFO("SessionManager::first time login");
        TA_THROW(SessionException(SessionException::USER_LOCKED, "User is locked for failed to login several times"));
    }
}

void PasswordChecker::login(TA_Base_Core::IOperator* theOperator,
                            TA_Base_Core::IProfile* theProfile,
                            TA_Base_Core::ILocation* theLocation,
                            CString& password,
                            bool displayOnly)
{
    TA_Base_Core::IOperatorPasswordPtr operatorPassword(TA_Base_Core::OperatorPasswordAccessFactory::getInstance().getOperatorPassword(theOperator->getKey(), false, false));
    time_t latestLoginDate = operatorPassword->getLatestLoginDate();
    time_t latestModifiedDate = operatorPassword->getLatestModifiedDate();
    CTime loginDate(latestLoginDate);
    CTime modifiedDate(latestModifiedDate);
    CTimeSpan modifiedTimeSpan = CTime::GetCurrentTime() - modifiedDate;
    LOG_INFO("The loginDate is %d ; the modifiedTimespan is :%d", loginDate.GetYear(), modifiedTimeSpan.GetDays() >= m_securitySetting->m_password_life_time);

    boolean turnPasswordExpireNotificationOn = true;

    if (TA_Base_Core::RunParams::getInstance().isSet("PasswordExpireNotificationSwitch"))
    {
        std::string passwordEditNotification = TA_Base_Core::RunParams::getInstance().get("PasswordExpireNotificationSwitch");
        LOG_INFO("The passwordEditNotification status is %s", passwordEditNotification.c_str());

        if (passwordEditNotification.compare("OFF") == 0 || passwordEditNotification.compare("off") == 0 || passwordEditNotification.compare("Off") == 0)
        {
            turnPasswordExpireNotificationOn = false;
        }
    }

    if (turnPasswordExpireNotificationOn)
    {
        if ((loginDate.GetYear() == 1970) || (modifiedTimeSpan.GetDays() >= m_securitySetting->m_password_life_time))
        {
            if (loginDate.GetYear() == 1970)
            {
                showMsgBox(IDS_UE_620024, theOperator->getName());
            }
            else if (modifiedTimeSpan.GetDays() >= m_securitySetting->m_password_life_time)
            {
                showMsgBox(IDS_UE_620025);
            }

            LOG_INFO("PopUpChangePasswordDialog");

            bool isSuccessfulChange;

            do
            {
                LOG_INFO("PopUpChangePasswordDialog => inWhileLoop");
                isSuccessfulChange = PopUpChangePasswordDialog();
            }
            while (!isSuccessfulChange);

            LOG_INFO("PopUpChangePasswordDialog Done");
        }
        else if ((modifiedTimeSpan.GetDays() < m_securitySetting->m_password_life_time) && (modifiedTimeSpan.GetDays() >= (m_securitySetting->m_password_life_time - m_securitySetting->m_password_warn_time)))
        {
            std::ostringstream outputMessage;
            outputMessage << ((LONGLONG)m_securitySetting->m_password_life_time - modifiedTimeSpan.GetDays());
            int resp = showMsgBox(IDS_UE_620026, outputMessage.str());

            if (resp == IDOK)
            {
                LOG_INFO("PopUpChangePasswordDialog => inWhileLoop");
                PopUpChangePasswordDialog();
            }
        }
    }
}

bool PasswordChecker::PopUpChangePasswordDialog()
{
    bool isSuccessfulChange = false;

    //TD641 - for reason similar to TD653 see the comment in onLogout
    //if (m_passwordDialog.m_hWnd != NULL)
    //{
    //    LOG_INFO("Already have 1 instance of change password dialog opened");
    //    return false;
    //}

    // Check if current operator is allowed to change password
    TA_ControlStation::EPermission permission = RightsManager::getInstance().isControlPermitted(TA_Base_Bus::aca_CONTROL_STATION_CHANGE_PASSWORD);

    CString actionName = IDS_CHANGE_PASSWORD;

    if (permission == TA_ControlStation::DENIED)
    {
        LOG_INFO("Operator tries to change password without the right permission");
        showMsgBox(IDS_UE_010021, Utilities::getOperatorNameFromSession(), actionName);
        //return S_OK;
        return false;
    }
    else if (permission == TA_ControlStation::UNKNOWN)
    {
        LOG_INFO("The system could not determine the rights of the current operator to change password");
        showMsgBox(IDS_UE_020056, actionName);
        //return S_OK;
        return false;
    }
    else if (permission == TA_ControlStation::GENERAL_ERROR)
    {
        LOG_INFO("Some kind of error has occurred while checking for rights to change password");
        showMsgBox(IDS_UE_010018, actionName);
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
    m_passwordDialog.setDialogType(TA_ControlStation::FORCEDTOCHANGEPASSWORD);

    do
    {
        // Call doModal on the dialog box. This will display and wait for the run()
        // method to complete before allowing the user to do anything.
        int response = m_passwordDialog.doModal();

        if (response == QDialog::Rejected)
        {
            LOG_INFO("User has cancelled the change password");
            break;
        }

        try
        {
            //BeginWaitCursor(); // display the hourglass cursor
            LOG_INFO("SessionManager::PopUpChangePasswordDialog():changepassword");

            changePassword(m_passwordDialog);
            LOG_INFO("SessionManager::PopUpChangePasswordDialog():changepassword successfully");

            isSuccessfulChange = true;
        }
        catch (const SessionException& ex)
        {
            CString agentName = IDS_AUTHENTICATION_AGENT;
            CString password = IDS_PASSWORD;

            switch (ex.getFailType())
            {
            case SessionException::AUTHENTICATION_FAILED:
                showMsgBox(IDS_UE_020046, actionName);
                break;

            case SessionException::AUTHENTICATION_AGENT_FAILED:
                showMsgBox(IDS_UE_030064, actionName, agentName);
                break;

            case SessionException::INVALID_PASSWORD_LENGTH:
                showMsgBox(IDS_UE_020055, password, TA_ControlStation::MIN_PASSWORD_LENGTH);
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

    return isSuccessfulChange;
}

void PasswordChecker::changePassword(const PasswordDlg& passwordDlg)
{
    FUNCTION_ENTRY("changePassword");

    TA_Base_Core::IOperator* theOperator = NULL;
    CString oldPassword, newPassword1, newPassword2;

    passwordDlg.getSelectedDetails(theOperator, oldPassword, newPassword1, newPassword2);

    // if the session is no longer valid then just throw an exception to prevent user
    // from continuing on
    if (!SessionDetails::getInstance().isSessionValid())
    {
        TA_THROW(SessionException(SessionException::INVALID_SESSION_CONFIGURATION, "Trying to change password when the session configuration has become invalid"));
    }

    try
    {
        // if both new passwords are not empty and the same
        if ((newPassword1 != "") && (newPassword1 == newPassword2))
        {
            // if new passwords are shorter than what's allowed
            if ((newPassword1.GetLength() < TA_ControlStation::MIN_PASSWORD_LENGTH) || (newPassword1.GetLength() > TA_ControlStation::MAX_PASSWORD_LENGTH))
            {
                TA_THROW(SessionException(SessionException::INVALID_PASSWORD_LENGTH, "Password is shorter than minimum length"));
            }

            m_authenticationLibrary->changePassword(SessionDetails::getInstance().getSessionId(), (LPCSTR)oldPassword, (LPCSTR)newPassword1);
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
            LOG_EXCEPTION("AuthenticationSecurityException::INVALID_PASSWORD", "Converting into a SessionException::CHANGE_PASSWORD_FAILED");
            // We shouldn't really get this since we've checked the length of the password in PasswordDlg, but
            // if for some reason it wasn't caught there,
            TA_THROW(SessionException(SessionException::INVALID_PASSWORD_LENGTH, "Failed to change password"));
        }
        else if (ex.getReason() == AuthenticationSecurityException::PASSWORD_USEDBEFORE)
        {
            LOG_EXCEPTION("AuthenticationSecurityException::PASSWORD_USEDBEFORE", "Converting into a SessionException::PASSWORD_USEDBEFORE");
            // We shouldn't really get this since we've checked the length of the password in PasswordDlg, but
            // if for some reason it wasn't caught there,
            TA_THROW(SessionException(SessionException::PASSWORD_USEDBEFORE, "Password has been used before"));
        }
        else  // Any other exceptions, just throw it as a general AUTHENTICATION_FAILED exception
        {
            LOG_EXCEPTION("AuthenticationSecurityException", "Converting into a SessionException");
            TA_THROW(SessionException(SessionException::AUTHENTICATION_FAILED, "Failed to authenticate to change password"));
        }
    }
    catch (const AuthenticationAgentException&)
    {
        LOG_EXCEPTION("AuthenticationAgentException", "Converting into a SessionException");
        TA_THROW(SessionException(SessionException::GENERAL_ERROR, "Authentication Agent failed when trying to change password"));
    }
    catch (...) // whatever exception that we got, just make it SessionException
    {
        LOG_EXCEPTION("Unknown Exception", "Some unknown exception has occurred while attempting to change password. Converting into a SessionException");
        TA_THROW(SessionException(SessionException::GENERAL_ERROR, "Unknown error encountered when attempting to change password"));
    }

    FUNCTION_EXIT;
}
