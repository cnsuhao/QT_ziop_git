#pragma once

namespace TA_Base_App
{
    struct IView;

    struct IModel
    {
        virtual ~IModel() {}
        virtual void onAccept() {}
        virtual void onReject() {}
        virtual void onServiceChanged() {}
    };
}
