#include "StdAfx.h"
#include "NamedRunningApplicationTerminator.h"
#include "Signals.h"
#include "EnumProcess.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TA_String.h"

using namespace TA_Base_Core;

NamedRunningApplicationTerminator::NamedRunningApplicationTerminator(const std::string& csv)
{
    boost::split(m_names, csv, boost::is_any_of(",:;/"));
    LOG_INFO("NamedRunningApplicationTerminator: %s", csv.c_str());
    boost::async(boost::bind(&NamedRunningApplicationTerminator::terminateApplications, this));
}

void NamedRunningApplicationTerminator::addApplicationName(const std::string& name)
{
    m_names.insert(name);
    LOG_INFO("NamedRunningApplicationTerminator::addApplicationName: %s", name.c_str());
    boost::async(boost::bind(&NamedRunningApplicationTerminator::terminateApplications, this));
}

void NamedRunningApplicationTerminator::terminateApplications()
{
    CEnumProcess enumeration;
    CEnumProcess::CProcessEntry entry;
    BOOL isMore = enumeration.GetProcessFirst(&entry);

    while (isMore)
    {
        if (m_names.find(entry.lpFilename) != m_names.end())
        {
            LOG_INFO("terminating %s (PID=%d)", entry.lpFilename, entry.dwPID);
            boost::async(boost::bind(&NamedRunningApplicationTerminator::terminateByPid, std::string(entry.lpFilename), entry.dwPID));
        }

        isMore = enumeration.GetProcessNext(&entry);
    }
}

void NamedRunningApplicationTerminator::terminateByPid(const std::string& name, unsigned long pid)
{
    HANDLE handle = ::OpenProcess(PROCESS_ALL_ACCESS, FALSE, pid);

    if (handle)
    {
        if (::TerminateProcess(handle, 0))
        {
            LOG_INFO("%s (PID=%d) terminated", name.c_str(), pid);
            return;
        }
    }

    LOG_INFO("%s (PID=%d) cannot be terminated, error: %d", name.c_str(), pid, GetLastError());
}
