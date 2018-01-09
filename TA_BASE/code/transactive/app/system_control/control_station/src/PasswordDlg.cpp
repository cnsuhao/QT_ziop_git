/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/PasswordDlg.cpp $
  * @author:  San Teo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Dialog box which allows the current operator to change password
  */

#include "StdAfx.h"
#include "QtUtilities.h"
#include "SessionDetails.h"
#include "controlstation.h"
#include "PasswordDlg.h"
#include "ControlStation.h"
#include "SessionManager.h"
#include "PasswordDlg.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/SessionException.h"
#include "core/data_access_interface/src/IOperator.h"
#include "core/utilities/src/DebugUtil.h"

using TA_Base_App::TA_ControlStation::APPLICATION_NAME;
using TA_Base_Core::DebugUtil;
using TA_Base_Core::DataException;
using TA_Base_Core::DatabaseException;
using TA_Base_Core::SessionException;
using TA_Base_App::TA_ControlStation::MAX_PASSWORD_LENGTH;

PasswordDlg::PasswordDlg()
    : m_useExistingData(false)  //TD726 - need to set this to false so that the old password will get focus the first time
{
    ui.setupUi(this);

    connect(ui.m_newPasswordEdit, SIGNAL(textEdited(const QString&)), this, SLOT(OnChangeNewPasswordEdit()));
    connect(ui.m_oldPasswordEdit, SIGNAL(textEdited(const QString&)), this, SLOT(OnChangePasswordEdit()));
    connect(ui.m_confirmPasswordEdit, SIGNAL(textEdited(const QString&)), this, SLOT(OnChangeConfirmPasswordEdit()));
    connect(ui.m_confirmPasswordEdit, SIGNAL(textEdited(const QString&)), this, SLOT(OnChangeConfirmPasswordEdit()));

    connect(ui.m_helpButton, SIGNAL(clicked(bool)), this, SLOT(OnHelpButton()));
    connect(ui.m_cancelButton, SIGNAL(clicked(bool)), this, SLOT(reject()));
    connect(ui.m_okButton, SIGNAL(clicked(bool)), this, SLOT(OnOK()));
}

void PasswordDlg::OnChangePasswordEdit()
{
    m_oldPassword = ui.m_oldPasswordEdit->text();
    ui.m_newPasswordEdit->setEnabled(!m_oldPassword.isEmpty());

    // clear the passwords
    ui.m_newPasswordEdit->setText("");
    ui.m_confirmPasswordEdit->setText("");

    ui.m_confirmPasswordEdit->setEnabled(FALSE);
    ui.m_okButton->setEnabled(FALSE);
}

void PasswordDlg::OnChangeNewPasswordEdit()
{
    m_newPassword1 = ui.m_newPasswordEdit->text();
    ui.m_confirmPasswordEdit->setEnabled(!m_newPassword1.isEmpty());
    ui.m_confirmPasswordEdit->setText("");
    ui.m_okButton->setEnabled(FALSE);
}

void PasswordDlg::OnChangeConfirmPasswordEdit()
{
    m_newPassword2 = ui.m_confirmPasswordEdit->text();
    ui.m_okButton->setEnabled(!m_newPassword2.isEmpty());
}

void PasswordDlg::onInitDialog()
{
    //Display dialog at the specified monitor
    //CPoint pt(GetMessagePos());
    QPoint pt = pos();
    LOG_INFO("Rect info- %d", pt.x());

    playLogo();

    populateNameComboBox();

    // Set the name to the current operator
    //TD15552++
    ui.m_nameCombo->setCurrentIndex(ui.m_nameCombo->findText(SessionDetails::getInstance().getOperatorName().c_str())); //TD15528s
    //++TD15552

    // Make the Window always on top.
    //SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);

    // limit our password length
    ui.m_oldPasswordEdit->setMaxLength(MAX_PASSWORD_LENGTH);
    ui.m_newPasswordEdit->setMaxLength(MAX_PASSWORD_LENGTH);
    ui.m_confirmPasswordEdit->setMaxLength(MAX_PASSWORD_LENGTH);

    m_newPassword1 = "";
    m_newPassword2 = "";
    ui.m_newPasswordEdit->setText("");
    ui.m_confirmPasswordEdit->setText("");

    if (m_useExistingData)
    {
        ui.m_oldPasswordEdit->setText(m_oldPassword);
        ui.m_newPasswordEdit->setFocus(); // if we already got the old password, then set focus to the new password
        m_useExistingData = false;
        return;
    }
}

void PasswordDlg::OnOK()
{
    m_oldPassword = ui.m_oldPasswordEdit->text();
    m_newPassword1 = ui.m_newPasswordEdit->text();
    m_newPassword2 = ui.m_confirmPasswordEdit->text();
    stopLogo();
    QDialog::accept();
}

void PasswordDlg::getSelectedDetails(
    TA_Base_Core::IOperator*& selectedOperator, CString& oldPassword,
    CString& newPassword1, CString& newPassword2) const
{
    oldPassword = m_oldPassword.toStdString().c_str();
    newPassword1 = m_newPassword1.toStdString().c_str();
    newPassword2 = m_newPassword2.toStdString().c_str();
}

void PasswordDlg::populateNameComboBox()
{
    BLOCK_SIGNALS(ui.m_nameCombo);

    ui.m_nameCombo->clear();
    SessionDetails& details = SessionDetails::getInstance();
    SessionDetails::OperatorVector& operators = details.getListOfOperators();

    for (TA_Base_Core::IOperator* oprtor : details.getListOfOperators())
    {
        if (oprtor)
        {
            try
            {
                ui.m_nameCombo->insertItem(ui.m_nameCombo->count(),
                                           QtUtilities::convertToUnicode<QString>(oprtor->getName()),
                                           reinterpret_cast<unsigned int>(oprtor));
            }
            catch (...)
            {
            }
        }
    }
}

void PasswordDlg::OnHelpButton()
{
    TA_Base_Bus::HelpLauncher::getInstance().displayHelp(true);
}

void PasswordDlg::OnCancel(WPARAM, LPARAM)
{
    QDialog::reject();
}

void PasswordDlg::setupMessageMap()
{
    m_messageMap[WM_FORCE_CANCEL] = boost::bind(&PasswordDlg::OnCancel, this, _1, _2);
}
