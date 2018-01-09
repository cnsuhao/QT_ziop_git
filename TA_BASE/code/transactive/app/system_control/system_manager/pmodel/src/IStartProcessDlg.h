#pragma once
#include "IView.h"

namespace TA_Base_App
{
    struct IStartProcessDlg : virtual IView
    {
        virtual void onModelSetDisabled(const std::set<size_t>& indexes) = 0;
    };
}
