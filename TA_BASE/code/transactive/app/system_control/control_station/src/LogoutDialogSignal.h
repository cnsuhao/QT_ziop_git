#pragma once
#include "Promise.h"
#include "AsyncSignal.h"

struct LogoutInfo
{
    enum class Request
    {
        Logout,
        ExitControlStation,
        Cancel
    };

    LogoutInfo(Request req = Request::Cancel)
        : m_request(req)
    {
    }

    Request m_request;
};

typedef Promise<LogoutInfo> LogoutInfoPromise;
typedef boost::shared_ptr<LogoutInfoPromise> LogoutInfoPromisePtr;

struct LogoutDialogSignal
{
    // Receive
    static AsyncSignal<void()> showDialog;
    static AsyncSignal<void()> closeDialog;
    static AsyncSignal<void(LogoutInfoPromisePtr)> waitLogoutInfo;

    // Emit
    static AsyncSignal<void()> dialogShowed;
    static AsyncSignal<void()> dialogClosed;
};
