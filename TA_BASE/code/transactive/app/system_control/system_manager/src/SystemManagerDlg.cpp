/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/SystemManagerDlg.cpp $
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
#include "SystemManagerDlg.h"
#include "QtUtilities.h"
#include "SystemManagerDlgModel.h"
#include <QStyleFactory>
#include <boost/foreach.hpp>
#include <boost/preprocessor/cat.hpp>


namespace TA_Base_App
{
    SystemManagerDlg::SystemManagerDlg(TA_Base_Bus::ITransActiveApp* app, SystemManagerDlgModel* model)
        : TA_Base_Bus::AbstractTransActiveDialog(app, "main", NULL),
          m_model(model)
    {
        ui.setupUi(this);
        m_model->attachView2(this);
        initControl();
        loadFromModel();
        setupSignalSlotConnection();
        autoResize();
    }


    SystemManagerDlg::~SystemManagerDlg()
    {
        m_model->attachView2(this);
    }


    void SystemManagerDlg::initControl()
    {
        statusBar()->addWidget(&m_status, 1);
        ui.processes->setColumnCount(m_model->ui.column_count);
        ui.processes->setSortingEnabled(false);
        ui.processes->setColumnWidth(0, 200);
        ui.processes->setColumnWidth(1, 60);
        ui.processes->setColumnWidth(2, 100);
        ui.processes->setColumnWidth(3, 100);
        ui.processes->setColumnWidth(4, 100);
        ui.processes->setColumnWidth(5, 120);
        ui.processes->setColumnWidth(6, 50);
        ui.processes->setColumnWidth(7, 70);
        ui.processes->setColumnWidth(8, 20);
        ui.processes->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui.processes->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui.processes->setContextMenuPolicy(Qt::CustomContextMenu);
        ui.processes->setShowGrid(false);
        ui.processes->setFocusPolicy(Qt::NoFocus);
        ui.processes->verticalHeader()->setVisible(false);
        ui.processes->horizontalHeader()->setHighlightSections(false);
        ui.processes->horizontalHeader()->setDefaultAlignment(Qt::AlignLeft);
        ui.processes->verticalHeader()->setDefaultSectionSize(24);
        ui.processes->verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
        ui.processes->verticalHeader()->setDefaultAlignment(Qt::AlignLeft);
        ui.actionSystemManagerHelp->setEnabled(true);
        ui.actionAboutSystemManager->setEnabled(true);
        ui.actionExit->setEnabled(true);
        m_menu.addAction(ui.actionStartProcess);
        m_menu.addAction(ui.actionStopProcess);
        m_menu.addAction(ui.actionChangeProcessOperationMode);
        m_menu.addAction(ui.actionSetProcessRuntimeParameters);
    }


    void SystemManagerDlg::loadFromModel()
    {
        loadTitleStatusFromModel();
        loadIconsFromModel();
        loadActionsFromModel();
        loadProcessFromModel();
    }


    void SystemManagerDlg::loadProcessFromModel()
    {
        ui.processes->setEnabled(m_model->ui.process_enabled);
        ui.processes->setHorizontalHeaderLabels(m_tr.tr(m_model->ui.header));
    }


    void SystemManagerDlg::loadIconsFromModel()
    {
        m_login_icon.addFile(m_model->ui.login_icon_file.c_str());
        m_logout_icon.addFile(m_model->ui.logout_icon_file.c_str());
        m_start_icon.addFile(m_model->ui.start_icon_file.c_str());
        m_stop_icon.addFile(m_model->ui.stop_icon_file.c_str());
        m_refresh_icon.addFile(m_model->ui.refresh_icon_file.c_str());
        ui.actionLogin->setIcon(m_login_icon);
        ui.actionLogout->setIcon(m_logout_icon);
        ui.actionStartProcess->setIcon(m_start_icon);
        ui.actionStopProcess->setIcon(m_stop_icon);
        ui.actionRefresh->setIcon(m_refresh_icon);
    }


    void SystemManagerDlg::loadActionsFromModel()
    {
        ui.actionLogin->setEnabled(m_model->ui.login_enabled);
        ui.actionLogout->setEnabled(m_model->ui.logout_enabled);
        ui.actionRefresh->setEnabled(m_model->ui.refresh_enabled);
        ui.actionOptions->setEnabled(m_model->ui.option_enabled);
        ui.actionStartProcess->setEnabled(m_model->ui.start_enabled);
        ui.actionStopProcess->setEnabled(m_model->ui.stop_enabled);
        ui.actionChangeProcessOperationMode->setEnabled(m_model->ui.change_mode_enabled);
        ui.actionShutdownSystemController->setEnabled(m_model->ui.shutdown_enabled);
        ui.actionSetProcessRuntimeParameters->setEnabled(m_model->ui.set_param_enabled);
    }


