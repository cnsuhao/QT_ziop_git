#include "StartProcessDlgModel.h"
#include "IStartProcessDlg.h"
#include "StartProcessDlgModelService.h"
#include "StdUtilities.h"
#include "ForeachCall.h"


namespace TA_Base_App
{
    StartProcessDlgModel::StartProcessDlgModel(StartProcessDlgModelService* service)
        : AbstractModel(service)
    {
        setupModel(this);
    }


    void StartProcessDlgModel::beforeAttachView()
    {
        ui.ok_enabled = pm.ok_enabled = true;
        ui.processes = pm.processes = m_service->getProcesses();
    }


    void StartProcessDlgModel::onAccept()
    {
        AbstractModel::onAccept();
        m_service->onStartProcess();
    }


    void StartProcessDlgModel::calculateStatus(SyncData& data)
    {
        data.disabled = StdUtilities::getIndexesOf(ui.processes, m_service->getNotStartAbleProcesses());
        data.ok_enabled = data.disabled.size() != data.processes.size();
    }


    void StartProcessDlgModel::notifyChangesToView(const SyncData& o, const SyncData& n)
    {
        NOTIFY(ok_enabled, &IStartProcessDlg::onModelEnableOkButton);
        NOTIFY(disabled, &IStartProcessDlg::onModelSetDisabled);
    }
}
