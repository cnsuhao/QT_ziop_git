#pragma once
#include "AbstractModel.h"
#include "ISystemManagerDlgModel.h"
#include "FormattedString.h"
#include "ProcessSort.h"
#include "CommonDefs.h"
#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

namespace TA_Base_App
{
    struct ISystemManagerDlg;
    struct SystemManagerDlgModelService;

    struct SystemManagerSynData
    {
        FormattedString title;
        FormattedString status;
        bool login_enabled;
        bool logout_enabled;
        bool start_enabled;
        bool stop_enabled;
        bool refresh_enabled;
        bool change_mode_enabled;
        bool shutdown_enabled;
        bool set_param_enabled;
        bool option_enabled;
        bool process_enabled;
        int sort_index;
        bool asscending;
        bool sort_enabled;
        ProcessList processes;
        IndexSet selection;
        static size_t column_count;
        static std::string login_icon_file;
        static std::string logout_icon_file;
        static std::string start_icon_file;
        static std::string stop_icon_file;
        static std::string refresh_icon_file;
        static std::vector<std::string> header;
    };

    struct SystemManagerDlgModel
        : ISystemManagerDlgModel,
          TA_Base_Bus::AbstractPModel,
          AbstractModel<ISystemManagerDlg, SystemManagerSynData, SystemManagerDlgModelService>
    {
    public:

        SystemManagerDlgModel(TA_Base_Bus::IApplicationController* appController, SystemManagerDlgModelService* service);
        virtual IndexSet& getSelection();
        void onLogin();
        void onLogout();
        void onStartProcess();
        void onStopProcess();
        void onChangeOperationMode();
        void onSetParameters();
        void onSetOptions();
        void onShutdownSystemController();
        void onRefresh();
        void onSelectionChanged(const IndexSet& selection);
        void onStyle();
        void onSwitchLanguage();
        void onSectionClicked(int index);
        virtual void onServiceProcessList(const ProcessList& processes);
        virtual void onServiceUpdateProcessList(const ProcessUpdates& updates);
        virtual void onServiceClearProcessList();
        virtual void onServiceLogin(const std::string& hostname);
        virtual void onServiceLogout();
        virtual void onServiceSystemControllerError(bool error);
        virtual void onServiceShutdownSystemControllerStarted();
        virtual void onServiceShutdownSystemControllerFinished();

    private:

        virtual void initSyncData();
        virtual void calculateStatus(SystemManagerSynData& data);
        virtual void notifyChangesToView(const SystemManagerSynData& o, const SystemManagerSynData& n);
        bool calculateStopEnabled(SystemManagerSynData& data);
        bool calculateStartEnabled(SystemManagerSynData& data);
        bool calculateChangeModeEnabled(SystemManagerSynData& data);

    private:

        bool m_logined;
        bool m_shutdowning;
        bool m_systemControllerInError;
        ProcessSort m_sort;
        ProcessUpdates m_updates;
    };
}