    void SystemManagerDlg::loadTitleStatusFromModel()
    {
        m_status.setText(m_tr.tr(m_model->ui.status));
        setWindowTitle(m_tr.tr(m_model->ui.title));
    }


    void SystemManagerDlg::setupSignalSlotConnection()
    {
        qRegisterMetaType< IndexSet >("IndexSet");
        qRegisterMetaType< ProcessList >("ProcessList");
        qRegisterMetaType< ProcessUpdates >("ProcessUpdates");
        connect(ui.actionLogin, SIGNAL(triggered()), this, SLOT(onLogin()));
        connect(ui.actionLogout, SIGNAL(triggered()), this, SLOT(onLogout()));
        connect(ui.actionStartProcess, SIGNAL(triggered()), this, SLOT(onStartProcess()));
        connect(ui.actionStopProcess, SIGNAL(triggered()), this, SLOT(onStopProcess()));
        connect(ui.actionSetProcessRuntimeParameters, SIGNAL(triggered()), this, SLOT(onSetParameters()));
        connect(ui.actionChangeProcessOperationMode, SIGNAL(triggered()), this, SLOT(onChangeOperationMode()));
        connect(ui.actionOptions, SIGNAL(triggered()), this, SLOT(onSetOptions()));
        connect(ui.actionShutdownSystemController, SIGNAL(triggered()), this, SLOT(onShutdownSystemController()));
        connect(ui.actionExit, SIGNAL(triggered()), this, SLOT(onClose()));
        connect(ui.actionRefresh, SIGNAL(triggered()), this, SLOT(onRefresh()));
        connect(ui.processes, SIGNAL(itemSelectionChanged()), this, SLOT(onItemSelectionChanged()));
        connect(ui.processes, SIGNAL(customContextMenuRequested(const QPoint&)), this, SLOT(onContextMenu(const QPoint&)));
        connect(ui.actionStyle, SIGNAL(triggered()), this, SLOT(onStyle()));
        connect(ui.processes->horizontalHeader(), SIGNAL(onSectionClicked(int)), this, SLOT(onSectionClicked(int)));
        connect(this, SIGNAL(updateWindowTitleSignal(const std::string&)), this, SLOT(updateWindowTitleSlot(const std::string&)));
        connect(this, SIGNAL(updateStatusSignal(const std::string&)), this, SLOT(updateStatusSlot(const std::string&)));
        connect(this, SIGNAL(updateProcessSignal()), this, SLOT(updateProcessSlot()));
        connect(this, SIGNAL(updateProcessSignal(const ProcessList&)), this, SLOT(updateProcessSlot(const ProcessList&)));
        connect(this, SIGNAL(updateProcessSignal(const ProcessUpdates&)), this, SLOT(updateProcessSlot(const ProcessUpdates&)));
        connect(this, SIGNAL(enableLoginSignal(bool)), this, SLOT(enableLoginSlot(bool)));
        connect(this, SIGNAL(enableLogoutSignal(bool)), this, SLOT(enableLogoutSlot(bool)));
        connect(this, SIGNAL(enableStartProcessSignal(bool)), this, SLOT(enableStartProcessSlot(bool)));
        connect(this, SIGNAL(enableStopProcessSignal(bool)), this, SLOT(enableStopProcessSlot(bool)));
        connect(this, SIGNAL(enableRefreshSignal(bool)), this, SLOT(enableRefreshSlot(bool)));
        connect(this, SIGNAL(enableChangeModeSignal(bool)), this, SLOT(enableChangeModeSlot(bool)));
        connect(this, SIGNAL(enableShutdownSignal(bool)), this, SLOT(enableShutdownSlot(bool)));
        connect(this, SIGNAL(enablSetParamsSignal(bool)), this, SLOT(enablSetParamsSlot(bool)));
        connect(this, SIGNAL(enableOptionSignal(bool)), this, SLOT(enableOptionSlot(bool)));
        connect(this, SIGNAL(enableProcessSignal(bool)), this, SLOT(enableProcessSlot(bool)));
        connect(this, SIGNAL(retranslateSignal()), this, SLOT(retranslateSlot()));
        connect(this, SIGNAL(setSelectionSignal(const IndexSet&)), this, SLOT(setSelectionSlot(const IndexSet&)));
        connect(this, SIGNAL(setSortIndicatorSignal(int, bool)), this, SLOT(setSortIndicatorSlot(int, bool)));
        connect(this, SIGNAL(setSortIndicatorShownSignal(bool)), this, SLOT(setSortIndicatorShownSlot(bool)));
    }


