#pragma once
#include "IService.h"
#include <string>
#include <vector>

namespace TA_Base_App
{
    struct StyleDlgModel;

    struct StyleDlgModelService : IService<StyleDlgModel>
    {
        std::vector<std::string> getStyleSheets();
        std::vector<std::wstring> getLanguages();
    };
}
