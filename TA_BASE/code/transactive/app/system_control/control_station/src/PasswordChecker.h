#pragma once
#include "PasswordDlg.h"
#include "MyFuture.h"
#include "bus/security/authentication_library/src/AuthenticationLibrary.h"

namespace TA_Base_Core
{
    class IOperator; class IProfile; class ILocation;
    struct SecuritySetting;
    typedef boost::shared_ptr<SecuritySetting> SecuritySettingPtr;
}

class ProfileLoginDlg;

class PasswordChecker
{
public:

    void login(const ProfileLoginDlg& loginDlg);

    void login(TA_Base_Core::IOperator* theOperator,
               TA_Base_Core::IProfile* theProfile,
               TA_Base_Core::ILocation* theLocation,
               CString& password,
               bool displayOnly);

    bool PopUpChangePasswordDialog();
    void changePassword(const PasswordDlg& passwordDlg);

private:

    TA_Base_Core::SecuritySettingPtr m_securitySetting;
    PasswordDlg m_passwordDialog;
    MyFuture<TA_Base_Bus::AuthenticationLibrary> m_authenticationLibrary;
};
