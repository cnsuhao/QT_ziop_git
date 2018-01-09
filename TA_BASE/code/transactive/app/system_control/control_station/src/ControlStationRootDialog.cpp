/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/ControlStationRootDialog.cpp $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This is an extension of CDialog and is the root dialog and is responsible
  * for displaying the appropriate dialog boxes, messages and GraphWorX displays.
  */
#include "StdAfx.h"
#include "Utilities.h"
#include "ExclusiveDutyDialog.h"
#include "OverRideExclusiveDuty.h"
#include "DutyManager.h"
#include "GraphWorxManager.h"
#include "TAToGxMessageInformation.h"
#include "RightsManager.h"
#include "ControlStationRootDialog.h"
#include "ControlStationConstants.h"
#include "EnumProcess.h"
#include "controlstation.h"
#include "SessionManager.h"
#include "ControlStation2.h"
#include "SchematicDisplay.h"
#include "DutyNotificationDialog.h"
#include "DutyRequestDialog.h"
#include "IGraphWorxManager.h"
#include "ScreenSaverInstall.h"
#include "Signals.h"
#include "SessionSignal.h"
#include "ControlStationSignal.h"
#include "LoginDialogSignal.h"
#include "LoginTask.h"
#include "LogoutTask.h"
#include "SessionDetails.h"
#include "resourceQt.h"
#include "DutyChangeNoteDlg.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/generic_gui_pmod/src/AppLauncher.h"
#include "bus/application_types/src/apptypes.h"
#include "bus/alarm/ATS_alarm_bridge_library/src/DataStoreUpdaterATSProfile.h"
#include "bus/alarm/ATS_alarm_bridge_library/src/DataStoreReceiverISCSProfile.h"
#include "bus/alarm/ATS_alarm_bridge_library/src/DataStoreUpdaterISCSProfile.h"
#include "bus/alarm/ATS_alarm_bridge_library/src/DataStoreUpdaterATSAlarmInfo.h"
#include "bus/security/authentication_library/src/AuthenticationLibrary.h"
#include "core/process_management/IDL/src/IManagedApplicationCorbaDef.h"
#include "core/exceptions/src/SessionException.h"
#include "core/exceptions/src/RightsException.h"
#include "core/exceptions/src/GraphWorxException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TA_String.h"
#include "core/utilities/src/RunParams.h"
#include "core/synchronisation/src/ReEntrantThreadLockable.h"
#include "core/data_access_interface/src/ILocation.h"
#include "core/data_access_interface/src/ISession.h"
#include "core/data_access_interface/src/IProfile.h"
#include "core/data_access_interface/src/IOperator.h"
#include "core/data_access_interface/src/IOperatorPassword.h"
#include "core/data_access_interface/src/OperatorPasswordAccessFactory.h"
#include "core/data_access_interface/src/SecuritySettingAccessFactory.h"
#include <comutil.h>
#include <QThread>

TA_Base_Core::ReEntrantThreadLockable ControlStationRootDialog::m_initDlgLock;

const std::string ControlStationRootDialog::MENU_RESUME_MESG = "MenuResume";
const std::string ControlStationRootDialog::MENU_SUSPEND_MESG = "MenuSuspend";
const std::string ControlStationRootDialog::SYSTEM_DEGRADED_MESG = "SystemDegraded";
const std::string ControlStationRootDialog::SYSTEM_NORMAL_MESG = "SystemNormal";
const std::string ControlStationRootDialog::PROFILE_DISPLAY_ONLY = "DisplayOnly";
const std::string ControlStationRootDialog::PROFILE_DISPLAY_NORMAL = "DisplayNormal";
const std::string ControlStationRootDialog::RPARAM_USE_SCREENSAVER = "UseScreenSaver";

using namespace TA_Base_Core;
using namespace TA_Base_Bus;
using namespace I_ATS_ISCS_ALARM;

namespace
{
    struct CursorGuard
    {
        ControlStationRootDialog& m_obj;
        CursorGuard(ControlStationRootDialog& obj) : m_obj(obj) { /*m_obj.BeginWaitCursor();*/ }
        ~CursorGuard() { /*m_obj.EndWaitCursor();*/ }
    };

#define CURSOR_GUARD() CursorGuard BOOST_PP_CAT(cursor_guard_, __line__)(*this)
}

ControlStationRootDialog& ControlStationRootDialog::getInstance()
{
    static ControlStationRootDialog* instance = (ControlStationRootDialog*)new char[sizeof(ControlStationRootDialog)];
    return *instance;
}

ControlStationRootDialog& ControlStationRootDialog::createInstance(TA_Base_Bus::ITransActiveApp* app)
{
    return *new (&ControlStationRootDialog::getInstance()) ControlStationRootDialog(app);
}

ControlStationRootDialog::ControlStationRootDialog(TA_Base_Bus::ITransActiveApp* app)
    : AbstractTransActiveDialog(app, "", NULL),
      m_comControlStation(NULL),
      m_comRippleControlStation(NULL),
      m_hasInitDialog(false),
      m_timerFlag(false),
      m_shouldStartTimer(true),
      m_timer(this)
{
    LOG_DEBUG("ControlStationRootDialog::ControlStationRootDialog - enter");

    ui.setupUi(this);
    setupMessageMap();
    setupSignalConnections();
    m_threadId = QThread::currentThreadId();

    setMainView(true);

    connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
    connect(this, SIGNAL(onControlStationExitSignal()), this, SLOT(onControlStationExitSlot()));

    {
        //Local Test
        connect(ui.m_loginButton, SIGNAL(clicked(bool)), this, SLOT(onLoginButton()));
        connect(ui.m_logoutButton, SIGNAL(clicked(bool)), this, SLOT(onLogoutButton()));
        connect(ui.m_changePasswordButton, SIGNAL(clicked(bool)), this, SLOT(onChangePasswordButton()));
        connect(ui.m_operatorOverrideButton, SIGNAL(clicked(bool)), this, SLOT(onOperatorOverrideButton()));
        connect(ui.m_changeProfileButton, SIGNAL(clicked(bool)), this, SLOT(onChangeProfileButton()));
        connect(ui.m_changeDisplayModeButton, SIGNAL(clicked(bool)), this, SLOT(onChangeDisplayModeButton()));
        connect(ui.m_changeOperatorButton, SIGNAL(clicked(bool)), this, SLOT(onChangeOperatorButton()));
        connect(ui.m_dutyNotificationButton, SIGNAL(clicked(bool)), this, SLOT(onDutyNottionButton()));
        connect(ui.m_dutyRequestButton, SIGNAL(clicked(bool)), this, SLOT(onDutyRequestButton()));
        connect(ui.m_exclusiveDutyButton, SIGNAL(clicked(bool)), this, SLOT(onExclusiveDutyButton()));
        connect(ui.m_overRideExclusiveDutyButton, SIGNAL(clicked(bool)), this, SLOT(onOverRideExclusiveDuty()));
    }

    onInitDialog();

    //m_automatic_logout_time = m_sessionManager.m_securitySetting->m_automatic_logout_time;

    LOG_DEBUG("ControlStationRootDialog::ControlStationRootDialog - exit");
}

ControlStationRootDialog::~ControlStationRootDialog()
{
}

void ControlStationRootDialog::setupSignalConnections()
{
    SessionSignal::logined.connect(boost::bind(&ControlStationRootDialog::startTimer, this));
    SessionSignal::logouted.connect(boost::bind(&ControlStationRootDialog::stopTimer, this));
    Signals::loginDetailChanged.connect(boost::bind(&ControlStationRootDialog::loginDetailsChanged, this));
    ControlStationSignal::exit.connect(boost::bind(&ControlStationRootDialog::onControlStationExit, this));
}

