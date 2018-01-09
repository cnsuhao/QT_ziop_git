#pragma once
#include "Promise.h"
#include "ControlStationConstants.h"
#include "Singleton.h"

using namespace TA_Base_App;
typedef Promise<TA_ControlStation::EPermission> PromisePermission;
typedef boost::shared_ptr<PromisePermission> PromisePermissionPtr;

class OperatorRightsChecker : public Singleton<OperatorRightsChecker>
{
protected:

    OperatorRightsChecker();
    void setupSignalConnections();
    void checkChangeProfilePermission(PromisePermissionPtr promise);
    void checkChangeDisplayModePermission(PromisePermissionPtr promise);
    void checkChangePasswordPermission(PromisePermissionPtr promise);
    void checkOperatorOverridePermission(PromisePermissionPtr promise);
};
