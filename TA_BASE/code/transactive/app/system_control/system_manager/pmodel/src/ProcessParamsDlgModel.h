#pragma once
#include "AbstractModel.h"
#include <string>
#include <vector>

namespace TA_Base_App
{
    struct IProcessParamsDlg;
    struct ProcessParamsDlgModelService;

    struct ProcessParamsSynData
    {
        int index;
        bool ok_enabled;
        static std::vector<std::string> debug_levels;
    };

    struct ProcessParamsDlgModel : AbstractModel<IProcessParamsDlg, ProcessParamsSynData, ProcessParamsDlgModelService>
    {
    public:

        ProcessParamsDlgModel(ProcessParamsDlgModelService* service);
        void onDebugLevelsIndexChanged(int index);
        virtual void calculateStatusOnViewChanged();
        virtual void calculateStatusOnModelChanged();
        virtual void notifyChangesToView(const SyncData& o, const SyncData& n);
        virtual void beforeAttachView();
        virtual void onAccept();
        virtual void initSyncData();

    private:

        bool m_mixed_debug_levels;
    };
}
