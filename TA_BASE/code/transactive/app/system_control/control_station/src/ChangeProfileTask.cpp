#include "StdAfx.h"
#include "ChangeProfileTask.h"
#include "Promise.h"
#include "Signals.h"
#include "UserExceptionHandler.h"

void ChangeProfileTask::operator()()
{
    struct DialogGuard
    {
        DialogGuard()
        {
            Signals::operatorRequestShowChangeProfileDialog();
        }

        ~DialogGuard()
        {
            Signals::closeChangeProfileDialog();
        }
    };

    DialogGuard guard;

    PromisePermissionPtr permission(new PromisePermission);
    Signals::checkChangeProfilePermission(permission);

    if (permission->get() != TA_ControlStation::PERMITTED)
    {
        UserExceptionHandler::getInstance().handlePermission(permission->get(), "Change Profile");
        return;
    }

    bool lostDuty = false;

    while (true)
    {
        LoginInfoPromisePtr loginInfo(new LoginInfoPromise);
        Signals::waitUserChangeProfileInfo(loginInfo);
        const LoginInfo& info = loginInfo->get();

        if (LoginInfo::CancelRequest == info.m_request)
        {
            return;
        }

        if (!lostDuty)
        {
            boost::shared_ptr<Promise<bool> > loseDuty(new Promise<bool>);
            Signals::loseExclusiveDuty(loseDuty);

            if (!loseDuty->get())
            {
                //TODO
                return;
            }

            lostDuty = true;
        }

        PromisePtr promise(new VoidPromise);
        Signals::operatorRequestChangeProfileInfo(info.m_operator, info.m_profile, info.m_location, info.m_password, info.m_displayOnly, promise);
        promise->wait();

        if (!promise->has_exception())
        {
            return;
        }

        UserExceptionHandler::getInstance().handleException(promise->get_exception_ptr(), "Change Profile");
    }
}
