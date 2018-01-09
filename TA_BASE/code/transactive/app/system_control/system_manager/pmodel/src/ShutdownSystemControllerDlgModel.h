#pragma once
#include "AbstractModel.h"
#include <string>

namespace TA_Base_App
{
    struct IShutdownSystemControllerDlg;
    struct ShutdownSystemControllerDlgModelService;

    struct ShutdownSystemControllerSynData
    {
        std::string hostname;
    };

    struct ShutdownSystemControllerDlgModel : AbstractModel<IShutdownSystemControllerDlg, ShutdownSystemControllerSynData, ShutdownSystemControllerDlgModelService>
    {
    public:

        ShutdownSystemControllerDlgModel(ShutdownSystemControllerDlgModelService* service);
        virtual void beforeAttachView();
        virtual void onAccept();
    };
}
