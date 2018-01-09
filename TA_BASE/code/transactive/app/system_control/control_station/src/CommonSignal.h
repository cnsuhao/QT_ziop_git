#pragma once
#include "AsyncSignal.h"
#include "TypeDefines.h"
#include "Promise.h"

struct CommonSignal
{
    static AsyncSignal<void(bool)> serverStateChanged;
    static AsyncSignal<void(bool, const std::string&)> systemControllerStateChanged;
};
