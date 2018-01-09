/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/AbstructLoginDialog.cpp $
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
#include "Utilities.h"
#include "SessionDetails.h"
#include "SystemControllerMessageHandler.h"
#include "app/system_control/control_station/src/AbstructLoginDialog.h"
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
#include "core/data_access_interface/entity_access/src/ControlStation.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/versioning/src/Version.h"
#include <QMovie>

using namespace TA_Base_Core;
using namespace TA_Base_App;

std::string AbstructLoginDialog::USE_TDS_PROFILE("UseTdsProfile");
boost::regex AbstructLoginDialog::TDS_PATTERN("^[A-Z,0-9]+[[:space:]]*\\(TRA\\)$|^[A-Z]+-TDS[[:space:]]*\\([A-Z,a-z]+\\)$");
TA_Base_Core::IOperator* AbstructLoginDialog::m_selectedOperator = NULL;
TA_Base_Core::IProfile*  AbstructLoginDialog::m_selectedProfile = NULL;
TA_Base_Core::ILocation*  AbstructLoginDialog::m_selectedLocation = NULL;
QString AbstructLoginDialog::m_password;
bool AbstructLoginDialog::m_bDisplayOnly = false;
LoginInfo AbstructLoginDialog::m_loginInfo;

AbstructLoginDialog::AbstructLoginDialog()
    : m_populateNameComboBoxOnce(BOOST_ONCE_INIT)
{
    ui.setupUi(this);
    QtUtilities::startMovie(ui.logoLabel, ":/transactiveintro.gif");

    ui.m_loginButton->setEnabled(false);
    ui.m_exitButton->setEnabled(true);
    ui.m_passwordEdit->setMaxLength(TA_ControlStation::MAX_PASSWORD_LENGTH);
    ui.m_helpButton->setEnabled(false);
    ui.m_displayOnlyCheck->setEnabled(false);

    connect(ui.m_exitButton, SIGNAL(clicked(bool)), this, SLOT(onExitButton()));
    connect(ui.m_helpButton, SIGNAL(clicked(bool)), this, SLOT(onHelpButton()));
    connect(ui.m_loginButton, SIGNAL(clicked(bool)), this, SLOT(onLoginButton()));
    connect(ui.m_displayOnlyCheck, SIGNAL(clicked(bool)), this, SLOT(OnDisplayOnlyClicked()));
    connect(ui.m_nameCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelchangeNameCombo()));
    connect(ui.m_profileCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelchangeProfileCombo()));
    connect(ui.m_locationCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onSelchangeLocationCombo()));
    connect(ui.m_passwordEdit, SIGNAL(textChanged(const QString&)), this, SLOT(onChangePasswordEdit()));
}

void AbstructLoginDialog::onInitDialog()
{
    boost::call_once(m_populateNameComboBoxOnce, boost::bind(&AbstructLoginDialog::populateNameComboBox, this));

    playLogo();
    drawControls();

    QString operatorEntry;
    QString profileEntry;
    QString locationEntry;

    if (m_selectedOperator)
    {
        operatorEntry = QtUtilities::convertToUnicode<QString>(m_selectedOperator->getName());

        if (m_selectedProfile)
        {
            profileEntry = QtUtilities::convertToUnicode<QString>(m_selectedProfile->getName());

            if (m_selectedProfile->requiresLocationToBeSelected() && m_selectedLocation)
            {
                locationEntry = QtUtilities::convertToUnicode<QString>(m_selectedLocation->getDisplayName());
            }
        }
    }

    ui.m_nameCombo->setCurrentIndex(ui.m_nameCombo->findText(operatorEntry));
    ui.m_profileCombo->setCurrentIndex(ui.m_profileCombo->findText(profileEntry));
    ui.m_locationCombo->setCurrentIndex(ui.m_locationCombo->findText(locationEntry));
    ui.m_passwordEdit->clear();
    ui.m_displayOnlyCheck->setCheckState(m_bDisplayOnly ? Qt::Checked : Qt::Unchecked);
}

void AbstructLoginDialog::onSelchangeNameCombo()
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

