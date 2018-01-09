/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/AdminManager.cpp $
  * @author:  Dominique Beauregard
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * AdminManager is a singleton class that is the central class of the
  * System Manager. It retreives data from a System Controller and
  * sends the GUI's orders to that System Controller.
  */

#pragma warning(disable:4786)

#include "AppServiceCenter.h"
#include "AppServiceConsts.h"
#include "bus/common_subsystem_facade/src/SystemControllerFacade.h"
#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/data_access_interface/entity_access/src/IProcess.h"
#include "core/process_management/IDL/src/IProcessManagerCorbaDef.h"
#include "core/process_management/IDL/src/IHeartbeatReceiverCorbaDef.h"
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/exceptions/src/SystemControllerException.h"
#include "core/threads/src/Thread.h"
#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include "core/message/src/MessagePublicationManager.h"
#include "core/message/types/OperatorAudit_MessageTypes.h"
#include "core/message/src/AuditMessageSender.h"


// Forward declarations
using TA_Base_Core::SystemControllerException;

// Define statics.
TA_Base_App::AppServiceCenter* TA_Base_App::AppServiceCenter::s_instance = NULL;
TA_Base_Core::NonReEntrantThreadLockable TA_Base_App::AppServiceCenter::m_singletonLock;


namespace TA_Base_App
{
    //TD8617 Mintao++
    const unsigned int AppServiceCenter::DEFAULT_REFRESH_RATE = 0;

    AppServiceCenter::AppServiceCenter()
        : m_isRunning(false),
          m_errorOccurred(false),
          m_systemController(NULL)
    {
        // Set the possible refresh intervals.
        m_refreshIntervals.push_back(1);
        m_refreshIntervals.push_back(5);
        m_refreshIntervals.push_back(10);
        m_refreshIntervals.push_back(20);
        m_refreshIntervals.push_back(60);
        //TD8617 Mintao++
        //default refresh rate is 1 sec
        m_currentRefreshRate = DEFAULT_REFRESH_RATE;
        nThreshhold = 0;

        m_systemController = new TA_Base_Bus::SystemControllerAccessor();
    }


    AppServiceCenter::~AppServiceCenter()
    {
        if (m_systemController != NULL)
        {
            delete m_systemController;
            m_systemController = NULL;
        }

        logout();
    }


    AppServiceCenter& AppServiceCenter::getInstance()
    {
        // Make this threadsafe.
        TA_Base_Core::ThreadGuard guard(m_singletonLock);

        if (s_instance == NULL)
        {
            s_instance = new AppServiceCenter();

            if (s_instance == NULL)
            {
                LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugFatal,
                            "New failed to allocate AdminManager");
                exit(1);
            }
        }

