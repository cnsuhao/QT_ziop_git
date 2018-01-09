/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/SystemManagerDlg.h $
  * @author:  Justin Ebedes
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * SystemManagerDlg is the class associated with the application's main window.
  * This class inherits from AdminObserver. The main window registers itself
  * as an observer with the AdminManager.
  *
  */
#pragma once
#include "ui_SystemManagerDlg.h"
#include "Translation.h"
#include "ISystemManagerDlg.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"
#include <QLabel>

namespace TA_Base_App
{
    struct SystemManagerDlgModel;
    struct ChangeOperationModeModel;

    struct SystemManagerDlg : TA_Base_Bus::AbstractTransActiveDialog, ISystemManagerDlg
    {
        Q_OBJECT

    public:

        SystemManagerDlg(TA_Base_Bus::ITransActiveApp* app, SystemManagerDlgModel* model);
        ~SystemManagerDlg();
        virtual void onModelClearProcessList();
        virtual void onModelProcessList(const ProcessList& processes);
        virtual void onModelUpdateProcessList(const ProcessUpdates& updates);
        virtual void onModelUpdateStatus(const FormattedString& status);
        virtual void onModelUpdateWindowTitle(const FormattedString& title);
        virtual void onModelEnableLogin(bool enable);
        virtual void onModelEnableLogout(bool enable);
        virtual void onModelEnableStartProcess(bool enable);
        virtual void onModelEnableStopProcess(bool enable);
        virtual void onModelEnableRefresh(bool enable);
        virtual void onModelEnableChangeMode(bool enable);
        virtual void onModelEnableShutdown(bool enable);
        virtual void onModelEnablSetParams(bool enable);
        virtual void onModelEnableOption(bool enable);
        virtual void onModelEnableProcess(bool enable);
        virtual void onModelSetSelection(const std::set<size_t>& selection);
        virtual void onModelSetSortIndicator(int column, bool asscending);
        virtual void onModelSetSortIndicatorShown(bool show);
        virtual void onSwitchLanguage();

    private slots:

        void onLogin();
        void onLogout();
        void onStartProcess();
        void onStopProcess();
        void onSetParameters();
        void onSetOptions();
        void onShutdownSystemController();
        void onChangeOperationMode();
        void onRefresh();
        void onItemSelectionChanged();
        void onStyle();
        void onContextMenu(const QPoint&);
        void onSectionClicked(int index);
        void updateWindowTitleSlot(const std::string& title);
        void updateStatusSlot(const std::string& status);
        void updateProcessSlot();
        void updateProcessSlot(const ProcessList& process_list);
        void updateProcessSlot(const ProcessUpdates&);
        void enableLoginSlot(bool enable);
        void enableLogoutSlot(bool enable);
        void enableStartProcessSlot(bool enable);
        void enableStopProcessSlot(bool enable);
        void enableRefreshSlot(bool enable);
        void enableChangeModeSlot(bool enable);
        void enableShutdownSlot(bool enable);
        void enablSetParamsSlot(bool enable);
        void enableOptionSlot(bool enable);
        void enableProcessSlot(bool enable);
        void setSelectionSlot(const std::set<size_t>& selection);
        void retranslateSlot();
        void setSortIndicatorSlot(int column, bool asscending);
        void setSortIndicatorShownSlot(bool show);

        void onClose();

    signals:

        void updateWindowTitleSignal(const std::string&);
        void updateStatusSignal(const std::string&);
        void updateProcessSignal();
        void updateProcessSignal(const ProcessList&);
        void updateProcessSignal(const ProcessUpdates&);
        void enableLoginSignal(bool enable);
        void enableLogoutSignal(bool enable);
        void enableStartProcessSignal(bool enable);
        void enableStopProcessSignal(bool enable);
        void enableRefreshSignal(bool enable);
        void enableChangeModeSignal(bool enable);
        void enableShutdownSignal(bool enable);
        void enablSetParamsSignal(bool enable);
        void enableOptionSignal(bool enable);
        void enableProcessSignal(bool enable);
        void setSelectionSignal(const std::set<size_t>& selection);
        void setSortIndicatorSignal(int column, bool asscending);
        void setSortIndicatorShownSignal(bool show);
        void retranslateSignal();

    private:

        void initControl();
        void loadProcessFromModel();
        void loadActionsFromModel();
        void loadIconsFromModel();
        void loadTitleStatusFromModel();
        void retranslateHeader();
        void retranslateTitleStatus();
        void retranslateProcesses();
        QString translate(const std::string& s);
        std::set<size_t> convertWidgetItem2Row(const QList<QTableWidgetItem*>& items);
        virtual void loadFromModel();
        virtual void setupSignalSlotConnection();
        virtual std::string getMyPModelName() { return ""; }
        virtual void setupPModel() {}

    private:

        QLabel m_status;
        QMenu m_menu;
        QIcon m_login_icon;
        QIcon m_logout_icon;
        QIcon m_start_icon;
        QIcon m_stop_icon;
        QIcon m_refresh_icon;
        Translation m_tr;
        Ui::SystemManagerDlg ui;
        SystemManagerDlgModel* m_model;
    };
}