    void SystemManagerDlg::onLogin()
    {
        m_model->onLogin();
    }


    void SystemManagerDlg::onLogout()
    {
        m_model->onLogout();
    }


    void SystemManagerDlg::onStartProcess()
    {
        m_model->onStartProcess();
    }


    void SystemManagerDlg::onStopProcess()
    {
        m_model->onStopProcess();
    }


    void SystemManagerDlg::onRefresh()
    {
        m_model->onRefresh();
    }


    void SystemManagerDlg::onChangeOperationMode()
    {
        m_model->onChangeOperationMode();
    }


    void SystemManagerDlg::onSetParameters()
    {
        m_model->onSetParameters();
    }


    void SystemManagerDlg::onSetOptions()
    {
        m_model->onSetOptions();
    }


    void SystemManagerDlg::onShutdownSystemController()
    {
        m_model->onShutdownSystemController();
    }


    void SystemManagerDlg::onStyle()
    {
        m_model->onStyle();
    }


    void SystemManagerDlg::onItemSelectionChanged()
    {
        IndexSet selection = convertWidgetItem2Row(ui.processes->selectedItems());
        m_model->ui.selection = selection;
        m_model->onSelectionChanged(selection);
    }


    void SystemManagerDlg::onContextMenu(const QPoint&)
    {
        m_menu.exec(QCursor::pos());
    }


    void SystemManagerDlg::updateProcessSlot(const ProcessUpdates& updates)
    {
        for (ProcessUpdates::const_iterator it = updates.begin(); it != updates.end(); ++it)
        {
            int row = it->first.first;
            int col = it->first.second;
            ui.processes->item(row, col)->setText(m_tr.tr(it->second));
        }
    }


    void SystemManagerDlg::updateProcessSlot()
    {
        ui.processes->clearContents();
        ui.processes->setRowCount(0);
    }


    void SystemManagerDlg::updateProcessSlot(const ProcessList& processes)
    {
        ui.processes->clearContents();
        ui.processes->setRowCount(processes.size());

        for (size_t i = 0; i < processes.size(); ++i)
        {
            for (size_t j = 0; j < m_model->ui.column_count; ++j)
            {
                ui.processes->setItem(i, j, new QTableWidgetItem(m_tr.tr(processes[i][j])));
            }
        }
    }


    void SystemManagerDlg::updateWindowTitleSlot(const std::string& title)
    {
        setWindowTitle(m_tr.tr(title));
    }


    void SystemManagerDlg::updateStatusSlot(const std::string& status)
    {
        m_status.setText(m_tr.tr(status));
    }


    void SystemManagerDlg::enableLoginSlot(bool enable)
    {
        ui.actionLogin->setEnabled(enable);
    }


    void SystemManagerDlg::enableLogoutSlot(bool enable)
    {
        ui.actionLogout->setEnabled(enable);
    }


    void SystemManagerDlg::enableStartProcessSlot(bool enable)
    {
        ui.actionStartProcess->setEnabled(enable);
    }


    void SystemManagerDlg::enableStopProcessSlot(bool enable)
    {
        ui.actionStopProcess->setEnabled(enable);
    }


    void SystemManagerDlg::enableRefreshSlot(bool enable)
    {
        ui.actionRefresh->setEnabled(enable);
    }


    void SystemManagerDlg::enableChangeModeSlot(bool enable)
    {
        ui.actionChangeProcessOperationMode->setEnabled(enable);
    }


    void SystemManagerDlg::enableShutdownSlot(bool enable)
    {
        ui.actionShutdownSystemController->setEnabled(enable);
    }


    void SystemManagerDlg::enablSetParamsSlot(bool enable)
    {
        ui.actionSetProcessRuntimeParameters->setEnabled(enable);
    }


    void SystemManagerDlg::enableOptionSlot(bool enable)
    {
        ui.actionOptions->setEnabled(enable);
    }


    void SystemManagerDlg::enableProcessSlot(bool enable)
    {
        ui.processes->setEnabled(enable);
    }


    void SystemManagerDlg::setSelectionSlot(const IndexSet& selection)
    {
        ui.processes->clearSelection();
        QAbstractItemView::SelectionMode selectionMode = ui.processes->selectionMode();
        ui.processes->setSelectionMode(QAbstractItemView::MultiSelection);

        BOOST_FOREACH(size_t row, selection)
        {
            ui.processes->selectRow(row);
        }

        ui.processes->setSelectionMode(selectionMode);
    }


