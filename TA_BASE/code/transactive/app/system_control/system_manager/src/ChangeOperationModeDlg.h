/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/ChangeOperationModeDlg.h $
  * @author:  Dominique Beauregard
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * ChangeOperationModeDlg is the class associated with the
  * Change Operation Mode dialog.
  *
  */
#pragma once
#include "ui_ChangeOperationModeDlg.h"
#include "AbstructQDialogTemplate.h"
#include "app/system_control/system_manager/pmodel/src/IChangeOperationModeDlg.h"

namespace TA_Base_App
{
    struct ChangeOperationModeDlgModel;

    struct ChangeOperationModeDlg : AbstructQDialogTemplate<IChangeOperationModeDlg, ChangeOperationModeDlgModel, Ui::ChangeOperationModeDlg>
    {
        Q_OBJECT

    public:

        ChangeOperationModeDlg(ChangeOperationModeDlgModel* model);
        virtual void loadFromModel();
        virtual void setupSignalSlotConnection();
        virtual void onModelSetDisabled(const std::set<size_t>& indexes);
        virtual void onModelSetAutoExclusive(bool auto_exclusive);
        virtual void onModelSetRadioEnabled(bool enabled);
        virtual void omModelSetRadioChecked(bool control_checked, bool monitor_checked);

    private slots:

        void onControlRadioToggled(bool checked);
        void onMonitorRadioToggled(bool checked);
        void setDisabledSlot(const std::set<size_t>& indexes);
        void setAutoExclusiveSlot(bool auto_exclusive);
        void setRadioEnabledSlot(bool enabled);
        void setRadioCheckedSlot(bool control_checked, bool monitor_checked);

    signals:

        void setDisabledSignal(const std::set<size_t>& indexes);
        void setAutoExclusiveSignal(bool auto_exclusive);
        void setRadioEnabledSignal(bool enabled);
        void setRadioCheckedSingnal(bool control_checked, bool monitor_checked);
    };
}
