#include "StdAfx.h"
#include "Signals.h"
#include "SessionSignal.h"
#include "ApplicationSignal.h"
#include "Utilities.h"
#include "RunningApplication.h"
#include "Application.h"
#include "RunningApplicationManager.h"
#include "CommonSignal.h"
#include "ControlStationSignal.h"
#include "core/data_access_interface/src/IGui.h"
#include "core/corba/src/CorbaUtil.h"
#include "core/exceptions/src/ApplicationException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"

using namespace TA_Base_Core;

const size_t DEFAULT_TERMINATE_TIMEOUT_IN_SECONDS = 2; // seconds

RunningApplication::RunningApplication(Application* application,
                                       const std::string& additionalCommands,
                                       unsigned long posFlag,
                                       unsigned long alignFlag,
                                       const RECT& objectDim,
                                       const RECT& boundaryDim,
                                       bool isVisible)
    : m_isVisibleToUserAsRunning(isVisible),
      m_terminateTimeout(DEFAULT_TERMINATE_TIMEOUT_IN_SECONDS),
      m_application(application),
      m_additionalCommands(additionalCommands),
      m_shoulsAutoRelaunch(false)
{
    ApplicationMover::WindowMoveInformation& info = m_applicationMover.getProcessDetails();
    info.posFlag = posFlag;
    info.alignFlag = alignFlag;
    info.objectDim = objectDim;
    info.boundaryDim = boundaryDim;

    if (RunParams::getInstance().isSet("StylePath"))
    {
        m_additionalCommands += " --StylePath=" + RunParams::getInstance().get("StylePath");
    }

    if (RunParams::getInstance().isSet("LanguagePath"))
    {
        m_additionalCommands += " --LanguagePath=" + RunParams::getInstance().get("LanguagePath");
    }

    boost::async(boost::bind(&RunningApplication::launch, this));
}

RunningApplication::~RunningApplication()
{
    deregisterEveryThinsForTerminating();
}

void RunningApplication::setupSignalConnections()
{
    RunningApplicationManager::getInstance().registerRunningApplication(this);

    m_connections.push_back(ApplicationSignal::launchApplicationByAppType[getAppType()].connect(boost::bind(&RunningApplication::launchApplicationByType, this, _1, _2, _3, _4, _5, _6)));

    if (CORBA::is_nil(m_managedProcess))
    {
        m_connections.push_back(ApplicationSignal::registerManagedProcessByPid[getProcessId()].connect(boost::bind(&RunningApplication::registerManagedProcessByPid, this, _1, _2)));
    }

    if (CORBA::is_nil(m_managedGUI))
    {
        m_connections.push_back(ApplicationSignal::registerManagedApplication[getProcessId()].connect(boost::bind(&RunningApplication::registerManagedApplication, this, _1)));
    }

    m_connections.push_back(CommonSignal::serverStateChanged.connect(boost::bind(&RunningApplication::handleServerStateChangedSignal, this, _1)));
    m_connections.push_back(SessionSignal::logined.connect(boost::bind(&RunningApplication::sessionLogin, this)));
    m_connections.push_back(SessionSignal::logouted.connect(boost::bind(&RunningApplication::onLogout, this)));
    m_connections.push_back(Signals::dutyChanged.connect(boost::bind(&RunningApplication::dutyChanged, this)));
    m_connections.push_back(Signals::sessionIdChanged.connect(boost::bind(&RunningApplication::updateSessionId, this, _1)));
    m_connections.push_back(ApplicationSignal::terminating[getProcessId()].connect(boost::bind(&RunningApplication::terminating, this, _1)));
    m_connections.push_back(ApplicationSignal::giveApplicationFocus[getProcessId()].connect(boost::bind(&RunningApplication::changeApplicationFocus, this, _1)));
    m_connections.push_back(ControlStationSignal::exit.connect(boost::bind(&RunningApplication::onControlStationExit, this)));
}