    void SystemManagerDlg::setSortIndicatorSlot(int column, bool asscending)
    {
        ui.processes->horizontalHeader()->setSortIndicator(column, asscending ? Qt::AscendingOrder : Qt::DescendingOrder);
    }


    void SystemManagerDlg::setSortIndicatorShownSlot(bool show)
    {
        ui.processes->horizontalHeader()->setSortIndicatorShown(show);
    }


    IndexSet SystemManagerDlg::convertWidgetItem2Row(const QList<QTableWidgetItem*>& items)
    {
        IndexSet rows;

        for (int i = 0; i < items.length(); ++i)
        {
            rows.insert(items[i]->row());
        }

        return rows;
    }


    void SystemManagerDlg::retranslateHeader()
    {
        for (size_t i = 0; i < m_model->ui.header.size(); i++)
        {
            ui.processes->horizontalHeaderItem(i)->setText(m_tr.tr(m_model->ui.header[i]));
        }
    }


    void SystemManagerDlg::retranslateTitleStatus()
    {
        m_status.setText(m_tr.tr(m_model->ui.status));
        setWindowTitle(m_tr.tr(m_model->ui.title));
    }


    void SystemManagerDlg::retranslateProcesses()
    {
        const size_t rowCount = ui.processes->rowCount();
        const size_t columnCount = ui.processes->columnCount();

        for (size_t i = 0; i < rowCount; ++i)
        {
            for (size_t j = 0; j < columnCount; ++j)
            {
                ui.processes->item(i, j)->setText(m_tr.tr(m_model->ui.processes[i][j]));
            }
        }
    }


    void SystemManagerDlg::retranslateSlot()
    {
        ui.retranslateUi(this);
        retranslateHeader();
        retranslateTitleStatus();
        retranslateProcesses();
    }


    void SystemManagerDlg::onSectionClicked(int index)
    {
        m_model->onSectionClicked(index);
    }


    void SystemManagerDlg::onModelProcessList(const ProcessList& process_list)
    {
        emit updateProcessSignal(process_list);
    }


    void SystemManagerDlg::onModelUpdateProcessList(const ProcessUpdates& updates)
    {
        emit updateProcessSignal(updates);
    }


    void SystemManagerDlg::onModelClearProcessList()
    {
        emit updateProcessSignal();
    }


    void SystemManagerDlg::onModelUpdateWindowTitle(const FormattedString& title)
    {
        emit updateWindowTitleSignal(m_tr.tr(title).toStdString());
    }


    void SystemManagerDlg::onModelUpdateStatus(const FormattedString& status)
    {
        emit updateStatusSignal(m_tr.tr(status).toStdString());
    }


    void SystemManagerDlg::onModelEnableLogin(bool enable)
    {
        emit enableLoginSignal(enable);
    }


    void SystemManagerDlg::onModelEnableLogout(bool enable)
    {
        emit enableLogoutSignal(enable);
    }


    void SystemManagerDlg::onModelEnableStartProcess(bool enable)
    {
        emit enableStartProcessSignal(enable);
    }


    void SystemManagerDlg::onModelEnableStopProcess(bool enable)
    {
        emit enableStopProcessSignal(enable);
    }


    void SystemManagerDlg::onModelEnableRefresh(bool enable)
    {
        emit enableRefreshSignal(enable);
    }


    void SystemManagerDlg::onModelEnableChangeMode(bool enable)
    {
        emit enableChangeModeSignal(enable);
    }


    void SystemManagerDlg::onModelEnableShutdown(bool enable)
    {
        emit enableShutdownSignal(enable);
    }


    void SystemManagerDlg::onModelEnablSetParams(bool enable)
    {
        emit enablSetParamsSignal(enable);
    }


    void SystemManagerDlg::onModelEnableOption(bool enable)
    {
        emit enableOptionSignal(enable);
    }


    void SystemManagerDlg::onModelEnableProcess(bool enable)
    {
        emit enableProcessSignal(enable);
    }


    void SystemManagerDlg::onSwitchLanguage()
    {
        emit retranslateSignal();
    }


    void SystemManagerDlg::onModelSetSelection(const IndexSet& selection)
    {
        emit setSelectionSignal(selection);
    }


    void SystemManagerDlg::onModelSetSortIndicator(int column, bool asscending)
    {
        emit setSortIndicatorSignal(column, asscending);
    }


    void SystemManagerDlg::onModelSetSortIndicatorShown(bool show)
    {
        emit setSortIndicatorShownSignal(show);
    }

    void SystemManagerDlg::onClose()
    {
        QMainWindow::close();
    }
}
