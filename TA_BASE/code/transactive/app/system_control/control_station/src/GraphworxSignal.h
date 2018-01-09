#pragma once
#include "AsyncSignal.h"
#include "TAToGxMessageInformation.h"
#include "core/process_management/IDL/src/IManagedApplicationCorbaDef.h"

struct GraphworxSignal
{
    static AsyncSignal<void(TAToGxMessageInformation*)> ta2GxMessage;
    static AsyncSignal<void(TA_Base_Core::IManagedApplicationCorbaDef::GXToTAMessage*)> gx2TaMessage;
};
