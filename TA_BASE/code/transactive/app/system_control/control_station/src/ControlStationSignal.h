#pragma once
#include "AsyncSignal.h"

struct ControlStationSignal
{
    static AsyncSignal<void(), AsyncSignalWait::WaitForAll> waitInitializationComplete;
    static AsyncSignal<void()> start;
    static AsyncSignal<void(), AsyncSignalWait::WaitForAll> exit;
};
