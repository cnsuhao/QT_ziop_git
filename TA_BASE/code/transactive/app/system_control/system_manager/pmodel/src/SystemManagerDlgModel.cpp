#include "SystemManagerDlgModel.h"
#include "ISystemManagerDlg.h"
#include "SystemManagerDlgModelService.h"
#include "ForeachCall.h"
#include "bus/common_subsystem_facade/src/SecuritySubsystemFacade.h"


namespace TA_Base_App
{
    size_t SystemManagerSynData::column_count;
    std::string SystemManagerSynData::login_icon_file;
    std::string SystemManagerSynData::logout_icon_file;
    std::string SystemManagerSynData::start_icon_file;
    std::string SystemManagerSynData::stop_icon_file;
    std::string SystemManagerSynData::refresh_icon_file;
    std::vector<std::string> SystemManagerSynData::header;


    SystemManagerDlgModel::SystemManagerDlgModel(TA_Base_Bus::IApplicationController* appController, SystemManagerDlgModelService* service)
        : TA_Base_Bus::AbstractPModel(appController, ""),
          AbstractModel(service)
    {
        m_service->setModel(this);
        m_sort.setupSort(m_service->getProcessData());
        initSyncData();
    }


    void SystemManagerDlgModel::initSyncData()
    {
        m_logined = false;
        m_systemControllerInError = false;
        m_shutdowning = false;
        ui.asscending = pm.asscending = true;
        ui.sort_enabled = pm.sort_enabled = false;
        ui.sort_index = pm.sort_index = 8;
        ui.column_count = pm.column_count = 9;
        ui.process_enabled = pm.process_enabled = false;
        ui.login_enabled = pm.login_enabled = false;
        ui.logout_enabled = pm.logout_enabled = false;
        ui.start_enabled = pm.start_enabled = false;
        ui.stop_enabled = pm.stop_enabled = false;
        ui.refresh_enabled = pm.refresh_enabled = false;
        ui.change_mode_enabled = pm.change_mode_enabled = false;
        ui.shutdown_enabled = pm.shutdown_enabled = false;
        ui.set_param_enabled = pm.set_param_enabled = false;
        ui.option_enabled = pm.option_enabled = false;
        ui.login_icon_file = pm.login_icon_file = m_service->getResourceFile(SystemManagerDlgModelService::LoginIcon);
        ui.logout_icon_file = pm.logout_icon_file = m_service->getResourceFile(SystemManagerDlgModelService::LogoutIcon);
        ui.start_icon_file = pm.start_icon_file = m_service->getResourceFile(SystemManagerDlgModelService::StartIcon);
        ui.stop_icon_file = pm.stop_icon_file = m_service->getResourceFile(SystemManagerDlgModelService::StopIcon);
        ui.refresh_icon_file = pm.refresh_icon_file = m_service->getResourceFile(SystemManagerDlgModelService::RefreshIcon);
        ui.status = pm.status = m_service->getStatus(SystemManagerDlgModelService::Initial);
        ui.title = pm.title = m_service->getTitle(SystemManagerDlgModelService::Initial);
        ui.header = pm.header = m_service->getHeader();
    }


    IndexSet& SystemManagerDlgModel::getSelection()
    {
        return pm.selection;
    }


    void SystemManagerDlgModel::onLogin()
    {
        m_service->onLogin();
    }


    void SystemManagerDlgModel::onLogout()
    {
        m_service->onLogout();
    }


    void SystemManagerDlgModel::onStartProcess()
    {
        m_service->onStartProcess();
    }


    void SystemManagerDlgModel::onStopProcess()
    {
        m_service->onStopProcess();
    }


    void SystemManagerDlgModel::onChangeOperationMode()
    {
        m_service->onChangeOperationMode();
    }


    void SystemManagerDlgModel::onSetParameters()
    {
        m_service->onProcessParams();
    }


    void SystemManagerDlgModel::onSetOptions()
    {
        m_service->onSetOptions();
    }


    void SystemManagerDlgModel::onShutdownSystemController()
    {
        m_service->onShutdownSystemController();
    }


