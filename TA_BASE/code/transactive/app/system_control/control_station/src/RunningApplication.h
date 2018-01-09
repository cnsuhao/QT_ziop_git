#pragma once
#include "ApplicationMover.h"
#include "core/process_management/IDL/src/IManagedProcessCorbaDef.h"
#include "core/process_management/IDL/src/IManagedGuiCorbaDef.h"

namespace TA_Base_Core
{
    class IGui;
    class IOperator; class IProfile; class ILocation;
}

class Application;
class IProcessManager;

class RunningApplication
{
public:

    RunningApplication() {}
    virtual ~RunningApplication();

    RunningApplication(Application* application,
                       const std::string& additionalCommands,
                       unsigned long posFlag,
                       unsigned long alignFlag,
                       const RECT& objectDim,
                       const RECT& boundaryDim,
                       bool isVisible,
                       bool autoRelaunch = false);

    virtual unsigned long getProcessId() const { return m_processId; }
    virtual unsigned long getAppType() const;
    virtual std::string getEntityName() const { return m_entityName; }
    virtual std::string getApplicationName() const;
    virtual std::string getApplicationFullName() const;
    virtual std::string getHelpFile() const;
    bool isVisibleToUserAsRunning() const { return m_isVisibleToUserAsRunning; }
    bool areInstancesLimited() const;

    void move(unsigned long posFlag,
              unsigned long alignFlag,
              const RECT& objectDim,
              const RECT& boundaryDim);

protected:

    virtual void launch();
    virtual void terminate();
    virtual void wait();
    virtual bool waitFor(time_t timeout); // return true if timeout
    virtual void kill();
    virtual void sessionLogin() {}

    virtual void launchApplicationByType(const std::string& additionalCommands,
                                         unsigned long posFlag,
                                         unsigned long alignFlag,
                                         const RECT& objectDim,
                                         const RECT& boundaryDim,
                                         bool isVisible);

    virtual void registerManagedProcessByPid(TA_Base_Core::IManagedProcessCorbaDef_var managedProcess, const std::string&);
    virtual void registerManagedApplication(TA_Base_Core::IManagedGUICorbaDef_var managedApp);

    virtual void setupSignalConnections();
    virtual void deregisterEveryThinsForTerminating();
    virtual void setParam(TA_Base_Core::RunParam& param);
    void passNewParameters(TA_Base_Core::RunParamSeq_var additionalParameters);
    void passNewParameters(const std::string& additionalCommands);
    void parseProcessAddress(std::string& startPath, std::string& argList, std::string& currentPath, const std::string& processAddress);
    void prepareProcessOption();
    static void addCommonCommand(std::string& command);

    void changeApplicationFocus(const TA_Base_Core::EFocusType focus);
    void dutyChanged();
    virtual void onLogout();
    virtual void onLogin();
    virtual void onThisProcessExit();
    virtual void onControlStationExit();
    virtual void terminating(TA_Base_Core::EManagedProcessTerminateCode p_code);
    virtual void updateSessionId(const std::string& sessionId);
    virtual void handleServerStateChangedSignal(bool isServerUp);

    static void waitThread(RunningApplication* app);
    static void waitForThread(RunningApplication* app, time_t timeout);
    static BOOL CALLBACK focusNonManagedGui(HWND hWnd, LPARAM lParam);

protected:

    Application* m_application;
    bool m_shoulsAutoRelaunch;
    bool m_shouldLaunchOnLogin;
    bool m_shouldExitOnLogout;
    time_t m_terminateTimeout;
    unsigned long m_processId;
    std::string m_entityName;
    std::string m_additionalCommands;
    boost::container::vector<boost::signals2::connection> m_connections;
    bool m_transActiveApplication;
    TA_Base_Core::IManagedProcessCorbaDef_var m_managedProcess;
    TA_Base_Core::IManagedGUICorbaDef_var m_managedGUI;
    ApplicationMover m_applicationMover;

    struct WindowFocusInfo
    {
        unsigned long processId;    // The processId of the application
        TA_Base_Core::EFocusType focus;  // The type of focus
    };

    WindowFocusInfo m_focusInfo;
    ACE_Process m_process;
    ACE_Process_Options m_options;
    bool m_isVisibleToUserAsRunning;
};
