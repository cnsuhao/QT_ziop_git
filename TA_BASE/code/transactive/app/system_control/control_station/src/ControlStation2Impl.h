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
    void isLoginAllowed(const std::string& bstrOperator, const std::string& bstrPassword, Promise<bool>& bLogin);
    void isActionPermittedOnEntity(const std::string& entityName, long actionKey, Promise<ResponseEnum>& response);
    void launchPlan(long planId);
    void launchPlanByName(const std::string& planPath);
    void launchHelp();
    void loadComplete(long leftPosition);
    void getSessionId(Promise<std::string>& sessionId);
    void getCurrentDisplayName(Promise<std::string>& display);
    void getRect(EScreen targetScreen, EArea targetArea, long val,  Promise<RECT>& dim);
    void setDisplayDimension(RECT dim);
    void launchDisplayEx(const std::string& display, const std::string& asset, long leftPosition);
    void changePassword();
    void isOperatorOverridden(Promise<bool>&);
    void changeOperatorOverride();
    void displayTaskManager();
    void isServerContactable(Promise<bool>&);
    void logout();
    void giveApplicationFocus(long processId);
    void getOperator(Promise<std::string>& operatorName);
    void getSite(Promise<std::string>&);
    void getProfile(Promise<std::string>&);
    void launchApplicationEx(long appType, std::string extraCommandLine, long posType, long alignType, RECT objectDim, RECT boundaryDim);
    void getAllApplications(Promise<std::vector<ApplicationProperties>>&);
    void getRunningApplications(Promise<std::map<unsigned long, std::string>>&);
    void getLocation(Promise<std::string>&);
    void setSelectedAlarms(const std::string& asset, long leftPosition);
    void getProfileDisplay(long leftPosition, Promise<std::string>&);
    void getProfileDisplayAtLocation(long leftPosition, const std::string& locationName, Promise<std::string>&);
    void changeProfile();
    void launchDisplay(const std::string& display, long leftPosition);
    void launchApplication(long appType, long leftPosition, long topPosition, long width, long height, const std::string& addtionalCommands);
    void changeDisplayMode();
    void getDisplayMode(Promise<std::string>&);
    void setRunParam(const std::string& paramName, const std::string& paramValue);

protected:

    ControlStation2Impl();
    void setupSignalConnections();
};
