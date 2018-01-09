#include "Factory.h"
#include "SystemManagerDlg.h"
#include "SystemManagerDlgModel.h"
#include "SystemManagerDlgModelService.h"
#include "ChangeOperationModeDlg.h"
#include "ChangeOperationModeDlgModel.h"
#include "ChangeOperationModeDlgModelService.h"
#include "TransActiveLoginDlg.h"
#include "TransActiveLoginDlgModel.h"
#include "TransActiveLoginDlgModelService.h"
#include "ShutdownSystemControllerDlg.h"
#include "ShutdownSystemControllerDlgModel.h"
#include "ShutdownSystemControllerDlgModelService.h"
#include "SystemControllerDlg.h"
#include "SystemControllerDlgModel.h"
#include "SystemControllerDlgModelService.h"
#include "StartProcessDlg.h"
#include "StartProcessDlgModel.h"
#include "StartProcessDlgModelService.h"
#include "StopProcessDlg.h"
#include "StopProcessDlgModel.h"
#include "StopProcessDlgModelService.h"
#include "ProcessParamsDlg.h"
#include "ProcessParamsDlgModel.h"
#include "ProcessParamsDlgModelService.h"
#include "OptionsDlg.h"
#include "OptionsDlgModel.h"
#include "OptionsDlgModelService.h"
#include "StyleDlg.h"
#include "StyleDlgModel.h"
#include "StyleDlgModelService.h"
#include "DatabaseService.h"
#include "SystemControllerService.h"


namespace TA_Base_App
{
    Factory& Factory::instance()
    {
        static Factory singleton;
        return singleton;
    }


    Factory::Factory()
    {
        m_systemManagerDlg = NULL;
        m_systemManagerDlgModel = NULL;
        m_systemManagerDlgModelService = NULL;
        m_changeOperationModeDlgModel = NULL;
        m_changeOperationModeDlgModelService = NULL;
        m_transActiveLoginDlgModel = NULL;
        m_transActiveLoginDlgModelService = NULL;
        m_shutdownSystemControllerDlgModel = NULL;
        m_shutdownSystemControllerDlgModelService = NULL;
        m_systemControllerDlgModel = NULL;
        m_systemControllerDlgModelService = NULL;
        m_startProcessDlgModel = NULL;
        m_startProcessDlgModelService = NULL;
        m_stopProcessDlgModel = NULL;
        m_stopProcessDlgModelService = NULL;
        m_processParamsDlgModel = NULL;
        m_processParamsDlgModelService = NULL;
        m_optionsDlgModel = NULL;
        m_optionsDlgModelService = NULL;
        m_styleDlgModel = NULL;
        m_styleDlgModelService = NULL;
    }


    void Factory::setMainView(SystemManagerDlg* dlg, SystemManagerDlgModel* model, SystemManagerDlgModelService* service)
    {
        m_systemManagerDlg = dlg;
        m_systemManagerDlgModel = model;
        m_systemManagerDlgModelService = service;
    }


    ISystemManagerDlg* Factory::getISystemManagerDlg()
    {
        return getSystemManagerDlg();
    }


    SystemManagerDlg* Factory::getSystemManagerDlg()
    {
        return m_systemManagerDlg;
    }


    SystemManagerDlgModel* Factory::getSystemManagerDlgModel()
    {
        return m_systemManagerDlgModel;
    }


    SystemManagerDlgModelService* Factory::getSystemManagerDlgModelService()
    {
        return m_systemManagerDlgModelService;
    }


    IChangeOperationModeDlg* Factory::getChangeOperationModeDlg()
    {
        return new ChangeOperationModeDlg(getChangeOperationModeDlgModel());
    }


    ChangeOperationModeDlgModel* Factory::getChangeOperationModeDlgModel()
    {
        if (NULL == m_changeOperationModeDlgModel)
        {
            m_changeOperationModeDlgModel = new ChangeOperationModeDlgModel(getChangeOperationModeDlgModelService());
        }

        return m_changeOperationModeDlgModel;
    }


    ChangeOperationModeDlgModelService* Factory::getChangeOperationModeDlgModelService()
    {
        if (NULL == m_changeOperationModeDlgModelService)
        {
            m_changeOperationModeDlgModelService = new ChangeOperationModeDlgModelService;
        }

        return m_changeOperationModeDlgModelService;
    }


    IChangeOperationModeDlg* Factory::createChangeOperationModeDlg()
    {
        return new ChangeOperationModeDlg(createChangeOperationModeDlgModel());
    }


    ChangeOperationModeDlgModel* Factory::createChangeOperationModeDlgModel()
    {
        return new ChangeOperationModeDlgModel(createChangeOperationModeDlgModelService());;
    }


    ChangeOperationModeDlgModelService* Factory::createChangeOperationModeDlgModelService()
    {
        return new ChangeOperationModeDlgModelService;
    }


    ITransActiveLoginDlg* Factory::getTransActiveLoginDlg()
    {
        return new TransActiveLoginDlg(getTransActiveLoginDlgModel());
    }


