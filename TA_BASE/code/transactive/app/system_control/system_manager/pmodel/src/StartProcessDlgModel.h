#pragma once
#include "AbstractModel.h"
#include <string>
#include <vector>
#include <set>

namespace TA_Base_App
{
    struct IStartProcessDlg;
    struct StartProcessDlgModelService;

    struct StartProcessSynData
    {
        bool ok_enabled;
        std::set<size_t> disabled;
        std::vector<std::string> processes;
    };

    struct StartProcessDlgModel : AbstractModel<IStartProcessDlg, StartProcessSynData, StartProcessDlgModelService>
    {
    public:

        StartProcessDlgModel(StartProcessDlgModelService* service);
        virtual void onAccept();
        virtual void beforeAttachView();
        virtual void calculateStatus(SyncData& data);
        virtual void notifyChangesToView(const SyncData& o, const SyncData& n);
    };
}
