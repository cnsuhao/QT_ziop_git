#include "StdAfx.h"
#include "ControlStationSignal.h"

AsyncSignal<void(), AsyncSignalWait::WaitForAll> ControlStationSignal::waitInitializationComplete;
AsyncSignal<void()> ControlStationSignal::start;
AsyncSignal<void(), AsyncSignalWait::WaitForAll> ControlStationSignal::exit;