    void SystemManagerDlgModel::onRefresh()
    {
        m_service->onRefresh();
    }


    void SystemManagerDlgModel::onStyle()
    {
        m_service->onStyle();
    }


    void SystemManagerDlgModel::onSwitchLanguage()
    {
        m_service->onSwitchLanguage();
    }


    void SystemManagerDlgModel::onSectionClicked(int index)
    {
        SyncData old = ui;
        m_sort.onSectionClicked(index);
        ui.processes = pm.processes = m_sort.m_processes;
        m_updates = m_sort.m_updates;
        ui.selection = m_sort.ui_selection;
        ui.asscending = m_sort.m_asscending;
        ui.sort_index = m_sort.m_index;
        ui.sort_enabled = m_sort.m_enabled;
        notifyChangesToView(old, ui);
    }


    void SystemManagerDlgModel::onServiceUpdateProcessList(const ProcessUpdates& updates)
    {
        SyncData old = ui;
        m_sort.onServiceUpdateProcessList(updates);
        pm.processes = m_sort.m_processes;
        m_updates = m_sort.m_updates;
        calculateStatus(pm);
        ui = pm;
        ui.selection = m_sort.ui_selection;
        notifyChangesToView(old, ui);
    }


    void SystemManagerDlgModel::onServiceProcessList(const ProcessList& process_list)
    {
        SyncData old = ui;
        m_sort.onServiceProcessList(process_list);
        pm.processes = m_sort.m_processes;
        m_updates = m_sort.m_updates;
        calculateStatus(pm);
        ui = pm;
        ui.selection = m_sort.ui_selection;
        notifyChangesToView(old, ui);
    }


    void SystemManagerDlgModel::onServiceClearProcessList()
    {
        pm.processes.clear();
        onModelChanged();
    }


    void SystemManagerDlgModel::onServiceLogin(const std::string& hostname)
    {
        m_logined = true;
        pm.status = m_service->getStatus(SystemManagerDlgModelService::Logined);
        pm.title = m_service->getTitle(SystemManagerDlgModelService::Logined);
        onModelChanged();
    }


    void SystemManagerDlgModel::onServiceLogout()
    {
        m_logined = false;
        m_systemControllerInError = false;
        m_shutdowning = false;
        pm.status = m_service->getStatus(SystemManagerDlgModelService::Logouted);
        pm.title = m_service->getTitle(SystemManagerDlgModelService::Logouted);
        m_sort.reset();
        onModelChanged();
    }


    void SystemManagerDlgModel::onServiceSystemControllerError(bool error)
    {
        m_systemControllerInError = error;
        pm.status = m_service->getStatus(error ? SystemManagerDlgModelService::SystemControllerError : SystemManagerDlgModelService::Initial);
        onModelChanged();
    }


    void SystemManagerDlgModel::onServiceShutdownSystemControllerStarted()
    {
        m_shutdowning = true;
        pm.status = m_service->getStatus(SystemManagerDlgModelService::ShutdownBegin);
        onModelChanged();
    }


    void SystemManagerDlgModel::onServiceShutdownSystemControllerFinished()
    {
        m_shutdowning = false;
        m_logined = false;
        pm.status = m_service->getStatus(SystemManagerDlgModelService::ShutdownFinished);
        onModelChanged();
    }


    void SystemManagerDlgModel::onSelectionChanged(const IndexSet& selection)
    {
        m_sort.onSelectionChanged(selection);
        pm.selection = m_sort.pm_selection;
        m_service->onSelectionChanged(pm.selection);
        onViewChanged();
    }


