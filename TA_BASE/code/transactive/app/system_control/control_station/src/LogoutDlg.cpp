/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/LogoutDlg.cpp $
  * @author:  San Teo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This is an extension of CDialog and is used to present user with
  * options of logging out, exitting control station and restarting workstation.
  * This class does not perform the above actions itself - that needs to be
  * done by whichever class that called it.
  */
#include "StdAfx.h"
#include "ControlStationConstants.h"
#include "LogoutDlg.h"
#include "Signals.h"
#include "QtUtilities.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"
#include "core/utilities/src/DebugUtil.h"
#include <QMovie>

using TA_Base_Core::DebugUtil;

LogoutDlg::LogoutDlg()
{
    ui.setupUi(this);
    QtUtilities::startMovie(ui.logoLabel, ":/transactiveintro.gif");

    connect(ui.m_exitButton, SIGNAL(clicked(bool)), this, SLOT(OnExitButton()));
    connect(ui.m_helpButton, SIGNAL(clicked(bool)), this, SLOT(OnHelpButton()));
    connect(ui.m_logoutButton, SIGNAL(clicked(bool)), this, SLOT(OnLogoutButton()));
    connect(ui.m_cancelButton, SIGNAL(clicked(bool)), this, SLOT(onCancelDialog()));

    setupSignalConnections();
}

void LogoutDlg::setupSignalConnections()
{
    LogoutDialogSignal::showDialog.connect(boost::bind(&LogoutDlg::doModal, this));
    LogoutDialogSignal::closeDialog.connect(boost::bind(&LogoutDlg::closeDialog, this));
    LogoutDialogSignal::waitLogoutInfo.connect(boost::bind(&LogoutDlg::waitLogoutInfo, this, _1));
}

void LogoutDlg::onInitDialog()
{
    LogoutDialogSignal::dialogShowed();
}

void LogoutDlg::onDialogClosed()
{
    m_logoutInfo->clear();
    LogoutDialogSignal::dialogClosed();
}

void LogoutDlg::waitLogoutInfo(LogoutInfoPromisePtr logoutInfo)
{
    m_logoutInfo = logoutInfo;
}

void LogoutDlg::OnLogoutButton()
{
    if (m_logoutInfo)
    {
        m_logoutInfo->set_value(LogoutInfo(LogoutInfo::Request::Logout));
    }
}

void LogoutDlg::OnExitButton()
{
    if (m_logoutInfo)
    {
        m_logoutInfo->set_value(LogoutInfo(LogoutInfo::Request::ExitControlStation));
    }
}

void LogoutDlg::onCancelDialog()
{
    if (m_logoutInfo)
    {
        m_logoutInfo->set_value(LogoutInfo());
    }
}

void LogoutDlg::OnHelpButton()
{
    TA_Base_Bus::HelpLauncher::getInstance().displayHelp(true);
}
