#include "ChangeOperationModeDlgModel.h"
#include "IChangeOperationModeDlg.h"
#include "ChangeOperationModeDlgModelService.h"
#include "StdUtilities.h"
#include "ForeachCall.h"


namespace TA_Base_App
{
    ChangeOperationModeDlgModel::ChangeOperationModeDlgModel(ChangeOperationModeDlgModelService* service)
        : AbstractModel(service)
    {
        setupModel(this);
    }


    void ChangeOperationModeDlgModel::initSyncData()
    {
        ui.control_checked = pm.control_checked = false;
        ui.monitor_checked = pm.monitor_checked = false;
        ui.radio_enabled = pm.radio_enabled = false;
        ui.auto_exclusive = pm.auto_exclusive = false;
        ui.ok_enabled = pm.ok_enabled = false;
    }


    void ChangeOperationModeDlgModel::beforeAttachView()
    {
        ui.processes = pm.processes = m_service->getProcesses();
        ui.disabled = pm.disabled = std::set<size_t>();
    }


    void ChangeOperationModeDlgModel::onControlRadioToggled(bool checked)
    {
        onViewChanged();
    }


    void ChangeOperationModeDlgModel::onMonitorRadioToggled(bool checked)
    {
        onViewChanged();
    }


    void ChangeOperationModeDlgModel::calculateInitialStatus()
    {
        pm.auto_exclusive = true;
        calculateRadioStatus(pm);

        if (m_service->isCanChangeToBothMode())
        {
            pm.control_checked = pm.monitor_checked = false;
        }

        calculateOkEnabled(pm);
    }


    void ChangeOperationModeDlgModel::calculateStatus(SyncData& data)
    {
        calculateRadioStatus(data);
        calculateDisabled(data);
        calculateRadioStatus(data);
        calculateOkEnabled(data);
    }


    void ChangeOperationModeDlgModel::calculateOkEnabled(SyncData& data)
    {
        data.ok_enabled = data.control_checked || data.monitor_checked;
    }


    void ChangeOperationModeDlgModel::calculateRadioStatus(SyncData& data)
    {
        data.radio_enabled = false;

        if (m_service->isAllCannotChangeMode())
        {
            data.control_checked = data.monitor_checked = false;
        }
        else if (m_service->isAllCannotChangeToControl())
        {
            data.control_checked = false;
            data.monitor_checked = true;
        }
        else if (m_service->isAllCannotChangeToMonitor())
        {
            data.control_checked = true;
            data.monitor_checked = false;
        }
        else
        {
            data.radio_enabled = true;
        }

        if (data.disabled.size() == data.processes.size())
        {
            data.control_checked = data.monitor_checked = false;
            data.auto_exclusive = false;
        }
        else
        {
            data.auto_exclusive = true;
        }
    }


    void ChangeOperationModeDlgModel::calculateDisabled(SyncData& data)
    {
        std::vector<std::string> disabledProcess;

        if (!data.control_checked && !data.monitor_checked)
        {
            disabledProcess = m_service->getCannotChangeModeProcesses();
        }
        else if (data.control_checked && !data.monitor_checked)
        {
            disabledProcess = m_service->getCannotChangeToControlProcesses();
        }
        else if (data.monitor_checked && !data.control_checked)
        {
            disabledProcess = m_service->getCannotChangeToMonitorProcesses();
        }

        data.disabled = StdUtilities::getIndexesOf(data.processes, disabledProcess);
    }


    void ChangeOperationModeDlgModel::notifyChangesToView(const SyncData& o, const SyncData& n)
    {
        NOTIFY(ok_enabled, &IChangeOperationModeDlg::onModelEnableOkButton);
        NOTIFY(disabled, &IChangeOperationModeDlg::onModelSetDisabled);
        NOTIFY(auto_exclusive, &IChangeOperationModeDlg::onModelSetAutoExclusive);
        NOTIFY2(control_checked, monitor_checked, &IChangeOperationModeDlg::omModelSetRadioChecked);
        NOTIFY(radio_enabled, &IChangeOperationModeDlg::onModelSetRadioEnabled);
    }


    void ChangeOperationModeDlgModel::onAccept()
    {
        AbstractModel::onAccept();
        m_service->onChangeOperationMode(pm.control_checked);
    }
}