        return *s_instance;
    }


    int AppServiceCenter::login(const std::string& hostName,
                                const std::string& portNumber,
                                const std::string& sessionId,
                                const std::string& operatorName)
    {
        if (atoi(portNumber.c_str()) < 1024)
        {
            return ERR_AUTH_SERVICE_ERROR;
        }

        getSystemController()->setEndPointInfo(hostName, atoi(portNumber.c_str()));

        bool isValidCorbaObject = getSystemController()->ping();

        if (!isValidCorbaObject)
        {
            return ERR_GET_ADMIN_ERROR ;
        }

        m_entityNameToDetails.clear();
        m_processIDToDetails.clear();

        // Start thread
        start();

        return ERR_AUTH_NO_ERROR;
    }


    void AppServiceCenter::logout()
    {
        TA_Base_Core::ThreadGuard guard(m_processLock);

        getSystemController()->resetEndPointInfo();

        terminateAndWait();
        m_processes.clear();
    }


    void AppServiceCenter::run()
    {
        const int SLEEP_TIME_MSECS = 500;

        int periodMsecs = m_refreshIntervals[m_currentRefreshRate] * 1000;
        int nIterations = periodMsecs / SLEEP_TIME_MSECS;
        int currIteration;

        m_isRunning = true;


        while (m_isRunning)
        {
            try
            {
                // Fetch process data.
                refreshData();
                m_errorOccurred = false;
            }
            catch (...)
            {
                // Couldn't retrieve process data. Notify observers, so
                // they get an exception when they call getProcesses().
                m_errorOccurred = true;
                notifyObservers(DOMAIN_DATA_CHANGE_NEW_PROCESS_DATA, NULL);
            }

            //sleep(30000);

            // Sleep for the required amount of time.
            currIteration = 0;

            while (m_isRunning && currIteration < nIterations)
            {
                sleep(SLEEP_TIME_MSECS);
                currIteration++;
            }
        }
    }


    void AppServiceCenter::terminate()
    {
        m_isRunning = false;
    }


    void AppServiceCenter::refreshData()
    {
        ProcessDataSeq_var pProcessSeq;

        try
        {
            // Get new process data.
            pProcessSeq = getSystemController()->getProcessData();
            nThreshhold = 0;
        }
        catch (const CORBA::SystemException& ex)
        {
            LOG_EXCEPTION_CATCH(SourceInfo, "CORBA::SystemException",
                                TA_Base_Core::CorbaUtil::getInstance().exceptionToString(ex).c_str());
            nThreshhold += 1;

            // Pass the exception on as a SystemControllerException.
            if (nThreshhold >= 3)
            {
                TA_THROW(SystemControllerException("Error calling getProcessData"));
            }

            return;
        }
        catch (...)
        {
            // Pass the exception on as a SystemControllerException.
            TA_THROW(SystemControllerException("Error calling getProcessData"));
        }

        TA_Base_Core::ThreadGuard guard(m_processLock);

        if (pProcessSeq->length() != m_processes.size())
        {
            // Size of sequence doesn't match vector. Repopulate vector.
            buildData(pProcessSeq);
            notifyObservers(DOMAIN_DATA_CHANGE_NEW_PROCESS_DATA, NULL);
        }
        else
        {
            for (int i = 0; i < pProcessSeq->length(); i++)
            {
                if (!compareProcesses(pProcessSeq[i], m_processes[i]))
                {
                    // Process vector out of date. Update it.
                    buildData(pProcessSeq);
                    notifyObservers(DOMAIN_DATA_CHANGE_NEW_PROCESS_DATA, NULL);
                    return;
                }
            }
        }
    }


    std::vector<int> AppServiceCenter::getRefreshIntervals()
    {
        return m_refreshIntervals;
    }


    unsigned int AppServiceCenter::getCurrentRefreshRate()
    {
        return m_currentRefreshRate;
    }


    void AppServiceCenter::setRefreshRate(unsigned int newRate)
    {
        if (newRate < m_refreshIntervals.size() &&
                newRate != m_currentRefreshRate)
        {
            m_currentRefreshRate = newRate;

            // Stop the thread and start a new one.
            terminateAndWait();
            start();
        }
    }


    void AppServiceCenter::startProcess(const std::string& processId)
    {
        FUNCTION_ENTRY("startProcess");

        try
        {
            // Convert the ProcessId (entity description) into an entity name.
            std::string entityName = m_processIDToDetails[processId].entityName;

            getSystemController()->startProcess(entityName.c_str());
        }
        catch (...)
        {
            TA_THROW(SystemControllerException("Error calling startProcess"));
        }

        FUNCTION_EXIT;
    }


    void AppServiceCenter::changeProcessOperationMode(const std::string& processId,
            TA_Base_Core::EOperationMode opMode)
    {
        FUNCTION_ENTRY("changeProcessOperationMode");

        try
        {
            // Convert the ProcessId (entity description) into an entity name.
            std::string entityName = m_processIDToDetails[processId].entityName;

            getSystemController()->changeAgentOperationMode(entityName.c_str(), opMode);
        }
        catch (TA_Base_Core::ISystemControllerAdminCorbaDef::ModeChangeException& ex)
        {

            TA_THROW(SystemControllerException(ex.reason.in()));
        }
        catch (...)
        {
            TA_THROW(SystemControllerException("Unknown error."));
        }

        FUNCTION_EXIT;
    }


    void AppServiceCenter::stopProcess(const std::string& processId)
    {
        FUNCTION_ENTRY("stopProcess");

        try
        {
            // Convert the ProcessId (entity description) into an entity name.
            std::string entityName = m_processIDToDetails[processId].entityName;

            getSystemController()->stopProcess(entityName.c_str());
        }
        catch (...)
        {
            TA_THROW(SystemControllerException("Error calling stopProcess"));
        }

        FUNCTION_EXIT;
    }


    void AppServiceCenter::shutdownSystemController()
    {
        FUNCTION_ENTRY("shutdownSystemController");

        try
        {
            std::string systemControlHostName = getSystemController()->getHostName();
            getSystemController()->shutdown(systemControlHostName.c_str());
        }
        catch (...)
        {
            TA_THROW(SystemControllerException("Error calling shutdown"));
        }

        FUNCTION_EXIT;
    }


    std::vector<ProcessData> AppServiceCenter::getProcesses()
    {
        if (m_errorOccurred)
        {
            TA_THROW(SystemControllerException("Error calling getProcessData"));
        }

        TA_Base_Core::ThreadGuard guard(m_processLock);
        return m_processes;
    }


    std::string AppServiceCenter::getEntityDescription(const std::string& entityName) const
    {
        ProcessInformationMap::const_iterator iter = m_entityNameToDetails.find(entityName);

        if (iter != m_entityNameToDetails.end())
        {
            return iter->second.entityDescription;
        }
        else
        {
            // Entity name not found. Just return "".
            return "";
        }
    }


    bool AppServiceCenter::isManagedProcessFromName(const std::string& entityName) const
    {
        ProcessInformationMap::const_iterator iter = m_entityNameToDetails.find(entityName);

        if (iter != m_entityNameToDetails.end())
        {
            return iter->second.isManagedProcess;
        }

        // Entity name not found. Just return false to be safe
        return false;
    }


    bool AppServiceCenter::isManagedProcessFromDescription(const std::string& entityDescription) const
    {
        ProcessInformationMap::const_iterator iter = m_processIDToDetails.find(entityDescription);

        if (iter != m_processIDToDetails.end())
        {
            return iter->second.isManagedProcess;
        }

        // Entity description not found. Just return false to be safe
        return false;
    }


    bool AppServiceCenter::isServiceProcessFromName(const std::string& entityName) const
    {
        ProcessInformationMap::const_iterator iter = m_entityNameToDetails.find(entityName);

        if (iter != m_entityNameToDetails.end())
        {
            return iter->second.isServiceProcess;
        }

        // Entity name not found. Just return false to be safe
        return false;
    }


    bool AppServiceCenter::isServiceProcessFromDescription(const std::string& entityDescription) const
    {
        ProcessInformationMap::const_iterator iter = m_processIDToDetails.find(entityDescription);

        if (iter != m_processIDToDetails.end())
        {
            return iter->second.isServiceProcess;
        }

        // Entity description not found. Just return false to be safe
        return false;
    }


    bool AppServiceCenter::isAutoFailBackFromDescription(const std::string& entityDescription) const
    {
        ProcessInformationMap::const_iterator iter = m_processIDToDetails.find(entityDescription);

        if (iter != m_processIDToDetails.end())
        {
            return iter->second.isAutoFailBack;
        }

        // Entity description not found. Just return false to be safe
        return false;
    }


    void AppServiceCenter::buildData(ProcessDataSeq_var processes)
    {
        m_processes.clear();
        std::string entityName;

        for (int i = 0; i < processes->length(); i++)
        {
            entityName = processes[i].ProcessId;

            ProcessInformationMap::iterator iter = m_entityNameToDetails.find(entityName);

            if (iter == m_entityNameToDetails.end())
            {
                // No details for this entity name. Try to fetch it from
                // the database.
                TA_Base_Core::IProcess* entityData = NULL;
                ProcessInformation info;

                try
                {
                    LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug,
                                "Fetching entity details for %s from database",
                                entityName.c_str());

                    entityData = dynamic_cast<TA_Base_Core::IProcess*>(TA_Base_Core::EntityAccessFactory::getInstance().getEntity(entityName));

                    if (entityData != NULL)
                    {
                        info.entityName = entityName;
                        info.entityDescription = entityData->getDescription();
                        info.isManagedProcess = entityData->isManagedProcess();
                        info.isServiceProcess = entityData->isServiceProcess();
                        info.isAutoFailBack = entityData->shouldAutoFailback();
                    }
                    else
                    {
                        // Error getting the details. Just use the default instead.
                        info.entityName = entityName;
                        info.entityDescription = entityName;
                        info.isManagedProcess = false;
                        info.isServiceProcess = false;
                        info.isAutoFailBack = false;
                    }
                }
                catch (...)
                {
                    LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,
                                "Error retrieving entity details for entity %s",
                                entityName.c_str());

                    // Error getting the details. Just use the default instead.
                    info.entityName = entityName;
                    info.entityDescription = entityName;
                    info.isManagedProcess = false;
                    info.isServiceProcess = false;
                }

                m_entityNameToDetails[entityName] = info;
                m_processIDToDetails[info.entityDescription] = info;

                if (entityData != NULL)
                {
                    delete entityData;
                    entityData = NULL;
                }
            }

            m_processes.push_back(processes[i]);

        }
    }


    void AppServiceCenter::setProcessRunParam(const std::string& processId, const TA_Base_Core::RunParam& param)
    {
        FUNCTION_ENTRY("setProcessRunParam");

        try
        {
            // Convert the ProcessId (entity description) into an entity name.
            std::string entityName = m_processIDToDetails[processId].entityName;

            getSystemController()->setRunParam(entityName.c_str(), param);
        }
        catch (...)
        {
            TA_THROW(SystemControllerException("Error calling setRunParam"));
        }

        FUNCTION_EXIT;
    }


    std::string AppServiceCenter::getProcessDebugLevel(const std::string& processId)
    {
        TA_Base_Core::ThreadGuard guard(m_processLock);

        // Convert the ProcessId (entity description) into an entity name.
        std::string entityName = m_processIDToDetails[processId].entityName;

        for (std::vector<ProcessData>::iterator iter = m_processes.begin();
                iter != m_processes.end(); iter++)
        {
            if (iter->ProcessId.in() == entityName)
            {
                return iter->debugLevel.in();
            }
        }

        return "";
    }


    bool AppServiceCenter::compareProcesses(const ProcessData& process1, const ProcessData& process2) const
    {
        if (strcmp(process1.ProcessId.in(), process2.ProcessId.in()) != 0)
        {
            return false;
        }

        if (process1.state != process2.state)
        {
            return false;
        }

        if (process1.requestedState != process2.requestedState)
        {
            return false;
        }

        if (strcmp(process1.host.in(), process2.host.in()) != 0)
        {
            return false;
        }

        if (process1.started != process2.started)
        {
            return false;
        }

        if (process1.restarts != process2.restarts)
        {
            return false;
        }

        if (process1.operationMode != process2.operationMode)
        {
            return false;
        }

        if (strcmp(process1.debugLevel.in(), process2.debugLevel.in()) != 0)
        {
            return false;
        }

        return true;
    }


    std::string AppServiceCenter::getSystemControllerHostName()
    {
        return getSystemController()->getHostName();
    }
} // namespace TA_Base_App
