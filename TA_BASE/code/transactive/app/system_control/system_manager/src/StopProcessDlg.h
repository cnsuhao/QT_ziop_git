/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/StopProcessDlg.h $
  * @author:  Dominique Beauregard
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * StopProcessDlg is associated with the dialog box allowing
  * operators to stop a running process.
  *
  */
#pragma once
#include "ui_StopProcessDlg.h"
#include "AbstructQDialogTemplate.h"
#include "app/system_control/system_manager/pmodel/src/IStopProcessDlg.h"

namespace TA_Base_App
{
    struct StopProcessDlgModel;

    struct StopProcessDlg : AbstructQDialogTemplate<IStopProcessDlg, StopProcessDlgModel, Ui::StopProcessDlg>
    {
        Q_OBJECT

    public:

        StopProcessDlg(StopProcessDlgModel* model);
        virtual void loadFromModel();
        virtual void setupSignalSlotConnection();
        virtual void onModelSetDisabled(const std::set<size_t>& indexes);

    private slots:

        void setDisabledSlot(const std::set<size_t>& indexes);

    signals:

        void setDisabledSignal(const std::set<size_t>& indexes);
    };
}
