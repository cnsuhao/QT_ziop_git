#pragma once
#include "Singleton.h"
#include "Promise.h"
#include "IProcessManager.h"
#include "ControlStation_i.h"

typedef IProcessManager::ApplicationProperties ApplicationProperties;

class ControlStation2Impl : public Singleton<ControlStation2Impl>
{
protected:

    void entityCommand(EntityMessageType entityType, const std::string& location, const std::string& sourceSchematicID, const std::string& message);
    void isLoginAllowed(const std::string& bstrOperator, const std::string& bstrPassword, BoolPromisePtr bLogin);
    void isActionPermittedOnEntity(const std::string& entityName, long actionKey, boost::shared_ptr<Promise<ResponseEnum>> response);
    void launchPlan(long planId);
    void launchPlanByName(const std::string& planPath);
    void launchHelp();
    void loadComplete(long leftPosition);
    void getSessionId(StringPromisePtr sessionId);
    void getCurrentDisplayName(StringPromisePtr display);
    void getRect(EScreen targetScreen, EArea targetArea, long val, boost::shared_ptr<Promise<RECT>> dim);
    void setDisplayDimension(RECT dim);
    void launchDisplayEx(const std::string& display, const std::string& asset, long leftPosition);
    void changePassword();
    void isOperatorOverridden(BoolPromisePtr);
    void changeOperatorOverride();
    void displayTaskManager();
    void isServerContactable(BoolPromisePtr);
    void logout();
    void giveApplicationFocus(long processId);
    void getOperator(StringPromisePtr operatorName);
    void getSite(StringPromisePtr);
    void getProfile(StringPromisePtr);
    void launchApplicationEx(long appType, std::string extraCommandLine, long posType, long alignType, RECT objectDim, RECT boundaryDim);
    void getAllApplications(boost::shared_ptr<Promise<std::vector<ApplicationProperties>>>);
    void getRunningApplications(boost::shared_ptr<Promise<std::map<unsigned long, std::string>>>);
    void getLocation(StringPromisePtr);
    void setSelectedAlarms(const std::string& asset, long leftPosition);
    void getProfileDisplay(long leftPosition, StringPromisePtr);
    void getProfileDisplayAtLocation(long leftPosition, const std::string& locationName, StringPromisePtr);
    void changeProfile();
    void launchDisplay(const std::string& display, long leftPosition);
    void launchApplication(long appType, long leftPosition, long topPosition, long width, long height, const std::string& addtionalCommands);
    void changeDisplayMode();
    void getDisplayMode(StringPromisePtr);
    void setRunParam(const std::string& paramName, const std::string& paramValue);

protected:

    ControlStation2Impl();
    void setupSignalConnections();
};
