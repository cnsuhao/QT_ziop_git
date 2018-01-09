/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/StyleDlg.h $
* @author:  Dominique Beauregard
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
*
* StyleDlg is associated with the dialog that allows
* operators to set the data refresh rate.
*
*/
#pragma once
#include "ui_StyleDlg.h"
#include "AbstructQDialogTemplate.h"
#include "app/system_control/system_manager/pmodel/src/IStyleDlg.h"

namespace TA_Base_App
{
    struct StyleDlgModel;

    struct StyleDlg : AbstructQDialogTemplate<IStyleDlg, StyleDlgModel, Ui::StyleDlg>
    {
        Q_OBJECT

    public:

        StyleDlg(StyleDlgModel* model);
        virtual void loadFromModel();
        virtual void setupSignalSlotConnection();

    private slots:

        void onLanguageIndexChanged(int index);
        void onStyleIndexChanged(int index);
        void onStyleSheetIndexChanged(int index);
        virtual void reject() { accept(); }
    };
}
