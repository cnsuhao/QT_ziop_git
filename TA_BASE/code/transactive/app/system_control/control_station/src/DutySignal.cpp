#include "StdAfx.h"
#include "DutySignal.h"

AsyncSignal<void()> DutySignal::dutyChanged;
AsyncSignal<void(DutyManager::DutyNotificationDetailPtr, unsigned long)> DutySignal::dutyChanged2;
AsyncSignal<void(BoolPromisePtr)> DutySignal::loseExclusiveDuty;
AsyncSignal<void(const std::string&, const TA_ControlStation::EDutyResponse)> DutySignal::responseDutyAgent;
