/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/LogoutDlg.h $
  * @author:  San Teo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This is an extension of CDialog and is used to present user with
  * options of logging out, exitting control station and restarting workstation
  */

#pragma once
#include "ui_LogoutDlg.h"
#include "LogoutDialogSignal.h"
#include "ControlStationConstants.h"
#include "CsDialog.h"
#include "Singleton.h"

class LogoutDlg : public CsDialog, public Singleton<LogoutDlg>
{
    Q_OBJECT

public slots:

    void OnLogoutButton();
    void OnExitButton();
    void OnHelpButton();

protected:

    LogoutDlg();
    void setupSignalConnections();
    virtual void onInitDialog() override;
    virtual void onDialogClosed() override;
    virtual void onCancelDialog() override;
    void waitLogoutInfo(LogoutInfoPromisePtr logoutInfo);

protected:

    Ui::LogoutDlg ui;
    LogoutInfoPromisePtr m_logoutInfo;
};
