/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/ProcessParamsDlg.h $
  * @author:  Dominique Beauregard
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * ProcessParamsDlg is associated with the dialog box allowing operators
  * to set the processes' runtime parameters. In the current version, it
  * only allows the debug level of processes to be changed.
  *
  */
#pragma once
#include "ui_ProcessParamsDlg.h"
#include "AbstructQDialogTemplate.h"
#include "app/system_control/system_manager/pmodel/src/IProcessParamsDlg.h"

namespace TA_Base_App
{
    struct ProcessParamsDlgModel;

    struct ProcessParamsDlg : AbstructQDialogTemplate<IProcessParamsDlg, ProcessParamsDlgModel, Ui::ProcessParamsDlg>
    {
        Q_OBJECT

    public:

        ProcessParamsDlg(ProcessParamsDlgModel* model);
        virtual void loadFromModel();
        virtual void setupSignalSlotConnection();
        virtual void onSwitchLanguage();
        virtual void onModelSetIndex(int index);

    private slots:

        void onDebugLevelsIndexChanged(int);
        void retranslateDebugLevels();
        void setIndexSlot(int index);

    signals:

        void setIndexSignal(int index);
    };
}
