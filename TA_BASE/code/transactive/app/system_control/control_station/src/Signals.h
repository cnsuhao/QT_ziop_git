#pragma once
#include "ControlStationConstants.h"
#include "Promise.h"
#include "LoginInfo.h"
#include "core/process_management/IDL/src/IManagedProcessCorbaDef.h"
#include "core/process_management/IDL/src/IManagedApplicationCorbaDef.h"
#include "core/process_management/IDL/src/IManagedGuiCorbaDef.h"
#include "core/process_management/IDL/src/IManagedApplicationCorbaDef.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"

using namespace TA_Base_App;

typedef Promise<TA_ControlStation::EPermission> PromisePermission;
typedef boost::shared_ptr<PromisePermission> PromisePermissionPtr;

namespace TA_Base_Core
{
    class IOperator; class IProfile; class ILocation;
}

struct TAToGxMessageInformation;

struct Signals
{
    //
    typedef boost::signals2::signal<void(LoginInfoPromisePtr)> OperatorSubmitLoginInfoSignal;
    static OperatorSubmitLoginInfoSignal operatorSubmitLoginInfo;

    typedef boost::signals2::signal<void(LoginInfoPromisePtr)> WaitUserChangeProfileInfoSignal;
    static WaitUserChangeProfileInfoSignal waitUserChangeProfileInfo;

    typedef boost::signals2::signal<void(LoginInfoPromisePtr)> WaitUserChangePasswordInfoSignal;
    static WaitUserChangePasswordInfoSignal waitUserChangePasswordInfo;

    typedef boost::signals2::signal<void(LoginInfoPromisePtr)> WaitUserOperatorOverrideInfoSignal;
    static WaitUserOperatorOverrideInfoSignal waitUserOperatorOverrideInfo;

    typedef boost::signals2::signal<void(LoginInfoPromisePtr)> WaitUserChangeOperatorInfoSignal;
    static WaitUserChangeOperatorInfoSignal waitUserChangeOperatorInfo;

    typedef boost::signals2::signal<void(PromisePermissionPtr)> CheckChangeProfilePermissionSignal;
    static CheckChangeProfilePermissionSignal checkChangeProfilePermission;

    typedef boost::signals2::signal<void(PromisePermissionPtr)> CheckChangeOperatorPermissionSignal;
    static CheckChangeOperatorPermissionSignal checkChangeOperatorPermission;

    typedef boost::signals2::signal<void(PromisePermissionPtr)> CheckChangePasswordPermissionSignal;
    static CheckChangePasswordPermissionSignal checkChangePasswordPermission;

    typedef boost::signals2::signal<void(boost::shared_ptr<Promise<bool> >)> LoseExclusiveDutySignal;
    static LoseExclusiveDutySignal loseExclusiveDuty;

    //
    // LoginDialog
    //

    typedef boost::signals2::signal<void()> LoginDialogShowSignal;
    static LoginDialogShowSignal loginDialogShowed;
    typedef boost::signals2::signal<void()> LoginDialogFirstShowSignal;
    static LoginDialogFirstShowSignal loginDialogFirstShowed;

    typedef boost::signals2::signal<void()> LoginDialogHideSignal;
    static LoginDialogHideSignal loginDialogClosed;

    //
    // ProcessManager
    //

    // process-id, terminate-code
    typedef boost::container::map<unsigned long, boost::signals2::signal<void(unsigned long)> > ProcessTerminatingSignal;
    static ProcessTerminatingSignal processTerminating;

    typedef boost::signals2::signal<void(unsigned long, const std::string&, unsigned long, unsigned long, const RECT&, const RECT&, bool)> LaunchApplicationSignal;
    static LaunchApplicationSignal launchApplication;

    typedef boost::container::map<unsigned long, boost::signals2::signal<void(const std::string&, unsigned long, unsigned long, const RECT&, const RECT&, bool)> > LaunchApplicationByAppTypeSignal;
    static LaunchApplicationByAppTypeSignal launchApplicationByAppType;

    typedef boost::signals2::signal<void(TA_Base_Core::IManagedProcessCorbaDef_var, unsigned long, unsigned long, const char*)> RegisterManagedProcessSignal;
    static RegisterManagedProcessSignal registerManagedProcess;

    typedef boost::container::map<unsigned long, boost::signals2::signal<void(unsigned long, TA_Base_Core::IManagedProcessCorbaDef_var, const std::string& entity)> > RegisterManagedProcessByAppTypeSignal;
    static RegisterManagedProcessByAppTypeSignal registerManagedProcessByAppType;

