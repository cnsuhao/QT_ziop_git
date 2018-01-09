/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/ProfileLoginDlg.h $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This dialog serves three purposes. It is firstly used as a Login dialog that will
  * be used each time the user wishes to log in to TransActive. Secondly, it is used
  * as a  Change Profile dialog that is used by an operator to select a different
  * profile. Finally, it is used as an Operator Override screen that is used to
  * augment the access rights of the logged in operator.
  */
#pragma once
#include "Singleton.h"
#include "AbstructLoginDialog.h"

class ProfileLoginDlg : public AbstructLoginDialog, public Singleton<ProfileLoginDlg>
{
    Q_OBJECT

protected slots:

    virtual void onLoginButton();
    virtual void onExitButton();
    virtual void onSelchangeProfileCombo();

protected:

    ProfileLoginDlg();
    void setupSignalConnections();
    virtual void onInitDialog() override;
    virtual void onCancelDialog() override;
    virtual void onDialogClosed() override;
    void waitLoginInfo(LoginInfoPromisePtr loginInfo);

protected:

    LoginInfoPromisePtr m_loginInfo;
};
