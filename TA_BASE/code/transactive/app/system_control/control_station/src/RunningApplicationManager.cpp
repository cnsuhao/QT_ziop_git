#include "StdAfx.h"
#include "RunningApplicationManager.h"
#include "RunningApplication.h"
#include "ExternalRunningApplication.h"

RunningApplication* RunningApplicationManager::createRunningApplication(Application* application,
                                                                        const std::string& additionalCommands,
                                                                        unsigned long posFlag,
                                                                        unsigned long alignFlag,
                                                                        const RECT& objectDim,
                                                                        const RECT& boundaryDim,
                                                                        bool isVisible)
{
    return new RunningApplication(application, additionalCommands, posFlag, alignFlag, objectDim, boundaryDim, isVisible);
}

ExternalRunningApplication* RunningApplicationManager::createRunningApplication(Application* application,
                                                                                TA_Base_Core::IManagedProcessCorbaDef_var managedProcess,
                                                                                TA_Base_Core::ProcessId processId,
                                                                                const std::string& entity)
{
    return new ExternalRunningApplication(application, managedProcess, processId, entity);
}

size_t RunningApplicationManager::getRunningApplicationCount(unsigned long appType)
{
    return m_appType2RunningApplicationsMap[appType].size();
}

bool RunningApplicationManager::isRunning(unsigned long pid)
{
    return m_pid2RunningApplicationMap.find(pid) != m_pid2RunningApplicationMap.end();
}

RunningApplication* RunningApplicationManager::getRunningApplication(unsigned long processId)
{
    Pid2RunningApplicationMap::iterator it = m_pid2RunningApplicationMap.find(processId);
    return (it != m_pid2RunningApplicationMap.end()) ? it->second : NULL;
}

std::set<RunningApplication*>& RunningApplicationManager::getRunningApplications(unsigned long appType)
{
    return m_appType2RunningApplicationsMap[appType];
}

void RunningApplicationManager::registerRunningApplication(RunningApplication* application)
{
    m_pid2RunningApplicationMap[application->getProcessId()] = application;
    m_appType2RunningApplicationsMap[application->getAppType()].insert(application);
    m_name2RunningApplicationMap[application->getApplicationName()] = application;
}

void RunningApplicationManager::deregisterRunningApplication(RunningApplication* application)
{
    m_pid2RunningApplicationMap.erase(application->getProcessId());
    m_appType2RunningApplicationsMap[application->getAppType()].erase(application);
    m_name2RunningApplicationMap.erase(application->getApplicationName());
}

std::map<unsigned long, std::string> RunningApplicationManager::getRunningApplications(bool areGettingJustVisible)
{
    std::map<unsigned long, std::string> mapOfRunningApps;

    for (Pid2RunningApplicationMap::value_type& v : m_pid2RunningApplicationMap)
    {
        if (areGettingJustVisible)
        {
            if (v.second->isVisibleToUserAsRunning())
            {
                mapOfRunningApps[v.first] = v.second->getApplicationName();
            }
        }
        else
        {
            mapOfRunningApps[v.first] = v.second->getApplicationName();
        }
    }

    return mapOfRunningApps;
}
