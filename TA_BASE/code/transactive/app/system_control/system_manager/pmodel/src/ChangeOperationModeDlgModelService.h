#pragma once
#include <string>
#include <vector>
#include "ProcessDataUser.h"
#include "IMessage.h"
#include "IService.h"
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"

namespace TA_Base_App
{
    struct ChangeOperationModeDlgModel;

    struct ChangeOperationModeDlgModelService : IService<ChangeOperationModeDlgModel>, ProcessDataUser, IMessage
    {
        ChangeOperationModeDlgModelService();
        bool isAllCannotChangeMode();
        bool isAllCannotChangeToControl();
        bool isAllCannotChangeToMonitor();
        bool isCanChangeToBothMode();
        std::vector<std::string> getProcesses();
        std::vector<std::string> getCannotChangeModeProcesses();
        std::vector<std::string> getCannotChangeToControlProcesses();
        std::vector<std::string> getCannotChangeToMonitorProcesses();
        void onChangeOperationMode(bool control);
        virtual void onMessageProcessDataChanged(const TA_Base_Core::ProcessDataSeq& data);
    };
}
