#include "StdAfx.h"
#include "ExternalRunningApplication.h"
#include "Signals.h"
#include "Application.h"
#include "RunningApplicationManager.h"
#include "core/data_access_interface/src/IGui.h"
#include "core/process_management/IDL/src/IManagedApplicationCorbaDef.h"

ExternalRunningApplication::ExternalRunningApplication(Application* application,
                                                       TA_Base_Core::IManagedProcessCorbaDef_var managedProcess,
                                                       TA_Base_Core::ProcessId processId,
                                                       const std::string& entity)
{
    m_processId = processId;
    m_managedProcess = managedProcess;
    m_processHandle = ::OpenProcess(PROCESS_ALL_ACCESS, false, processId);
    RunningApplication::setupSignalConnections();
    boost::async(boost::bind(&RunningApplication::waitThread, this));
}

void ExternalRunningApplication::wait()
{
    if (m_processHandle)
    {
        ::WaitForSingleObject(m_processHandle, INFINITE);
    }
}

bool ExternalRunningApplication::waitFor(time_t timeout)
{
    int result = 0;

    if (m_processHandle)
    {
        result = ::WaitForSingleObject(m_processHandle, timeout * 1000);
    }

    return WAIT_TIMEOUT == result;
}

void ExternalRunningApplication::kill()
{
    ::TerminateProcess(m_processHandle, 0);
}