void RunningApplication::deregisterEveryThinsForTerminating()
{
    RunningApplicationManager::getInstance().deregisterRunningApplication(this);

    for (auto& c : m_connections)
    {
        c.disconnect();
    }

    m_connections.clear();
}

void RunningApplication::prepareProcessOption()
{
    std::string executable;
    std::string arglist;
    std::string currentDir;
    std::string startInstruction = m_application->getGui().getExecutable();
    parseProcessAddress(executable, arglist, currentDir, startInstruction);

    m_options.command_line((startInstruction + " " + m_additionalCommands).c_str());
    m_options.working_directory(currentDir.c_str());
}

void RunningApplication::launch()
{
    prepareProcessOption();

    pid_t pid = m_process.spawn(m_options);

    if (pid != -1)
    {
        m_processId = pid;
        m_shouldExitOnLogout = m_application->getGui().shouldTerminateOnLogout();
        m_applicationMover.getProcessDetails().processId = pid;
        m_applicationMover.start();
        setupSignalConnections();
        boost::async(boost::bind(&RunningApplication::waitThread, this));
    }
    else
    {
        m_processId = 0;
    }
}

void RunningApplication::waitThread(RunningApplication* app)
{
    app->wait();
    app->onThisProcessExit();
}

void RunningApplication::wait()
{
    ACE_exitcode exitcode;
    m_process.wait(&exitcode);
}

bool RunningApplication::waitFor(time_t timeout)
{
    ACE_exitcode exitcode;
    ACE_Time_Value t(timeout);
    pid_t pid = m_process.wait(t, &exitcode);
    return (0 == pid);
}

void RunningApplication::onThisProcessExit()
{
    deregisterEveryThinsForTerminating();

    m_processId = 0;

    if (m_shoulsAutoRelaunch)
    {
        launch();
    }
    else
    {
        delete this;
    }
}

void RunningApplication::onLogout()
{
    if (m_shouldExitOnLogout)
    {
        boost::async(boost::bind(&RunningApplication::terminate, this));
        waitForThread(this, m_terminateTimeout); //TODO: use a better name
    }
}

void RunningApplication::onLogin()
{
    if (m_shouldLaunchOnLogin)
    {
        launch();
    }
}

void RunningApplication::onControlStationExit()
{
    m_shoulsAutoRelaunch = false;
    m_shouldExitOnLogout = true;
    onLogout();
}

void RunningApplication::waitForThread(RunningApplication* app, time_t timeout)
{
    if (app->waitFor(timeout))
    {
        app->kill();
    }
}

void RunningApplication::terminate()
{
    LOG_INFO("Terminating application - %s", getApplicationName().c_str());

    try
    {
        if (!CORBA::is_nil(m_managedProcess))
        {
            LOG_INFO("Terminating as a Managed Process");
            m_managedProcess->terminate();
            return;
        }
    }
    catch (const CORBA::Exception& ex)
    {
        LOG_EXCEPTION(TA_Base_Core::CorbaUtil::getInstance().exceptionToString(ex).c_str(),
                      "Exception caught from Corba actions when trying to terminate. Will now terminate like a normal Windows application.");
    }
}

void RunningApplication::kill()
{
    m_process.terminate();
}

void RunningApplication::launchApplicationByType(const std::string& additionalCommands,
                                                 unsigned long posFlag,
                                                 unsigned long alignFlag,
                                                 const RECT& objectDim,
                                                 const RECT& boundaryDim,
                                                 bool isVisible)
{
    if (m_application->getGui().shouldPassParameters())
    {
        passNewParameters(additionalCommands);
    }

    if (m_application->getGui().shouldReposition())
    {
        //TODO
    }
}

void RunningApplication::registerManagedProcessByPid(TA_Base_Core::IManagedProcessCorbaDef_var managedProcess, const std::string& entity)
{
    m_managedProcess = managedProcess;
    m_entityName = entity;
}

