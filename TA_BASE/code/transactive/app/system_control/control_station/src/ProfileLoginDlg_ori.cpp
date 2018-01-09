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
#include "QtUtilities.h"
#include "LoginDialogSignal.h"
#include "Signals.h"
#include "DataCache.h"
#include "Promise.h"
#include "SystemControllerMessageHandler.h"
#include "app/system_control/control_station/src/ControlStation.h"
#include "app/system_control/control_station/src/ProfileLoginDlg.h"
#include "app/system_control/control_station/src/SessionManager.h"
#include "app/system_control/control_station/src/ControlStationConstants.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/SessionException.h"
#include "core/exceptions/src/GraphWorxException.h"
#include "core/data_access_interface/src/IOperator.h"
#include "core/data_access_interface/src/IProfile.h"
#include "core/data_access_interface/src/ILocation.h"
#include "core/data_access_interface/src/ProfileAccessFactory.h"
#include "core/data_access_interface/src/SessionAccessFactory.h"
#include "core/data_access_interface/src/ISession.h"
#include "core/data_access_interface/src/LocationAccessFactory.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/versioning/src/Version.h"

using namespace TA_Base_Core;
using namespace TA_Base_App;

std::string ProfileLoginDlg::USE_TDS_PROFILE("UseTdsProfile");
boost::regex ProfileLoginDlg::TDS_PATTERN("^[A-Z,0-9]+[[:space:]]*\\(TRA\\)$|^[A-Z]+-TDS[[:space:]]*\\([A-Z,a-z]+\\)$");

// External versioning variables. See VersionGen.cpp
namespace TA_Base_Core
{
    extern char buildVersion[];
}

/////////////////////////////////////////////////////////////////////////////
// ProfileLoginDlg dialog

ProfileLoginDlg& ProfileLoginDlg::getInstance()
{
    return *ACE_Unmanaged_Singleton<ProfileLoginDlg, ACE_Null_Mutex>::instance();
}

void ProfileLoginDlg::setupSignalConnections()
{
    LoginDialogSignal::closeDialog.connect(boost::bind(&ProfileLoginDlg::closeDialog, this));
    LoginDialogSignal::showDialog.connect(boost::bind(&ProfileLoginDlg::showLoginDialog, this));
    Signals::operatorRequestShowChangeProfileDialog.connect(boost::bind(&ProfileLoginDlg::showChangeProfileDialog, this));
    Signals::operatorRequestShowChangeDisplayModeDialog.connect(boost::bind(&ProfileLoginDlg::showChangeDisplayModeDialog, this));
    Signals::operatorRequestShowChangePasswordDialog.connect(boost::bind(&ProfileLoginDlg::showChangePasswordDialog, this));
    Signals::operatorRequestShowOperatorOverrideDialog.connect(boost::bind(&ProfileLoginDlg::showOperatorOverrideDialog, this));
}

void ProfileLoginDlg::showLoginDialog()
{
    m_mode = TA_ControlStation::LOGIN_MODE;
    LoginDialogSignal::preShowDialog();
    DoModal();
}

void ProfileLoginDlg::showChangeProfileDialog()
{
    m_mode = TA_ControlStation::CHANGE_PROFILE_MODE;
    DoModal();
}

void ProfileLoginDlg::showChangeDisplayModeDialog()
{
    m_mode = TA_ControlStation::CHANGE_DISPLAY_MODE;
    DoModal();
}

void ProfileLoginDlg::showChangePasswordDialog()
{
    m_mode = TA_ControlStation::CHANGEPASSWORD;
    DoModal();
}

void ProfileLoginDlg::showOperatorOverrideDialog()
{
    m_mode = TA_ControlStation::OPERATOR_OVERRIDE_MODE;
    DoModal();
}

ProfileLoginDlg::ProfileLoginDlg()
    : m_selectedOperator(NULL),
      m_selectedProfile(NULL),
      m_selectedLocation(NULL),
      m_dbConnection(""),
      m_dbConnectionString(""),
      m_mode(TA_ControlStation::LOGIN_MODE),
      m_isWindowActivated(false),
      m_useExistingData(false),
      m_terminateCode(TA_ControlStation::SHUTDOWN), // shutdown by default if the control station couldn't start up properly
      m_isRestartAlwaysEnabled(false),
      m_isExitAlwaysEnabled(false),
      m_bDisplayOnly(false)
{
    ui.setupUi(this);
    ui.m_loginButton->setEnabled(false);
    ui.m_exitButton->setEnabled(false);

    connect(ui.m_exitButton, SIGNAL(clicked(bool)), this, SLOT(onExit()));
    connect(ui.m_helpButton, SIGNAL(clicked(bool)), this, SLOT(onHelpButton()));
    connect(ui.m_loginButton, SIGNAL(clicked(bool)), this, SLOT(onLoginButton()));
    connect(ui.m_restartButton, SIGNAL(clicked(bool)), this, SLOT(onRestartButton()));
    connect(ui.m_displayOnlyCheck, SIGNAL(clicked(bool)), this, SLOT(OnDisplayOnlyClicked()));
    connect(ui.m_nameCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelchangeNameCombo()));
    connect(ui.m_profileCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelchangeProfileCombo()));
    connect(ui.m_locationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelchangeLocationCombo()));
    connect(ui.m_passwordEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onChangePasswordEdit()));
    connect(this, SIGNAL(activateWindow_signal()), this, SLOT(activateWindow()));
}

ProfileLoginDlg::~ProfileLoginDlg()
{
}