void ControlStationRootDialog::startTimer()
{
    if (m_shouldStartTimer == TRUE && m_timerFlag == FALSE)
    {
        m_timer.start(60000);
        m_timerFlag = TRUE;
        LOG_INFO("onDisplayLoginDialogue()::Timer is successfully set up!");
    }
}

void ControlStationRootDialog::stopTimer()
{
    //[2011-11-10]
    if (m_timerFlag == TRUE)
    {
        //KillTimer(123123);
        m_timer.stop();
        m_timerFlag = FALSE;
    }
}

void ControlStationRootDialog::onInitDialog()
{
    LOG_DEBUG("ControlStationRootDialog::onInitDialog - enter");

    m_timerFlag = false;

    //LoginDialogSignal::showDialog();

    TA_Base_Core::ThreadGuard guard(m_initDlgLock);

    m_hasInitDialog = true;

    std::string timerSwitch = TA_Base_Core::RunParams::getInstance().get("AutoLogoffSwitch");
    m_shouldStartTimer = (boost::iequals("on", timerSwitch) || boost::iequals("1", timerSwitch));
    LOG_INFO("Timer is turned %s", (m_shouldStartTimer ? "on" : "off"));
}

void ControlStationRootDialog::fireDutyChanged()
{
    LOG_DEBUG("before ControlStationRootDialog::fireDutyChanged");

    if (m_comControlStation)
    {
        m_comControlStation->Fire_dutyChanged();
    }

    LOG_DEBUG("after ControlStationRootDialog::fireDutyChanged");
}

void ControlStationRootDialog::postDutyChanged()
{
    boost::thread t(boost::bind(&ControlStationRootDialog::fireDutyChanged, this));
}
/////////////////////////////////////////////////////////////////////////////
// Custom message handlers
/////////////////////////////////////////////////////////////////////////////

void ControlStationRootDialog::onDisplayLoginDialogue(WPARAM, LPARAM)
{
    //LoginDialogSignal::showDialog();
}

void ControlStationRootDialog::onDisplayChangeProfileDialogue(WPARAM, LPARAM)
{
#if 0
    FUNCTION_ENTRY("onDisplayChangeProfileDialogue");

    // TD 8214 check if window is already active
    if (m_loginDialog.isVisible())
    {
        return;
    }

    CString actionName = IDS_CHANGE_PROFILE;

    // Check if current operator is allowed to change profile
    BeginWaitCursor(); // display the hourglass cursor
    TA_ControlStation::EPermission permission = RightsManager::getInstance().isControlPermitted(TA_Base_Bus::aca_CONTROL_STATION_CHANGE_PROFILE);
    EndWaitCursor(); // remove the hourglass cursor

    if (permission != TA_Base_App::TA_ControlStation::PERMITTED)
    {
        showNoPermissionMessage(actionName, permission);
        FUNCTION_EXIT;
        return;
    }

    // fire off MENU_SUSPEND_MESG message to GraphWorx so that the menu won't be active
    // when the logout dialog is showing.  This prevents a selected menu from
    // remaining visible when graphworx is asked to hide (i.e. user logout).
    if (m_comControlStation != NULL)
    {
        m_comControlStation->Fire_shortMessage(_bstr_t(MENU_SUSPEND_MESG));
    }

    // We got rights to do this, so continue

    bool isSuccessfulChange = false;
    m_loginDialog.setDialogType(TA_Base_App::TA_ControlStation::CHANGE_PROFILE_MODE);

    do
    {
        // Call doModal on the dialog box. This will display and wait for the run()
        // method to complete before allowing the user to do anything.
        int response = m_loginDialog.doModal();

        if (response == QDialog::Rejected)
        {
            LOG_INFO("User has cancelled the change profile");
            break;
        }

        if (!DutyManager::getInstance().loseExclusiveDuty())
        {
            break;
        }

        try
        {
            CURSOR_GUARD();
            //m_sessionManager.changeProfile(m_loginDialog);
            loginDetailsChanged();
            isSuccessfulChange = true;
        }
        catch (const SessionException& exception)
        {
            CString agentName = IDS_AUTHENTICATION_AGENT;
            handleSessionException(exception, actionName, agentName);
            m_loginDialog.useExistingDataOnNextDoModal();
        }
    }
    while (!isSuccessfulChange);

    // fire off MENU_RESUME_MESG message to GraphWorx to get it enable its menus again
    if (m_comControlStation != NULL)
    {
        m_comControlStation->Fire_shortMessage(_bstr_t(MENU_RESUME_MESG));
    }

    FUNCTION_EXIT;
    return;
#endif
}

void ControlStationRootDialog::onDisplayChangeDisplayModeDialogue(WPARAM, LPARAM)
{
#if 0
    FUNCTION_ENTRY("onDisplayChangeDisplayModeDialogue");

    LOG_INFO("Change Display Mode");

    // TD 8214 check if window is already active
    if (m_loginDialog.isVisible())
    {
        // already open
        return;
    }

    CString actionName = IDS_CHANGE_DISPLAY_MODE;
    LOG_INFO("Change Display Mode:TransActiveMessage");

    // fire off MENU_SUSPEND_MESG message to GraphWorx so that the menu won't be active
    // when the logout dialog is showing.  This prevents a selected menu from
    // remaining visible when graphworx is asked to hide (i.e. user logout).
    if (m_comControlStation != NULL)
    {
        m_comControlStation->Fire_shortMessage(_bstr_t(MENU_SUSPEND_MESG));
    }

    LOG_INFO("Change Display Mode:m_comControlStation");
    // We got rights to do this, so continue

    bool isSuccessfulChange = false;
    m_loginDialog.setDialogType(TA_Base_App::TA_ControlStation::CHANGE_DISPLAY_MODE);

    do
    {
        // Call doModal on the dialog box. This will display and wait for the run()
        // method to complete before allowing the user to do anything.
        int response = m_loginDialog.doModal();

        if (response == QDialog::Rejected)
        {
            LOG_INFO("User has cancelled the change display mode");
            break;
        }

        try
        {
            CURSOR_GUARD();
            LOG_INFO("Change Display Mode:try");
            //m_sessionManager.changeProfile(m_loginDialog);
            loginDetailsChanged();
            isSuccessfulChange = true;
            LOG_INFO("Change Display Mode:try Done");
        }
        catch (const SessionException& exception)
        {
            CString agentName = IDS_AUTHENTICATION_AGENT;
            handleSessionException(exception, actionName, agentName);
            m_loginDialog.useExistingDataOnNextDoModal();
        }
    }
    while (!isSuccessfulChange);

    // fire off MENU_RESUME_MESG message to GraphWorx to get it enable its menus again
    if (m_comControlStation != NULL)
    {
        m_comControlStation->Fire_shortMessage(_bstr_t(MENU_RESUME_MESG));
    }

    FUNCTION_EXIT;
    return;
#endif
}

