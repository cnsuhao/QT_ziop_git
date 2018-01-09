/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/StartProcessDlg.h $
  * @author:  Dominique Beauregard
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * StartProcessDlg is associated with the dialog box allowing
  * operators to launch a process.
  *
  */
#pragma once
#include "ui_StartProcessDlg.h"
#include "AbstructQDialogTemplate.h"
#include "IStartProcessDlg.h"

namespace TA_Base_App
{
    struct StartProcessDlgModel;

    struct StartProcessDlg : AbstructQDialogTemplate<IStartProcessDlg, StartProcessDlgModel, Ui::StartProcessDlg>
    {
        Q_OBJECT

    public:

        StartProcessDlg(StartProcessDlgModel* model);
        virtual void loadFromModel();
        virtual void setupSignalSlotConnection();
        virtual void onModelSetDisabled(const std::set<size_t>& indexes);

    private slots:

        void setDisabledSlot(const std::set<size_t>& indexes);

    signals:

        void setDisabledSignal(const std::set<size_t>& indexes);
    };
}
