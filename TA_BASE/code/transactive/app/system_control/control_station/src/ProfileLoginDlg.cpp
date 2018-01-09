/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/ProfileLoginDlg.cpp $
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
#pragma warning(disable:4786)
#include "StdAfx.h"
#include "ProfileLoginDlg.h"
#include "QtUtilities.h"
#include "LoginDialogSignal.h"
#include "Signals.h"
#include "DataCache.h"
#include "Promise.h"
#include "SessionDetails.h"
#include "core/data_access_interface/src/IOperator.h"
#include "core/data_access_interface/src/IProfile.h"
#include "core/data_access_interface/src/ILocation.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/versioning/src/Version.h"
#include <QPixmap>
#include <QMovie>

using namespace TA_Base_Core;
using namespace TA_Base_App;

namespace TA_Base_Core
{
    extern char buildVersion[];
}

ProfileLoginDlg::ProfileLoginDlg()
{
    connect(ui.m_exitButton, SIGNAL(clicked(bool)), this, SLOT(onExitButton()));
    connect(ui.m_loginButton, SIGNAL(clicked(bool)), this, SLOT(onLoginButton()));

    setWindowTitle("Operator Login");
    ui.m_version->setText(QString::fromStdString(str(boost::format("Version %s") % TA_Base_Core::buildVersion)));
    ui.m_nameCombo->setEnabled(true);
    ui.m_displayOnlyCheck->setEnabled(true);
    ui.m_loginButton->setText("Login");
    ui.m_exitButton->setText("Exit");
    ui.m_exitButton->setEnabled(true);
    ui.m_restartButton->setEnabled(false);
    ui.m_helpButton->setEnabled(true);
    ui.m_restartButton->setEnabled(false);

    setupSignalConnections();
}

void ProfileLoginDlg::setupSignalConnections()
{
    LoginDialogSignal::showDialog.connect(boost::bind(&CsDialog::showDialog, this));
    LoginDialogSignal::closeDialog.connect(boost::bind(&CsDialog::closeDialog, this));
    LoginDialogSignal::waitLoginInfo.connect(boost::bind(&ProfileLoginDlg::waitLoginInfo, this, _1));
}

void ProfileLoginDlg::onInitDialog()
{
    LOG_INFO(BOOST_CURRENT_FUNCTION);
    LoginDialogSignal::dialogShowed();
    AbstructLoginDialog::onInitDialog();
}

void ProfileLoginDlg::onDialogClosed()
{
    m_loginInfo->clear();
    LoginDialogSignal::dialogClosed();
}

void ProfileLoginDlg::waitLoginInfo(LoginInfoPromisePtr loginInfo)
{
    m_loginInfo = loginInfo;
}

void ProfileLoginDlg::onLoginButton()
{
    if (!areDetailsValid())
    {
        return;
    }

    if (m_loginInfo)
    {
        LoginInfo info(m_selectedOperator, m_selectedProfile, m_selectedLocation, m_password.toStdString(), m_bDisplayOnly, LoginInfo::Request::Login);
        m_loginInfo->set_value(info);
    }
}

void ProfileLoginDlg::onExitButton()
{
    if (m_loginInfo)
    {
        m_loginInfo->set_value(LoginInfo());
    }
}

void ProfileLoginDlg::onCancelDialog()
{
    onExitButton();
}

void ProfileLoginDlg::onSelchangeProfileCombo()
{
    int selected = ui.m_profileCombo->currentIndex();

    if (selected == CB_ERR)
    {
        m_selectedProfile = NULL;
        m_selectedLocation = NULL;

        ui.m_locationCombo->setCurrentIndex(CB_ERR);
        ui.m_locationCombo->setEnabled(false);
        ui.m_passwordEdit->setEnabled(false);
        ui.m_profileCombo->setFocus();
        return;
    }

    // Update/Set the selected profile member variable
    m_selectedProfile = reinterpret_cast<IProfile*>(ui.m_profileCombo->itemData(selected).toUInt());

    m_selectedLocation = NULL;

    // Check if this profile requires a location to be selected.
    if (m_selectedProfile->requiresLocationToBeSelected())
    {
        populateLocationComboBox();

        // Set the default location
        int index = ui.m_locationCombo->findText(SessionDetails::getInstance().getLocationDisplayName().c_str());

        if (index == CB_ERR)
        {
            // just select the first index if it couldn't find the default one
            index = 0;
        }

        ui.m_locationCombo->setEnabled(true);
        ui.m_locationCombo->setCurrentIndex(index);
    }
    else // central profile - so disable location
    {
        ui.m_locationCombo->setCurrentIndex(CB_ERR);
        ui.m_locationCombo->setEnabled(false);
        ui.m_passwordEdit->setEnabled(true);
        m_selectedLocation = NULL;
    }

    if ((0 == m_selectedProfile->getName().compare("LTOC")) || (0 == m_selectedProfile->getName().compare("DSM")))
    {
        ui.m_displayOnlyCheck->setChecked(true);
        ui.m_displayOnlyCheck->setEnabled(false);
        m_bDisplayOnly = ui.m_displayOnlyCheck->isChecked();
    }
    else
    {
        ui.m_displayOnlyCheck->setEnabled(true);
    }

    refreshButtonAvailability();
}