    typedef boost::container::map<unsigned long, boost::signals2::signal<void(TA_Base_Core::IManagedProcessCorbaDef_var, const std::string& entity)> > RegisterManagedProcessByPidSignal;
    static RegisterManagedProcessByPidSignal registerManagedProcessByPid;

    typedef boost::container::map<unsigned long, boost::signals2::signal<void(TA_Base_Core::IManagedGUICorbaDef_var)> > RegisterManagedApplicationSignal;
    static RegisterManagedApplicationSignal registerManagedApplication;

    typedef boost::container::map<unsigned long, boost::signals2::signal<void(TA_Base_Core::EProcessStatus)> > StatusChangedSignal;
    static StatusChangedSignal statusChanged;

    typedef boost::container::map<unsigned long, boost::signals2::signal<void(TA_Base_Core::EManagedProcessTerminateCode)> > TerminatingSignal;
    static TerminatingSignal terminating;

    typedef boost::container::map<unsigned long, boost::signals2::signal<void(TA_Base_Core::EFocusType focus)> > GiveApplicationFocusSignal;
    static GiveApplicationFocusSignal giveApplicationFocus;

    //
    // ServerMonitor
    //

    typedef boost::signals2::signal<void(bool)> ServerStateChangedSignal;
    static ServerStateChangedSignal handleServerStateChangedSignal;

    //
    // SessionManager
    //

    typedef boost::signals2::signal<void()> SessionLoginSignal;
    static SessionLoginSignal sessionLogin;
    typedef boost::signals2::signal<void(const std::string&, TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*, bool)> SessionLoginInfoSignal;
    static SessionLoginInfoSignal sessionLoginInfo;

    typedef boost::signals2::signal<void(const std::string&)> RenewSessionSignal;
    static RenewSessionSignal renewSession;

    typedef boost::signals2::signal<void()> ProfileChangedSignal;
    static ProfileChangedSignal profileChanged;
    typedef boost::signals2::signal<void(const std::string&, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*)> ProfileChangedInfoSignal;
    static ProfileChangedInfoSignal profileChangedInfo;

    typedef boost::signals2::signal<void()> OperatorChangedSignal;
    static OperatorChangedSignal operatorChanged;
    typedef boost::signals2::signal<void(const std::string&, TA_Base_Core::IOperator*)> OperatorChangedInfoSignal;
    static OperatorChangedInfoSignal operatorChangedInfo;

    typedef boost::signals2::signal<void(TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*)> OperatorOverrideInfoSignal;
    static OperatorOverrideInfoSignal operatorOverrideInfo;
    typedef boost::signals2::signal<void()> OperatorOverrideSignal;
    static OperatorOverrideSignal operatorOverride;
    typedef boost::signals2::signal<void()> RemoveOperatorOverrideSignal;
    static RemoveOperatorOverrideSignal removeOperatorOverride;

    typedef boost::signals2::signal<void()> SessionLogoutSignal;
    static SessionLogoutSignal sessionLogout;

    typedef boost::signals2::signal<void(const std::string&)> SessionIdChangedSignal;
    static SessionIdChangedSignal sessionIdChanged;

    typedef boost::signals2::signal<void()> ControlStationExitSignal;
    static ControlStationExitSignal controlStationExit;

    typedef boost::signals2::signal<void()> ControlStationRestartSignal;
    static ControlStationRestartSignal controlStationRestart;

    //
    // DutyManager
    //

    typedef boost::signals2::signal<void(const std::string&, const TA_Base_App::TA_ControlStation::EDutyResponse)> ResponseDutyAgentSignal;
    static ResponseDutyAgentSignal responseDutyAgent;

    typedef boost::signals2::signal<void()> DutyChangedSignal;
    static DutyChangedSignal dutyChanged;

    //
    // SystemController Message
    //

    typedef boost::signals2::signal<void(bool, const std::string&)> SystemControllerStateChangedSignal;
    static SystemControllerStateChangedSignal systemControllerStateChanged;

    //
    // Graphworx Message
    //

    typedef boost::signals2::signal<void(TAToGxMessageInformation*)> Ta2GxMessageSignal;
    static Ta2GxMessageSignal ta2GxMessage;

