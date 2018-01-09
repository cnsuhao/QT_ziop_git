#pragma once
#include "AsyncSignal.h"
#include "Promise.h"
#include "DutyManager.h"
#include "ControlStationConstants.h"

using namespace TA_Base_App;

struct DutySignal
{
    static AsyncSignal<void()> dutyChanged;
    static AsyncSignal<void(DutyManager::DutyNotificationDetailPtr, unsigned long)> dutyChanged2;
    static AsyncSignal<void(BoolPromisePtr)> loseExclusiveDuty;
    static AsyncSignal<void(const std::string&, const TA_ControlStation::EDutyResponse)> responseDutyAgent;
};