void ControlStationRootDialog::onDisplayOperatorOverrideDialogue(WPARAM, LPARAM)
{
#if 0
    FUNCTION_ENTRY("onDisplayOperatorOverrideDialogue");

    bool static isShowingRemoveOverrideMsgBox = false;

    //TD641 - for reason similar to TD653 see the comment in onLogout
    //if ((m_loginDialog.m_hWnd != NULL) || isShowingRemoveOverrideMsgBox)
    if (m_loginDialog.isVisible() || isShowingRemoveOverrideMsgBox)
    {
        //Qt TODO: ShowOwnedPopups(); // show the popup if it's already being displayed
        LOG_INFO("Already have 1 instance of operator override dialog/remove override message box opened");
        return;
    }

    // Check if current operator is allowed to do operator override
    BeginWaitCursor(); // display the hourglass cursor
    TA_ControlStation::EPermission permission = RightsManager::getInstance().isControlPermitted(TA_Base_Bus::aca_CONTROL_STATION_OVERRIDE);
    EndWaitCursor(); // remove the hourglass cursor

    CString actionName = IDS_OPERATOR_OVERRIDE;

    if (permission != TA_Base_App::TA_ControlStation::PERMITTED)
    {
        showNoPermissionMessage(actionName, permission);
        FUNCTION_EXIT;
        return;
    }

    // fire off MENU_SUSPEND_MESG message to GraphWorx so that the menu won't be active
    // when the logout dialog is showing.  This prevents a selected menu from
    // remaining visible when graphworx is asked to hide (i.e. user logout).
    if (m_comControlStation != NULL)
    {
        m_comControlStation->Fire_shortMessage(_bstr_t(MENU_SUSPEND_MESG));
    }

    // We got rights to do this, so continue

    // If we are already in operator override mode then remove it.
    if (SessionDetails::getInstance().isOperatorOverridden())
    {
        LOG_INFO("Operator override already applied so we are removing it");

        isShowingRemoveOverrideMsgBox = true;
        actionName = IDS_REMOVE_OPERATOR_OVERRIDE;

        int response = showMsgBox(IDS_UW_010003, actionName);

        if ((response == IDYES) && DutyManager::getInstance().loseExclusiveDuty())
        {
            CURSOR_GUARD();
            //m_sessionManager.handleRequestRemoveOperatorOverrideSignal();
            PromisePtr promise(new VoidPromise);
            Signals::operatorRequestRemoveOperatorOverride(promise);
            loginDetailsChanged();
        }

        // fire off MENU_RESUME_MESG message to GraphWorx to get it enable its menus again
        if (m_comControlStation != NULL)
        {
            m_comControlStation->Fire_shortMessage(_bstr_t(MENU_RESUME_MESG));
        }

        isShowingRemoveOverrideMsgBox = false;
        FUNCTION_EXIT;
        return;
    }

    // Otherwise we want to add an operator override

    // Display operator override dialogue here.
    bool isSuccessfulOverride = false;
    m_loginDialog.setDialogType(TA_Base_App::TA_ControlStation::OPERATOR_OVERRIDE_MODE);

    do
    {
        // Call doModal on the dialog box. This will display and wait for the run()
        // method to complete before allowing the user to do anything.
        int response = m_loginDialog.doModal();

        if (response == QDialog::Rejected)
        {
            LOG_INFO("Operator cancelled the operator override");
            break;
        }

        try
        {
            CURSOR_GUARD();
            //m_sessionManager.operatorOverride(m_loginDialog);
            loginDetailsChanged();
            isSuccessfulOverride = true;
        }
        catch (SessionException& exception)
        {
            CString agentName = IDS_AUTHENTICATION_AGENT;
            handleSessionException(exception, actionName, agentName);
            m_loginDialog.useExistingDataOnNextDoModal();
        }
    }
    while (!isSuccessfulOverride);

    // fire off MENU_RESUME_MESG message to GraphWorx to get it enable its menus again
    if (m_comControlStation != NULL)
    {
        m_comControlStation->Fire_shortMessage(_bstr_t(MENU_RESUME_MESG));
    }

    FUNCTION_EXIT;
    return;
#endif
}

void ControlStationRootDialog::onDisplayChangeOperatorDialogue(WPARAM, LPARAM)
{
#if 0
    FUNCTION_ENTRY("onDisplayChangeOperatorDialogue");

    //TD641 - for reason similar to TD653 see the comment in onLogout
    //if (m_loginDialog.m_hWnd != NULL)
    if (m_loginDialog.isVisible())
    {
        LOG_INFO("Already have 1 instance of change operator dialog opened");
        return;
    }

    // Check if current operator is allowed to change operator
    BeginWaitCursor(); // display the hourglass cursor
    TA_ControlStation::EPermission permission = RightsManager::getInstance().isControlPermitted(TA_Base_Bus::aca_CONTROL_STATION_CHANGE_OPERATOR);
    EndWaitCursor(); // remove the hourglass cursor

    CString actionName = IDS_CHANGE_OPERATOR;

    if (permission != TA_Base_App::TA_ControlStation::PERMITTED)
    {
        showNoPermissionMessage(actionName, permission);
        FUNCTION_EXIT;
        return;
    }

    // fire off MENU_SUSPEND_MESG message to GraphWorx so that the menu won't be active
    // when the logout dialog is showing.  This prevents a selected menu from
    // remaining visible when graphworx is asked to hide (i.e. user logout).
    if (m_comControlStation != NULL)
    {
        m_comControlStation->Fire_shortMessage(_bstr_t(MENU_SUSPEND_MESG));
    }

    // We got rights to do this, so continue

    bool isSuccessfulChange = false;
    m_loginDialog.setDialogType(TA_Base_App::TA_ControlStation::CHANGE_OPERATOR_MODE);

    do
    {
        // Call doModal on the dialog box. This will display and wait for the run()
        // method to complete before allowing the user to do anything.
        int response = m_loginDialog.doModal();

        if (response == QDialog::Rejected)
        {
            LOG_INFO("User has cancelled the change operator");
            break;
        }

        try
        {
            CURSOR_GUARD();
            //m_sessionManager.changeOperator(m_loginDialog);
            loginDetailsChanged();
            isSuccessfulChange = true;
        }
        catch (const SessionException& exception)
        {
            CString agentName = IDS_AUTHENTICATION_AGENT;
            handleSessionException(exception, actionName, agentName);
            m_loginDialog.useExistingDataOnNextDoModal();
        }
    }
    while (!isSuccessfulChange);

    // fire off MENU_RESUME_MESG message to GraphWorx to get it enable its menus again
    if (m_comControlStation != NULL)
    {
        m_comControlStation->Fire_shortMessage(_bstr_t(MENU_RESUME_MESG));
    }

    FUNCTION_EXIT;
    return;
#endif
}

