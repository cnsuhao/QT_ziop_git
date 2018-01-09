#include "StdAfx.h"
#include "OperatorInteractionHandler.h"
#include "Signals.h"
#include "Resource.h"
#include "Utilities.h"
#include "bus/TA_MessageResource/Resource.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "core/exceptions/src/SessionException.h"
#include "core/exceptions/src/GraphWorxException.h"
#include "core/utilities/src/DebugUtil.h"

using namespace TA_Base_Core;

OperatorInteractionHandler::OperatorInteractionHandler()
{
    setupSignalConnections();
}

void OperatorInteractionHandler::setupSignalConnections()
{
    Signals::operatorRequestLoginInfo.connect(boost::bind(&OperatorInteractionHandler::handleOperatorRequestLoginInfoSignal, this, _1, _2, _3, _4, _5, _6));
    Signals::operatorRequestChangeProfileInfo.connect(boost::bind(&OperatorInteractionHandler::handleRequestChangeProfileSignal, this, _1, _2, _3, _4, _5, _6));
    Signals::operatorRequestShowChangeProfileDialog.connect(boost::bind(&OperatorInteractionHandler::showChangeProfileDialog, this));
    Signals::operatorRequestShowChangeDisplayModeDialog.connect(boost::bind(&OperatorInteractionHandler::showChangeDisplayModeDialog, this));
    Signals::operatorRequestShowChangePasswordDialog.connect(boost::bind(&OperatorInteractionHandler::showChangePasswordDialog, this));
}

void OperatorInteractionHandler::handleOperatorRequestLoginInfoSignal(IOperator*, IProfile*, ILocation* location, const std::string&, bool, PromisePtr promise)
{
    //handleExceptionalPromise(promise, "Login");
}

void OperatorInteractionHandler::showChangeProfileDialog()
{
    //handlePermissionPromise(promise, "change profile");
}

void OperatorInteractionHandler::showChangeDisplayModeDialog()
{
    //DO Nothing
}

void OperatorInteractionHandler::showChangePasswordDialog()
{
    //handlePermissionPromise(promise, "change password");
}

void OperatorInteractionHandler::handleRequestChangeProfileSignal(TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation* location, const std::string&, bool, PromisePtr promise)
{
    promise->wait();

    if (promise->has_exception())
    {
        handleExceptionalPromise(promise, "Change Profile");
    }
    else
    {
        Signals::closeChangeProfileDialog();
    }
}

void OperatorInteractionHandler::handleExceptionalPromise(PromisePtr promise, const std::string& actionName)
{
    promise->wait();

    if (promise->has_exception())
    {
        handleException(promise->get_exception_ptr(), actionName);
    }
}

void OperatorInteractionHandler::handleException(boost::exception_ptr exp, const std::string& actionName)
{
    try
    {
        boost::rethrow_exception(exp);
    }
    catch (SessionException& exception)
    {
        switch (exception.getFailType())
        {
        case SessionException::AUTHENTICATION_FAILED:
            showMsgBox(IDS_UE_020046, actionName);
            break;

        case SessionException::AUTHENTICATION_AGENT_FAILED:
            showMsgBox(IDS_UE_030064, actionName, IDS_AUTHENTICATION_AGENT);
            break;

        //TODO: Password
        //case SessionException::INVALID_PASSWORD_LENGTH:
        //    showMsgBox(IDS_UE_020055, password, MIN_PASSWORD_LENGTH);
        //    m_passwordDialog.useExistingDataOnNextDoModal();
        //    break;

        //case SessionException::PASSWORD_MISMATCH:
        //    showMsgBox(IDS_UE_020045);
        //    m_passwordDialog.useExistingDataOnNextDoModal();
        //    break;

        //case SessionException::PASSWORD_USEDBEFORE:
        //    showMsgBox(IDS_UE_620023);
        //    break;

        default:
            showMsgBox(IDS_UE_010018, actionName);
            break;
        }
    }
    catch (GraphWorxException&)
    {
        showMsgBox(IDS_UE_020049);
    }
    catch (const TransactiveException&)
    {
        showMsgBox(IDS_UE_010018, IDS_CHANGE_PROFILE);
        //TODO
        // TransactiveException need to log operator off cos we don't have a valid session.
        //m_rootDialog->postMessage(WM_FORCE_LOGOUT);
    }
    catch (...)
    {
    }
}

void OperatorInteractionHandler::handlePermissionPromise(PromisePermissionPtr promise, const std::string& actionName)
{
    promise->wait();

    if (promise->get() != TA_Base_App::TA_ControlStation::PERMITTED)
    {
        handlePermission(promise->get(), actionName);
    }
}

void OperatorInteractionHandler::handlePermission(TA_ControlStation::EPermission permission, const std::string& actionName)
{
    if (permission == TA_Base_App::TA_ControlStation::DENIED)
    {
        LOG_INFO("Operator tries to %s without the right permission", actionName);
        showMsgBox(IDS_UE_010021, Utilities::getOperatorNameFromSession(), actionName);
    }
    else if (permission == TA_Base_App::TA_ControlStation::UNKNOWN)
    {
        LOG_INFO("The system could not determine the rights of the current operator to %s", actionName);
        showMsgBox(IDS_UE_020056, actionName);
    }
    else if (permission == TA_Base_App::TA_ControlStation::GENERAL_ERROR)
    {
        LOG_INFO("Some kind of error has occurred while checking for rights to %s", actionName);
        showMsgBox(IDS_UE_010018, actionName);
    }
}