void AbstructLoginDialog::onSelchangeLocationCombo()
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

void AbstructLoginDialog::onChangePasswordEdit()
{
    m_password = ui.m_passwordEdit->text();
    refreshButtonAvailability();
}

bool AbstructLoginDialog::areDetailsValid()
{
    TA_ASSERT(m_selectedOperator != NULL, "Must select an operator");
    TA_ASSERT(m_selectedProfile != NULL, "Must select a profile");

    if (SessionDetails::getInstance().hasOperatorUpdated())
    {
        bool found = false;

        try
        {
            IOperatorList operators = SessionDetails::getInstance().getListOfOperators();
            found = boost::algorithm::one_of_equal(Utilities::getEntityKeys(operators), m_selectedOperator->getKey());
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

            IProfileList profiles = SessionDetails::getInstance().getListOfProfiles(m_selectedOperator, isCentral, false);
            found = boost::algorithm::one_of_equal(Utilities::getEntityKeys(profiles), m_selectedProfile->getKey());
        }
        catch (...)
        {
            // just in case the actual object got deleted from another thread
        }

        if (!found)
        {
            showMsgBox(IDS_UE_020057, IDS_PROFILE);
            onDropdownProfileCombo();
            ui.m_profileCombo->setCurrentIndex(CB_ERR);
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
            ILocationList locations = SessionDetails::getInstance().getListOfLocations(m_selectedProfile);
            found = boost::algorithm::one_of_equal(Utilities::getEntityKeys(locations), m_selectedLocation->getKey());
        }
        catch (...)
        {
            // just in case the actual object got deleted from another thread
        }

        if (!found)
        {
            showMsgBox(IDS_UE_020057, IDS_LOCATION);
            onDropdownLocationCombo();
            ui.m_locationCombo->setCurrentIndex(CB_ERR);
            //onSelchangeLocationCombo();
            return false;
        }
    }

    return true;
}

void AbstructLoginDialog::onHelpButton()
{
    TA_Base_Bus::HelpLauncher::getInstance().displayHelp(true);
}

bool AbstructLoginDialog::hasFilledAllDetails()
{
    m_password = ui.m_passwordEdit->text();
    // Because of the way we enable and disable our fields, the only
    // way all fields could be filled in is when the user has filled
    // the password field.
    // TD18944
    return (ui.m_passwordEdit->isEnabled() && !m_password.isEmpty() && m_password.length() >= 8);
}

void AbstructLoginDialog::refreshButtonAvailability()
{
    bool enable = hasFilledAllDetails();
    ui.m_loginButton->setEnabled(enable);
}

void AbstructLoginDialog::populateNameComboBox()
{
    LOG_DEBUG("AbstructLoginDialog::populateNameComboBox");

    if (0 == ui.m_nameCombo->count())
    {
        BLOCK_SIGNALS(ui.m_nameCombo);

        try
        {
            std::vector<TA_Base_Core::IOperator*>& allOperators = DataCache::getInstance().getAllOperators();

            for (TA_Base_Core::IOperator* oprtor : allOperators)
            {
                try
                {
                    ui.m_nameCombo->insertItem(ui.m_nameCombo->count(), QtUtilities::convertToUnicode<QString>(oprtor->getName()), reinterpret_cast<unsigned int>(oprtor));
                }
                catch (...)
                {
                }
            }
        }
        catch (...)
        {
        }

        ui.m_nameCombo->setCurrentIndex(-1);
    }
}