void ControlStationRootDialog::onDisplayChangePasswordDialogue(WPARAM, LPARAM)
{
#if 0
    FUNCTION_ENTRY("onDisplayChangePasswordDialogue");
    bool isSuccessfulChange = false;

    //TD641 - for reason similar to TD653 see the comment in onLogout
    if (m_passwordDialog.isVisible())
    {
        LOG_INFO("Already have 1 instance of change password dialog opened");
        return;
    }

    // Check if current operator is allowed to change password
    BeginWaitCursor(); // display the hourglass cursor
    TA_ControlStation::EPermission permission = RightsManager::getInstance().isControlPermitted(TA_Base_Bus::aca_CONTROL_STATION_CHANGE_PASSWORD);
    EndWaitCursor(); // remove the hourglass cursor

    CString actionName = IDS_CHANGE_PASSWORD;

    if (permission != TA_Base_App::TA_ControlStation::PERMITTED)
    {
        showNoPermissionMessage(actionName, permission);
        FUNCTION_EXIT;
        return;
    }

    // fire off MENU_SUSPEND_MESG message to GraphWorx so that the menu won't be active
    // when the logout dialog is showing.  This prevents a selected menu from
    // remaining visible when graphworx is asked to hide (i.e. user logout).
    if (m_comControlStation != NULL)
    {
        m_comControlStation->Fire_shortMessage(_bstr_t(MENU_SUSPEND_MESG));
    }

    // We got rights to do this, so continue

    do
    {
        // Call doModal on the dialog box. This will display and wait for the run()
        // method to complete before allowing the user to do anything.
        int response = m_passwordDialog.doModal();

        if (response == QDialog::Rejected)
        {
            LOG_INFO("User has cancelled the change password");
            break;
        }

        try
        {
            CURSOR_GUARD();
            //m_sessionManager.changePassword(m_passwordDialog);
            loginDetailsChanged();
            isSuccessfulChange = true;
        }
        catch (const SessionException& ex)
        {
            CString agentName = IDS_AUTHENTICATION_AGENT;
            std::string password = IDS_PASSWORD;
            CString MaxLoginTimes;
            CString ReuseTime;

            switch (ex.getFailType())
            {
            case SessionException::AUTHENTICATION_FAILED:
            {
                TA_Base_Core::IOperator* theOperator = NULL;
                TA_Base_Core::IProfile*  theProfile = NULL;
                TA_Base_Core::ILocation*  theLocation = NULL;
                m_loginDialog.getSelectedDetails(theOperator, theProfile, theLocation, password);
                TA_Base_Core::IOperatorPasswordPtr operatorPassword(TA_Base_Core::OperatorPasswordAccessFactory::getInstance().getOperatorPassword(theOperator->getKey(), false, false));
                int isLockedOut = operatorPassword->getLockedTimes();
                time_t lockedTime = operatorPassword->getLatestLockedTime();
                CTime cLockedTime(lockedTime);
                CTimeSpan timeSpan = CTime::GetCurrentTime() - cLockedTime;

                if (isLockedOut >= m_sessionManager.m_securitySetting->m_failed_login_attempts && timeSpan.GetTotalMinutes() <= m_sessionManager.m_securitySetting->m_password_lock_time)
                {
                    MaxLoginTimes.Format("%d", m_sessionManager.m_securitySetting->m_failed_login_attempts);
                    ReuseTime.Format("%d", m_sessionManager.m_securitySetting->m_password_lock_time);
                    showMsgBox(IDS_UE_620022, actionName, MaxLoginTimes, ReuseTime);
                }
                else
                {
                    showMsgBox(IDS_UE_020046, actionName);
                }
            }
            break;

            case SessionException::AUTHENTICATION_AGENT_FAILED:
                showMsgBox(IDS_UE_030064, actionName, agentName);
                break;

            case SessionException::INVALID_PASSWORD_LENGTH:
                showMsgBox(IDS_UE_020055, password, MIN_PASSWORD_LENGTH);
                m_passwordDialog.useExistingDataOnNextDoModal();
                break;

            case SessionException::PASSWORD_MISMATCH:
                showMsgBox(IDS_UE_020045);
                m_passwordDialog.useExistingDataOnNextDoModal();
                break;

            case SessionException::PASSWORD_USEDBEFORE:
                showMsgBox(IDS_UE_620023);
                break;

            default:
                showMsgBox(IDS_UE_010018, actionName);
                break;
            }
        }
    }
    while (!isSuccessfulChange);

    // fire off MENU_RESUME_MESG message to GraphWorx to get it enable its menus again
    if (m_comControlStation != NULL)
    {
        m_comControlStation->Fire_shortMessage(_bstr_t(MENU_RESUME_MESG));
    }

    FUNCTION_EXIT;
#endif
}

void ControlStationRootDialog::onLogout(WPARAM, LPARAM)
{
#if 0
    // TD641, TD653 If this method get called from GraphWorx multiple times, multiple
    // dialog boxes will be launched even if the dialog box is modal.
    // For some reason the same thread could run this method again without finishing
    // the one that is already running even if a normal thread guard is used here.
    // To get around this, we need to check to see if the dialog is already showing
    // before launching again.
    static bool isShowingConfirmationError = false;
    FUNCTION_ENTRY("onLogout");

    if ((m_logoutDialog.isVisible()) || isShowingConfirmationError)
    {
        LOG_INFO("Already showing logout dialog/confirmation message box");
        return;
    }

    // fire off MENU_SUSPEND_MESG message to GraphWorx so that the menu won't be active
    // when the logout dialog is showing.  This prevents a selected menu from
    // remaining visible when graphworx is asked to hide (i.e. user logout).
    if (m_comControlStation != NULL)
    {
        m_comControlStation->Fire_shortMessage(_bstr_t(MENU_SUSPEND_MESG));
    }

    int response = m_logoutDialog.doModal();

    if (response == QDialog::Rejected)
    {
        // fire off MENU_RESUME_MESG message to GraphWorx to get it enable its menus again
        if (m_comControlStation != NULL)
        {
            m_comControlStation->Fire_shortMessage(_bstr_t(MENU_RESUME_MESG));
        }

        LOG_INFO("User has cancelled action");
        FUNCTION_EXIT;
        return;
    }

    isShowingConfirmationError = true;

    CString actionName;
    //unsigned long resourceId = 0;

    // otherwise, it's IDOK.  Determine if it is logout, exit or restart.
    switch (m_logoutDialog.getTerminateCode())
    {
    case TA_Base_App::TA_ControlStation::EXIT:
        actionName = IDS_EXIT;
        break;

    case TA_Base_App::TA_ControlStation::RESTART:
        actionName = IDS_RESTART;
        break;

    case TA_Base_App::TA_ControlStation::LOGOUT:
    default:
        actionName = IDS_LOGOUT;
        break;
    }

    int resp = showMsgBox(IDS_UW_010003, actionName);

    if (resp == IDNO)
    {
        LOG_INFO("User chosen not to %s", actionName);

        // fire off MENU_RESUME_MESG message to GraphWorx to get it enable its menus again
        if (m_comControlStation != NULL)
        {
            m_comControlStation->Fire_shortMessage(_bstr_t(MENU_RESUME_MESG));
        }
    }
    else
    {
        // td8635 give user a chance to stop logout after finding out what
        // exclusive duties they have
        if (DutyManager::getInstance().loseExclusiveDuty())
        {
            switch (m_logoutDialog.getTerminateCode())
            {
            case TA_Base_App::TA_ControlStation::LOGOUT:
                logout();
                break;

            case TA_Base_App::TA_ControlStation::EXIT:
                exit();
                break;

            case TA_Base_App::TA_ControlStation::RESTART:
                exit(true);
                break;

            default:
                TA_ASSERT(false, "Some unhandled terminate type");
                break;
            }
        }
        else
        {
            m_comControlStation->Fire_shortMessage(_bstr_t(MENU_RESUME_MESG));
        }
    }

    isShowingConfirmationError = false;
    FUNCTION_EXIT;
    return;
#endif
}

void ControlStationRootDialog::onForceLogout(WPARAM, LPARAM)
{
    static bool isShowing = false;
    FUNCTION_ENTRY("onForceLogout");

    // if it's already showing, don't show another message box
    if (isShowing)
    {
        LOG_INFO("Already showing a force logout message box");
        return;
    }

    isShowing = true;

    // fire off MENU_SUSPEND_MESG message to GraphWorx so that the menu won't be active
    // when the logout dialog is showing.  This prevents a selected menu from
    // remaining visible when graphworx is asked to hide (i.e. user logout).
    if (m_comControlStation != NULL)
    {
        m_comControlStation->Fire_shortMessage(_bstr_t(MENU_SUSPEND_MESG.c_str()));
    }

    CString sessionType = IDS_LOGIN;
    showMsgBox(IDS_UE_020052, sessionType);

    logout();
    isShowing = false;
    FUNCTION_EXIT;
}

