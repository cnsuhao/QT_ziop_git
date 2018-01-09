#include "StdAfx.h"
#include "LogoutDialogSignal.h"

// Receive
AsyncSignal<void()> LogoutDialogSignal::showDialog;
AsyncSignal<void()> LogoutDialogSignal::closeDialog;
AsyncSignal<void(LogoutInfoPromisePtr)> LogoutDialogSignal::waitLogoutInfo;

// Emit
AsyncSignal<void()> LogoutDialogSignal::dialogShowed;
AsyncSignal<void()> LogoutDialogSignal::dialogClosed;
