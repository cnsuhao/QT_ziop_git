#pragma once
#include <set>

namespace TA_Base_App
{
    struct IView
    {
        virtual void display() {}
        virtual void onModelEnableOkButton(bool enable) {}
        virtual void onSwitchLanguage() {}
        IView() { allViews().insert(this); }
        virtual ~IView() { allViews().erase(this); }
        static std::set<IView*>& allViews() { static std::set<IView*> views; return views; }
    };
}