void ProfileLoginDlg::setupMessageMap()
{
    m_messageMap[WM_FORCE_CANCEL] = boost::bind(&ProfileLoginDlg::OnCancel, this, _1, _2);
    m_messageMap[WM_SYSTEMCONTROLLER_STATE_CHANGE] = boost::bind(&ProfileLoginDlg::onSystemControllerStateChange, this, _1, _2);
}

int ProfileLoginDlg::exec()
{
    static bool firstTime = true;

    if (firstTime)
    {
        Signals::loginDialogFirstShowed();
    }

    Signals::loginDialogShowed();

    int result = QDialog::exec();
    firstTime = false;
    Signals::loginDialogClosed();
    return result;
}

void ProfileLoginDlg::onInitDialog()
{
    playLogo();
    makeWindowAlwaysOnTop();
    adjustWindowPosition();
    addAboutMenu();

    ui.m_passwordEdit->setMaxLength(TA_ControlStation::MAX_PASSWORD_LENGTH);
    ui.m_helpButton->setEnabled(false);
    ui.m_displayOnlyCheck->setCheckState(m_bDisplayOnly ? Qt::Checked : Qt::Unchecked);
    ui.m_displayOnlyCheck->setEnabled(false);

    // First save the current selections as they will be reset once we start changing things.
    QString operatorEntry;
    QString profileEntry;
    QString locationEntry;

    // Select previous data if required
    if (m_useExistingData)
    {
        try
        {
            if (m_selectedOperator != NULL)
            {
                operatorEntry = m_selectedOperator->getName().c_str(); //TD15528

                if (m_selectedProfile != NULL)
                {
                    profileEntry = m_selectedProfile->getName().c_str();

                    if (m_selectedProfile->requiresLocationToBeSelected() && (m_selectedLocation != NULL))
                    {
                        locationEntry = m_selectedLocation->getDisplayName().c_str();
                    }
                }
            }
        }
        catch (const DatabaseException&)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "DatabaseException", "Thrown while trying to retrieve details to set the combo box selections in the Login Dialog");
        }
        catch (const DataException&)
        {
            LOG(SourceInfo, DebugUtil::ExceptionCatch, "DataException", "Thrown while trying to retrieve details to set the combo box selections in the Login Dialog");
        }
        catch (...)
        {
            // just in case the selected item has been deleted
        }
    }

    // activate and display the controls
    drawControls();

    // It is only the first time the dialogue is loaded that we need to wait for
    // the SessionManager to finish its processing. After this the window will always
    // be active so we can call the activateWindow method to ensure that everything
    // is enabled/disabled correctly.
    activateWindow();

    // Select previous data if required
    if (m_useExistingData)
    {
        ui.m_nameCombo->setCurrentIndex(ui.m_nameCombo->findText(operatorEntry));
        ui.m_profileCombo->setCurrentIndex(ui.m_profileCombo->findText(profileEntry));
        ui.m_locationCombo->setCurrentIndex(ui.m_locationCombo->findText(locationEntry));
        ui.m_passwordEdit->setFocus();
        m_useExistingData = false;
    }
}