    typedef boost::signals2::signal<void(TA_Base_Core::IManagedApplicationCorbaDef::GXToTAMessage*)> Gx2TaMessageSignal;
    static Gx2TaMessageSignal gx2TaMessage;

    //
    // LoginDialog
    //

    typedef boost::signals2::signal<void()> OperatorRequestLoginSignal;
    static OperatorRequestLoginSignal operatorRequestLogin;
    typedef boost::signals2::signal<void(TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*, const std::string&, bool, PromisePtr)> OperatorRequestLoginInfoSignal;
    static OperatorRequestLoginInfoSignal operatorRequestLoginInfo;

    typedef boost::signals2::signal<void()> OperatorRequestChangeOperatorSignal;
    static OperatorRequestChangeOperatorSignal operatorRequestChangeOperator;
    typedef boost::signals2::signal<void(TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*, const std::string&, bool, PromisePtr)> OperatorRequestChangeOperatorInfoSignal;
    static OperatorRequestChangeOperatorInfoSignal operatorRequestChangeOperatorInfo;

    typedef boost::signals2::signal<void()> OperatorRequestChangeProfileSignal;
    static OperatorRequestChangeProfileSignal operatorRequestChangeProfile;
    typedef boost::signals2::signal<void(TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*, const std::string&, bool, PromisePtr)> OperatorRequestChangeProfileInfoSignal;
    static OperatorRequestChangeProfileInfoSignal operatorRequestChangeProfileInfo;

    typedef boost::signals2::signal<void()> OperatorRequestOperatorOverrideSignal;
    static OperatorRequestOperatorOverrideSignal operatorRequestOperatorOverride;
    typedef boost::signals2::signal<void(TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*, const std::string&, bool, PromisePtr)> OperatorRequestOperatorOverrideInfoSignal;
    static OperatorRequestOperatorOverrideInfoSignal operatorRequestOperatorOverrideInfo;

    typedef boost::signals2::signal<void(PromisePtr)> OperatorRequestRemoveOperatorOverrideSignal;
    static OperatorRequestRemoveOperatorOverrideSignal operatorRequestRemoveOperatorOverride;

    typedef boost::signals2::signal<void()> LoginDetailChangedSignal;
    static LoginDetailChangedSignal loginDetailChanged;

    //
    // LogoutDialog
    //

    typedef boost::signals2::signal<void(PromisePtr)> OperatorRequestLogoutSignal;
    static OperatorRequestLogoutSignal operatorRequestLogout;

    typedef boost::signals2::signal<void(PromisePtr)> OperatorRequestExitControlStationSignal;
    static OperatorRequestExitControlStationSignal operatorRequestExitControlStation;

    typedef boost::signals2::signal<void(PromisePtr)> OperatorRequestRestartControlStationSignal;
    static OperatorRequestRestartControlStationSignal operatorRequestRestartControlStation;

    typedef boost::signals2::signal<void()> CloseLogoutDialogSignal;
    static CloseLogoutDialogSignal closeLogoutDialog;

    //
    // ControlStationRootDialog
    //

    typedef boost::signals2::signal<void()> OperatorRequestShowLoginDialogSignal;
    static OperatorRequestShowLoginDialogSignal showLoginDialog;

    typedef boost::signals2::signal<void()> OperatorRequestShowLogoutDialogSignal;
    static OperatorRequestShowLogoutDialogSignal operatorRequestShowLogoutDialog;

    typedef boost::signals2::signal<void()> OperatorRequestShowChangeProfileDialogSignal;
    static OperatorRequestShowChangeProfileDialogSignal operatorRequestShowChangeProfileDialog;

    typedef boost::signals2::signal<void()> OperatorRequestShowChangeDisplayModeDialogSignal;
    static OperatorRequestShowChangeDisplayModeDialogSignal operatorRequestShowChangeDisplayModeDialog;

    typedef boost::signals2::signal<void()> OperatorRequestShowChangePasswordDialogSignal;
    static OperatorRequestShowChangePasswordDialogSignal operatorRequestShowChangePasswordDialog;

    typedef boost::signals2::signal<void()> OperatorRequestShowOperatorOverrideDialogSignal;
    static OperatorRequestShowOperatorOverrideDialogSignal operatorRequestShowOperatorOverrideDialog;

    typedef boost::signals2::signal<void()> OperatorRequestShowChangeOperatorDialogSignal;
    static OperatorRequestShowChangeOperatorDialogSignal operatorRequestShowChangeOperatorDialog;
};
