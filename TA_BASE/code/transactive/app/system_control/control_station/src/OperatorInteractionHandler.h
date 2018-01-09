#pragma once
#include "Promise.h"
#include "ControlStationConstants.h"
#include "Singleton.h"

using namespace TA_Base_App;

namespace TA_Base_Core
{
    class IOperator;
    class IProfile;
    class ILocation;
}

typedef Promise<TA_ControlStation::EPermission> PromisePermission;
typedef boost::shared_ptr<PromisePermission> PromisePermissionPtr;

class OperatorInteractionHandler : public Singleton<OperatorInteractionHandler>
{
protected:

    OperatorInteractionHandler();
    void setupSignalConnections();
    void handleOperatorRequestLoginInfoSignal(TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*, const std::string&, bool, PromisePtr promise);
    void handleRequestChangeProfileSignal(TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*, const std::string&, bool, PromisePtr promise);
    void showChangeProfileDialog();
    void showChangeDisplayModeDialog();
    void showChangePasswordDialog();

    void handleExceptionalPromise(PromisePtr promise, const std::string& actionName);
    void handleException(boost::exception_ptr exp, const std::string& actionName);
    void handlePermissionPromise(PromisePermissionPtr, const std::string& actionName);
    void handlePermission(TA_ControlStation::EPermission permission, const std::string& actionName);
};