void ProfileLoginDlg::onSelchangeNameCombo()
{
    // Get the key for the currently selected operator name
    int selected = ui.m_nameCombo->currentIndex();

    if (selected == CB_ERR)
    {
        m_selectedOperator = NULL;
        m_selectedProfile = NULL;
        m_selectedLocation = NULL;

        // disable the buttons
        ui.m_profileCombo->setEnabled(false);
        ui.m_locationCombo->setEnabled(false);
        ui.m_passwordEdit->setEnabled(false);

        // clear the selections
        ui.m_profileCombo->setCurrentIndex(CB_ERR);
        ui.m_locationCombo->setCurrentIndex(CB_ERR);
        ui.m_passwordEdit->setText("");

        ui.m_nameCombo->setFocus();
        refreshButtonAvailability();

        return;
    }

    // Update/Set the selected operator member variable
    m_selectedOperator = reinterpret_cast<IOperator*>(ui.m_nameCombo->itemData(selected).toUInt());
    m_selectedProfile = NULL;
    m_selectedLocation = NULL;

    // Because we are going to populate the profile combo boxes regardless
    // of whether the list have been updated, we'll just call these methods so
    // that they won't be updated again when we give focus to the combos
    // TD 12050
    SessionDetails::getInstance().resetProfileUpdateFlag();

    // Reload the profile combo box with relevant profiles for this operator
    populateProfileComboBox();

    // enable/disable the buttons
    ui.m_profileCombo->setEnabled(true);

    // TD1847 - if there's only 1 profile, then just select it
    if (1 == ui.m_profileCombo->count())
    {
        ui.m_profileCombo->setCurrentIndex(0);
        //onSelchangeProfileCombo();
        ui.m_passwordEdit->setText("");
        return;
    }

    ui.m_locationCombo->setEnabled(false);
    ui.m_passwordEdit->setEnabled(false);

    // clear the selections
    ui.m_profileCombo->setCurrentIndex(CB_ERR);
    ui.m_locationCombo->setCurrentIndex(CB_ERR);
    ui.m_passwordEdit->setText("");

    refreshButtonAvailability();
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

        // PW2844, TD1847 - Only enable the combo box if it is not in operator override mode
        if (m_mode == TA_ControlStation::OPERATOR_OVERRIDE_MODE)
        {
            // Set the current login location
            int index = ui.m_locationCombo->findText(SessionDetails::getInstance().getLocationDisplayName().c_str());

            if (index == CB_ERR)
            {
                //TD15554++

                //found a local profile
                //to perform an override console location key must equal session location
                if (SessionDetails::getInstance().getConsoleLocationName() != SessionDetails::getInstance().getLocationName())
                {
                    index = CB_ERR;
                }
                else
                {
                    index = ui.m_locationCombo->findText(SessionDetails::getInstance().getLocationDisplayName().c_str());
                }

                //++TD15554
            }

            // if we're able to select either the login or console location, then set the selection
            if (index != CB_ERR)
            {
                ui.m_locationCombo->setCurrentIndex(index);
                // Update/Set the selected location member variable - otherwise
                // the session manager will think that this is a central profile.
                m_selectedLocation = reinterpret_cast<ILocation*>(ui.m_locationCombo->itemData(index).toUInt());
                ui.m_passwordEdit->setEnabled(true);
            }
            else
            {
                m_selectedLocation = NULL;
                ui.m_passwordEdit->setEnabled(false); //TD11707 - only enable it if the profile is associated with the login location
            }

            ui.m_locationCombo->setEnabled(false);
        }
        else
        {
            // Set the default location
            int index = ui.m_locationCombo->findText(SessionDetails::getInstance().getLocationDisplayName().c_str());

            if (index == CB_ERR)
            {
                // just select the first index if it couldn't find the default one
                index = 0;
            }

            ui.m_locationCombo->setEnabled(true);
            ui.m_locationCombo->setCurrentIndex(index);
            //onSelchangeLocationCombo();
        }
    }
    else // central profile - so disable location
    {
        ui.m_locationCombo->setCurrentIndex(CB_ERR);
        ui.m_locationCombo->setEnabled(false);
        ui.m_passwordEdit->setEnabled(true);
        m_selectedLocation = NULL;
    }

    switch (m_mode)
    {
    case TA_ControlStation::LOGIN_MODE:
    {
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
    }
    break;

    case TA_ControlStation::CHANGE_PROFILE_MODE:
    {
        if ((0 == m_selectedProfile->getName().compare("LTOC")) || (0 == m_selectedProfile->getName().compare("DSM")))
        {
            ui.m_displayOnlyCheck->setChecked(true);
            ui.m_displayOnlyCheck->setEnabled(false);
            m_bDisplayOnly = ui.m_displayOnlyCheck->isChecked();
        }
        else
        {
            m_bDisplayOnly = SessionDetails::getInstance().getDisplayMode();
            ui.m_displayOnlyCheck->setCheckState(m_bDisplayOnly ? Qt::Checked : Qt::Unchecked);
            ui.m_displayOnlyCheck->setEnabled(true);
        }
    }
    break;

    case TA_ControlStation::CHANGE_DISPLAY_MODE:
    {
        if ((0 == m_selectedProfile->getName().compare("LTOC")) || (0 == m_selectedProfile->getName().compare("DSM")))
        {
            ui.m_displayOnlyCheck->setChecked(true);
            ui.m_displayOnlyCheck->setEnabled(false);
            m_bDisplayOnly = ui.m_displayOnlyCheck->isChecked();
        }
        else
        {
            m_bDisplayOnly = SessionDetails::getInstance().getDisplayMode();
            ui.m_displayOnlyCheck->setCheckState(m_bDisplayOnly ? Qt::Checked : Qt::Unchecked);
            ui.m_displayOnlyCheck->setEnabled(true);
        }
    }
    break;

    default:
        break;
    }

    refreshButtonAvailability();
}

void ProfileLoginDlg::onSelchangeLocationCombo()
{
    // Don't worry about it if the location combo is disabled - this call
    // is probably called by ourselves explicitly, so just ignore
    if (!ui.m_locationCombo->isEnabled())
    {
        // TD18944
        //m_selectedLocation = NULL;
        return;
    }

    int selected = ui.m_locationCombo->currentIndex();

    if (selected == CB_ERR)
    {
        m_selectedLocation = NULL;
        ui.m_passwordEdit->setEnabled(false);
        ui.m_locationCombo->setFocus();
        return;
    }

    // Update/Set the selected location member variable
    m_selectedLocation = reinterpret_cast<ILocation*>(ui.m_locationCombo->itemData(selected).toUInt());
    ui.m_passwordEdit->setEnabled(true);
    refreshButtonAvailability();
}

void ProfileLoginDlg::onChangePasswordEdit()
{
    m_password = ui.m_passwordEdit->text();
    refreshButtonAvailability();
}

void ProfileLoginDlg::onLoginButton()
{
    if ((m_selectedOperator == NULL) || (m_selectedProfile == NULL) || (m_password.isEmpty()))
    {
        // We are not ready to login yet
        // We should never get in here as the enabling and disabling should prevent us.
        // We have to check anyway though.
        return;
    }

    if (!areDetailsValid())
    {
        return;
    }

    stopLogo();

    PromiseVoidPtr promise(new PromiseVoid);

    switch (m_mode)
    {
    case TA_ControlStation::CHANGE_PROFILE_MODE:
        Signals::operatorRequestChangeProfile();
        Signals::operatorRequestChangeProfileInfo(m_selectedOperator, m_selectedProfile, m_selectedLocation, m_password.toStdString(), m_bDisplayOnly, promise);
        break;

    case TA_ControlStation::OPERATOR_OVERRIDE_MODE:
        Signals::operatorRequestOperatorOverride();
        Signals::operatorRequestOperatorOverrideInfo(m_selectedOperator, m_selectedProfile, m_selectedLocation, m_password.toStdString(), m_bDisplayOnly, promise);
        break;

    case TA_ControlStation::CHANGE_OPERATOR_MODE:
        Signals::operatorRequestChangeOperator();
        Signals::operatorRequestChangeOperatorInfo(m_selectedOperator, m_selectedProfile, m_selectedLocation, m_password.toStdString(), m_bDisplayOnly, promise);
        break;

    case TA_ControlStation::CHANGE_DISPLAY_MODE:
        break;

    case TA_ControlStation::LOGIN_MODE:
    default:
        Signals::operatorRequestLogin();
        Signals::operatorRequestLoginInfo(m_selectedOperator, m_selectedProfile, m_selectedLocation, m_password.toStdString(), m_bDisplayOnly, promise);
        break;
    }

    QDialog::accept();
}