    TransActiveLoginDlgModel* Factory::getTransActiveLoginDlgModel()
    {
        if (NULL == m_transActiveLoginDlgModel)
        {
            m_transActiveLoginDlgModel = new TransActiveLoginDlgModel(getTransActiveLoginDlgModelService());
        }

        return m_transActiveLoginDlgModel;
    }


    TransActiveLoginDlgModelService* Factory::getTransActiveLoginDlgModelService()
    {
        if (NULL == m_transActiveLoginDlgModelService)
        {
            m_transActiveLoginDlgModelService = new TransActiveLoginDlgModelService;
        }

        return m_transActiveLoginDlgModelService;
    }


    ITransActiveLoginDlg* Factory::createTransActiveLoginDlg()
    {
        return new TransActiveLoginDlg(createTransActiveLoginDlgModel());
    }


    TransActiveLoginDlgModel* Factory::createTransActiveLoginDlgModel()
    {
        return new TransActiveLoginDlgModel(createTransActiveLoginDlgModelService());
    }


    TransActiveLoginDlgModelService* Factory::createTransActiveLoginDlgModelService()
    {
        return new TransActiveLoginDlgModelService;
    }


    IShutdownSystemControllerDlg* Factory::getShutdownSystemControllerDlg()
    {
        return new ShutdownSystemControllerDlg(getShutdownSystemControllerDlgModel());
    }


    ShutdownSystemControllerDlgModel* Factory::getShutdownSystemControllerDlgModel()
    {
        if (NULL == m_shutdownSystemControllerDlgModel)
        {
            m_shutdownSystemControllerDlgModel = new ShutdownSystemControllerDlgModel(getShutdownSystemControllerDlgModelService());
        }

        return m_shutdownSystemControllerDlgModel;
    }


    ShutdownSystemControllerDlgModelService* Factory::getShutdownSystemControllerDlgModelService()
    {
        if (NULL == m_shutdownSystemControllerDlgModelService)
        {
            m_shutdownSystemControllerDlgModelService = new ShutdownSystemControllerDlgModelService;
        }

        return m_shutdownSystemControllerDlgModelService;
    }


    IShutdownSystemControllerDlg* Factory::createShutdownSystemControllerDlg()
    {
        return new ShutdownSystemControllerDlg(createShutdownSystemControllerDlgModel());
    }


    ShutdownSystemControllerDlgModel* Factory::createShutdownSystemControllerDlgModel()
    {
        return new ShutdownSystemControllerDlgModel(createShutdownSystemControllerDlgModelService());
    }


    ShutdownSystemControllerDlgModelService* Factory::createShutdownSystemControllerDlgModelService()
    {
        return new ShutdownSystemControllerDlgModelService;
    }


    ISystemControllerDlg* Factory::getSystemControllerDlg()
    {
        return new SystemControllerDlg(getSystemControllerDlgModel());
    }


    SystemControllerDlgModel* Factory::getSystemControllerDlgModel()
    {
        if (NULL == m_systemControllerDlgModel)
        {
            m_systemControllerDlgModel = new SystemControllerDlgModel(getSystemControllerDlgModelService());
        }

        return m_systemControllerDlgModel;
    }


    SystemControllerDlgModelService* Factory::getSystemControllerDlgModelService()
    {
        if (NULL == m_systemControllerDlgModelService)
        {
            m_systemControllerDlgModelService = new SystemControllerDlgModelService;
        }

        return m_systemControllerDlgModelService;
    }


    ISystemControllerDlg* Factory::createSystemControllerDlg()
    {
        return new SystemControllerDlg(createSystemControllerDlgModel());
    }


    SystemControllerDlgModel* Factory::createSystemControllerDlgModel()
    {
        return new SystemControllerDlgModel(createSystemControllerDlgModelService());
    }


    SystemControllerDlgModelService* Factory::createSystemControllerDlgModelService()
    {
        return new SystemControllerDlgModelService;
    }


    IStartProcessDlg* Factory::getStartProcessDlg()
    {
        return new StartProcessDlg(getStartProcessDlgModel());
    }


    StartProcessDlgModel* Factory::getStartProcessDlgModel()
    {
        if (NULL == m_startProcessDlgModel)
        {
            m_startProcessDlgModel = new StartProcessDlgModel(getStartProcessDlgModelService());
        }

        return m_startProcessDlgModel;
    }


    StartProcessDlgModelService* Factory::getStartProcessDlgModelService()
    {
        if (NULL == m_startProcessDlgModelService)
        {
            m_startProcessDlgModelService = new StartProcessDlgModelService;
        }

        return m_startProcessDlgModelService;
    }


    IStartProcessDlg* Factory::createStartProcessDlg()
    {
        return new StartProcessDlg(createStartProcessDlgModel());
    }


    StartProcessDlgModel* Factory::createStartProcessDlgModel()
    {
        return new StartProcessDlgModel(createStartProcessDlgModelService());
    }


