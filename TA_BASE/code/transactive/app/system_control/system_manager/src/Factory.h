#pragma once

namespace TA_Base_App
{
    struct ISystemManagerDlg;
    struct SystemManagerDlg;
    struct SystemManagerDlgModel;
    struct SystemManagerDlgModelService;
    struct IChangeOperationModeDlg;
    struct ChangeOperationModeDlgModel;
    struct ChangeOperationModeDlgModelService;
    struct ITransActiveLoginDlg;
    struct TransActiveLoginDlgModel;
    struct TransActiveLoginDlgModelService;
    struct IShutdownSystemControllerDlg;
    struct ShutdownSystemControllerDlgModel;
    struct ShutdownSystemControllerDlgModelService;
    struct ISystemControllerDlg;
    struct SystemControllerDlgModel;
    struct SystemControllerDlgModelService;
    struct IStartProcessDlg;
    struct StartProcessDlgModel;
    struct StartProcessDlgModelService;
    struct IStopProcessDlg;
    struct StopProcessDlgModel;
    struct StopProcessDlgModelService;
    struct IProcessParamsDlg;
    struct ProcessParamsDlgModel;
    struct ProcessParamsDlgModelService;
    struct IOptionsDlg;
    struct OptionsDlgModel;
    struct OptionsDlgModelService;
    struct IStyleDlg;
    struct StyleDlgModel;
    struct StyleDlgModelService;
    struct IDatabaseService;
    struct ISystemControllerService;

    struct Factory
    {
        static Factory& instance();

        void setMainView(SystemManagerDlg* dlg, SystemManagerDlgModel* model, SystemManagerDlgModelService* service);
        ISystemManagerDlg* getISystemManagerDlg();
        SystemManagerDlg* getSystemManagerDlg();
        SystemManagerDlgModel* getSystemManagerDlgModel();
        SystemManagerDlgModelService* getSystemManagerDlgModelService();

        IChangeOperationModeDlg* getChangeOperationModeDlg();
        ChangeOperationModeDlgModel* getChangeOperationModeDlgModel();
        ChangeOperationModeDlgModelService* getChangeOperationModeDlgModelService();
        IChangeOperationModeDlg* createChangeOperationModeDlg();
        ChangeOperationModeDlgModel* createChangeOperationModeDlgModel();
        ChangeOperationModeDlgModelService* createChangeOperationModeDlgModelService();

        ITransActiveLoginDlg* getTransActiveLoginDlg();
        TransActiveLoginDlgModel* getTransActiveLoginDlgModel();
        TransActiveLoginDlgModelService* getTransActiveLoginDlgModelService();
        ITransActiveLoginDlg* createTransActiveLoginDlg();
        TransActiveLoginDlgModel* createTransActiveLoginDlgModel();
        TransActiveLoginDlgModelService* createTransActiveLoginDlgModelService();

        IShutdownSystemControllerDlg* getShutdownSystemControllerDlg();
        ShutdownSystemControllerDlgModel* getShutdownSystemControllerDlgModel();
        ShutdownSystemControllerDlgModelService* getShutdownSystemControllerDlgModelService();
        IShutdownSystemControllerDlg* createShutdownSystemControllerDlg();
        ShutdownSystemControllerDlgModel* createShutdownSystemControllerDlgModel();
        ShutdownSystemControllerDlgModelService* createShutdownSystemControllerDlgModelService();

        ISystemControllerDlg* getSystemControllerDlg();
        SystemControllerDlgModel* getSystemControllerDlgModel();
        SystemControllerDlgModelService* getSystemControllerDlgModelService();
        ISystemControllerDlg* createSystemControllerDlg();
        SystemControllerDlgModel* createSystemControllerDlgModel();
        SystemControllerDlgModelService* createSystemControllerDlgModelService();

        IStartProcessDlg* getStartProcessDlg();
        StartProcessDlgModel* getStartProcessDlgModel();
        StartProcessDlgModelService* getStartProcessDlgModelService();
        IStartProcessDlg* createStartProcessDlg();
        StartProcessDlgModel* createStartProcessDlgModel();
        StartProcessDlgModelService* createStartProcessDlgModelService();

        IStopProcessDlg* getStopProcessDlg();
        StopProcessDlgModel* getStopProcessDlgModel();
        StopProcessDlgModelService* getStopProcessDlgModelService();
        IStopProcessDlg* createStopProcessDlg();
        StopProcessDlgModel* createStopProcessDlgModel();
        StopProcessDlgModelService* createStopProcessDlgModelService();

        IProcessParamsDlg* getProcessParamsDlg();
        ProcessParamsDlgModel* getProcessParamsDlgModel();
        ProcessParamsDlgModelService* getProcessParamsDlgModelService();
        IProcessParamsDlg* createProcessParamsDlg();
        ProcessParamsDlgModel* createProcessParamsDlgModel();
        ProcessParamsDlgModelService* createProcessParamsDlgModelService();

        IOptionsDlg* getOptionsDlg();
        OptionsDlgModel* getOptionsDlgModel();
        OptionsDlgModelService* getOptionsDlgModelService();
        IOptionsDlg* createOptionsDlg();
        OptionsDlgModel* createOptionsDlgModel();
        OptionsDlgModelService* createOptionsDlgModelService();

        IStyleDlg* getStyleDlg();
        StyleDlgModel* getStyleDlgModel();
        StyleDlgModelService* getStyleDlgModelService();
        IStyleDlg* createStyleDlg();
        StyleDlgModel* createStyleDlgModel();
        StyleDlgModelService* createStyleDlgModelService();

        IDatabaseService* getDatabaseService();
        ISystemControllerService* getSystemControllerService();

    private:

        Factory();

        SystemManagerDlg* m_systemManagerDlg;

        SystemManagerDlgModel* m_systemManagerDlgModel;
        SystemManagerDlgModelService* m_systemManagerDlgModelService;

        ChangeOperationModeDlgModel* m_changeOperationModeDlgModel;
        ChangeOperationModeDlgModelService* m_changeOperationModeDlgModelService;

        TransActiveLoginDlgModel* m_transActiveLoginDlgModel;
        TransActiveLoginDlgModelService* m_transActiveLoginDlgModelService;

        ShutdownSystemControllerDlgModel* m_shutdownSystemControllerDlgModel;
        ShutdownSystemControllerDlgModelService* m_shutdownSystemControllerDlgModelService;

        SystemControllerDlgModel* m_systemControllerDlgModel;
        SystemControllerDlgModelService* m_systemControllerDlgModelService;

        StartProcessDlgModel* m_startProcessDlgModel;
        StartProcessDlgModelService* m_startProcessDlgModelService;

        StopProcessDlgModel* m_stopProcessDlgModel;
        StopProcessDlgModelService* m_stopProcessDlgModelService;

        ProcessParamsDlgModel* m_processParamsDlgModel;
        ProcessParamsDlgModelService* m_processParamsDlgModelService;

        OptionsDlgModel* m_optionsDlgModel;
        OptionsDlgModelService* m_optionsDlgModelService;

        StyleDlgModel* m_styleDlgModel;
        StyleDlgModelService* m_styleDlgModelService;

        IDatabaseService* m_databaseService;
        ISystemControllerService* m_systemControllerService;
    };
}
