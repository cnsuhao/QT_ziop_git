#pragma once
#include "IService.h"
#include <string>
#include <vector>
#include <set>

namespace TA_Base_App
{
    struct ShutdownSystemControllerDlgModel;

    struct ShutdownSystemControllerDlgModelService : IService<ShutdownSystemControllerDlgModel>
    {
        std::string getHostname();
        void onShutdownSystemController(const std::string& hostname);
    };
}
