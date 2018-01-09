#include "StdAfx.h"
#include "ComSignal.h"

boost::signals2::signal<void()> ComSignal::logout;
boost::container::map<unsigned long, boost::signals2::signal<void()>> ComSignal::giveApplicationFocus;
boost::signals2::signal<void(Promise<std::string>&)> ComSignal::getOperator;
boost::signals2::signal<void(Promise<std::string>&)> ComSignal::getSite;
boost::signals2::signal<void(Promise<std::string>&)> ComSignal::getProfile;
boost::signals2::signal<void(long, const std::string&, long, long, const RECT&, const RECT&)> ComSignal::launchApplicationEx;
boost::signals2::signal<void(Promise<std::vector<ApplicationProperties>>&)> ComSignal::getAllApplications;
boost::signals2::signal<void(Promise<std::map<unsigned long, std::string>>&)> ComSignal::getRunningApplications;
boost::signals2::signal<void(Promise<std::string>&)> ComSignal::getLocation;
boost::signals2::signal<void(const std::string&, long)> ComSignal::setSelectedAlarms;
boost::signals2::signal<void(long, Promise<std::string>&)> ComSignal::getProfileDisplay;
boost::signals2::signal<void(long, const std::string&, Promise<std::string>&)> ComSignal::getProfileDisplayAtLocation;
boost::signals2::signal<void()> ComSignal::changeProfile;
boost::signals2::signal<void()> ComSignal::changeDisplayMode;
boost::signals2::signal<void()> ComSignal::changePassword;
boost::signals2::signal<void(Promise<bool>&)> ComSignal::isServerContactable;
boost::signals2::signal<void()> ComSignal::displayTaskManager;
boost::signals2::signal<void()> ComSignal::changeOperatorOverride;
boost::signals2::signal<void(Promise<bool>&)> ComSignal::isOperatorOverridden;
boost::signals2::signal<void(const std::string&, const std::string&, long)> ComSignal::launchDisplayEx;
boost::signals2::signal<void(RECT)> ComSignal::setDisplayDimension;
boost::signals2::signal<void(EScreen, EArea, long, Promise<RECT>&)> ComSignal::getRect;
boost::signals2::signal<void(Promise<std::string>&)> ComSignal::getCurrentDisplayName;
boost::signals2::signal<void(long, long, long, long, long, const std::string&)> ComSignal::launchApplication;
boost::signals2::signal<void(const std::string&, long)> ComSignal::launchDisplay;
boost::signals2::signal<void(Promise<std::string>&)> ComSignal::getSessionId;
boost::signals2::signal<void(long)> ComSignal::loadComplete;
boost::signals2::signal<void()> ComSignal::launchHelp;
boost::signals2::signal<void(long)> ComSignal::launchPlan;
boost::signals2::signal<void(const std::string&)> ComSignal::launchPlanByName;
boost::signals2::signal<void(const std::string&, const std::string&)> ComSignal::setRunParam;
boost::signals2::signal<void(const std::string&, long, Promise<ResponseEnum>&)> ComSignal::isActionPermittedOnEntity;
boost::signals2::signal<void(EntityMessageType, const std::string&, const std::string&, const std::string&)> ComSignal::entityCommand;
boost::signals2::signal<void(const std::string&, const std::string&, Promise<bool>&)> ComSignal::isLoginAllowed;
boost::signals2::signal<void(Promise<std::string>&)> ComSignal::getDisplayMode;
