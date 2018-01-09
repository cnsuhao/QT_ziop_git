#include "ViewFactory.h"
#include "SystemControllerDlg.h"
#include "ChangeOperationModeDlg.h"
#include "ProcessParamsDlg.h"
#include "OptionsDlg.h"
#include "ShutdownSystemControllerDlg.h"
#include "StartProcessDlg.h"
#include "StopProcessDlg.h"
#include "TransActiveLoginDlg.h"


namespace TA_Base_App
{
    ViewFactory::ViewFactory()
    {
    }


    ISystemControllerDlg* ViewFactory::createSystemControllerDlg(SystemControllerDlgModel* model)
    {
        return new SystemControllerDlg(model);
    }


    IChangeOperationModeDlg* ViewFactory::createChangeOperationModeDlg(ChangeOperationModeDlgModel* model)
    {
        return new ChangeOperationModeDlg(model);
    }


    IProcessParamsDlg* ViewFactory::createProcessParamsDlg(ProcessParamsDlgModel* model)
    {
        return new ProcessParamsDlg(model);
    }


    IOptionsDlg* ViewFactory::createOptionsDlg(OptionsDlgModel* model)
    {
        return new OptionsDlg(model);
    }


    IShutdownSystemControllerDlg* ViewFactory::createShutdownSystemControllerDlg(ShutdownSystemControllerDlgModel* model)
    {
        return new ShutdownSystemControllerDlg(model);
    }


    IStartProcessDlg* ViewFactory::createStartProcessDlg(StartProcessDlgModel* model)
    {
        return new StartProcessDlg(model);
    }


    IStopProcessDlg* ViewFactory::createStopProcessDlg(StopProcessDlgModel* model)
    {
        return new StopProcessDlg(model);
    }


    ITransActiveLoginDlg* ViewFactory::createTransActiveLoginDlg(TransActiveLoginDlgModel* model)
    {
        return new TransActiveLoginDlg(model);
    }
}
