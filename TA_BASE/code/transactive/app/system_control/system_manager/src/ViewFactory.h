#pragma once
#include "app/system_control/system_manager/pmodel/src/IViewFactory.h"

namespace TA_Base_App
{
    struct ViewFactory : public IViewFactory
    {
        ViewFactory();
        virtual ISystemControllerDlg* createSystemControllerDlg(SystemControllerDlgModel* model);
        virtual IChangeOperationModeDlg* createChangeOperationModeDlg(ChangeOperationModeDlgModel* model);
        virtual IProcessParamsDlg* createProcessParamsDlg(ProcessParamsDlgModel* model);
        virtual IOptionsDlg* createOptionsDlg(OptionsDlgModel* model);
        virtual IShutdownSystemControllerDlg* createShutdownSystemControllerDlg(ShutdownSystemControllerDlgModel* model);
        virtual IStartProcessDlg* createStartProcessDlg(StartProcessDlgModel* model);
        virtual IStopProcessDlg* createStopProcessDlg(StopProcessDlgModel* model);
        virtual ITransActiveLoginDlg* createTransActiveLoginDlg(TransActiveLoginDlgModel* model);
    };
}