void RunningApplication::registerManagedApplication(TA_Base_Core::IManagedGUICorbaDef_var managedApp)
{
    m_managedGUI = managedApp;
}

unsigned long RunningApplication::getAppType() const
{
    return m_application->getGui().getKey();
}

void RunningApplication::handleServerStateChangedSignal(bool isServerUp)
{
    try
    {
        LOG_INFO("Going to change the status of %s", getApplicationName().c_str());

        if (!CORBA::is_nil(m_managedGUI))
        {
            //boost::async(boost::bind(&_objref_IManagedGUICorbaDef::serverStateChange, &(*m_managedGUI), isServerUp));
            m_managedGUI->serverStateChange(isServerUp);
        }
    }
    catch (const CORBA::Exception& ex)
    {
        LOG_EXCEPTION(TA_Base_Core::CorbaUtil::getInstance().exceptionToString(ex).c_str(),
                      "Caught while trying to change the server state. There is no point alerting the user so we'll log and carry on. The request will just have to fail");
    }
}

std::string RunningApplication::getApplicationName() const
{
    return m_application->getGui().getName();
}

std::string RunningApplication::getApplicationFullName() const
{
    return m_application->getGui().getFullName();
}

std::string RunningApplication::getHelpFile() const
{
    return m_application->getGui().getHelpFilePath();
}

void RunningApplication::updateSessionId(const std::string& sessionId)
{
    setParam(Utilities::makeRunParam(RPARAM_SESSIONID, sessionId));
}

void RunningApplication::setParam(TA_Base_Core::RunParam& param)
{
    try
    {
        if (!CORBA::is_nil(m_managedProcess))
        {
            //TODO: async
            m_managedProcess->setParam(param);
            LOG_INFO("Successfully set %s=%s for %s, process ID = %d", param.name.in(), param.value.in(), getApplicationFullName().c_str(), getProcessId());
        }
    }
    catch (const CORBA::Exception& ex)
    {
        LOG_ERROR("Caught exception '%s' while trying to update session ID on the managed process %s, process ID = %d\n"
                  "There is no point alerting the user so we'll log and carry on. The request will just have to fail",
                  TA_Base_Core::CorbaUtil::getInstance().exceptionToString(ex).c_str(), getApplicationFullName().c_str(), getProcessId());
    }
}

void RunningApplication::passNewParameters(const std::string& additionalCommands)
{
    if (CORBA::is_nil(m_managedProcess))
    {
        LOG_WARNING("Could not pass new parameters as m_managedProcess was NULL");
        return;
    }

    std::map<std::string, std::string> parameters;

    if (!RunParams::getInstance().parseCmdLineAndReturnIt(additionalCommands, parameters))
    {
        LOG_WARNING("Additional commands passed to the application could not be parsed correctly\n%s", additionalCommands.c_str());
        return;
    }

    // parameters will now hold the parsed command line
    // Now finally we can pass all these parameters on to the application

    try
    {
        LOG_INFO("Setting parameters in application: ");

        for (std::map<std::string, std::string>::iterator iter = parameters.begin(); iter != parameters.end(); ++iter)
        {
            if (!CORBA::is_nil(m_managedProcess))
            {
                // Do not pass the parameter if it is empty.
                // Do not pass the parameter if it is the entity name. This just holds configuration for the
                // GUI and should not change.
                if ((iter->second.size()) && (iter->first != RPARAM_ENTITYNAME))
                {
                    LOGMORE(SourceInfo, "%s=%s", iter->first.c_str(), iter->second.c_str());
                    TA_Base_Core::RunParam parameter;
                    parameter.name = CORBA::string_dup(iter->first.c_str());
                    parameter.value = CORBA::string_dup(iter->second.c_str());
                    m_managedProcess->setParam(Utilities::makeRunParam(iter->first, iter->second));
                }
            }
        }
    }
    catch (const CORBA::Exception& ex)
    {
        LOG_EXCEPTION(TA_Base_Core::CorbaUtil::getInstance().exceptionToString(ex),
                      "Caught while trying to setParam() on the managed process.");
        LOGMORE(SourceInfo, " There is no point alerting the user so we'll log and carry on. The request will just have to fail");
    }
}

