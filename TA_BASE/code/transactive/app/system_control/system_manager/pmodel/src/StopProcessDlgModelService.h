#pragma once
#include "ProcessDataUser.h"
#include "IMessage.h"
#include "IService.h"
#include <string>
#include <vector>

namespace TA_Base_App
{
    struct StopProcessDlgModel;

    struct StopProcessDlgModelService : IService<StopProcessDlgModel>, ProcessDataUser, IMessage
    {
        StopProcessDlgModelService();
        std::vector<std::string> getProcesses();
        std::vector<std::string> getNotStopAbleProcesses();
        void onStopProcess();
        virtual void onMessageProcessDataChanged(const TA_Base_Core::ProcessDataSeq& data);
    };
}
