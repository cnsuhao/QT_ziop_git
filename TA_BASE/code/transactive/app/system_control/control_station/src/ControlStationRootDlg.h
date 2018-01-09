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
#include "ui_ControlStationRootDlg.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"
#include <QIcon>

namespace TA_Base_App
{
    struct ControlStationAppController;
    struct ControlStationRootDlgModel;

    struct ControlStationRootDlg : TA_Base_Bus::AbstractTransActiveDialog
    {
        Q_OBJECT

    public:

        ControlStationRootDlg(TA_Base_Bus::ITransActiveApp* app, ControlStationRootDlgModel* model);

    private:

        virtual std::string getMyPModelName() { return ""; }
        virtual void setupPModel() {}

    private:

        QIcon m_icon;
        Ui::ControlStationRootDlg ui;
        ControlStationAppController* m_controller;
        ControlStationRootDlgModel* m_model;
    };
}