void RunningApplication::passNewParameters(TA_Base_Core::RunParamSeq_var additionalParameters)
{
    try
    {
        if (CORBA::is_nil(m_managedProcess))
        {
            LOG_WARNING("Could not pass new parameters as m_managedProcess was NULL");
            return;
        }

        // Now finally we can pass all these parameters on to the application
        LOG_INFO("Repassing parameters to application: ");
        int size = additionalParameters->length();

        for (int i = 0; i < size; ++i)
        {
            RunParam& param = additionalParameters[i];

            if (strlen(param.name) && (strcmp(param.name.in(), RPARAM_ENTITYNAME) != 0))
            {
                LOGMORE(SourceInfo, "%s=%s", param.name, param.value);
                m_managedProcess->setParam(param);
            }
        }
    }
    catch (const CORBA::Exception& ex)
    {
        LOG_EXCEPTION(TA_Base_Core::CorbaUtil::getInstance().exceptionToString(ex).c_str(),
                      "Caught while trying to setParam() on the managed process.\n"
                      " There is no point alerting the user so we'll log and carry on. The request will just have to fail");
    }
}

void RunningApplication::parseProcessAddress(std::string& startPath, std::string& argList, std::string& currentPath, const std::string& processAddress)
{
    // Eg processAddress is "c:\transactive\alarm.exe --param=blah"
    boost::smatch m;
    boost::regex e("(?x) ((^.*?) \\\\? \\w+\\.exe) (.+)"); //TODO: use static will cause race condition issue

    if (boost::regex_search(processAddress, m, e))
    {
        startPath = boost::trim_copy(m.str(1));
        currentPath = boost::trim_copy(m.str(2));
        argList = boost::trim_copy(m.str(3));

        if (currentPath.empty())
        {
            currentPath = "C:\\transactive\\bin";
        }
    }
}

void RunningApplication::move(unsigned long posFlag,
                              unsigned long alignFlag,
                              const RECT& objectDim,
                              const RECT& boundaryDim)
{
    m_applicationMover.setProcessDetails(m_processId, posFlag, alignFlag, objectDim, boundaryDim);

    if (!CORBA::is_nil(m_managedGUI))
    {
        try
        {
            // convert normal windows rect to corba RECT
            TA_Base_Core::IManagedGUICorbaDef::RECT obj, bound;
            obj.bottom = objectDim.bottom;
            obj.top = objectDim.top;
            obj.left = objectDim.left;
            obj.right = objectDim.right;

            bound.bottom = boundaryDim.bottom;
            bound.top = boundaryDim.top;
            bound.left = boundaryDim.left;
            bound.right = boundaryDim.right;

            m_managedGUI->setWindowPosition(posFlag, alignFlag, obj, bound);
        }
        catch (const CORBA::Exception& ex)
        {
            using TA_Base_Core::ApplicationException;
            LOG_EXCEPTION(TA_Base_Core::CorbaUtil::getInstance().exceptionToString(ex).c_str(),
                          "Caught while trying to move application.");
            TA_THROW(ApplicationException("Could not move application. It may not be running"));
        }
    }
    else
    {
        m_applicationMover.terminateAndWait();
        m_applicationMover.resetThread(); // reset thread
        m_applicationMover.start();
    }
}

