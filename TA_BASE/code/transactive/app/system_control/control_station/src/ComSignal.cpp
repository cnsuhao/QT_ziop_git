#include "StdAfx.h"
#include "ComSignal.h"

AsyncSignal<void()> ComSignal::logout;
boost::container::map<unsigned long, AsyncSignal<void()>> ComSignal::giveApplicationFocus;
AsyncSignal<void(StringPromisePtr)> ComSignal::getOperator;
AsyncSignal<void(StringPromisePtr)> ComSignal::getSite;
AsyncSignal<void(StringPromisePtr)> ComSignal::getProfile;
AsyncSignal<void(long, const std::string&, long, long, const RECT&, const RECT&)> ComSignal::launchApplicationEx;
AsyncSignal<void(boost::shared_ptr<Promise<std::vector<ApplicationProperties>>>)> ComSignal::getAllApplications;
AsyncSignal<void(boost::shared_ptr<Promise<std::map<unsigned long, std::string>>>)> ComSignal::getRunningApplications;
AsyncSignal<void(StringPromisePtr)> ComSignal::getLocation;
AsyncSignal<void(const std::string&, long)> ComSignal::setSelectedAlarms;
AsyncSignal<void(long, StringPromisePtr)> ComSignal::getProfileDisplay;
AsyncSignal<void(long, const std::string&, StringPromisePtr)> ComSignal::getProfileDisplayAtLocation;
AsyncSignal<void()> ComSignal::changeProfile;
AsyncSignal<void()> ComSignal::changeDisplayMode;
AsyncSignal<void()> ComSignal::changePassword;
AsyncSignal<void(BoolPromisePtr)> ComSignal::isServerContactable;
AsyncSignal<void()> ComSignal::displayTaskManager;
AsyncSignal<void()> ComSignal::changeOperatorOverride;
AsyncSignal<void(BoolPromisePtr)> ComSignal::isOperatorOverridden;
AsyncSignal<void(const std::string&, const std::string&, long)> ComSignal::launchDisplayEx;
AsyncSignal<void(RECT)> ComSignal::setDisplayDimension;
AsyncSignal<void(EScreen, EArea, long, boost::shared_ptr<Promise<RECT>>)> ComSignal::getRect;
AsyncSignal<void(StringPromisePtr)> ComSignal::getCurrentDisplayName;
AsyncSignal<void(long, long, long, long, long, const std::string&)> ComSignal::launchApplication;
AsyncSignal<void(const std::string&, long)> ComSignal::launchDisplay;
AsyncSignal<void(StringPromisePtr)> ComSignal::getSessionId;
AsyncSignal<void(long)> ComSignal::loadComplete;
AsyncSignal<void()> ComSignal::launchHelp;
AsyncSignal<void(long)> ComSignal::launchPlan;
AsyncSignal<void(const std::string&)> ComSignal::launchPlanByName;
AsyncSignal<void(const std::string&, const std::string&)> ComSignal::setRunParam;
AsyncSignal<void(const std::string&, long, boost::shared_ptr<Promise<ResponseEnum>>)> ComSignal::isActionPermittedOnEntity;
AsyncSignal<void(EntityMessageType, const std::string&, const std::string&, const std::string&)> ComSignal::entityCommand;
AsyncSignal<void(const std::string&, const std::string&, BoolPromisePtr)> ComSignal::isLoginAllowed;
AsyncSignal<void(StringPromisePtr)> ComSignal::getDisplayMode;
