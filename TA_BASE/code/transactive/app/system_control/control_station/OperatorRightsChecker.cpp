#include "StdAfx.h"
#include "OperatorRightsChecker.h"
#include "Signals.h"
#include "RightsManager.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"

using namespace TA_Base_Core;

OperatorRightsChecker::OperatorRightsChecker()
{
    setupSignalConnections();
}

void OperatorRightsChecker::setupSignalConnections()
{
    Signals::checkChangeProfilePermission.connect(boost::bind(&OperatorRightsChecker::checkChangeProfilePermission, this, _1));
    //Signals::operatorRequestShowChangeDisplayModePermission.connect(boost::bind(&OperatorRightsChecker::checkChangeDisplayModePermission, this, _1));
    Signals::checkChangePasswordPermission.connect(boost::bind(&OperatorRightsChecker::checkChangePasswordPermission, this, _1));
    Signals::checkChangeOperatorPermission.connect(boost::bind(&OperatorRightsChecker::checkOperatorOverridePermission, this, _1));
}

void OperatorRightsChecker::checkChangeProfilePermission(PromisePermissionPtr promise)
{
    TA_ControlStation::EPermission permission = RightsManager::getInstance().isControlPermitted(TA_Base_Bus::aca_CONTROL_STATION_CHANGE_PROFILE);
    promise->set_value(permission);
}

void OperatorRightsChecker::checkChangeDisplayModePermission(PromisePermissionPtr promise)
{
    promise->set_value(TA_ControlStation::PERMITTED);
}

void OperatorRightsChecker::checkChangePasswordPermission(PromisePermissionPtr promise)
{
    TA_ControlStation::EPermission permission = RightsManager::getInstance().isControlPermitted(TA_Base_Bus::aca_CONTROL_STATION_CHANGE_PASSWORD);
    promise->set_value(permission);
}

void OperatorRightsChecker::checkOperatorOverridePermission(PromisePermissionPtr promise)
{
    TA_ControlStation::EPermission permission = RightsManager::getInstance().isControlPermitted(TA_Base_Bus::aca_CONTROL_STATION_OVERRIDE);
    promise->set_value(permission);
}