    void SystemManagerDlgModel::calculateStatus(SystemManagerSynData& data)
    {
        if (m_logined)
        {
            if (m_shutdowning)
            {
                data.login_enabled = false;
                data.logout_enabled = true;
                data.start_enabled = false;
                data.stop_enabled = false;
                data.refresh_enabled = false;
                data.change_mode_enabled = false;
                data.shutdown_enabled = false;
                data.set_param_enabled = false;
                data.option_enabled = false;
                data.process_enabled = false;
            }
            else if (m_systemControllerInError)
            {
                data.login_enabled = false;
                data.logout_enabled = true;
                data.start_enabled = false;
                data.stop_enabled = false;
                data.refresh_enabled = true;
                data.change_mode_enabled = false;
                data.shutdown_enabled = false;
                data.set_param_enabled = false;
                data.option_enabled = false;
                data.process_enabled = false;
            }
            else
            {
                data.login_enabled = false;
                data.logout_enabled = true;
                data.start_enabled = calculateStartEnabled(data);
                data.stop_enabled = calculateStopEnabled(data);
                data.refresh_enabled = true;
                data.change_mode_enabled = calculateChangeModeEnabled(data);
                data.shutdown_enabled = true;
                data.set_param_enabled = !data.selection.empty();
                data.option_enabled = true;
                data.process_enabled = true;
            }
        }
        else
        {
            data.processes.clear();
            data.selection.clear();
            data.login_enabled = true;
            data.logout_enabled = false;
            data.start_enabled = false;
            data.stop_enabled = false;
            data.refresh_enabled = false;
            data.change_mode_enabled = false;
            data.shutdown_enabled = false;
            data.set_param_enabled = false;
            data.option_enabled = false;
            data.process_enabled = false;
        }
    }


    void SystemManagerDlgModel::notifyChangesToView(const SystemManagerSynData& o, const SystemManagerSynData& n)
    {
        NOTIFY(login_enabled, &ISystemManagerDlg::onModelEnableLogin);
        NOTIFY(logout_enabled, &ISystemManagerDlg::onModelEnableLogout);
        NOTIFY(start_enabled, &ISystemManagerDlg::onModelEnableStartProcess);
        NOTIFY(stop_enabled, &ISystemManagerDlg::onModelEnableStopProcess);
        NOTIFY(refresh_enabled, &ISystemManagerDlg::onModelEnableRefresh);
        NOTIFY(change_mode_enabled, &ISystemManagerDlg::onModelEnableChangeMode);
        NOTIFY(shutdown_enabled, &ISystemManagerDlg::onModelEnableShutdown);
        NOTIFY(set_param_enabled, &ISystemManagerDlg::onModelEnablSetParams);
        NOTIFY(option_enabled, &ISystemManagerDlg::onModelEnableOption);
        NOTIFY(process_enabled, &ISystemManagerDlg::onModelEnableProcess);
        NOTIFY(status, &ISystemManagerDlg::onModelUpdateStatus);
        NOTIFY(title, &ISystemManagerDlg::onModelUpdateWindowTitle);
        NOTIFY(selection, &ISystemManagerDlg::onModelSetSelection);
        NOTIFY(sort_enabled, &ISystemManagerDlg::onModelSetSortIndicatorShown);

        if ((false == o.processes.empty()) && n.processes.empty())
        {
            foreachCall(m_views, &ISystemManagerDlg::onModelClearProcessList);
        }

        if (o.processes.empty() && (false == n.processes.empty()))
        {
            foreachCall(m_views, &ISystemManagerDlg::onModelProcessList, n.processes);
        }

        if (false == o.processes.empty() && (false == n.processes.empty()) && (false == m_updates.empty()))
        {
            foreachCall(m_views, &ISystemManagerDlg::onModelUpdateProcessList, m_updates);
            m_updates.clear();
        }

        if (n.sort_enabled && (o.sort_index != n.sort_index || o.asscending != n.asscending))
        {
            foreachCall(m_views, &ISystemManagerDlg::onModelSetSortIndicator, n.sort_index, n.asscending);
        }
    }


    bool SystemManagerDlgModel::calculateStartEnabled(SystemManagerSynData& data)
    {
        return m_service->isAnyStartAble();
    }


    bool SystemManagerDlgModel::calculateStopEnabled(SystemManagerSynData& data)
    {
        return m_service->isAnyStopAble();
    }


    bool SystemManagerDlgModel::calculateChangeModeEnabled(SystemManagerSynData& data)
    {
        return m_service->isAnyCanChangeMode();
    }
}
