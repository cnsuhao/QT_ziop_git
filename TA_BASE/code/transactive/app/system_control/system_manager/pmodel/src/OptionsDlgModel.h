#pragma once
#include "AbstractModel.h"
#include <string>
#include <vector>

namespace TA_Base_App
{
    struct IOptionsDlg;
    struct OptionsDlgModelService;

    struct OptionsSynData
    {
        int index;
        bool ok_enabled;
        static std::vector<std::string> refreshRates;
    };

    struct OptionsDlgModel : AbstractModel<IOptionsDlg, OptionsSynData, OptionsDlgModelService>
    {
    public:

        OptionsDlgModel(OptionsDlgModelService* service);
        void onRefreshRatesIndexChanged(int index);
        virtual void calculateStatus(SyncData& data);
        virtual void notifyChangesToView(const SyncData& o, const SyncData& n);
        virtual void onAccept();
        virtual void initSyncData();
    };
}
