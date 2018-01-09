#pragma once
#include "IView.h"

namespace TA_Base_App
{
    struct IChangeOperationModeDlg : virtual IView
    {
        virtual void onModelSetAutoExclusive(bool auto_exclusive) = 0;
        virtual void onModelSetDisabled(const std::set<size_t>& indexes) = 0;
        virtual void onModelSetRadioEnabled(bool enabled) = 0;
        virtual void omModelSetRadioChecked(bool control_checked, bool monitor_checked) = 0;
    };
}