bool ProfileLoginDlg::areDetailsValid()
{
    TA_ASSERT(m_selectedOperator != NULL, "Must select an operator");
    TA_ASSERT(m_selectedProfile != NULL, "Must select a profile");

    if (SessionDetails::getInstance().hasOperatorUpdated())
    {
        bool found = false;

        try
        {
            SessionDetails::OperatorVector operators = SessionDetails::getInstance().getListOfOperators();

            for (SessionDetails::OperatorVector::iterator iter = operators.begin(); iter != operators.end(); iter++)
            {
                // if we found the operator, then it must have been updated, it's ok
                if ((*iter != NULL) && ((*iter)->getKey() == m_selectedOperator->getKey()))
                {
                    found = true;
                    break;
                }
            }
        }
        catch (...)
        {
            // just in case the actual object got deleted from another thread
        }

        if (!found)
        {
            CString itemName = IDS_OPERATOR;
            showMsgBox(IDS_UE_020057, itemName);
            int sel = CB_ERR;
            onDropdownNameCombo();
            ui.m_nameCombo->setCurrentIndex(sel);
            //onSelchangeNameCombo();
            return false;
        }
    }

    if (SessionDetails::getInstance().hasProfileUpdated())
    {
        bool found = false;

        try
        {
            bool isCentral = false;
            SessionDetails& details = SessionDetails::getInstance();

            if (details.getConsoleLocationId() == 1)
            {
                isCentral = true;
            }

            SessionDetails::ProfileVector profiles =
                SessionDetails::getInstance().getListOfProfiles(m_selectedOperator, isCentral, (m_mode == TA_ControlStation::OPERATOR_OVERRIDE_MODE));

            for (SessionDetails::ProfileVector::iterator iter = profiles.begin(); iter != profiles.end(); iter++)
            {
                // if we found the profile, then it must have been updated, it's ok
                if ((*iter != NULL) && ((*iter)->getKey() == m_selectedProfile->getKey()))
                {
                    found = true;
                    break;
                }
            }
        }
        catch (...)
        {
            // just in case the actual object got deleted from another thread
        }

        if (!found)
        {
            CString itemName = IDS_PROFILE;
            showMsgBox(IDS_UE_020057, itemName);
            int sel = CB_ERR;
            onDropdownProfileCombo();
            ui.m_profileCombo->setCurrentIndex(sel);
            //onSelchangeProfileCombo();
            return false;
        }
    }

    if (SessionDetails::getInstance().hasLocationUpdated() && m_selectedProfile->requiresLocationToBeSelected())
    {
        TA_ASSERT(m_selectedLocation != NULL, "Must select an operator");
        bool found = false;

        try
        {
            SessionDetails::LocationVector locations = SessionDetails::getInstance().getListOfLocations(m_selectedProfile);

            for (SessionDetails::LocationVector::iterator iter = locations.begin(); iter != locations.end(); iter++)
            {
                // if we found the profile, then it must have been updated, it's ok
                if ((*iter != NULL) && ((*iter)->getKey() == m_selectedLocation->getKey()))
                {
                    found = true;
                    break;
                }
            }
        }
        catch (...)
        {
            // just in case the actual object got deleted from another thread
        }

        if (!found)
        {
            CString itemName = IDS_LOCATION;
            showMsgBox(IDS_UE_020057, itemName);
            int sel = CB_ERR;
            onDropdownLocationCombo();
            ui.m_locationCombo->setCurrentIndex(sel);
            //onSelchangeLocationCombo();
            return false;
        }
    }

    return true;
}

void ProfileLoginDlg::OnCancel(WPARAM, LPARAM)
{
    // TD11102 - Do nothing when the operator hits the 'ESC' button
    if (m_mode != TA_ControlStation::LOGIN_MODE)
    {
        onExit();
    }
}

void ProfileLoginDlg::onExit()
{
//limin todo
#if 0

    // If this is not the login dialog, then it's just a normal
    // cancel action, just call CDialog's OnCancel
    if (m_mode == TA_ControlStation::LOGIN_MODE)
    {
        if (!areDetailsValid())
        {
            return;
        }

        m_terminateCode = TA_ControlStation::EXIT;
    }

#endif
    m_terminateCode = TA_ControlStation::EXIT;
    stopLogo();
    QDialog::reject();
}

void ProfileLoginDlg::onHelpButton()
{
    TA_Base_Bus::HelpLauncher::getInstance().displayHelp(true);
}

void ProfileLoginDlg::onRestartButton()
{
    if (!areDetailsValid())
    {
        return;
    }

    m_terminateCode = TA_ControlStation::RESTART;
    stopLogo();

    QDialog::reject();
    // We'll close the dialog box just as though we had a Cancel event.
    // Calling CDialog::OnCancel() will make DoModal return IDCANCEL
}

/////////////////////////////////////////////////////////////////////////////
// Other Methods
/////////////////////////////////////////////////////////////////////////////

