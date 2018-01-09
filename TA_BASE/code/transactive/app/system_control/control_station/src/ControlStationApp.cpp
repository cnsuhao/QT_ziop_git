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
#include "StdAfx.h"
#include "ControlStationApp.h"
#include "ControlStationRootDialog.h"
#include "ControlStationAppController.h"
#include "ResourceService.h"
#include "ControlStationComModule.h"
#include "ControlStationInstanceChecker.h"
//#include "bus/generic_gui_view/src/transactivemessage.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"
#define TIME_OUT (500) // 500ms

using namespace TA_Base_App;

ControlStationApp::ControlStationApp(int argc, char* argv[])
    : TA_Base_Bus::TransActiveWinApp(argc, argv, TA_Base_Bus::MVT_DIALOG, false),
      m_icon(ResourceService::app_icon_file.c_str())
{
    setWindowIcon(m_icon);
}

int ControlStationApp::startApp()
{
    if (showAboutBox())
    {
        return 0;
    }

    ControlStationInstanceChecker::getInstance().check();

    if (!_Module.initATL())
    {
        return 0;
    }

    return TransActiveWinApp::startApp();
}

bool ControlStationApp::showAboutBox()
{
    std::string cmd = GetCommandLine();

    if (boost::regex_search(cmd, boost::regex("(?ix)--version\\b")))
    {
        TA_Base_Bus::HelpLauncher::getInstance().displayAboutBox();
        QApplication::exec();
        return true;
    }

    return false;
}

TA_Base_Bus::IApplicationController* ControlStationApp::createApplicationController()
{
    return (m_controller = new ControlStationAppController(this, "ControlStation"));
}

TA_Base_Bus::ITransActiveView* ControlStationApp::createMainView()
{
    return (m_mainView = &ControlStationRootDialog::createInstance(this));
}

TA_Base_Bus::ITransActiveView* ControlStationApp::getMainView()
{
    return m_mainView;
}
