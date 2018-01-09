#pragma once
#include "IProcessDataObserver.h"
#include "IMessage.h"
#include "FormattedString.h"
#include "ProcessUtilities.h"
#include "core/process_management/IDL/src/ProcessManagementDataDefinitionsCorbaDef.h"
#include <string>
#include <vector>
#include <map>

namespace TA_Base_App
{
    struct Factory;
    struct ProcessDataPuller;
    struct ISystemManagerDlgModel;
    struct ISystemControllerService;
    typedef std::set<size_t> IndexSet;

    struct SystemManagerDlgModelService : IProcessDataObserver, IMessage
    {
        enum EResource { LoginIcon, LogoutIcon, StartIcon, StopIcon, RefreshIcon };
        enum EStatus { Initial, SystemControllerError, Logined, Logouted, ShutdownBegin, ShutdownFinished };

    public:

        SystemManagerDlgModelService();
        void setModel(ISystemManagerDlgModel* model) { m_model = model; }
        IndexSet& getSelection() { return m_selection; };
        TA_Base_Core::ProcessDataSeq& getProcessData() { return m_data; }
        const std::string& getHostname() { return m_hostname; }
        const std::string& getResourceFile(EResource resource);
        FormattedString& getStatus(EStatus status);
        FormattedString& getTitle(EStatus status);
        const std::vector<std::string>& getHeader();
        bool isAnyStartAble();
        bool isAnyStopAble();
        bool isAnyCanChangeMode();
        void onLogin();
        void onLogout();
        void onStartProcess();
        void onStopProcess();
        void onChangeOperationMode();
        void onShutdownSystemController();
        void onRefresh();
        void onProcessParams();
        void onSetOptions();
        void onStyle();
        void onSwitchLanguage();
        void onSelectionChanged(const IndexSet& selection);
        virtual void onReceivedProcessData(const TA_Base_Core::ProcessDataSeq&);
        virtual void onSystemControllerError();
        virtual void onMessageLogin(const std::string& hostname);
        virtual void onMessageShutdownBegin();

    private:

        void setupStatus();
        void setupResource();

    private:

        bool m_shutdowning;
        bool m_systemControllerInError;
        Factory& m_factory;
        IndexSet m_selection;
        TA_Base_Core::ProcessDataSeq m_data;
        ISystemManagerDlgModel* m_model;
        std::string m_hostname;
        ProcessUtilities m_util;
        ISystemControllerService* m_systemControllerService;
        std::map<EStatus, FormattedString> m_statusMap;
        std::map<EStatus, FormattedString> m_titleMap;
        std::map<EResource, std::string> m_resourceMap;
    };
}