    StartProcessDlgModelService* Factory::createStartProcessDlgModelService()
    {
        return new StartProcessDlgModelService;
    }


    IStopProcessDlg* Factory::getStopProcessDlg()
    {
        return new StopProcessDlg(getStopProcessDlgModel());
    }


    StopProcessDlgModel* Factory::getStopProcessDlgModel()
    {
        if (NULL == m_stopProcessDlgModel)
        {
            m_stopProcessDlgModel = new StopProcessDlgModel(getStopProcessDlgModelService());
        }

        return m_stopProcessDlgModel;
    }


    StopProcessDlgModelService* Factory::getStopProcessDlgModelService()
    {
        if (NULL == m_stopProcessDlgModelService)
        {
            m_stopProcessDlgModelService = new StopProcessDlgModelService;
        }

        return m_stopProcessDlgModelService;
    }


    IStopProcessDlg* Factory::createStopProcessDlg()
    {
        return new StopProcessDlg(createStopProcessDlgModel());
    }


    StopProcessDlgModel* Factory::createStopProcessDlgModel()
    {
        return new StopProcessDlgModel(createStopProcessDlgModelService());
    }


    StopProcessDlgModelService* Factory::createStopProcessDlgModelService()
    {
        return new StopProcessDlgModelService;
    }


    IProcessParamsDlg* Factory::getProcessParamsDlg()
    {
        return new ProcessParamsDlg(getProcessParamsDlgModel());
    }


    ProcessParamsDlgModel* Factory::getProcessParamsDlgModel()
    {
        if (NULL == m_processParamsDlgModel)
        {
            m_processParamsDlgModel = new ProcessParamsDlgModel(getProcessParamsDlgModelService());
        }

        return m_processParamsDlgModel;
    }


    ProcessParamsDlgModelService* Factory::getProcessParamsDlgModelService()
    {
        if (NULL == m_processParamsDlgModelService)
        {
            m_processParamsDlgModelService = new ProcessParamsDlgModelService;
        }

        return m_processParamsDlgModelService;
    }


    IProcessParamsDlg* Factory::createProcessParamsDlg()
    {
        return new ProcessParamsDlg(createProcessParamsDlgModel());
    }


    ProcessParamsDlgModel* Factory::createProcessParamsDlgModel()
    {
        return new ProcessParamsDlgModel(createProcessParamsDlgModelService());
    }


    ProcessParamsDlgModelService* Factory::createProcessParamsDlgModelService()
    {
        return new ProcessParamsDlgModelService;
    }


    IOptionsDlg* Factory::getOptionsDlg()
    {
        return new OptionsDlg(getOptionsDlgModel());
    }


    OptionsDlgModel* Factory::getOptionsDlgModel()
    {
        if (NULL == m_optionsDlgModel)
        {
            m_optionsDlgModel = new OptionsDlgModel(getOptionsDlgModelService());
        }

        return m_optionsDlgModel;
    }


    OptionsDlgModelService* Factory::getOptionsDlgModelService()
    {
        if (NULL == m_optionsDlgModelService)
        {
            m_optionsDlgModelService = new OptionsDlgModelService;
        }

        return m_optionsDlgModelService;
    }


    IOptionsDlg* Factory::createOptionsDlg()
    {
        return new OptionsDlg(createOptionsDlgModel());
    }


    OptionsDlgModel* Factory::createOptionsDlgModel()
    {
        return new OptionsDlgModel(createOptionsDlgModelService());
    }


    OptionsDlgModelService* Factory::createOptionsDlgModelService()
    {
        return new OptionsDlgModelService;
    }


    IStyleDlg* Factory::getStyleDlg()
    {
        return new StyleDlg(getStyleDlgModel());
    }


    StyleDlgModel* Factory::getStyleDlgModel()
    {
        if (NULL == m_styleDlgModel)
        {
            m_styleDlgModel = new StyleDlgModel(getStyleDlgModelService());
        }

        return m_styleDlgModel;
    }


    StyleDlgModelService* Factory::getStyleDlgModelService()
    {
        if (NULL == m_styleDlgModelService)
        {
            m_styleDlgModelService = new StyleDlgModelService;
        }

        return m_styleDlgModelService;
    }


    IStyleDlg* Factory::createStyleDlg()
    {
        return new StyleDlg(createStyleDlgModel());
    }


    StyleDlgModel* Factory::createStyleDlgModel()
    {
        return new StyleDlgModel(createStyleDlgModelService());
    }


    StyleDlgModelService* Factory::createStyleDlgModelService()
    {
        return new StyleDlgModelService;
    }


    IDatabaseService* Factory::getDatabaseService()
    {
        if (NULL == m_databaseService)
        {
            m_databaseService = new DatabaseService;
        }

        return m_databaseService;
    }


    ISystemControllerService* Factory::getSystemControllerService()
    {
        if (NULL == m_systemControllerService)
        {
            m_systemControllerService = new SystemControllerService;
        }

        return m_systemControllerService;
    }
}
