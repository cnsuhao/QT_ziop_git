#include "StdAfx.h"
#include "GraphworxSignal.h"

AsyncSignal<void(TAToGxMessageInformation*)> GraphworxSignal::ta2GxMessage;
AsyncSignal<void(TA_Base_Core::IManagedApplicationCorbaDef::GXToTAMessage*)> GraphworxSignal::gx2TaMessage;
