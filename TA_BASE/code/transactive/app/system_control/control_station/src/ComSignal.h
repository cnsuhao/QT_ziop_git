#pragma once
#include "IProcessManager.h"
#include "Promise.h"
#include "AsyncSignal.h"
#include "ControlStation_i.h"

typedef IProcessManager::ApplicationProperties ApplicationProperties;

struct ComSignal
{
    static AsyncSignal<void()> logout;
    static boost::container::map<unsigned long, AsyncSignal<void()>> giveApplicationFocus;
    static AsyncSignal<void(StringPromisePtr)> getOperator;
    static AsyncSignal<void(StringPromisePtr)> getSite;
    static AsyncSignal<void(StringPromisePtr)> getProfile;
    static AsyncSignal<void(long, const std::string&, long, long, const RECT&, const RECT&)> launchApplicationEx;
    static AsyncSignal<void(boost::shared_ptr<Promise<std::vector<ApplicationProperties>>>)> getAllApplications;
    static AsyncSignal<void(boost::shared_ptr<Promise<std::map<unsigned long, std::string>>>)> getRunningApplications;
    static AsyncSignal<void(StringPromisePtr)> getLocation;
    static AsyncSignal<void(const std::string&, long)> setSelectedAlarms;
    static AsyncSignal<void(long, StringPromisePtr)> getProfileDisplay;
    static AsyncSignal<void(long, const std::string&, StringPromisePtr)> getProfileDisplayAtLocation;
    static AsyncSignal<void()> changeProfile;
    static AsyncSignal<void()> changeDisplayMode;
    static AsyncSignal<void()> changePassword;
    static AsyncSignal<void(BoolPromisePtr)> isServerContactable;
    static AsyncSignal<void()> displayTaskManager;
    static AsyncSignal<void()> changeOperatorOverride;
    static AsyncSignal<void(BoolPromisePtr)> isOperatorOverridden;
    static AsyncSignal<void(const std::string&, const std::string&, long)> launchDisplayEx;
    static AsyncSignal<void(RECT)> setDisplayDimension;
    static AsyncSignal<void(EScreen, EArea, long, boost::shared_ptr<Promise<RECT>>)> getRect;
    static AsyncSignal<void(StringPromisePtr)> getCurrentDisplayName;
    static AsyncSignal<void(long, long, long, long, long, const std::string&)> launchApplication;
    static AsyncSignal<void(const std::string&, long)> launchDisplay;
    static AsyncSignal<void(StringPromisePtr)> getSessionId;
    static AsyncSignal<void(long)> loadComplete;
    static AsyncSignal<void()> launchHelp;
    static AsyncSignal<void(long)> launchPlan;
    static AsyncSignal<void(const std::string&)> launchPlanByName;
    static AsyncSignal<void(const std::string&, const std::string&)> setRunParam;
    static AsyncSignal<void(const std::string&, long, boost::shared_ptr<Promise<ResponseEnum>>)> isActionPermittedOnEntity;
    static AsyncSignal<void(EntityMessageType, const std::string&, const std::string&, const std::string&)> entityCommand;
    static AsyncSignal<void(const std::string&, const std::string&, BoolPromisePtr)> isLoginAllowed;
    static AsyncSignal<void(StringPromisePtr)> getDisplayMode;
};