void ProfileLoginDlg::getSelectedDetails(TA_Base_Core::IOperator*& selectedOperator,
                                         TA_Base_Core::IProfile*& selectedProfile,
                                         TA_Base_Core::ILocation*& selectedLocation,
                                         std::string& password) const
{
    selectedOperator = m_selectedOperator;
    selectedProfile  = m_selectedProfile;
    selectedLocation  = m_selectedLocation;
    password = m_password.toStdString();
}

bool ProfileLoginDlg::hasFilledAllDetails()
{
    m_password = ui.m_passwordEdit->text();
    // Because of the way we enable and disable our fields, the only
    // way all fields could be filled in is when the user has filled
    // the password field.
    // TD18944
    return (ui.m_passwordEdit->isEnabled() && !m_password.isEmpty() && m_password.length() >= 8);
}

void ProfileLoginDlg::refreshButtonAvailability()
{
    bool enable = hasFilledAllDetails();
    ui.m_loginButton->setEnabled(enable);

    try
    {
        if (DataCache::getInstance().getControlStationEntity()->shouldShowRestartButton())
        {
            // enable the restart button if it's supposed to be enabled always
            // or when all the details are filled in
            ui.m_restartButton->setEnabled(m_isRestartAlwaysEnabled || enable);
        }
    }
    catch (...)
    {
        LOG_EXCEPTION_CATCH(SourceInfo, "DAI Exception", "Could not determine if restart button should be shown so it will be shown");
        // enable the restart button if it's supposed to be enabled always
        // or when all the details are filled in
        ui.m_restartButton->setEnabled(m_isRestartAlwaysEnabled || enable);
    }

    if (m_mode == TA_ControlStation::LOGIN_MODE)
    {
        ui.m_exitButton->setEnabled(m_isExitAlwaysEnabled || enable);
    }
}

void ProfileLoginDlg::populateNameComboBox()
{
    ui.m_nameCombo->clear();

    BOOST_FOREACH(TA_Base_Core::IOperator* oprtor, SessionDetails::getInstance().getListOfOperators())
    {
        if (oprtor)
        {
            try
            {
                BLOCK_SIGNALS(ui.m_nameCombo);

                // Add each operator name to the combo box and store the IOperator* as the associated data.
                ui.m_nameCombo->insertItem(ui.m_nameCombo->count(),
                                           QtUtilities::convertToUnicode<QString>(oprtor->getName()),
                                           reinterpret_cast<unsigned int>(oprtor));
            }
            catch (...)
            {
            }
        }
    }

    ui.m_nameCombo->setCurrentIndex(-1);
}

void ProfileLoginDlg::populateLocationComboBox()
{
    ui.m_locationCombo->clear();

    if (NULL == m_selectedProfile)
    {
        return;
    }

    bool isOnline = SystemControllerMessageHandler::getInstance().isGroupOnline();
    std::string consoleLocation = SessionDetails::getInstance().getConsoleLocationName().c_str();
    unsigned long locationkey = SessionDetails::getInstance().getConsoleLocationId();
    boost::shared_ptr<TA_Base_Core::ILocation> location(TA_Base_Core::LocationAccessFactory::getInstance().getLocationByKey(locationkey));
    SessionDetails::LocationVector locations;

    try
    {
        locations = SessionDetails::getInstance().getListOfLocations(m_selectedProfile);
    }
    catch (...)
    {
        LOG(SourceInfo, DebugUtil::ExceptionCatch, "DataException", "Could not load the list of available operators or locations.");
    }

    std::vector<unsigned long> localOrderIdList = getStationLocationOrderIdList();
    unsigned long minLocalLocationOrderId = localOrderIdList[0];
    unsigned long maxLocalLocationOrderId = localOrderIdList[localOrderIdList.size() - 1];
    LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugInfo, "maxLocalLocationOrderId:%u,minLocalLocationOrderId:%u", maxLocalLocationOrderId, minLocalLocationOrderId);

    BOOST_FOREACH(TA_Base_Core::ILocation* location, locations)
    {
        if (location != NULL && location->getKey() != 0)
        {
            try
            {
                BLOCK_SIGNALS(ui.m_locationCombo);

                std::string name = location->getName();
                std::string displayName = location->getDisplayName();

                if (isOnline)
                {
                    //get the m_previousLocationid and m_nextLocationid
                    getPreviousNextLocationOrderId();

                    // if it is a station, only shows the current, prevous and next location(first station only current and nect,
                    //while last station only current and previous, DT1 and DT16 also shows TMF station)
                    if (isStation(location->getName()) && isLocationValidToBePopulated(location, minLocalLocationOrderId, maxLocalLocationOrderId))
                    {
                        ui.m_locationCombo->insertItem(ui.m_locationCombo->count(),
                                                       QtUtilities::convertToUnicode<QString>(displayName),
                                                       reinterpret_cast<unsigned int>(location));
                    }
                    //if it is a depot, show all locations.
                    else if (isDepot(location->getName()) && isDepot(SessionDetails::getInstance().getConsoleLocationName()))
                    {
                        ui.m_locationCombo->insertItem(ui.m_locationCombo->count(),
                                                       QtUtilities::convertToUnicode<QString>(displayName),
                                                       reinterpret_cast<unsigned int>(location));
                    }
                }
                else if (!isOnline && (consoleLocation == name))
                {
                    ui.m_locationCombo->insertItem(ui.m_locationCombo->count(),
                                                   QtUtilities::convertToUnicode<QString>(displayName),
                                                   reinterpret_cast<unsigned int>(location));
                }
            }
            catch (...)
            {
            }
        }
    }

    ui.m_locationCombo->setCurrentIndex(-1);
}

