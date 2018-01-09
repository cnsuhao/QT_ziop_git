#include "StdAfx.h"
#include "LoginDialogSignal.h"

// Receive
AsyncSignal<void()> LoginDialogSignal::showDialog;
AsyncSignal<void()> LoginDialogSignal::closeDialog;
AsyncSignal<void(LoginInfoPromisePtr)> LoginDialogSignal::waitLoginInfo;

// Emit
AsyncSignal<void()> LoginDialogSignal::dialogShowed;
AsyncSignal<void()> LoginDialogSignal::dialogFirstShowed;
AsyncSignal<void()> LoginDialogSignal::dialogClosed;