void RunningApplication::changeApplicationFocus(const TA_Base_Core::EFocusType focus)
{
    try
    {
        if (!CORBA::is_nil(m_managedGUI))
        {
            m_managedGUI->changeFocus(focus);
        }
        else
        {
            m_focusInfo.processId = getProcessId();
            m_focusInfo.focus = focus;
            // if it is not a managed gui, then find the window and change the focus
            ::EnumWindows(focusNonManagedGui, reinterpret_cast<long>(&m_focusInfo));
        }
    }
    catch (const CORBA::Exception& ex)
    {
        using TA_Base_Core::ApplicationException;
        LOG_EXCEPTION(TA_Base_Core::CorbaUtil::getInstance().exceptionToString(ex).c_str(),
                      "Caught while trying to change the application focus.");
        TA_THROW(ApplicationException("Could not give the application focus. It may not be running"));
    }
}

BOOL CALLBACK RunningApplication::focusNonManagedGui(HWND hWnd, LPARAM lParam)
{
    WindowFocusInfo* info = reinterpret_cast<WindowFocusInfo*>(lParam);

    // find out which process & thread created this window
    DWORD dwThisWindowProcessID;
    DWORD dwThisWindowThreadID;
    dwThisWindowThreadID = ::GetWindowThreadProcessId(hWnd, &dwThisWindowProcessID);

    //Raymond Pau++ TD16246 Each process can have more than 1 window without a parent.
    //                      Need to ensure that all of them get focus.

    // if it's the process we are interested in then we want to move it
    if (info->processId == dwThisWindowProcessID)
    {
        LOG_DEBUG("focusNonManagedGui::Found a window with matching ID");

        // Get some information about it to help determine if it is the correct one
        if (NULL != ::GetParent(hWnd))
        {
            // This child is not visible or it is not the parent window
            // Therefore we are not interested.
            LOG_DEBUG("It's a subwindow. Ignore.");
            return TRUE;
        }

        switch (info->focus)
        {
        case (TA_Base_Core::Minimise) :

            // If window is not already minimised then minimise it
            if (FALSE == ::IsIconic(hWnd))
            {
                ::ShowWindow(hWnd, SW_MINIMIZE);
            }

            break;

        case (TA_Base_Core::Maximise) :
            ::ShowWindow(hWnd, SW_MAXIMIZE);
            ::SetForegroundWindow(hWnd);
            // Set the window to be the top most so it is given focus and then move it back again.
            ::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            ::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            break;

        case (TA_Base_Core::GetFocus) :
            if (TRUE == ::IsIconic(hWnd))
            {
                ::ShowWindow(hWnd, SW_RESTORE);
            }

            ::SetForegroundWindow(hWnd);
            // Set the window to be the top most so it is given focus and then move it back again.
            ::SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            ::SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
            break;

        default:
            TA_ASSERT(false, "Invalid enumerated type passed for window focus change");
        }

        //return FALSE;
    }

    //++Raymond Pau TD16246
    return TRUE;
}

void RunningApplication::dutyChanged()
{
    try
    {
        if (!CORBA::is_nil(m_managedGUI))
        {
            m_managedGUI->dutyChanged();
            LOG_INFO("Notified process %s, process ID = %d of duty changes", getApplicationFullName().c_str(), getProcessId());
        }
    }
    catch (const CORBA::Exception& ex)
    {
        LOG_ERROR("Caught %s while trying to call dutyChanged() on the managed process %s, process ID = %d, "
                  "There is no point alerting the user so we'll log and carry on. The request will just have to fail.",
                  TA_Base_Core::CorbaUtil::getInstance().exceptionToString(ex), getApplicationFullName().c_str(), getProcessId());
    }
}

void RunningApplication::terminating(TA_Base_Core::EManagedProcessTerminateCode p_code)
{
    static const char* strCode[] = { "Requested", "Init Error", "Comms Error", "User Exit", "Dependency Failure", "Fatal Error", "Access Denied", "Init Error" };
    LOG_INFO("Managed Process has terminated with id %d and state '%s'", getProcessId(), strCode[p_code]);
}

bool RunningApplication::areInstancesLimited() const
{
    return m_application->getGui().areInstancesLimited();
}
