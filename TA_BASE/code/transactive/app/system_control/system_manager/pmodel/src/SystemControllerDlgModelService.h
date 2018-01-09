#pragma once
#include "IService.h"
#include <string>
#include <vector>

namespace TA_Base_App
{
    struct SystemControllerDlgModel;

    struct SystemControllerDlgModelService : IService<SystemControllerDlgModel>
    {
        std::vector<std::string>& getHostnames();
        std::string& getPort();
        void onLogin(const std::string& hostname, const std::string& port);
    };
}
