#include "SystemControllerDlgModel.h"
#include "ISystemControllerDlg.h"
#include "SystemControllerDlgModelService.h"
#include "ForeachCall.h"


namespace TA_Base_App
{
    std::vector<std::string> SystemControllerSynData::hostnames;
    std::string SystemControllerSynData::port;


    SystemControllerDlgModel::SystemControllerDlgModel(SystemControllerDlgModelService* service)
        : AbstractModel(service)
    {
        setupModel(this);
    }


    void SystemControllerDlgModel::initSyncData()
    {
        if (ui.hostnames.empty())
        {
            ui.hostnames = pm.hostnames = m_service->getHostnames();
        }

        if (ui.port.empty())
        {
            ui.port = pm.port = m_service->getPort();
        }

        ui.hostnames_index = pm.hostnames_index = 0;
        ui.ok_enabled = pm.ok_enabled = false;
    }


    void SystemControllerDlgModel::onPortChanged(const std::string& p)
    {
        onViewChanged();
    }


    void SystemControllerDlgModel::onHostnameIndexChanged(int index)
    {
        onViewChanged();
    }


    void SystemControllerDlgModel::calculateStatus(SyncData& data)
    {
        data.ok_enabled = !(ui.port.empty() || ui.hostnames.empty());
    }


    void SystemControllerDlgModel::notifyChangesToView(const SyncData& o, const SyncData& n)
    {
        NOTIFY(ok_enabled, &ISystemControllerDlg::onModelEnableOkButton);
    }


    void SystemControllerDlgModel::onAccept()
    {
        AbstractModel::onAccept();
        m_service->onLogin(pm.hostnames[ui.hostnames_index], pm.port);
    }
}
