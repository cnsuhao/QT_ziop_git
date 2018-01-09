#pragma once
#include "IService.h"
#include <string>
#include <vector>

namespace TA_Base_App
{
    struct OptionsDlgModel;

    struct OptionsDlgModelService : IService<OptionsDlgModel>
    {
        std::vector<std::string> getRefereshRates();
        void onSetRefreshRate(size_t rate);
    };
}