void ControlStationRootDialog::onForceEndOverride(WPARAM, LPARAM)
{
    static bool isShowing = false;
    FUNCTION_ENTRY("onForceEndOverride");

    // if it's already showing, don't show another message box
    if (isShowing)
    {
        LOG_INFO("Already showing a force end override message box");
        return;
    }

    isShowing = true;

    // fire off MENU_SUSPEND_MESG message to GraphWorx so that the menu won't be active
    // when the logout dialog is showing.  This prevents a selected menu from
    // remaining visible when graphworx is asked to hide (i.e. user logout).
    if (m_comControlStation != NULL)
    {
        m_comControlStation->Fire_shortMessage(_bstr_t(MENU_SUSPEND_MESG.c_str()));
    }

    CString sessionType = IDS_LOGIN;
    showMsgBox(IDS_UE_020052, sessionType);

    //m_sessionManager.handleRequestRemoveOperatorOverrideSignal();
    PromisePtr promise(new VoidPromise);
    Signals::operatorRequestRemoveOperatorOverride(promise);
    loginDetailsChanged();
    isShowing = false;
    FUNCTION_EXIT;
}

void ControlStationRootDialog::onLaunchDisplay(WPARAM display, LPARAM)
{
    FUNCTION_ENTRY("onLaunchDisplay");

    DisplayInformation* info = reinterpret_cast<DisplayInformation*>(display);

    if (info == NULL)
    {
        LOG_WARNING("Display passed was NULL. Cannot carry on and launch display");
        FUNCTION_EXIT;
        return;
    }

    if (m_comControlStation != NULL)
    {
        // Need to find out if there is a GraphWorX display located on the screen requested.
        // First need to find out what screen that is...
        unsigned int screenNumber = ScreenPositioning::getInstance().getWhichScreenCoordinateIsOn(info->position);

        // Now that we know the screen, we can determine if there is a graphworx display actually present on that
        // screen. If not, will need to "find" one.
        CRect screenArea = ScreenPositioning::getInstance().getRect(TA_Base_Bus::TA_GenericGui::SCREEN_SPECIFIC, TA_Base_Bus::TA_GenericGui::AREA_SCHEMATIC, screenNumber);

        if (screenArea.IsRectEmpty())
        {
            // The current screen does not have any graphworx areas on it, so need to search to see if we can find
            // one.
            CRect screenAreaLeft;

            if (screenNumber >= 2)
            {
                screenAreaLeft = ScreenPositioning::getInstance().getRect(TA_Base_Bus::TA_GenericGui::SCREEN_SPECIFIC, TA_Base_Bus::TA_GenericGui::AREA_SCHEMATIC, screenNumber - 1);
            }
            else
            {
                screenAreaLeft.SetRectEmpty();
            }

            CRect screenAreaRight;

            if (screenNumber < ScreenPositioning::getInstance().getNumberOfMonitors())
            {
                screenAreaRight = ScreenPositioning::getInstance().getRect(TA_Base_Bus::TA_GenericGui::SCREEN_SPECIFIC, TA_Base_Bus::TA_GenericGui::AREA_SCHEMATIC, screenNumber + 1);
            }
            else
            {
                screenAreaRight.SetRectEmpty();
            }

            CRect screenAreaLaunchBar = ScreenPositioning::getInstance().getAppRectNextToLaunchBar();

            // Now out of the three rectangles, find one that works
            if (!screenAreaLeft.IsRectEmpty())
            {
                // If there was one available to the left, move the position one screen left.
                info->position = info->position - ScreenPositioning::getInstance().getEachScreenWidth();
            }
            else if (!screenAreaRight.IsRectEmpty())
            {
                // If the left wasn't available check the right
                info->position = info->position + ScreenPositioning::getInstance().getEachScreenWidth();
            }
            else if (!screenAreaLaunchBar.IsRectEmpty())
            {
                // Finally, if the right wasn't available, check the launch bar area
                info->position = screenAreaLaunchBar.left + 1;
            }
            else
            {
                // There aren't any graphworx displays, so log and exit
                LOG(DEBUG_WARN, "There were no active GraphWorX displays, so cannot launch display %s", info->display);
                return;
            }
        }

        LOG_INFO("launching display %s with asset %s on screen %li", info->display, info->asset, info->position);
        //TA_Base_Core::Thread::sleep(250); // Allow some time for the COM's launchDisplay to return before firing the events
        m_comControlStation->Fire_loadDisplayEx(_bstr_t(info->display), _bstr_t(info->asset), info->position);
        m_comControlStation->Fire_loadDisplay(_bstr_t(info->display), info->position);
        delete info;
    }

    FUNCTION_EXIT;
}

void ControlStationRootDialog::onServerStateChange(WPARAM isContactable, LPARAM)
{
    FUNCTION_ENTRY("onServerStateChange");

    if (m_comControlStation != NULL)
    {
        m_comControlStation->Fire_networkStatusChanged(isContactable);
    }

    FUNCTION_EXIT;
}

void ControlStationRootDialog::onSystemControllerStateChange(WPARAM isOnline, LPARAM group)
{
#if 0
    FUNCTION_ENTRY("onSystemControllerStateChange");

    // For any change in the system controller status, need to update the login dialog (if it is visible) to
    // restrict the visible locations as necessary
    //if (m_loginDialog.m_hWnd != NULL)
    if (m_loginDialog.isVisible())
    {
        // Notify the login dialog only if it exists. Use the same message name as we used for this method.
        m_loginDialog.postMessage(WM_SYSTEMCONTROLLER_STATE_CHANGE);
    }

    // TD 11910: Send messages to GraphWorx to change the colour of the operator/profile panel
    // depending on the state of the system.
    if (m_comControlStation != NULL)
    {
        if (isOnline)
        {
            m_comControlStation->Fire_shortMessage(_bstr_t(SYSTEM_NORMAL_MESG.c_str()));
        }
        else
        {
            m_comControlStation->Fire_shortMessage(_bstr_t(SYSTEM_DEGRADED_MESG.c_str()));
        }
    }

    // Retrieve the group that has changed state
    // ++ AZ: TD6972 : check for NULL
    if (group != 0)
    {
        boost::shared_ptr<std::string> groupName(reinterpret_cast <std::string*>(group));
        showMsgBox(IDS_UE_020058, *groupName, isOnline ? IDS_ONLINE : IDS_OFFLINE);
    }

    FUNCTION_EXIT;
#endif
}

void ControlStationRootDialog::onDutyChange(WPARAM dutyNotificationDetail, LPARAM nSeqFlag)
{
    // pop up message to see if user wants to launch the duty manager
    FUNCTION_ENTRY("onDutyChange");

    // if there's no active session, then don't display it
    if (!SessionDetails::getInstance().isSessionValid())
    {
        LOG_INFO("Logging out, so not going to show dialog");
        return;
    }

    // TD19075 ++
    // here we create a new dlg
    if (1 == nSeqFlag)
    {
        DutyChangeNoteDlg* pNoteDlg = new DutyChangeNoteDlg();

        if (pNoteDlg->doModal() == QDialog::Rejected)
        {
            delete pNoteDlg;
            this->postDutyChanged();
            LOG(SourceInfo, DebugUtil::FunctionExit, "onDutyChange");
            return;
        }
    }

    // Allow multiple Duty Notification dialogs to appear
    // have to create this on the heap so that it remains valid when the method ends
    DutyNotificationDialog* dutyNotificationDialog = new DutyNotificationDialog(reinterpret_cast<DutyManager::DutyNotificationDetail*>(dutyNotificationDetail));

    dutyNotificationDialog->create();

    referenceDialog(*dutyNotificationDialog);

    //m_comControlStation->Fire_dutyChanged();
    postDutyChanged();

    FUNCTION_EXIT;
}

