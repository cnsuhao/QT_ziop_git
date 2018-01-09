#include "StdAfx.h"
#include "LogoutTask.h"
#include "LoginTask.h"
#include "Promise.h"
#include "SessionSignal.h"
#include "ControlStationSignal.h"
#include "LogoutDialogSignal.h"
#include "UserExceptionHandler.h"

void LogoutTask::operator()()
{
    struct LogoutDialogGuard
    {
        LogoutDialogGuard() { LogoutDialogSignal::showDialog(); }
        ~LogoutDialogGuard() { LogoutDialogSignal::closeDialog(); }
    };

    LogoutDialogGuard guard;

    while (true)
    {
        LogoutInfoPromisePtr logoutInfo(new LogoutInfoPromise);
        LogoutDialogSignal::waitLogoutInfo(logoutInfo);
        const LogoutInfo& info = logoutInfo->get();

        if (LogoutInfo::Request::Cancel == info.m_request)
        {
            return;
        }

        PromisePtr promise(new VoidPromise);
        SessionSignal::requestEndSession(promise);
        promise->wait();

        if (!promise->has_exception())
        {
            LogoutDialogSignal::closeDialog();
            SessionSignal::logouted();

            if (LogoutInfo::Request::ExitControlStation == info.m_request)
            {
                ControlStationSignal::exit();
            }
            else
            {
                boost::async(LoginTask());
            }

            return;
        }

        UserExceptionHandler::getInstance().handleException(promise->get_exception_ptr(), "Logout");
    }
}