void AbstructLoginDialog::populateLocationComboBox()
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
        LOG_EXCEPTION("DataException", "Could not load the list of available operators or locations.");
    }

    std::vector<unsigned long> localOrderIdList = getStationLocationOrderIdList();
    unsigned long minLocalLocationOrderId = localOrderIdList[0];
    unsigned long maxLocalLocationOrderId = localOrderIdList[localOrderIdList.size() - 1];
    LOG_INFO("maxLocalLocationOrderId:%u,minLocalLocationOrderId:%u", maxLocalLocationOrderId, minLocalLocationOrderId);

    for (TA_Base_Core::ILocation* location : locations)
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
                    if (Utilities::isStation(location->getName()) && isLocationValidToBePopulated(location, minLocalLocationOrderId, maxLocalLocationOrderId))
                    {
                        ui.m_locationCombo->insertItem(ui.m_locationCombo->count(),
                                                       QtUtilities::convertToUnicode<QString>(displayName),
                                                       reinterpret_cast<unsigned int>(location));
                    }
                    //if it is a depot, show all locations.
                    else if (Utilities::isDepot(location->getName()) && Utilities::isDepot(SessionDetails::getInstance().getConsoleLocationName()))
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

void AbstructLoginDialog::populateProfileComboBox()
{
    ui.m_profileCombo->clear();

    if (m_selectedOperator == NULL)
    {
        return;
    }

    bool usetds = TA_Base_Core::RunParams::getInstance().isSet(USE_TDS_PROFILE.c_str());
    bool isCentral = false;
    unsigned long consoleLocationId = DataCache::getInstance().getConsoleLocation()->getKey();

    if (1 == consoleLocationId)
    {
        isCentral = true;
    }

    LOG_INFO("details.getConsoleLocationId() %lu", consoleLocationId);
    SessionDetails::ProfileVector& profiles = SessionDetails::getInstance().getListOfProfiles(m_selectedOperator, isCentral, false);

    for (TA_Base_Core::IProfile* profile : profiles)
    {
        try
        {
            std::string name = profile->getName();

            if (isTdsProfile(name) && !usetds)
            {
                continue;
            }

            BLOCK_SIGNALS(ui.m_locationCombo);
            ui.m_profileCombo->insertItem(ui.m_profileCombo->count(), QtUtilities::convertToUnicode<QString>(name), reinterpret_cast<unsigned int>(profile));
        }
        catch (...)
        {
        }
    }

    ui.m_profileCombo->setCurrentIndex(-1);
}

bool AbstructLoginDialog::shouldShowRestartButton()
{
    try
    {
        return DataCache::getInstance().getControlStationEntity()->shouldShowRestartButton();
    }
    catch (...)
    {
        LOG_EXCEPTION("DAI Exception", "Could not determine if restart button should be shown so it will be shown");
    }

    return false;
}

void AbstructLoginDialog::drawControls()
{
    ui.m_version->setText("");
}

void AbstructLoginDialog::onDropdownNameCombo()
{
    if (SessionDetails::getInstance().hasOperatorUpdated())
    {
        populateNameComboBox();
        SessionDetails::getInstance().resetOperatorUpdateFlag();
    }
}

void AbstructLoginDialog::onDropdownProfileCombo()
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

void AbstructLoginDialog::onDropdownLocationCombo()
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

void AbstructLoginDialog::onSystemControllerStateChange()
{
    // If there is a system controller state change, simply need to re-populate the locations drop-down
    populateLocationComboBox();
}

bool AbstructLoginDialog::isTdsProfile(std::string profile_str)
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

void AbstructLoginDialog::OnDisplayOnlyClicked()
{
    // TODO: Add your control notification handler code here
    //m_bDisplayOnly = !m_bDisplayOnly;
    m_bDisplayOnly = ui.m_displayOnlyCheck->isChecked();
}

bool AbstructLoginDialog::isLocationValidToBePopulated(TA_Base_Core::ILocation* location, unsigned long minLocalLocationOrderId, unsigned long maxLocalLocationOrderId)
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

std::vector<unsigned long>  AbstructLoginDialog::getStationLocationOrderIdList()
{
    std::set<unsigned long> orders;
    ILocationList locations = DataCache::getInstance().getAllLocations();

    for (TA_Base_Core::ILocation* location : locations)
    {
        if (location->getKey() != 0 && Utilities::isStation(location->getName()))
        {
            orders.insert(location->getOrderId());
        }
    }

    return std::vector<unsigned long>(orders.begin(), orders.end());
}

void AbstructLoginDialog::getPreviousNextLocationOrderId()
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