void ControlStationRootDialog::onDutyRequest(WPARAM dutyRequestDetail, LPARAM)
{
    // pop up message to see if user wants to launch the duty manager
    FUNCTION_ENTRY("onDutyRequest");

    // if there's no active session, then don't display it
    if (!SessionDetails::getInstance().isSessionValid())
    {
        LOG_INFO("Logging out, so not going to show dialog");
        return;
    }

    // Allow multiple Duty Notification dialogs to appear
    // have to create this on the heap so that it remains valid when the method ends
    DutyManager::DutyRequestDetail* detail = reinterpret_cast<DutyManager::DutyRequestDetail*>(dutyRequestDetail);
    DutyRequestDialog* dutyRequestDialog = new DutyRequestDialog(detail);

    // if could not create the duty request dialog successfully, then just accept request
    if (!dutyRequestDialog->create())
    {
        LOG(SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugWarn,
            "Unable to display the duty request dialog, so automatically accept the request");
        onDutyResponse(reinterpret_cast<WPARAM>(&detail->uniqueId), static_cast<LPARAM>(TA_Base_App::TA_ControlStation::ACCEPT));
    }

    referenceDialog(*dutyRequestDialog);
    FUNCTION_EXIT;
}

void ControlStationRootDialog::onDutyResponse(WPARAM uniqueId, LPARAM response)
{
    // pop up message to see if user wants to launch the duty manager
    FUNCTION_ENTRY("onDutyResponse");

    std::string* id = reinterpret_cast<std::string*>(uniqueId);
    TA_Base_App::TA_ControlStation::EDutyResponse resp = static_cast<TA_Base_App::TA_ControlStation::EDutyResponse>(response);
    DutyManager::getInstance().respond(*id, resp);
    delete id;

    FUNCTION_EXIT;
}

void ControlStationRootDialog::onShowTaskManager(WPARAM, LPARAM)
{
    FUNCTION_ENTRY("onShowTaskManager");
    FUNCTION_EXIT;
}

void ControlStationRootDialog::onHideGraphWorxDisplays(WPARAM, LPARAM)
{
    FUNCTION_ENTRY("onHideGraphWorxDisplays");

    if (m_comControlStation != NULL)
    {
        // Send an event to GraphWorx to tell it to hide all displays
        m_comControlStation->Fire_hideDisplays();
    }

    FUNCTION_EXIT;
}

void ControlStationRootDialog::onLoginDetailsChanged(WPARAM, LPARAM)
{
    loginDetailsChanged();
}

void ControlStationRootDialog::onCreateDialogs(WPARAM, LPARAM)
{
    FUNCTION_ENTRY("onCreateDialogs");
    //TODO: TaskManagerDlg, LaunchBar(ControlStationBar)
    FUNCTION_EXIT;
}

void ControlStationRootDialog::dereferenceDialog(WPARAM dialog, LPARAM)
{
    m_childDialog.erase(reinterpret_cast<CsDialog*>(dialog));
}

void ControlStationRootDialog::displayLaunchApplicationError(WPARAM wp, LPARAM lp)
{
    CString applicationName(reinterpret_cast<std::string*>(wp)->c_str());
    delete reinterpret_cast<std::string*>(wp);
    showMsgBox(IDS_UE_010002, applicationName);
}

void ControlStationRootDialog::onTAToGXMessage(WPARAM pMsg, LPARAM)
{
    FUNCTION_ENTRY("onTAToGXMessage");

    TAToGxMessageInformation* pGxMsg = reinterpret_cast<TAToGxMessageInformation*>(pMsg);

    if (pMsg == NULL)
    {
        LOG_WARNING("onTAToGXMessage passed was NULL. Ignore notification");
        return;
    }

    if (m_comControlStation != NULL)
    {
        //LOG( SourceInfo, DebugUtil::GenericLog, DebugUtil::DebugDebug, "Sending TAToGXMessage %s with asset %s on screen %li", info->display, info->asset, info->position);
        m_comControlStation->Fire_gxMessage(_bstr_t(pGxMsg->targetSchematicID), _bstr_t(pGxMsg->sourceEntityName), _bstr_t(pGxMsg->message));
    }

    delete pGxMsg;
    pGxMsg = NULL;

    FUNCTION_EXIT;
}

//lizettejl++ (DP-changes)
void ControlStationRootDialog::onGXToTAMessage(WPARAM pMsg, LPARAM)
{
    FUNCTION_ENTRY("onGxToTAMessage");

    TA_Base_Core::IManagedApplicationCorbaDef::GXToTAMessage* pGxMsg = reinterpret_cast<TA_Base_Core::IManagedApplicationCorbaDef::GXToTAMessage*>(pMsg);

    if (pMsg == NULL)
    {
        LOG_WARNING("onGxToTAMessage passed was NULL. Ignore notification");
        return;
    }

    //UINT selectedButton = userMsg.showMsgBox(IDS_UE_020070);
    const char* msg = pGxMsg->message.in();
    std::string token;
    std::vector<std::string> args;

    // parse pGxMsg->message
    while (*msg != '\0')
    {
        if ((*msg == ':') && (*(msg + 1) == ':'))
        {
            if (!token.empty())
            {
                args.push_back(token);
                token.erase();
            }

            msg += 2;
        }

        token += *msg;

        if (*(msg + 1) == '\0')
        {
            args.push_back(token);
        }

        msg++;
    }

    //message must be in this format:
    //TransActiveMessage::<Transactive message resource ID>[::<message param1>::<message paramN>]
    if (args.size() >= 2)
    {
        // we are expecting a transactive message to passed from graphworx
        // initialize message ID that we are expecting an invalid message format
        UINT messageId = IDS_UE_803011;

        if (strcmp(args[0].c_str(), "TransActiveMessage") == 0)
        {
            if (strcmp(args[1].c_str(), "IDS_UW_650005") == 0)
            {
                //messageId for "Operator to proceed with Normal to DP (NDP) Operation"
                messageId = IDS_UW_650005;
            }
            else if (strcmp(args[1].c_str(), "IDS_UW_650006") == 0)
            {
                //messageId for ""
                messageId = IDS_UW_650006;
            }
        }

        showMsgBox(messageId);
    }

    delete pGxMsg;
    pGxMsg = NULL;

    FUNCTION_EXIT;
}

/////////////////////////////////////////////////////////////////////////////
// Other methods
/////////////////////////////////////////////////////////////////////////////

