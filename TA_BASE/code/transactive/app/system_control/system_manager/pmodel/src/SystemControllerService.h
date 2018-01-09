/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/SystemControllerService.h $
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
#pragma once
#include "ISystemControllerService.h"
#include "core/process_management/IDL/src/ISystemControllerAdminCorbaDef.h"
#include <string>
#include <vector>
#include <map>
#include <set>

namespace TA_Base_Core
{
    class AuditMessageSender;
}

namespace TA_Base_Bus
{
    class ISystemControllerAccessor;
}

namespace TA_Base_App
{
    struct IProcessDataObserver;
    struct ProcessDataPuller;
    struct SystemManagerDlgModel;
    struct ISystemManagerDlgModel;
    typedef std::map<std::string, TA_Base_Bus::ISystemControllerAccessor*> SystemControllerMap;
    typedef std::map<std::string, ProcessDataPuller*> ProcessDataPullerMap;

    struct SystemControllerService : ISystemControllerService
    {
    public:

        void refresh();
        void startPullProcessData();
        void stopPullProcessData();
        void setCurrentSystemController(const std::string& hostname, unsigned long port);
        void registerProcessDataObserver(const std::string& hostname, IProcessDataObserver* observer);
        void unregisterProcessDataObserver(const std::string& hostname, IProcessDataObserver* observer);
        void setRefreshRate(unsigned int newRate);
        void startProcess(const std::string& processName);
        void startProcess(const std::vector<std::string>& processNames);
        void changeProcessOperationMode(const std::string& processName, TA_Base_Core::EOperationMode opMode);
        void changeProcessOperationMode(const std::vector<std::string>& processNames, TA_Base_Core::EOperationMode opMode);
        void stopProcess(const std::string& processName);
        void stopProcess(const std::vector<std::string>& processNames);
        void shutdownSystemController(const std::string& hostname);
        void setProcessRunParam(const std::string& processName, const TA_Base_Core::RunParam& param);
        void setProcessRunParam(const std::vector<std::string>& processNames, const TA_Base_Core::RunParam& param);

    private:

        static ProcessDataPuller* m_puller;
        static TA_Base_Bus::ISystemControllerAccessor* m_systemController;
        ProcessDataPullerMap m_pullers;
        SystemControllerMap m_systemControllers;
        std::map<std::string, std::set<IProcessDataObserver*> > m_observers;
    };
}
