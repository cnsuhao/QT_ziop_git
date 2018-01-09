#include "StdAfx.h"
#include "LoginTask.h"
#include "Promise.h"
#include "SessionSignal.h"
#include "LoginDialogSignal.h"
#include "ControlStationSignal.h"
#include "UserExceptionHandler.h"
#include "ControlStationSignal.h"
#include "ApplicationManager.h"
#include "core/utilities/src/DebugUtil.h"

void LoginTask::operator()()
{
    struct LoginDialogGuard
    {
        LoginDialogGuard()
        {
            LOG_DEBUG("LoginTask - LoginDialogSignal::showDialog()");
            LoginDialogSignal::showDialog();
        }

        ~LoginDialogGuard()
        {
            LOG_DEBUG("LoginTask - LoginDialogSignal::closeDialog()");
            LoginDialogSignal::closeDialog();
        }
    };

    LoginDialogGuard guard;

    while (true)
    {
        LoginInfoPromisePtr loginInfo(new LoginInfoPromise);
        LoginDialogSignal::waitLoginInfo(loginInfo);

        const LoginInfo& info = loginInfo->get();

        if (LoginInfo::Request::ExitControlStation == info.m_request)
        {
            ControlStationSignal::exit();
            return;
        }

        StringPromisePtr sessionId(new StringPromise);
        SessionSignal::requestSessionId(sessionId, info.m_operator, info.m_profile, info.m_location, info.m_password, info.m_displayOnly);

        sessionId->wait();

        if (!sessionId->has_exception())
        {
            SessionSignal::logined(sessionId->get(), info.m_operator, info.m_profile, info.m_location, info.m_displayOnly);
            return;
        }

        UserExceptionHandler::getInstance().handleException(sessionId->get_exception_ptr(), "Login");
    }
}
