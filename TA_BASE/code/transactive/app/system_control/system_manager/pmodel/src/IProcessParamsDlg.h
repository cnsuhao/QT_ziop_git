#pragma once
#include "IView.h"

namespace TA_Base_App
{
    struct IProcessParamsDlg : virtual IView
    {
        virtual void onModelSetIndex(int index) = 0;
    };
}
