#include "StdAfx.h"
#include "ChangeProfileDialogSignal.h"

// Receive
AsyncSignal<void()> ChangeProfileDialogSignal::showDialog;
AsyncSignal<void()> ChangeProfileDialogSignal::closeDialog;

// Emit
AsyncSignal<void()> ChangeProfileDialogSignal::dialogShowed;
AsyncSignal<void()> ChangeProfileDialogSignal::dialogClosed;
