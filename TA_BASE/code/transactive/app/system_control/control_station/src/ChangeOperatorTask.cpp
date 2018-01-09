#include "StdAfx.h"
#include "ChangeOperatorTask.h"
#include "Promise.h"
#include "Signals.h"
#include "UserExceptionHandler.h"

void ChangeOperatorTask::operator()()
{
    struct DialogGuard
    {
        DialogGuard()
        {
            Signals::operatorRequestShowChangeOperatorDialog();
        }

        ~DialogGuard()
        {
            Signals::closeChangeOperatorDialog();
        }
    };

    DialogGuard guard;

    PromisePermissionPtr permission(new PromisePermission);
    Signals::checkChangeOperatorPermission(permission);

    if (permission->get() != TA_ControlStation::PERMITTED)
    {
        UserExceptionHandler::getInstance().handlePermission(permission->get(), "Change Operator");
        return;
    }

    while (true)
    {
        LoginInfoPromisePtr loginInfo(new LoginInfoPromise);
        Signals::waitUserChangeProfileInfo(loginInfo);
        const LoginInfo& info = loginInfo->get();

        if (LoginInfo::CancelRequest == info.m_request)
        {
            return;
        }

        PromisePtr promise(new VoidPromise);
        Signals::operatorRequestChangeOperatorInfo(info.m_operator, info.m_profile, info.m_location, info.m_password, info.m_displayOnly, promise);
        promise->wait();

        if (!promise->has_exception())
        {
            return;
        }

        UserExceptionHandler::getInstance().handleException(promise->get_exception_ptr(), "Change Operator");
    }
}