void ProfileLoginDlg::populateProfileComboBox()
{
    ui.m_profileCombo->clear();

    if (m_selectedOperator == NULL)
    {
        return;
    }

    bool usetds = TA_Base_Core::RunParams::getInstance().isSet(USE_TDS_PROFILE.c_str());
    bool isCentral = false;
    SessionDetails& details = SessionDetails::getInstance();

    if (details.getConsoleLocationId() == 1)
    {
        isCentral = true;
    }

    LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugInfo, "details.getConsoleLocationId() %lu", details.getConsoleLocationId());

    SessionDetails::ProfileVector& profiles = details.getListOfProfiles(m_selectedOperator, isCentral, (m_mode == TA_ControlStation::OPERATOR_OVERRIDE_MODE));

    BOOST_FOREACH(TA_Base_Core::IProfile* profile, profiles)
    {
        if (profile != NULL)
        {
            try
            {
                std::string name = profile->getName();

                if (isTdsProfile(name) && !usetds)
                {
                    continue;
                }

                BLOCK_SIGNALS(ui.m_locationCombo);

                ui.m_profileCombo->insertItem(ui.m_profileCombo->count(),
                                              QtUtilities::convertToUnicode<QString>(name),
                                              reinterpret_cast<unsigned int>(profile));
            }
            catch (...)
            {
            }
        }
    }

    // TD1146, TD1572 - Remove the current profile from profile list
    if (//(m_mode == TA_ControlStation::CHANGE_PROFILE_MODE) ||
        m_mode == TA_ControlStation::OPERATOR_OVERRIDE_MODE)
    {
        // Remove the profile from the list of profiles available
        ui.m_profileCombo->removeItem(ui.m_profileCombo->findText(SessionDetails::getInstance().getProfileName().c_str()));
    }

    ui.m_profileCombo->setCurrentIndex(-1);
}

bool ProfileLoginDlg::shouldShowRestartButton()
{
    try
    {
        return DataCache::getInstance().getControlStationEntity()->shouldShowRestartButton();
    }
    catch (...)
    {
        LOG_EXCEPTION_CATCH(SourceInfo, "DAI Exception", "Could not determine if restart button should be shown so it will be shown");
    }

    return false;
}

void ProfileLoginDlg::drawControls()
{
    static bool hasRestart = shouldShowRestartButton();

    // set the version number
    QString version;

    if (m_mode == TA_ControlStation::LOGIN_MODE)
    {
        version = "Version ";
        version += TA_Base_Core::buildVersion;
    }

    ui.m_version->setText(version);

    if ((m_mode == TA_ControlStation::LOGIN_MODE) && hasRestart)
    {
        // We don't have to move any button, just show the restart button
        ui.m_restartButton->show();
    }
    else
    {
        // Move the Cancel/Exit button across between the side buttons
        // to make them look equally distributed
        adjustButtons();
    }
}

void ProfileLoginDlg::activateWindow()
{
    m_isWindowActivated = true;

    playLogo();

    // Populate the combo boxes.
    populateNameComboBox();

    switch (m_mode)
    {
    case TA_ControlStation::CHANGE_PROFILE_MODE:
        setToChangeProfileDialog();
        break;

    case TA_ControlStation::OPERATOR_OVERRIDE_MODE:
        setToOperatorOverrideDialog();
        break;

    case TA_ControlStation::CHANGE_OPERATOR_MODE:
        setToChangeOperatorDialog();
        break;

    case TA_ControlStation::CHANGE_DISPLAY_MODE:
        setToChangeDisplayModeDialog();
        break;

    case TA_ControlStation::LOGIN_MODE:
    default:
        setToLoginDialog();
        break;
    }

    ui.m_helpButton->setEnabled(true);

    if (m_mode == TA_ControlStation::LOGIN_MODE)
    {
        m_isRestartAlwaysEnabled = !DataCache::getInstance().getControlStationEntity()->isRestartAccessControlled();
        m_isExitAlwaysEnabled = !DataCache::getInstance().getControlStationEntity()->isExitAccessControlled();

        // Enable the required controls
        ui.m_exitButton->setEnabled(m_isExitAlwaysEnabled);
        ui.m_restartButton->setEnabled(m_isRestartAlwaysEnabled);
    }
    else
    {
        ui.m_profileCombo->setEnabled(true);
    }
}

void ProfileLoginDlg::setToLoginDialog()
{
    // Change the mode of the dialog, now that it will be used for logging in.

    ui.m_nameCombo->setEnabled(true);
    ui.m_displayOnlyCheck->setEnabled(true);
    // Update the button captions and the window title.
    ui.m_loginButton->setText("Login");
    ui.m_exitButton->setText("Exit");
    setWindowTitle("Operator Login");

    refreshButtonAvailability();
}

void ProfileLoginDlg::setToChangeProfileDialog()
{
    // Change the mode of the dialog, now that it will be used to change profiles.
    m_bDisplayOnly = SessionDetails::getInstance().getDisplayMode();
    ui.m_displayOnlyCheck->setCheckState(m_bDisplayOnly ? Qt::Checked : Qt::Unchecked);
    ui.m_displayOnlyCheck->setEnabled(true);

    // Make the dialog display the correct name and profile.
    QString operatorName = QtUtilities::convertToUnicode<QString>(SessionDetails::getInstance().getOperatorName());
    ui.m_nameCombo->setCurrentIndex(ui.m_nameCombo->findText(operatorName));   //TD15528
    //onSelchangeNameCombo();
    // Disable the name combo as this cannot be changed.
    ui.m_nameCombo->setEnabled(false);

    refreshButtonAvailability();

    // Update the button captions and the window title.
    ui.m_loginButton->setText("OK");
    ui.m_exitButton->setText("Cancel");
    setWindowTitle("Change Profile / Mode");
}

