/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/alarm/alarm_manager/src/AlarmsGui.cpp $
  * @author:  A. Siow
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * Defines the class behaviors for the application. Launches the Alarm Manager as
  * a managed application using Generic GUI.
  */
#include "SystemManagerApp.h"
#include "SystemManagerDlg.h"
#include "Factory.h"
#include "SystemManagerDlg.h"
#include "SystemManagerDlgModel.h"
#include "SystemManagerDlgModelService.h"
#include "SystemManagerAppController.h"
#include "SystemManagerDlgModel.h"
#include "ResourceService.h"
#include <QStyleFactory>


namespace TA_Base_App
{
    SystemManagerApp::SystemManagerApp(int argc, char* argv[])
        : TA_Base_Bus::TransActiveWinApp(argc, argv, TA_Base_Bus::MVT_DIALOG, false),
          m_icon(ResourceService::app_icon_file.c_str())
    {
        TA_Base_Core::RunParams::getInstance().set(RPARAM_ENTITYNAME, "SystemManager");
        changeLanguage("zh_CN");
        setStyle(QStyleFactory::create("Fusion"));
        changeStyleSheet("qmc2-sandy-0.10.qss");
        //setStyle(QStyleFactory::create("WindowsVista"));
        //changeStyleSheet("Default.qss");

        setWindowIcon(m_icon);
        m_controller = new TA_Base_App::SystemManagerAppController(this, "SystemManager");
        SystemManagerDlgModelService* service = new SystemManagerDlgModelService;
        SystemManagerDlgModel* model = new SystemManagerDlgModel(m_controller, service);
        SystemManagerDlg* mainView = new SystemManagerDlg(this, model);
        Factory::instance().setMainView(mainView, model, service);
        m_mainView = mainView;
    }


    TA_Base_Bus::IApplicationController* SystemManagerApp::createApplicationController()
    {
        return m_controller;
    }


    TA_Base_Bus::ITransActiveView* SystemManagerApp::createMainView()
    {
        return m_mainView;
    }


    TA_Base_Bus::ITransActiveView* SystemManagerApp::getMainView()
    {
        return m_mainView;
    }
}
