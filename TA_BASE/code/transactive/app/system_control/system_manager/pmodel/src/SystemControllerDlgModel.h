#pragma once
#include "AbstractModel.h"
#include <string>
#include <vector>

namespace TA_Base_App
{
    struct ISystemControllerDlg;
    struct SystemControllerDlgModelService;

    struct SystemControllerSynData
    {
        int hostnames_index;
        bool ok_enabled;
        static std::string port;
        static std::vector<std::string> hostnames;
    };

    struct SystemControllerDlgModel : AbstractModel<ISystemControllerDlg, SystemControllerSynData, SystemControllerDlgModelService>
    {
        SystemControllerDlgModel(SystemControllerDlgModelService* service);
        void onPortChanged(const std::string& port);
        void onHostnameIndexChanged(int index);
        virtual void calculateStatus(SyncData& data);
        virtual void notifyChangesToView(const SyncData& o, const SyncData& n);
        virtual void onAccept();
        virtual void initSyncData();
    };
}
