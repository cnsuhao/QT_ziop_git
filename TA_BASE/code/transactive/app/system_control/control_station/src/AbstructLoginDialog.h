/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/AbstructLoginDialog.h $
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
#include "ui_ProfileLoginDlg.h"
#include "CsDialog.h"
#include "resourceQt.h"
#include "Promise.h"
#include "TypeDefines.h"
#include "ControlStationConstants.h"
#include "LoginInfo.h"

using namespace TA_Base_App;
typedef Promise<TA_ControlStation::EPermission> PromisePermission;
typedef boost::shared_ptr<PromisePermission> PromisePermissionPtr;

class AbstructLoginDialog : public CsDialog
{
    Q_OBJECT

public:

    virtual bool getIsDisplayOnly() const { return m_bDisplayOnly; }

public slots:

    virtual void onHelpButton();
    virtual void onSelchangeNameCombo();
    virtual void onSelchangeProfileCombo() = 0;
    virtual void onSelchangeLocationCombo();
    virtual void onChangePasswordEdit();
    virtual void onDropdownNameCombo();
    virtual void onDropdownProfileCombo();
    virtual void onDropdownLocationCombo();
    virtual void OnDisplayOnlyClicked();
    virtual void onSystemControllerStateChange();

protected:

    AbstructLoginDialog();

    virtual void onInitDialog() override;
    bool isTdsProfile(std::string profile_str);
    virtual void populateNameComboBox();
    virtual void populateLocationComboBox();
    virtual void populateProfileComboBox();
    virtual void refreshButtonAvailability();
    virtual bool hasFilledAllDetails();
    virtual void drawControls();
    virtual bool areDetailsValid();
    virtual bool isLocationValidToBePopulated(TA_Base_Core::ILocation* locaiton, unsigned long minLocalLocationOrderId, unsigned long maxLocalLocationOrderId);
    virtual void getPreviousNextLocationOrderId();
    virtual std::vector<unsigned long> getStationLocationOrderIdList();
    virtual void playLogo() {}
    virtual void stopLogo() {}
    virtual void addAboutMenu() {}
    virtual bool shouldShowRestartButton();

protected:

    static std::string USE_TDS_PROFILE;
    static boost::regex TDS_PATTERN;
    std::string m_dbConnection;
    unsigned long m_previousLocationOrderId;
    unsigned long m_nextLocationOrderId;
    static TA_Base_Core::IOperator* m_selectedOperator;
    static TA_Base_Core::IProfile*  m_selectedProfile;
    static TA_Base_Core::ILocation*  m_selectedLocation;
    static QString m_password;
    static bool m_bDisplayOnly;
    static LoginInfo m_loginInfo;
    Ui::ProfileLoginDlg ui;
    boost::once_flag m_populateNameComboBoxOnce;
};
