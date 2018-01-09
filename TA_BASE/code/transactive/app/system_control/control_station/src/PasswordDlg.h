/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/PasswordDlg.h $
  * @author:  San Teo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Dialog box which allows the current operator to change password
  */

#pragma once
#include "ui_PasswordDlg.h"
#include "CsDialog.h"
#include "app/system_control/control_station/src/resourceQt.h"
#include "app/system_control/control_station/src/ControlStationConstants.h"

// Forward declare classes
namespace TA_Base_Core
{
    class IOperator;
    class IProfile;
    class ILocation;
}

/////////////////////////////////////////////////////////////////////////////
// PasswordDlg dialog

class PasswordDlg : public CsDialog
{
    Q_OBJECT

public:

    PasswordDlg();

    /**
     * getSelectedDetails
     *
     * This is called to retrieve the details the user has entered into the password dialog.
     * This should be called after the user has clicked OK.
     *
     * @param IOperator*& - A reference to an IOperator pointer. This is because we want to update
     *                      the pointer passed in to be the same as the operator pointer we have
     *                      stored. (This is an OUT parameter)
     * @param std::string& oldPassword - Old password that user has entered
     * @param std::string& newPassword1 - New password that user has entered the first time
     * @param std::string& newPassword2 - New password that user has entered the second time
     */
    void getSelectedDetails(
        TA_Base_Core::IOperator*& selectedOperator, CString& oldPassword,
        CString& newPassword1, CString& newPassword2) const;

    /**
     * useExistingDataOnNextDoModal
     *
     * This method should be called if the next time the dialog is shown it should
     * automatically populate itself with the old password that was entered the
     * last time the dialog was shown
     */
    void useExistingDataOnNextDoModal()
    {
        m_useExistingData = true;
    }

    /**
    * setDialogType
    *
    * This sets the dialog to a Login screen.
    *
    * @param EDialogMode - The type of dialog to set up.
    */
    void setDialogType(TA_Base_App::TA_ControlStation::EDialogMode mode)
    {
        m_mode = mode;
    }

    CAnimateCtrl m_introMovie;

public slots:

    void OnChangeNewPasswordEdit();
    void OnChangePasswordEdit();
    void OnChangeConfirmPasswordEdit();
    void onInitDialog();
    void OnOK();
    void OnHelpButton();
    void OnSysCommand(UINT nID, LPARAM lParam) {}
    void OnCancel(WPARAM, LPARAM);
    virtual void setupMessageMap();

private:
    /**
     * populateNameComboBox
     *
     * This will populate the operator name combo box with a list of operator descriptions
     * retrieved from the SessionDetails object.
     */
    void populateNameComboBox();
    void playLogo() {}
    void stopLogo() {}

    TA_Base_App::TA_ControlStation::EDialogMode m_mode;

    // old password string
    QString m_oldPassword;

    // new password string entered the first time
    QString m_newPassword1;

    // new password string entered the second time
    QString m_newPassword2;

    // When the dialog is displayed to the user it may need to be displayed using the
    // old password (when there is a mismatch in the new password).  We then
    // display the dialog with the old password previously typed in.
    bool m_useExistingData;

    Ui::PasswordDlg ui;
};
