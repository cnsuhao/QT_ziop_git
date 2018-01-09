#pragma once
#include "AbstractModel.h"
#include <string>
#include <vector>
#include <set>

namespace TA_Base_App
{
    struct IStopProcessDlg;
    struct StopProcessDlgModelService;

    struct StopProcessSynData
    {
        bool ok_enabled;
        std::set<size_t> disabled;
        std::vector<std::string> processes;
    };

    struct StopProcessDlgModel : AbstractModel<IStopProcessDlg, StopProcessSynData, StopProcessDlgModelService>
    {
    public:

        StopProcessDlgModel(StopProcessDlgModelService* service);
        virtual void onAccept();
        virtual void beforeAttachView();
        virtual void calculateStatus(SyncData& data);
        virtual void notifyChangesToView(const SyncData& o, const SyncData& n);
    };
}