void ProfileLoginDlg::setToChangeDisplayModeDialog()
{
    // Change the mode of the dialog, now that it will be used to change profiles.
    m_bDisplayOnly = SessionDetails::getInstance().getDisplayMode();
    ui.m_displayOnlyCheck->setCheckState(m_bDisplayOnly ? Qt::Checked : Qt::Unchecked);
    ui.m_displayOnlyCheck->setEnabled(true);

    // Make the dialog display the correct name and profile.
    QString operatorName = QtUtilities::convertToUnicode<QString>(SessionDetails::getInstance().getOperatorName());
    ui.m_nameCombo->setCurrentIndex(ui.m_nameCombo->findText(operatorName));   //TD15528
    int selected = ui.m_nameCombo->currentIndex();

    if (selected == CB_ERR)
    {
        LOG(DEBUG_INFO, "setToChangeDisplayModeDialog:selected == CB_ERR 1");
        m_selectedOperator = NULL;
        m_selectedProfile = NULL;
        m_selectedLocation = NULL;
        ui.m_profileCombo->setEnabled(false);
        ui.m_locationCombo->setEnabled(false);
        ui.m_passwordEdit->setEnabled(false);
        ui.m_profileCombo->setCurrentIndex(CB_ERR);
        ui.m_locationCombo->setCurrentIndex(CB_ERR);
        ui.m_passwordEdit->setText("");
        ui.m_nameCombo->setFocus();
        refreshButtonAvailability();
        return;
    }

    // Update/Set the selected operator member variable
    m_selectedOperator = reinterpret_cast<IOperator*>(ui.m_nameCombo->itemData(selected).toUInt());
    // Disable the name combo as this cannot be changed.
    ui.m_nameCombo->setEnabled(false);

    populateProfileComboBox();
    ui.m_profileCombo->setCurrentIndex(ui.m_profileCombo->findText(SessionDetails::getInstance().getProfileName().c_str())); //TD15528
    int profile_selected = ui.m_profileCombo->currentIndex();

    if (profile_selected == CB_ERR)
    {
        LOG(DEBUG_INFO, "setToChangeDisplayModeDialog 2:selected == CB_ERR");
        m_selectedProfile = NULL;
        m_selectedLocation = NULL;
        ui.m_locationCombo->setEnabled(false);
        ui.m_passwordEdit->setEnabled(false);
        ui.m_locationCombo->setCurrentIndex(CB_ERR);
        ui.m_passwordEdit->setText("");
        ui.m_profileCombo->setFocus();
        refreshButtonAvailability();
        return;
    }

    // Update/Set the selected operator member variable
    m_selectedProfile = reinterpret_cast<IProfile*>(ui.m_profileCombo->itemData(profile_selected).toUInt());
    ui.m_profileCombo->setEnabled(false);

    // Check if this profile requires a location to be selected.
    if (m_selectedProfile->requiresLocationToBeSelected())
    {
        populateLocationComboBox();
        ui.m_locationCombo->setCurrentIndex(ui.m_locationCombo->findText(SessionDetails::getInstance().getLocationDisplayName().c_str()));

        int location_selected = ui.m_locationCombo->currentIndex();

        if (location_selected == CB_ERR)
        {
            m_selectedLocation = NULL;
            ui.m_passwordEdit->setEnabled(false);
            ui.m_passwordEdit->setText("");
            ui.m_locationCombo->setFocus();
            refreshButtonAvailability();
            return;
        }

        // Update/Set the selected operator member variable
        m_selectedLocation = reinterpret_cast<ILocation*>(ui.m_profileCombo->itemData(location_selected).toUInt());
    }
    else
    {
        ui.m_locationCombo->setCurrentIndex(CB_ERR);
        m_selectedLocation = NULL;
    }

    ui.m_locationCombo->setEnabled(false);

    ui.m_passwordEdit->setEnabled(true);
    refreshButtonAvailability();

    // Update the button captions and the window title.
    ui.m_loginButton->setText("OK");
    ui.m_exitButton->setText("Cancel");
    setWindowTitle("Change Profile /Mode");
}

void ProfileLoginDlg::setToOperatorOverrideDialog()
{
    // Change the mode of the dialog, now that it will be used to override operators.
    ui.m_nameCombo->setEnabled(true);

    refreshButtonAvailability();

    // Update the button captions and the window title.
    ui.m_loginButton->setText("OK");
    ui.m_exitButton->setText("Cancel");
    setWindowTitle("Operator Override");
}

void ProfileLoginDlg::setToChangeOperatorDialog()
{
    // Change the mode of the dialog, now that it will be used to change operator.

    // Make the dialog display the correct name and profile.
    ui.m_nameCombo->removeItem(ui.m_nameCombo->findText(SessionDetails::getInstance().getOperatorName().c_str())); //TD15528
    ui.m_nameCombo->setEnabled(true);

    refreshButtonAvailability();

    // Update the button captions and the window title.
    ui.m_loginButton->setText("OK");
    ui.m_exitButton->setText("Cancel");
    setWindowTitle("Change Operator");
}

void ProfileLoginDlg::onDropdownNameCombo()
{
    if (SessionDetails::getInstance().hasOperatorUpdated())
    {
        populateNameComboBox();
        SessionDetails::getInstance().resetOperatorUpdateFlag();
    }
}

