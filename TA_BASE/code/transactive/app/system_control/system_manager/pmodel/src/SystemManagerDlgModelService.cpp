#include "SystemManagerDlgModelService.h"
#include "ISystemManagerDlgModel.h"
#include "ISystemManagerDlg.h"
#include "Factory.h"
#include "ProcessUtilities.h"
#include "SystemControllerService.h"
#include "ISystemControllerDlg.h"
#include "IChangeOperationModeDlg.h"
#include "ITransActiveLoginDlg.h"
#include "IShutdownSystemControllerDlg.h"
#include "ISystemControllerDlg.h"
#include "IStartProcessDlg.h"
#include "IStopProcessDlg.h"
#include "IProcessParamsDlg.h"
#include "IOptionsDlg.h"
#include "IStyleDlg.h"
#include "SendMessage.h"
#include "ResourceService.h"


namespace TA_Base_App
{
    SystemManagerDlgModelService::SystemManagerDlgModelService()
        : m_factory(Factory::instance()),
          m_model(NULL),
          m_systemControllerInError(false),
          m_shutdowning(false),
          m_systemControllerService(Factory::instance().getSystemControllerService())
    {
        setupStatus();
        setupResource();
        m_util.set(m_data);
    }


    void SystemManagerDlgModelService::setupStatus()
    {
        m_statusMap[Initial] = "Standby";
        m_statusMap[SystemControllerError] = "Cannot connect to system controller";
        m_statusMap[Logined] = FormattedString("User logined to %1%", m_hostname);
        m_statusMap[Logouted] = "Standby";
        m_statusMap[ShutdownBegin] = "Shutdown system controller...";
        m_statusMap[ShutdownFinished] = "System controller is shutdown";

        m_titleMap[Initial] = "System Manager";
        m_titleMap[Logined] = FormattedString("System Manager - %1%", m_hostname);
        m_titleMap[Logouted] = "System Manager";
    }


    void SystemManagerDlgModelService::setupResource()
    {
        m_resourceMap[LoginIcon] = ResourceService::login_icon_file;
        m_resourceMap[LogoutIcon] = ResourceService::logout_icon_file;
        m_resourceMap[StartIcon] = ResourceService::start_icon_file;
        m_resourceMap[StopIcon] = ResourceService::stop_icon_file;
        m_resourceMap[RefreshIcon] = ResourceService::refresh_icon_file;
    }


    FormattedString& SystemManagerDlgModelService::getStatus(EStatus status)
    {
        return m_statusMap[status];
    }


    FormattedString& SystemManagerDlgModelService::getTitle(EStatus status)
    {
        return m_titleMap[status];
    }


    const std::string& SystemManagerDlgModelService::getResourceFile(EResource resource)
    {
        return m_resourceMap[resource];
    }


    const std::vector<std::string>& SystemManagerDlgModelService::getHeader()
    {
        const char* str[] = { "Name", "Location", "Mode", "Status", "Request", "Start", "Restarts", "Log", "" };
        static std::vector<std::string> header(str, str + sizeof(str) / sizeof(char*));
        return header;
    }


    bool SystemManagerDlgModelService::isAnyStartAble()
    {
        return m_util.isAnyStartAble();
    }


    bool SystemManagerDlgModelService::isAnyStopAble()
    {
        return m_util.isAnyStopAble();
    }


    bool SystemManagerDlgModelService::isAnyCanChangeMode()
    {
        return m_util.isAnyCanChangeMode();
    }


    void SystemManagerDlgModelService::onLogin()
    {
        //m_factory.createTransActiveLoginDlg()->display();
        m_factory.createSystemControllerDlg()->display();
    }


    void SystemManagerDlgModelService::onLogout()
    {
        m_systemControllerService->unregisterProcessDataObserver(m_hostname, this);
        m_systemControllerService->stopPullProcessData();
        m_data = TA_Base_Core::ProcessDataSeq();
        m_model->onServiceLogout();
        m_shutdowning = false;
        m_systemControllerInError = false;
    }


    void SystemManagerDlgModelService::onStartProcess()
    {
        m_factory.createStartProcessDlg()->display();
    }


    void SystemManagerDlgModelService::onStopProcess()
    {
        m_factory.createStopProcessDlg()->display();
    }


    void SystemManagerDlgModelService::onChangeOperationMode()
    {
        m_factory.createChangeOperationModeDlg()->display();
    }


    void SystemManagerDlgModelService::onShutdownSystemController()
    {
        m_factory.createShutdownSystemControllerDlg()->display();
    }


    void SystemManagerDlgModelService::onRefresh()
    {
        m_systemControllerService->refresh();
    }


    void SystemManagerDlgModelService::onSetOptions()
    {
        m_factory.createOptionsDlg()->display();
    }


    void SystemManagerDlgModelService::onProcessParams()
    {
        m_factory.createProcessParamsDlg()->display();
    }


    void SystemManagerDlgModelService::onStyle()
    {
        m_factory.createStyleDlg()->display();
    }


    void SystemManagerDlgModelService::onSwitchLanguage()
    {
        m_data = TA_Base_Core::ProcessDataSeq();
        onRefresh();
    }


    void SystemManagerDlgModelService::onSelectionChanged(const IndexSet& selection)
    {
        m_selection = selection;
        m_util.set(selection);
    }


    void SystemManagerDlgModelService::onReceivedProcessData(const TA_Base_Core::ProcessDataSeq& data)
    {
        if (m_systemControllerInError)
        {
            m_systemControllerInError = false;
            m_model->onServiceSystemControllerError(false);
        }

        if (0 == m_data.length())
        {
            m_data = data;
            sendMessage(&IMessage::onMessageBeforeUseProcessData, m_hostname, data);
            m_model->onServiceProcessList(m_util.convertToVector());
            sendMessage(&IMessage::onMessageProcessDataChanged, m_data);
        }
        else
        {
            ProcessUpdates updates = m_util.getUpdates(m_data, data);

            if (!updates.empty())
            {
                m_data = data;
                m_model->onServiceUpdateProcessList(updates);
                sendMessage(&IMessage::onMessageProcessDataChanged, m_data);
            }
        }
    }


    void SystemManagerDlgModelService::onSystemControllerError()
    {
        if (m_shutdowning)
        {
            m_shutdowning = false;
            m_data = TA_Base_Core::ProcessDataSeq();
            m_systemControllerService->unregisterProcessDataObserver(m_hostname, this);
            m_model->onServiceShutdownSystemControllerFinished();
        }
        else
        {
            if (!m_systemControllerInError)
            {
                m_systemControllerInError = true;
                m_model->onServiceSystemControllerError(true);
            }
        }
    }


    void SystemManagerDlgModelService::onMessageLogin(const std::string& hostname)
    {
        m_hostname = hostname;
        m_statusMap[Logined].setParam(0, hostname);
        m_titleMap[Logined].setParam(0, hostname);
        m_systemControllerService->registerProcessDataObserver(hostname, this);
        m_systemControllerService->startPullProcessData();
        m_model->onServiceLogin(hostname);
    }


    void SystemManagerDlgModelService::onMessageShutdownBegin()
    {
        m_shutdowning = true;
        m_model->onServiceShutdownSystemControllerStarted();
    }
}
