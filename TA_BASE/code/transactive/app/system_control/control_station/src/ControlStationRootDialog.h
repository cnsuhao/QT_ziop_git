/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/ControlStationRootDialog.h $
  * @author:  Karen Graham
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * This is an extension of CDialog and is the root dialog and is responsible
  * for displaying the appropriate dialog boxes, messages and GraphWorX displays.
  */

#pragma once
#include "ui_ControlStationRootDialog.h"
#include "CsDialog.h"
#include "ScreenPositioning.h"
#include "ControlStationConstants.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"
#include "core/threads/src/IWorkItem.h"
#include <QMainWindow>
#include <QTimer>

namespace TA_Base_Core
{
    class NonReEntrantThreadLockable;
    class ReEntrantThreadLockable;
    class SessionException;
}

namespace TA_Base_Bus
{
    class ITransActiveApp;
}

class CTaskManagerDlg;
class CControlStation2;
class CSchematicDisplay;
class ScreenPositioning;
class SessionManager;

// structure for display related information
struct DisplayInformation
{
    CString display;     // The name of the display
    CString asset;       // The identifier located on the display that is centred on the the screen
    long position;       // The position on the screen that this display is displayed on
    // (the display will take up the entire screen area where this point is located)
};

class ControlStationRootDialog : public TA_Base_Bus::AbstractTransActiveDialog
{
    Q_OBJECT

public:

    /**
      * getInstance
      *
      * This method makes this class a singleton. This should be called to retrieve an instance
      * of the class.
      */
    static ControlStationRootDialog& getInstance();
    static ControlStationRootDialog& createInstance(TA_Base_Bus::ITransActiveApp* app);

    void setupSignalConnections();
    void startTimer();
    void stopTimer();

    void onControlStationExit();

signals:

    void onControlStationExitSignal();

public slots:

    void onControlStationExitSlot();

public:

    /**
      * setControlStationComObject
      *
      * This sets the Control Station COM object that can be used
      * to send events. Because this dialog is the main thread of the application
      * this must be used to send the COM events. Therefore we need a pointer to
      * the object that can send the events so we can initiate it.
      *
      */
    void setControlStationComObject(CControlStation2* obj)
    {
        m_comControlStation = obj;
    }

    /**
      * setRippleControlStationComObject
      *
      * This sets the second Control Station COM object that will be used
      * by the third party clients.  The control station will only send login
      * details changed event to these clients.
      *
      */
    void setRippleControlStationComObject(CSchematicDisplay* obj)
    {
        m_comRippleControlStation = obj;
    }

    bool hasInitDialog();
    void fireDutyChanged();
    inline void postDutyChanged();
    void showNoPermissionMessage(const CString& actionName, TA_Base_App::TA_ControlStation::EPermission permission);
    void handleSessionException(const TA_Base_Core::SessionException& exception, const CString& actionName, const CString& agentName);

    virtual void showView(bool modal);
    virtual std::string getMyPModelName() { return ""; }
    virtual void setupPModel() {}

public slots:

    void OnTimer();
    virtual void onMessageSlot(unsigned int messageId, WPARAM wparam, LPARAM lparam);

    void onLoginButton();
    void onLogoutButton();
    void onChangePasswordButton();
    void onOperatorOverrideButton();
    void onChangeProfileButton();
    void onChangeDisplayModeButton();
    void onChangeOperatorButton();
    void onDutyNottionButton();
    void onDutyRequestButton();
    void onExclusiveDutyButton();
    void onOverRideExclusiveDuty();

signals:

    void sendMessageSignal(unsigned int messageId, WPARAM wparam, LPARAM lparam);
    void sendMessageThreadSignal(unsigned int messageId, WPARAM wparam, LPARAM lparam);
    void postMessageSignal(unsigned int messageId, WPARAM wparam, LPARAM lparam);

protected:

    /**
      * OnOK
      *
      * Override this with empty method as we don't want the control station
      * to end whenever the user hits the enter key
      */
    virtual void OnOK() {}

    /**
      * OnCancel
      *
      * Override this with empty method as we don't want the control station
      * to end whenever the user hits the escape key
      */
    virtual void OnCancel() {}

    /**
      * referenceDialog
      *
      * References the child dialog
      *
      * @param dialog Child dialog
      */
    void referenceDialog(CsDialog& dialog);

    /**
      * cancelChildDialogs
      *
      * Calls the OnCancel method of all child dialogs
      *
      */
    void cancelChildDialogs();

    unsigned long m_numberOfMonitors;    // This is the number of monitors the machine has

