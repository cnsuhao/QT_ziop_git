#pragma once
#include "AbstractModel.h"
#include "bus/generic_gui_pmod/src/transactiveviewmodel.h"

namespace TA_Base_App
{
    struct ITransActiveLoginDlg;
    struct TransActiveLoginDlgModelService;

    struct TransActiveLoginSynData
    {
        int name_index;
        static std::vector<std::string> names;

        bool profile_enabled;
        int profile_index;
        std::vector<std::string> profiles;

        bool location_enabled;
        int location_index;
        std::vector<std::string> locations;

        bool password_enabled;
        std::string password;

        bool ok_enabled;
    };

    struct TransActiveLoginDlgModel : AbstractModel<ITransActiveLoginDlg, TransActiveLoginSynData, TransActiveLoginDlgModelService>
    {
    public:

        TransActiveLoginDlgModel(TransActiveLoginDlgModelService* service);
        void onNameIndexChanged(int index);
        void onProfileIndexChanged(int index);
        void onLocationIndexChanged(int index);
        void onPasswordChanged(const std::string& password);
        virtual void notifyChangesToView(const SyncData& o, const SyncData& n);
        virtual void onAccept();
        virtual void initSyncData();

    private:

        bool calculateLocationEnabled(SyncData& data);
        bool calculateOkEnabled(SyncData& data);

    private:

        static std::string m_currentLocationDescription;
    };
}
