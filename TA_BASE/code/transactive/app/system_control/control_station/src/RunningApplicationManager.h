#pragma once
#include "Singleton.h"
#include "core/process_management/IDL/src/IManagedProcessCorbaDef.h"

class Application;
class RunningApplication;
class ExternalRunningApplication;
typedef std::map<unsigned long, RunningApplication*> Pid2RunningApplicationMap;
typedef std::map<unsigned long, std::set<RunningApplication*> > AppType2RunningApplicationsMap;
typedef std::map<std::string, RunningApplication*> Name2RunningApplicationMap;

class RunningApplicationManager  : public Singleton<RunningApplicationManager>
{
public:

    RunningApplication* createNamedRunningApplication(const std::string& name);

    RunningApplication* createRunningApplication(Application* application,
                                                 const std::string& additionalCommands,
                                                 unsigned long posFlag,
                                                 unsigned long alignFlag,
                                                 const RECT& objectDim,
                                                 const RECT& boundaryDim,
                                                 bool isVisible,
                                                 bool autoRelaunch = false);

    ExternalRunningApplication* createRunningApplication(Application* application,
                                                         TA_Base_Core::IManagedProcessCorbaDef_var managedProcess,
                                                         TA_Base_Core::ProcessId processId,
                                                         const std::string& entity);

    size_t getRunningApplicationCount(unsigned long appType);
    bool isRunning(unsigned long pid);
    RunningApplication* getRunningApplication(unsigned long processId);
    std::set<RunningApplication*>& getRunningApplications(unsigned long appType);
    std::map<unsigned long, std::string> getRunningApplications(bool areGettingJustVisible = true);
    void registerRunningApplication(RunningApplication* application);
    void deregisterRunningApplication(RunningApplication* application);

public:

    RunningApplicationManager() {}

protected:

    Pid2RunningApplicationMap m_pid2RunningApplicationMap;
    AppType2RunningApplicationsMap m_appType2RunningApplicationsMap;
    Name2RunningApplicationMap m_name2RunningApplicationMap;
};
