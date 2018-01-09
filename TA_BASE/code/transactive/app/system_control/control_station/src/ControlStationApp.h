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
#include <QString>

struct ControlStationAppController;
struct ControlStationRootDlg;

struct ControlStationApp : TA_Base_Bus::TransActiveWinApp
{
    Q_OBJECT

public:

    ControlStationApp(int argc, char* argv[]);
    TA_Base_Bus::IApplicationController* createApplicationController();
    virtual TA_Base_Bus::ITransActiveView* getMainView() override;
    virtual TA_Base_Bus::ITransActiveView* createMainView() override;
    virtual int startApp() override;

private:

    bool showAboutBox();

private:

    QIcon m_icon;
    ControlStationAppController* m_controller;
    TA_Base_Bus::ITransActiveView* m_mainView;
};
