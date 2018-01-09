/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/alarm/alarm_manager/src/AlarmsGui.h $
  * @author:  A. Siow
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * main header file for the ALARMSGUI application
  */
#pragma once
#include "bus/generic_gui_view/src/TransActiveWinApp.h"
#include <QIcon>

namespace TA_Base_App
{
    struct SystemManagerDlg;

    struct SystemManagerApp : TA_Base_Bus::TransActiveWinApp
    {
    public:

        SystemManagerApp(int argc, char* argv[]);
        TA_Base_Bus::IApplicationController* createApplicationController();
        virtual TA_Base_Bus::ITransActiveView* getMainView();
        virtual TA_Base_Bus::ITransActiveView* createMainView();

    private:

        QIcon m_icon;
        SystemManagerDlg* m_mainView;
        TA_Base_Bus::IApplicationController* m_controller;
    };
}