void ProfileLoginDlg::onDropdownProfileCombo()
{
    bool profileHasUpdated = SessionDetails::getInstance().hasProfileUpdated();

    // there's either a profile change or operator has associated/dissociated
    // from a profile, then update the profile list.
    if (SessionDetails::getInstance().hasOperatorUpdated() || profileHasUpdated)
    {
        populateProfileComboBox();
        SessionDetails::getInstance().resetProfileUpdateFlag();
    }
}

void ProfileLoginDlg::onDropdownLocationCombo()
{
    // there's either a location change or profile has associated/dissociated
    // from a location, then update the location list.
    if (SessionDetails::getInstance().hasLocationUpdated() ||
            SessionDetails::getInstance().hasProfileUpdated())
    {
        populateLocationComboBox();
        SessionDetails::getInstance().resetLocationUpdateFlag();
    }
}

void ProfileLoginDlg::onSystemControllerStateChange(WPARAM, LPARAM)
{
    // If there is a system controller state change, simply need to re-populate the locations drop-down
    populateLocationComboBox();
}

bool ProfileLoginDlg::isTdsProfile(std::string profile_str)
{
    boost::match_results<std::string::const_iterator> what;

    if (0 == boost::regex_match(profile_str, what, TDS_PATTERN, boost::match_default))
    {
        return false;
    }

    if (what[0].matched)
    {
        return true;
    }

    return false;
}

void ProfileLoginDlg::OnDisplayOnlyClicked()
{
    // TODO: Add your control notification handler code here
    //m_bDisplayOnly = !m_bDisplayOnly;
    m_bDisplayOnly = ui.m_displayOnlyCheck->isChecked();
}

bool ProfileLoginDlg::isLocationValidToBePopulated(TA_Base_Core::ILocation* location, unsigned long minLocalLocationOrderId, unsigned long maxLocalLocationOrderId)
{
    unsigned long locationkey = SessionDetails::getInstance().getConsoleLocationId();
    boost::shared_ptr<TA_Base_Core::ILocation> currentlocaiton(TA_Base_Core::LocationAccessFactory::getInstance().getLocationByKey(locationkey));

    if (location->getOrderId() == currentlocaiton->getOrderId() || location->getOrderId() == m_previousLocationOrderId || location->getOrderId() == m_nextLocationOrderId)
    {
        return true;
    }

    if (boost::iequals(currentlocaiton->getName(), "DT01") || boost::iequals(currentlocaiton->getName(), "DT16"))
    {
        if (boost::iequals(location->getName(), "TMF"))
        {
            return true;
        }
    }

    return false;
}

std::vector<unsigned long>  ProfileLoginDlg::getStationLocationOrderIdList()
{
    std::vector<unsigned long> localLocationOrderId;
    SessionDetails::LocationVector locations = TA_Base_Core::LocationAccessFactory::getInstance().getAllLocations();

    BOOST_FOREACH(TA_Base_Core::ILocation* location, locations)
    {
        if (location != NULL && location->getKey() != 0 && isStation(location->getName()))
        {
            localLocationOrderId.push_back(location->getOrderId());
        }

        delete location;
    }

    sort(localLocationOrderId.begin(), localLocationOrderId.end());

    return localLocationOrderId;
}

bool ProfileLoginDlg::isStation(std::string stationName)
{
    if (boost::iequals(stationName, "OCC"))
    {
        return false;
    }
    else if (boost::iequals(stationName, "All Locations"))
    {
        return false;
    }
    else if (boost::iequals(stationName, "TDS"))
    {
        return false;
    }
    else if (boost::iequals(stationName, "KCD"))
    {
        return false;
    }
    else if (boost::iequals(stationName, "DCC"))
    {
        return false;
    }
    else if (boost::iequals(stationName, "KOCC"))
    {
        return false;
    }
    else if (boost::iequals(stationName, "TMF"))
    {
        return false;
    }
    else
    {
        return TRUE;
    }
}

bool ProfileLoginDlg::isDepot(std::string stationName)
{
    return boost::iequals(stationName, "KCD");
}

void ProfileLoginDlg::getPreviousNextLocationOrderId()
{
    std::vector<unsigned long> orderIdVector = getStationLocationOrderIdList();
    unsigned long locationkey = SessionDetails::getInstance().getConsoleLocationId();
    boost::shared_ptr<TA_Base_Core::ILocation> currentlocaiton(TA_Base_Core::LocationAccessFactory::getInstance().getLocationByKey(locationkey));

    for (std::vector<unsigned long>::iterator iter = orderIdVector.begin(); iter != orderIdVector.end(); ++iter)
    {
        if ((*iter) == currentlocaiton->getOrderId())
        {
            if (iter == orderIdVector.begin() && iter < (orderIdVector.end() - 1))
            {
                m_previousLocationOrderId = ULONG_MAX;
                m_nextLocationOrderId = (*(iter + 1));
                return;
            }
            else if (iter == orderIdVector.begin())
            {
                m_previousLocationOrderId = ULONG_MAX;
                m_nextLocationOrderId = ULONG_MAX;
                return;
            }
            else if (iter == (orderIdVector.end() - 1) && iter > orderIdVector.begin())
            {
                m_previousLocationOrderId = (*(iter - 1));
                m_nextLocationOrderId = ULONG_MAX;
                return;
            }
            else if (iter < (orderIdVector.end() - 1) && iter > orderIdVector.begin())
            {
                m_previousLocationOrderId = (*(iter - 1));
                m_nextLocationOrderId = (*(iter + 1));
                return;
            }
        }
    }

    m_previousLocationOrderId = ULONG_MAX;
    m_nextLocationOrderId = ULONG_MAX;
}
