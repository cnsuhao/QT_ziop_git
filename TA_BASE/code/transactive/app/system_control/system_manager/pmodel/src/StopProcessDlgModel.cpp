#include "StopProcessDlgModel.h"
#include "IStopProcessDlg.h"
#include "StopProcessDlgModelService.h"
#include "StdUtilities.h"
#include "ForeachCall.h"


namespace TA_Base_App
{
    StopProcessDlgModel::StopProcessDlgModel(StopProcessDlgModelService* service)
        : AbstractModel(service)
    {
        setupModel(this);
    }


    void StopProcessDlgModel::beforeAttachView()
    {
        ui.ok_enabled = pm.ok_enabled = true;
        ui.processes = pm.processes = m_service->getProcesses();
    }


    void StopProcessDlgModel::onAccept()
    {
        AbstractModel::onAccept();
        m_service->onStopProcess();
    }


    void StopProcessDlgModel::calculateStatus(SyncData& data)
    {
        data.disabled = StdUtilities::getIndexesOf(ui.processes, m_service->getNotStopAbleProcesses());
        data.ok_enabled = data.disabled.size() != data.processes.size();
    }


    void StopProcessDlgModel::notifyChangesToView(const SyncData& o, const SyncData& n)
    {
        NOTIFY(ok_enabled, &IStopProcessDlg::onModelEnableOkButton);
        NOTIFY(disabled, &IStopProcessDlg::onModelSetDisabled);
    }
}
