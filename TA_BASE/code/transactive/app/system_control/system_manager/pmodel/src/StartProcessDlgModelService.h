#pragma once
#include "ProcessDataUser.h"
#include "IMessage.h"
#include "IService.h"
#include <string>
#include <vector>

namespace TA_Base_App
{
    struct StartProcessDlgModel;

    struct StartProcessDlgModelService : IService<StartProcessDlgModel>, ProcessDataUser, IMessage
    {
        StartProcessDlgModelService();
        std::vector<std::string> getProcesses();
        std::vector<std::string> getNotStartAbleProcesses();
        void onStartProcess();
        virtual void onMessageProcessDataChanged(const TA_Base_Core::ProcessDataSeq& data);
    };
}
