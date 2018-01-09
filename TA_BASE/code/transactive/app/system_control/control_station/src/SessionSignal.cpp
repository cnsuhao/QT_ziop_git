#include "StdAfx.h"
#include "SessionSignal.h"

AsyncSignal<void(StringPromisePtr, TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*, const std::string&, bool)> SessionSignal::requestSessionId;
AsyncSignal<void(StringPromisePtr, TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*, const std::string&, bool)> SessionSignal::requestChangeProfile;
AsyncSignal<void(StringPromisePtr, TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*, const std::string&, bool)> SessionSignal::requestChangeOperator;
AsyncSignal<void(PromisePtr, TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*, const std::string&, bool)> SessionSignal::requestOperatorOverride;
AsyncSignal<void(PromisePtr)> SessionSignal::requestRemoveOperatorOverride;
AsyncSignal<void(PromisePtr)> SessionSignal::requestEndSession;
AsyncSignal<void(const std::string&, TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*, bool)> SessionSignal::logined;
AsyncSignal<void(), AsyncSignalWait::WaitForAll> SessionSignal::logouted;
AsyncSignal<void(const std::string&)> SessionSignal::sessionChanged;
AsyncSignal<void(const std::string&, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*)> SessionSignal::profileChanged;
AsyncSignal<void(const std::string&, TA_Base_Core::IOperator*)> SessionSignal::operatorChanged;
AsyncSignal<void(TA_Base_Core::IOperator*, TA_Base_Core::IProfile*, TA_Base_Core::ILocation*)> SessionSignal::operatorOverrided;
AsyncSignal<void()> SessionSignal::operatorOverrideRemoved;