    void onInitDialog();
    void OnClose();
    void onLaunchDisplay(WPARAM display, LPARAM);
    void onServerStateChange(WPARAM isContactable, LPARAM);
    void onSystemControllerStateChange(WPARAM isOnline, LPARAM group);
    void onDutyChange(WPARAM dutyNotificationDetail, LPARAM);
    void onDutyRequest(WPARAM dutyRequestDetail, LPARAM);
    void onDutyResponse(WPARAM uniqueId, LPARAM response);
    void onShowTaskManager(WPARAM, LPARAM);
    void onHideGraphWorxDisplays(WPARAM, LPARAM);
    void onCreateDialogs(WPARAM, LPARAM);
    void onDisplayLoginDialogue(WPARAM, LPARAM);
    void onDisplayChangeProfileDialogue(WPARAM, LPARAM);
    void onDisplayChangeDisplayModeDialogue(WPARAM, LPARAM);
    void onDisplayOperatorOverrideDialogue(WPARAM, LPARAM);
    void onDisplayChangeOperatorDialogue(WPARAM, LPARAM);
    void onDisplayChangePasswordDialogue(WPARAM, LPARAM);
    void onLogout(WPARAM, LPARAM);
    void onForceLogout(WPARAM, LPARAM);
    void onForceEndOverride(WPARAM, LPARAM);
    void onLoginDetailsChanged(WPARAM, LPARAM);
    void dereferenceDialog(WPARAM dialog, LPARAM);
    void displayLaunchApplicationError(WPARAM, LPARAM);
    void onTAToGXMessage(WPARAM pMsg, LPARAM);
    void onGXToTAMessage(WPARAM pMsg, LPARAM); //DP-changes

private:

    //
    //Hide constructors and operator= as this is a singleton
    //
    ControlStationRootDialog(TA_Base_Bus::ITransActiveApp* app);
    virtual ~ControlStationRootDialog();

    /**
      * loginDetailsChanged
      *
      * This is called to update all necessary components (ie ControlStationBar, GraphWorX)
      * that the login details have changed a refresh is required.
      */
    void loginDetailsChanged();

    /**
      * exit
      *
      * Exits from the control station.
      *
      * @param isRestart True if the control station needs to restart the workstation upon termination
      *
      */
    void exit();

    /**
      * logout
      *
      * Terminates the current session
      *
      */
    void logout();

    virtual void setupMessageMap();

private:

    // This is the Control Station COM object that will be used to fire events
    // to the clients.
    CControlStation2* m_comControlStation;

    // This is a Control Station COM object that will be used mainly by third party
    // clients to get relevant information to display the right schematics.
    // It will only get the login details change event.
    CSchematicDisplay* m_comRippleControlStation;

    // vector for storing the references to child dialogs
    std::set<CsDialog*> m_childDialog;

    // Message that is sent to GraphWorx to tell it to disable itself to prevent
    // operator from selecting from the GraphWorx menu when the logout dialog
    // is being displayed
    static const std::string MENU_SUSPEND_MESG;

    // Message that is sent to GraphWorx to tell it to enable itself as operator
    // has just cancelled the logout dialog
    static const std::string MENU_RESUME_MESG;

    // Message that is sent to GraphWorx to tell it to change the colour of the
    // operator/profile panel to indicate the system is in degraded mode.
    static const std::string SYSTEM_DEGRADED_MESG;

    // Message that is sent to GraphWorx to tell it to change the colour of the
    // operator/profile panel to indicate the system is in normal mode.
    static const std::string SYSTEM_NORMAL_MESG;

    // Message that is sent to GraphWorx to tell it to change the colour of the
    // operator/profile panel to indicate the system is in displayOnly mode.
    static const std::string PROFILE_DISPLAY_ONLY ;

    // Message that is sent to GraphWorx to tell it to change the colour of the
    // operator/profile panel to indicate the system is in normal display mode.
    static const std::string PROFILE_DISPLAY_NORMAL ;

    //run param to enable or disable the screensaver
    static const std::string RPARAM_USE_SCREENSAVER;

    volatile bool m_hasInitDialog; //whether call onInitDialog
    static TA_Base_Core::ReEntrantThreadLockable m_initDlgLock;

    int m_automatic_logout_time;
    bool m_timerFlag ;
    bool m_shouldStartTimer;

    Ui::ControlStationRootDialog ui;
    QTimer m_timer;
    Qt::HANDLE m_threadId;
    typedef std::map<unsigned int, boost::function<void(WPARAM, LPARAM)>> MessageMap;
    MessageMap m_messageMap;
};
