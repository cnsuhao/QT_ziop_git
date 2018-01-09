#pragma once
#include "Promise.h"
#include "LoginInfo.h"
#include "AsyncSignal.h"

struct ChangeProfileDialogSignal
{
    // Receive
    static AsyncSignal<void()> showDialog;
    static AsyncSignal<void()> closeDialog;

    // Emit
    static AsyncSignal<void()> dialogShowed;
    static AsyncSignal<void()> dialogClosed;
};