void ControlStationRootDialog::loginDetailsChanged()
{
#if 0
    FUNCTION_ENTRY("loginDetailsChanged");

    CString profileName = SessionDetails::getInstance().getProfileName().c_str();

    // Send an event to GraphWorX to alert it that the login details such as profile and operator have changed.
    if (m_comControlStation != NULL)
    {
        CString operatorName = SessionDetails::getInstance().getOperatorName().c_str();
        BOOL displayOnly = SessionDetails::getInstance().getDisplayMode();

        if (displayOnly)
        {
            m_comControlStation->Fire_loginDetailsChanged(_bstr_t(operatorName), _bstr_t(profileName), _bstr_t(PROFILE_DISPLAY_ONLY));
        }
        else
        {
            m_comControlStation->Fire_loginDetailsChanged(_bstr_t(operatorName), _bstr_t(profileName), _bstr_t(PROFILE_DISPLAY_NORMAL));
        }

        LOG_INFO("Fired login details changed to ISCS Main Navigator with operator = %s, profile = %s", operatorName, profileName);
    }

    LOG_INFO("Clear ATS alarm queue");
    TA_Base_Bus::DataStoreUpdaterATSAlarmInfo::getInstance()->clearAllData();
    TA_Base_Bus::DataStoreUpdaterATSAlarmInfo::getInstance()->release();

    LOG_INFO("Clear ATS alarm cache begin");
    //clean up ATS alarm
    boost::shared_ptr<I_ATS_ISCS_ALARM::ISCSProfileInfo>  returnProfile;
    returnProfile = TA_Base_Bus::DataStoreReceiverISCSProfile::getInstance()->getProfile();
    returnProfile->displayOnly = m_loginDialog.getIsDisplayOnly();
    returnProfile->updateTime = time(NULL);
    std::vector<boost::shared_ptr<I_ATS_ISCS_ALARM::ISCSProfileInfo> > ISCSbatch;
    ISCSbatch.push_back(returnProfile);
    TA_Base_Bus::DataStoreUpdaterISCSProfile::getInstance()->onDispatchObjects(ISCSbatch);
    LOG_INFO("Clear ATS alarm cache end");

    //send  update details information to ATS ( C952 )
    LOG_INFO("Send profile to ATS operator  before");
    std::vector<boost::shared_ptr<ATSProfileInfo> > batch;
    boost::shared_ptr<ATSProfileInfo> atsProfile(new ATSProfileInfo());
    atsProfile->loginLocation = SessionDetails::getInstance().getLocationName();
    atsProfile->operatorId = TA_Base_Core::toString(SessionDetails::getInstance().getOperatorName());
    atsProfile->profileId = SessionDetails::getInstance().getProfileName();
    atsProfile->updateTime = time(NULL);
    atsProfile->displayOnly = m_loginDialog.getIsDisplayOnly();

    LOG_INFO("operatorId=%s, profile=%s, Location=%s  updateTime=%ld, displayOnly=%d", atsProfile->operatorId.c_str(), atsProfile->profileId.c_str(), atsProfile->loginLocation.c_str(), atsProfile->updateTime, atsProfile->displayOnly);

    //TA_Base_Bus::DataStoreUpdaterATSProfile::getInstance()->Initialize(batch);
    batch.push_back(atsProfile);
    TA_Base_Bus::DataStoreUpdaterATSProfile::getInstance()->onDispatchObjects(batch);
    LOG_INFO("Send profile to ATS operator  end");
    //SessionDetails::getInstance().getOperatorName()

    // Send an event to third party clients to alert them that the login details such as session Id and profile have changed.
    if (m_comRippleControlStation != NULL)
    {
        CString sessionId = SessionDetails::getInstance().getSessionId().c_str();
        m_comRippleControlStation->Fire_detailsChanged(sessionId.AllocSysString(), profileName.AllocSysString());
        LOG_INFO("Fired login details changed to ATS displays with session ID = %s, profile = %s", sessionId, profileName);
    }

    //TD15156++
    if (TA_Base_Core::RunParams::getInstance().isSet(RPARAM_USE_SCREENSAVER.c_str()))
    {
        //install the screen saver
        // construct the full path name of the screen saver
        ScreenSaverInstall install;
        install.InitInstance();
    }

    //[2011-11-10]
    if (m_shouldStartTimer == TRUE && m_timerFlag == FALSE)
    {
        //SetTimer(123123, 60000, 0);
        m_timer.start(60000);
        LOG_INFO("Timer is successfully set up!");
        m_timerFlag = TRUE;
    }

    FUNCTION_EXIT;
#endif
}

void ControlStationRootDialog::OnClose()
{
    if (m_timerFlag == TRUE)
    {
        //KillTimer(123123);
        m_timer.stop();
        m_timerFlag = FALSE;
    }
}

void ControlStationRootDialog::onControlStationExit()
{
    emit onControlStationExitSignal();
}

void ControlStationRootDialog::onControlStationExitSlot()
{
    exit();
}

void ControlStationRootDialog::exit()
{
    cancelChildDialogs();
    QMainWindow::close();
}

void ControlStationRootDialog::logout()
{
    loginDetailsChanged();
    cancelChildDialogs();
    stopTimer();
}

void ControlStationRootDialog::referenceDialog(CsDialog& dialog)
{
    m_childDialog.insert(&dialog);
}

void ControlStationRootDialog::cancelChildDialogs()
{
    TransActiveMessage::closeAllMessages();

    // make all the child dialogs call OnCancel
    for (CsDialog* dialog : m_childDialog)
    {
        dialog->postMessage(WM_FORCE_CANCEL);
    }

    // TD834 - send a WM_CANCELMODE message to all top level windows to get rid of any popup menus.
    // TD1608 - changed from sendMessage to postMessage to prevent control station from hanging
    // on some systems
    //::postMessage(HWND_BROADCAST, WM_CANCELMODE, 0, 0);
    CsDialog::broadcastMessage(WM_CANCELMODE);
}

bool ControlStationRootDialog::hasInitDialog()
{
    TA_THREADGUARD(m_initDlgLock);
    return m_hasInitDialog;
}

void ControlStationRootDialog::OnTimer()
{
    LASTINPUTINFO lpi;
    lpi.cbSize = sizeof(LASTINPUTINFO);

    if (!GetLastInputInfo(&lpi))
    {
        LOG_ERROR("Failed to get the time of last input event!");
        return;
    }

    DWORD tillTime = GetTickCount();
    long noActionTimePeriod = tillTime - lpi.dwTime;

    if (noActionTimePeriod >= m_automatic_logout_time * 60000)
    {
        //if (m_sessionManager.getLoginStatus() == TRUE)
        {
            m_timer.stop();
            m_timerFlag = false;
            std::ostringstream outputMessage;
            outputMessage << "No action in " << m_automatic_logout_time << " minutes!";
            LOG(DEBUG_WARN, "auto logout due to %s", outputMessage.str().c_str());
            logout();
        }
    }
}

void ControlStationRootDialog::showNoPermissionMessage(const CString& actionName, TA_Base_App::TA_ControlStation::EPermission permission)
{
    if (permission == TA_Base_App::TA_ControlStation::DENIED)
    {
        LOG_INFO("Operator tries to %s without the right permission", actionName);
        showMsgBox(IDS_UE_010021, Utilities::getOperatorNameFromSession(), actionName);
    }
    else if (permission == TA_Base_App::TA_ControlStation::UNKNOWN)
    {
        LOG_INFO("The system could not determine the rights of the current operator to %s", actionName);
        showMsgBox(IDS_UE_020056, actionName);
    }
    else if (permission == TA_Base_App::TA_ControlStation::GENERAL_ERROR)
    {
        LOG_INFO("Some kind of error has occurred while checking for rights to %s", actionName);
        showMsgBox(IDS_UE_010018, actionName);
    }
}

void ControlStationRootDialog::handleSessionException(const TA_Base_Core::SessionException& exception, const CString& actionName, const CString& agentName)
{
    switch (exception.getFailType())
    {
    case TA_Base_Core::SessionException::AUTHENTICATION_FAILED:
        showMsgBox(IDS_UE_020046, actionName);
        break;

    case TA_Base_Core::SessionException::AUTHENTICATION_AGENT_FAILED:
        showMsgBox(IDS_UE_030064, actionName, agentName);
        break;

    default:
        showMsgBox(IDS_UE_010018, actionName);
        break;
    }
}

