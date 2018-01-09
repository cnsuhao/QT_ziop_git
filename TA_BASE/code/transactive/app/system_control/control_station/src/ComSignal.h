#pragma once
#include "IProcessManager.h"
#include "Promise.h"
#include "ControlStation_i.h"

typedef IProcessManager::ApplicationProperties ApplicationProperties;

struct ComSignal
{
    static boost::signals2::signal<void()> logout;
    static boost::container::map<unsigned long, boost::signals2::signal<void()>> giveApplicationFocus;
    static boost::signals2::signal<void(Promise<std::string>&)> getOperator;
    static boost::signals2::signal<void(Promise<std::string>&)> getSite;
    static boost::signals2::signal<void(Promise<std::string>&)> getProfile;
    static boost::signals2::signal<void(long, const std::string&, long, long, const RECT&, const RECT&)> launchApplicationEx;
    static boost::signals2::signal<void(Promise<std::vector<ApplicationProperties>>&)> getAllApplications;
    static boost::signals2::signal<void(Promise<std::map<unsigned long, std::string>>&)> getRunningApplications;
    static boost::signals2::signal<void(Promise<std::string>&)> getLocation;
    static boost::signals2::signal<void(const std::string&, long)> setSelectedAlarms;
    static boost::signals2::signal<void(long, Promise<std::string>&)> getProfileDisplay;
    static boost::signals2::signal<void(long, const std::string&, Promise<std::string>&)> getProfileDisplayAtLocation;
    static boost::signals2::signal<void()> changeProfile;
    static boost::signals2::signal<void()> changeDisplayMode;
    static boost::signals2::signal<void()> changePassword;
    static boost::signals2::signal<void(Promise<bool>&)> isServerContactable;
    static boost::signals2::signal<void()> displayTaskManager;
    static boost::signals2::signal<void()> changeOperatorOverride;
    static boost::signals2::signal<void(Promise<bool>&)> isOperatorOverridden;
    static boost::signals2::signal<void(const std::string&, const std::string&, long)> launchDisplayEx;
    static boost::signals2::signal<void(RECT)> setDisplayDimension;
    static boost::signals2::signal<void(EScreen, EArea, long, Promise<RECT>&)> getRect;
    static boost::signals2::signal<void(Promise<std::string>&)> getCurrentDisplayName;
    static boost::signals2::signal<void(long, long, long, long, long, const std::string&)> launchApplication;
    static boost::signals2::signal<void(const std::string&, long)> launchDisplay;
    static boost::signals2::signal<void(Promise<std::string>&)> getSessionId;
    static boost::signals2::signal<void(long)> loadComplete;
    static boost::signals2::signal<void()> launchHelp;
    static boost::signals2::signal<void(long)> launchPlan;
    static boost::signals2::signal<void(const std::string&)> launchPlanByName;
    static boost::signals2::signal<void(const std::string&, const std::string&)> setRunParam;
    static boost::signals2::signal<void(const std::string&, long, Promise<ResponseEnum>&)> isActionPermittedOnEntity;
    static boost::signals2::signal<void(EntityMessageType, const std::string&, const std::string&, const std::string&)> entityCommand;
    static boost::signals2::signal<void(const std::string&, const std::string&, Promise<bool>&)> isLoginAllowed;
    static boost::signals2::signal<void(Promise<std::string>&)> getDisplayMode;
};
