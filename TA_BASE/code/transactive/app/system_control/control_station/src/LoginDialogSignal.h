#pragma once
#include "Promise.h"
#include "LoginInfo.h"
#include "AsyncSignal.h"

struct LoginDialogSignal
{
    // Receive
    static AsyncSignal<void()> showDialog;
    static AsyncSignal<void()> closeDialog;
    static AsyncSignal<void(LoginInfoPromisePtr)> waitLoginInfo;

    // Emit
    static AsyncSignal<void()> dialogShowed;
    static AsyncSignal<void()> dialogFirstShowed;
    static AsyncSignal<void()> dialogClosed;
};
