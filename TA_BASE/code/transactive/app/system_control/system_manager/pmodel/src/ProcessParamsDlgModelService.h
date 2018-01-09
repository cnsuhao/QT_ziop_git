#pragma once
#include "ProcessDataUser.h"
#include "IService.h"
#include "IMessage.h"
#include <string>
#include <vector>

namespace TA_Base_App
{
    struct ProcessParamsDlgModel;

    struct ProcessParamsDlgModelService : IService<ProcessParamsDlgModel>, ProcessDataUser, IMessage
    {
        bool isAllSameDebugLevel();
        size_t getDebugLevelsIndex();
        const std::vector<std::string>& getDebugLevels();
        void onChangeDebugLevel(const std::string& debugLevel);
        virtual void onMessageProcessDataChanged(const TA_Base_Core::ProcessDataSeq& data);
    };
}
