#pragma once
#include "RunningApplication.h"
#include "core/process_management/IDL/src/IManagedProcessCorbaDef.h"
#include <ace/Process.h>

namespace TA_Base_Core
{
    class IGui;
}

class Application;
class IProcessManager;

class ExternalRunningApplication : public RunningApplication
{
public:

    ExternalRunningApplication(Application* application,
                               TA_Base_Core::IManagedProcessCorbaDef_var managedProcess,
                               TA_Base_Core::ProcessId processId,
                               const std::string& entity);

    virtual void launch() {}
    virtual void terminate() {}
    virtual void wait();
    virtual bool waitFor(time_t timeout); // return true if timeout(in seconds)
    virtual void kill();

    virtual unsigned long getProcessId() const
    {
        return m_processId;
    }

protected:

    virtual void deregisterEveryThinsForTerminating() {}

protected:

    HANDLE m_processHandle;
};
