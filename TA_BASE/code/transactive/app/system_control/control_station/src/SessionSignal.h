#pragma once
#include "AsyncSignal.h"
#include "TypeDefines.h"
#include "Promise.h"

struct SessionSignal
{
    static AsyncSignal<void(StringPromisePtr, TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*, const std::string&, bool)> requestSessionId;
    static AsyncSignal<void(StringPromisePtr, TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*, const std::string&, bool)> requestChangeProfile;
    static AsyncSignal<void(StringPromisePtr, TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*, const std::string&, bool)> requestChangeOperator;
    static AsyncSignal<void(PromisePtr, TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*, const std::string&, bool)> requestOperatorOverride;
    static AsyncSignal<void(PromisePtr)> requestRemoveOperatorOverride;
    static AsyncSignal<void(PromisePtr)> requestEndSession;
    static AsyncSignal<void(const std::string&, TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*, bool)> logined;
    static AsyncSignal<void(), AsyncSignalWait::WaitForAll> logouted;
    static AsyncSignal<void(const std::string&)> sessionChanged;
    static AsyncSignal<void(const std::string&, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*)> profileChanged;
    static AsyncSignal<void(const std::string&, TA_Base_Core::IOperator*)> operatorChanged;
    static AsyncSignal<void(TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*)> operatorOverrided;
    static AsyncSignal<void()> operatorOverrideRemoved;
};
