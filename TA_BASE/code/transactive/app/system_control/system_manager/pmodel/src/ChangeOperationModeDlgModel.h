#pragma once
#include "AbstractModel.h"
#include <string>
#include <vector>
#include <set>

namespace TA_Base_App
{
    struct IChangeOperationModeDlg;
    struct ChangeOperationModeDlgModelService;

    struct ChangeOperationModeSynData
    {
        bool ok_enabled;
        bool control_checked;
        bool monitor_checked;
        bool radio_enabled;
        bool auto_exclusive;
        std::set<size_t> disabled;
        std::vector<std::string> processes;
    };

    struct ChangeOperationModeDlgModel : AbstractModel<IChangeOperationModeDlg, ChangeOperationModeSynData, ChangeOperationModeDlgModelService>
    {
    public:

        ChangeOperationModeDlgModel(ChangeOperationModeDlgModelService* service);
        void onControlRadioToggled(bool checked);
        void onMonitorRadioToggled(bool checked);

    public:

        virtual void calculateInitialStatus();
        virtual void calculateStatus(SyncData& data);
        virtual void notifyChangesToView(const SyncData& o, const SyncData& n);
        virtual void beforeAttachView();
        virtual void onAccept();
        virtual void initSyncData();

    private:

        void calculateOkEnabled(SyncData& data);
        void calculateDisabled(SyncData& data);
        void calculateRadioStatus(SyncData& data);
    };
}
