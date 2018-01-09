#pragma once

namespace TA_Base_App
{
    struct ISystemControllerDlg;
    struct SystemControllerDlgModel;
    struct IChangeOperationModeDlg;
    struct ChangeOperationModeDlgModel;
    struct IProcessParamsDlg;
    struct ProcessParamsDlgModel;
    struct IOptionsDlg;
    struct OptionsDlgModel;
    struct IOptionsDlg;
    struct OptionsDlgModel;
    struct IShutdownSystemControllerDlg;
    struct ShutdownSystemControllerDlgModel;
    struct IStartProcessDlg;
    struct StartProcessDlgModel;
    struct IStopProcessDlg;
    struct StopProcessDlgModel;
    struct ITransActiveLoginDlg;
    struct TransActiveLoginDlgModel;

    struct IViewFactory
    {
        virtual ~IViewFactory() {}
        virtual ISystemControllerDlg* createSystemControllerDlg(SystemControllerDlgModel& model) = 0;
        virtual IChangeOperationModeDlg* createChangeOperationModeDlg(ChangeOperationModeDlgModel& model) = 0;
        virtual IProcessParamsDlg* createProcessParamsDlg(ProcessParamsDlgModel& model) = 0;
        virtual IOptionsDlg* createOptionsDlg(OptionsDlgModel& model) = 0;
        virtual IShutdownSystemControllerDlg* createShutdownSystemControllerDlg(ShutdownSystemControllerDlgModel& model) = 0;
        virtual IStartProcessDlg* createStartProcessDlg(StartProcessDlgModel& model) = 0;
        virtual IStopProcessDlg* createStopProcessDlg(StopProcessDlgModel& model) = 0;
        virtual ITransActiveLoginDlg* createTransActiveLoginDlg(TransActiveLoginDlgModel& model) = 0;
    };
}
