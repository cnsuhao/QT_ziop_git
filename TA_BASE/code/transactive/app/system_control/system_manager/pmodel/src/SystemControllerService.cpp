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
#include "SystemControllerService.h"
#include "ProcessDataPuller.h"
#include "bus/common_subsystem_facade/src/SystemControllerFacade.h"
#include "core/corba/src/CorbaUtil.h"
#include "core/process_management/IDL/src/ISystemControllerAdminCorbaDef.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include <boost/foreach.hpp>


namespace TA_Base_App
{
    TA_Base_Bus::ISystemControllerAccessor* SystemControllerService::m_systemController = NULL;
    ProcessDataPuller* SystemControllerService::m_puller = NULL;


    void SystemControllerService::setCurrentSystemController(const std::string& hostname, unsigned long port)
    {
        if (m_systemControllers.find(hostname) == m_systemControllers.end())
        {
            TA_Base_Bus::SystemControllerAccessor* systemController = new TA_Base_Bus::SystemControllerAccessor;
            systemController->setEndPointInfo(hostname, port);
            systemController->setAdminCallTimeout(1000);
            m_systemControllers.insert(std::make_pair(hostname, systemController));
            m_pullers[hostname] = new ProcessDataPuller(*systemController);
            m_pullers[hostname]->pause();
            m_pullers[hostname]->start();
        }

        m_systemController = m_systemControllers[hostname];
        m_puller = m_pullers[hostname];
    }


    void SystemControllerService::startPullProcessData()
    {
        m_puller->continuing();
    }


    void SystemControllerService::stopPullProcessData()
    {
        m_puller->pause();
    }


    void SystemControllerService::registerProcessDataObserver(const std::string& hostname, IProcessDataObserver* observer)
    {
        m_pullers[hostname]->registerProcessDataObserver(observer);
    }


    void SystemControllerService::unregisterProcessDataObserver(const std::string& hostname, IProcessDataObserver* observer)
    {
        m_pullers[hostname]->unregisterProcessDataObserver(observer);
    }


    void SystemControllerService::setRefreshRate(unsigned int rate)
    {
        size_t oldRate = ProcessDataPuller::getInterval();
        ProcessDataPuller::setInterval(rate);

        if (rate < oldRate)
        {
            m_puller->refresh();
        }
    }


    void SystemControllerService::startProcess(const std::string& processName)
    {
        m_systemController->startProcess(processName.c_str());
    }


    void SystemControllerService::startProcess(const std::vector<std::string>& processNames)
    {
        BOOST_FOREACH(const std::string& process, processNames)
        {
            startProcess(process);
        }
    }


    void SystemControllerService::changeProcessOperationMode(const std::string& processName, TA_Base_Core::EOperationMode opMode)
    {
        try
        {
            m_systemController->changeAgentOperationMode(processName.c_str(), opMode);
        }
        catch (...)
        {
        }
    }


    void SystemControllerService::changeProcessOperationMode(const std::vector<std::string>& processNames, TA_Base_Core::EOperationMode opMode)
    {
        BOOST_FOREACH(const std::string& process, processNames)
        {
            changeProcessOperationMode(process, opMode);
        }
    }


    void SystemControllerService::stopProcess(const std::string& processName)
    {
        m_systemController->stopProcess(processName.c_str());
    }


    void SystemControllerService::stopProcess(const std::vector<std::string>& processNames)
    {
        BOOST_FOREACH(const std::string& process, processNames)
        {
            stopProcess(process);
        }
    }


    void SystemControllerService::shutdownSystemController(const std::string& hostname)
    {
        m_systemController->shutdown(hostname.c_str());
    }


    void SystemControllerService::setProcessRunParam(const std::string& processName, const TA_Base_Core::RunParam& param)
    {
        m_systemController->setRunParam(processName.c_str(), param);
    }


    void SystemControllerService::setProcessRunParam(const std::vector<std::string>& processNames, const TA_Base_Core::RunParam& param)
    {
        BOOST_FOREACH(const std::string& process, processNames)
        {
            setProcessRunParam(process, param);
        }
    }


    void SystemControllerService::refresh()
    {
        m_puller->refresh();
    }
}
