#include "StdAfx.h"
#include "Signals.h"

Signals::WaitUserChangeProfileInfoSignal Signals::waitUserChangeProfileInfo;
Signals::WaitUserChangePasswordInfoSignal Signals::waitUserChangePasswordInfo;
Signals::WaitUserOperatorOverrideInfoSignal Signals::waitUserOperatorOverrideInfo;
Signals::WaitUserChangeOperatorInfoSignal Signals::waitUserChangeOperatorInfo;
Signals::CheckChangeProfilePermissionSignal Signals::checkChangeProfilePermission;
Signals::CheckChangeOperatorPermissionSignal Signals::checkChangeOperatorPermission;
Signals::CheckChangePasswordPermissionSignal Signals::checkChangePasswordPermission;
Signals::LoseExclusiveDutySignal Signals::loseExclusiveDuty;

//
// LoginDialog
//

Signals::LoginDialogShowSignal Signals::loginDialogShowed;
Signals::LoginDialogFirstShowSignal Signals::loginDialogFirstShowed;

Signals::LoginDialogHideSignal Signals::loginDialogClosed;

//
// ProcessManager
//

Signals::ProcessTerminatingSignal Signals::processTerminating;
Signals::LaunchApplicationSignal Signals::launchApplication;
Signals::LaunchApplicationByAppTypeSignal Signals::launchApplicationByAppType;
Signals::RegisterManagedProcessSignal Signals::registerManagedProcess;
Signals::RegisterManagedProcessByAppTypeSignal Signals::registerManagedProcessByAppType;
Signals::RegisterManagedProcessByPidSignal Signals::registerManagedProcessByPid;
Signals::RegisterManagedApplicationSignal Signals::registerManagedApplication;
Signals::StatusChangedSignal Signals::statusChanged;
Signals::TerminatingSignal Signals::terminating;
Signals::GiveApplicationFocusSignal Signals::giveApplicationFocus;

//
// ServerMonitor
//

Signals::ServerStateChangedSignal Signals::handleServerStateChangedSignal;

//
// SessionManager
//

Signals::SessionLoginInfoSignal Signals::sessionLoginInfo;
Signals::RenewSessionSignal Signals::renewSession;
Signals::ProfileChangedInfoSignal Signals::profileChangedInfo;
Signals::OperatorChangedSignal Signals::operatorChanged;
Signals::OperatorChangedInfoSignal Signals::operatorChangedInfo;
Signals::ProfileChangedSignal Signals::profileChanged;
Signals::OperatorOverrideInfoSignal Signals::operatorOverrideInfo;
Signals::OperatorOverrideSignal Signals::operatorOverride;
Signals::RemoveOperatorOverrideSignal Signals::removeOperatorOverride;
Signals::SessionIdChangedSignal Signals::sessionIdChanged;
Signals::ControlStationExitSignal Signals::controlStationExit;

//
// DutyManager
//

Signals::ResponseDutyAgentSignal Signals::responseDutyAgent;
Signals::DutyChangedSignal Signals::dutyChanged;

//
// SystemController Message
//

Signals::SystemControllerStateChangedSignal Signals::systemControllerStateChanged;

//
// Graphworx Message
//

Signals::Ta2GxMessageSignal Signals::ta2GxMessage;
Signals::Gx2TaMessageSignal Signals::gx2TaMessage;

//
// LoginDialog
//

Signals::OperatorRequestLoginSignal Signals::operatorRequestLogin;
Signals::OperatorRequestLoginInfoSignal Signals::operatorRequestLoginInfo;
Signals::OperatorRequestChangeOperatorSignal Signals::operatorRequestChangeOperator;
Signals::OperatorRequestChangeOperatorInfoSignal Signals::operatorRequestChangeOperatorInfo;
Signals::OperatorRequestChangeProfileSignal Signals::operatorRequestChangeProfile;
Signals::OperatorRequestChangeProfileInfoSignal Signals::operatorRequestChangeProfileInfo;
Signals::OperatorRequestOperatorOverrideSignal Signals::operatorRequestOperatorOverride;
Signals::OperatorRequestOperatorOverrideInfoSignal Signals::operatorRequestOperatorOverrideInfo;
Signals::OperatorRequestRemoveOperatorOverrideSignal Signals::operatorRequestRemoveOperatorOverride;
Signals::LoginDetailChangedSignal Signals::loginDetailChanged;

//
// LogoutDialog
//

Signals::OperatorRequestLogoutSignal Signals::operatorRequestLogout;
Signals::OperatorRequestExitControlStationSignal Signals::operatorRequestExitControlStation;
Signals::OperatorRequestRestartControlStationSignal Signals::operatorRequestRestartControlStation;
Signals::CloseLogoutDialogSignal Signals::closeLogoutDialog;

//
// ControlStationRootDialog
//

Signals::OperatorRequestShowLogoutDialogSignal Signals::operatorRequestShowLogoutDialog;
Signals::OperatorRequestShowChangeProfileDialogSignal Signals::operatorRequestShowChangeProfileDialog;
Signals::OperatorRequestShowChangeDisplayModeDialogSignal Signals::operatorRequestShowChangeDisplayModeDialog;
Signals::OperatorRequestShowChangePasswordDialogSignal Signals::operatorRequestShowChangePasswordDialog;
Signals::OperatorRequestShowOperatorOverrideDialogSignal Signals::operatorRequestShowOperatorOverrideDialog;
Signals::OperatorRequestShowChangeOperatorDialogSignal Signals::operatorRequestShowChangeOperatorDialog;
