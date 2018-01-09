#include "StdAfx.h"
#include "ManagedProcess.h"
#include "core/utilities/src/RunParams.h"

using namespace TA_Base_Core;

ManagedProcess::ManagedProcess(ProcessData& data,
                               const std::string& ProcessId,
                               const std::string& host,
                               EProcessStatus state,
                               EProcessStatus requestedState,
                               EOperationMode operationMode,
                               time_t started,
                               size_t restarts,
                               const std::string& debugLevel)
    : m_data(data)
{
    data.ProcessId = ProcessId.c_str();
    data.host = host.c_str();
    data.state = state;
    data.requestedState = requestedState;
    data.operationMode = operationMode;
    data.started = started;
    data.restarts = restarts;
    data.debugLevel = debugLevel.c_str();
}

void ManagedProcess::stopProcess()
{
    tryCatchAny([&]
    {
        if (!CORBA::is_nil(m_managedProcess))
        {
            m_managedProcess->terminate();
            m_data.requestedState = Stopped;
            m_data.started = 0;
        }
    });
}

void ManagedProcess::changeOperationMode(EOperationMode mode)
{
    tryCatchAny([&]
    {
        if (!CORBA::is_nil(m_managedAgent))
        {
            m_managedAgent->setOperationMode(mode);
            m_data.operationMode = mode;
        }
    });
}

void ManagedProcess::setParam(const std::string& name, const std::string& value)
{
    tryCatchAny([&]
    {
        if (!CORBA::is_nil(m_managedProcess))
        {
            TA_Base_Core::RunParam param = { name.c_str(), value.c_str() };
            m_managedProcess->setParam(param);

            if (RPARAM_DEBUGLEVEL == name)
            {
                m_data.debugLevel = value.c_str();
            }
        }
    });
}

void ManagedProcess::tryCatchAny(boost::function<void()> f)
{
    try
    {
        f();
    }
    catch (...)
    {
    }
}
