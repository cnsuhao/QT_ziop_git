#include "StdAfx.h"
#include "CommonSignal.h"

AsyncSignal<void(bool)> CommonSignal::serverStateChanged;
AsyncSignal<void(bool, const std::string&)> CommonSignal::systemControllerStateChanged;
