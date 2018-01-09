#pragma once
#include "IView.h"

namespace TA_Base_App
{
    struct ITransActiveLoginDlg : virtual IView
    {
        virtual void onModelSetProfileEnabled(bool enabled) = 0;
        virtual void onModelSetProfiles(const std::vector<std::string>& profiles) = 0;
        virtual void onModelSetProfileIndex(int index) = 0;
        virtual void onModelSetLocationEnabled(bool enabled) = 0;
        virtual void onModelSetLocations(const std::vector<std::string>& locations) = 0;
        virtual void onModelSetLocationIndex(int index) = 0;
        virtual void onModelSetPasswordEnabled(bool enabled) = 0;
    };
}
