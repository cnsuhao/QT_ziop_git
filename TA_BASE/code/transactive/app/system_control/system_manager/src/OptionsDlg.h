/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/OptionsDlg.h $
  * @author:  Dominique Beauregard
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * OptionsDlg is associated with the dialog that allows
  * operators to set the data refresh rate.
  *
  */
#pragma once
#include "ui_OptionsDlg.h"
#include "AbstructQDialogTemplate.h"
#include "app/system_control/system_manager/pmodel/src/IOptionsDlg.h"

namespace TA_Base_App
{
    struct OptionsDlgModel;

    struct OptionsDlg : AbstructQDialogTemplate<IOptionsDlg, OptionsDlgModel, Ui::OptionsDlg>
    {
        Q_OBJECT

    public:

        OptionsDlg(OptionsDlgModel* model);
        virtual void loadFromModel();
        virtual void setupSignalSlotConnection();

    private slots:

        void onRefreshRatesIndexChanged(int);
    };
}