void ControlStationRootDialog::setupMessageMap()
{
    m_messageMap[WM_LAUNCH_DISPLAY]                     = boost::bind(&ControlStationRootDialog::onLaunchDisplay, this, _1, _2);
    m_messageMap[WM_SYSTEMCONTROLLER_STATE_CHANGE]      = boost::bind(&ControlStationRootDialog::onSystemControllerStateChange, this, _1, _2);
    m_messageMap[WM_DUTY_CHANGE]                        = boost::bind(&ControlStationRootDialog::onDutyChange, this, _1, _2);
    m_messageMap[WM_DUTY_REQUEST]                       = boost::bind(&ControlStationRootDialog::onDutyRequest, this, _1, _2);
    m_messageMap[WM_DUTY_RESPONSE]                      = boost::bind(&ControlStationRootDialog::onDutyResponse, this, _1, _2);
    m_messageMap[WM_SHOW_TASKMANAGER]                   = boost::bind(&ControlStationRootDialog::onShowTaskManager, this, _1, _2);
    m_messageMap[WM_HIDE_GRAPHWORX]                     = boost::bind(&ControlStationRootDialog::onHideGraphWorxDisplays, this, _1, _2);
    m_messageMap[WM_CREATE_DIALOGS]                     = boost::bind(&ControlStationRootDialog::onCreateDialogs, this, _1, _2);
    m_messageMap[WM_LOGIN_DETAILS_CHANGED]              = boost::bind(&ControlStationRootDialog::onLoginDetailsChanged, this, _1, _2);
    m_messageMap[WM_DISPLAY_LOGIN_DIALOG]               = boost::bind(&ControlStationRootDialog::onDisplayLoginDialogue, this, _1, _2);
    m_messageMap[WM_DISPLAY_OVERRIDE_DIALOG]            = boost::bind(&ControlStationRootDialog::onDisplayOperatorOverrideDialogue, this, _1, _2);
    m_messageMap[WM_DISPLAY_CHANGE_PROFILE_DIALOG]      = boost::bind(&ControlStationRootDialog::onDisplayChangeProfileDialogue, this, _1, _2);
    m_messageMap[WM_DISPLAY_CHANGE_DISPLAY_MODE_DIALOG] = boost::bind(&ControlStationRootDialog::onDisplayChangeDisplayModeDialogue, this, _1, _2);
    m_messageMap[WM_DISPLAY_CHANGE_OPERATOR_DIALOG]     = boost::bind(&ControlStationRootDialog::onDisplayChangeOperatorDialogue, this, _1, _2);
    m_messageMap[WM_DISPLAY_CHANGE_PASSWORD_DIALOG]     = boost::bind(&ControlStationRootDialog::onDisplayChangePasswordDialogue, this, _1, _2);
    m_messageMap[WM_LOGOUT]                             = boost::bind(&ControlStationRootDialog::onLogout, this, _1, _2);
    m_messageMap[WM_FORCE_LOGOUT]                       = boost::bind(&ControlStationRootDialog::onForceLogout, this, _1, _2);
    m_messageMap[WM_FORCE_END_OVERRIDE]                 = boost::bind(&ControlStationRootDialog::onForceEndOverride, this, _1, _2);
    m_messageMap[WM_CANCEL_CHILD]                       = boost::bind(&ControlStationRootDialog::dereferenceDialog, this, _1, _2);
    m_messageMap[WM_DISPLAY_LAUNCH_APPLICATION_ERROR]   = boost::bind(&ControlStationRootDialog::displayLaunchApplicationError, this, _1, _2);
    m_messageMap[WM_TA_TO_GX_MESSAGE]                   = boost::bind(&ControlStationRootDialog::onTAToGXMessage, this, _1, _2);
}

void ControlStationRootDialog::onMessageSlot(unsigned int messageId, WPARAM wparam, LPARAM lparam)
{
    MessageMap::iterator findIt = m_messageMap.find(messageId);

    if (findIt != m_messageMap.end())
    {
        findIt->second(wparam, lparam);
    }
}

void ControlStationRootDialog::showView(bool modal)
{
    //hide();
    //setVisible(false);
    show();
}

/////////
//
// TEST
//
/////////

void ControlStationRootDialog::onLoginButton()
{
    LoginTask task;
    boost::thread t(task);
    //postMessage(WM_DISPLAY_LOGIN_DIALOG);
}

void ControlStationRootDialog::onLogoutButton()
{
    LogoutTask task;
    boost::thread t(task);
    //postMessage(WM_LOGOUT);
}

void ControlStationRootDialog::onChangePasswordButton()
{
    postMessage(WM_DISPLAY_CHANGE_PASSWORD_DIALOG);
}

void ControlStationRootDialog::onOperatorOverrideButton()
{
    postMessage(WM_DISPLAY_OVERRIDE_DIALOG);
}

void ControlStationRootDialog::onChangeProfileButton()
{
    postMessage(WM_DISPLAY_CHANGE_PROFILE_DIALOG);
}

void ControlStationRootDialog::onChangeDisplayModeButton()
{
    postMessage(WM_DISPLAY_CHANGE_DISPLAY_MODE_DIALOG);
}

void ControlStationRootDialog::onChangeOperatorButton()
{
    postMessage(WM_DISPLAY_CHANGE_OPERATOR_DIALOG);
}

void ControlStationRootDialog::onDutyNottionButton()
{
    DutyManager::SubsystemDetail subsystem_detail;
    subsystem_detail.regionName = "Region";
    subsystem_detail.subsystemNames.push_back("Subsystem");

    DutyManager::DutyDetail duty_detail;
    duty_detail.operatorName = "Operator";
    duty_detail.profileName = "Profile";
    duty_detail.locationName = "Location";
    duty_detail.subsystemDetails.push_back(subsystem_detail);

    DutyManager::DutyNotificationDetail notification_detail;
    notification_detail.gained.push_back(duty_detail);
    notification_detail.denied.push_back(duty_detail);
    notification_detail.lost.push_back(duty_detail);
    notification_detail.changed.push_back(duty_detail);

    DutyManager::DutyRequestDetail request_detail;
    request_detail.duty = duty_detail;
    request_detail.uniqueId = 1;

    postMessage(WM_DUTY_CHANGE, reinterpret_cast<WPARAM>(new DutyManager::DutyNotificationDetail(notification_detail)), 1);
    //ControlStationRootDialog::getInstance().postMessage(WM_DUTY_REQUEST, reinterpret_cast<WPARAM>(new DutyManager::DutyRequestDetail(request_detail)), 1);
}

void ControlStationRootDialog::onDutyRequestButton()
{
    DutyManager::SubsystemDetail subsystem_detail;
    subsystem_detail.regionName = "Region";
    subsystem_detail.subsystemNames.push_back("Subsystem");

    DutyManager::DutyDetail duty_detail;
    duty_detail.operatorName = "Operator";
    duty_detail.profileName = "Profile";
    duty_detail.locationName = "Location";
    duty_detail.subsystemDetails.push_back(subsystem_detail);

    DutyManager::DutyNotificationDetail notification_detail;
    notification_detail.gained.push_back(duty_detail);
    notification_detail.denied.push_back(duty_detail);
    notification_detail.lost.push_back(duty_detail);
    notification_detail.changed.push_back(duty_detail);

    DutyManager::DutyRequestDetail request_detail;
    request_detail.duty = duty_detail;
    request_detail.uniqueId = 1;

    postMessage(WM_DUTY_REQUEST, reinterpret_cast<WPARAM>(new DutyManager::DutyRequestDetail(request_detail)));
}

void ControlStationRootDialog::onExclusiveDutyButton()
{
    QString str = "hello, world!";
    //ExclusiveDutyDialog exclusiveDutyDialog(str);
    //exclusiveDutyDialog.doModal();
}

void ControlStationRootDialog::onOverRideExclusiveDuty()
{
    QString str = "hello, world!";
    //OverRideExclusiveDuty dialog(str);
    //dialog.doModal();
}
