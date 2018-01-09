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
#include "AbstructQDialog.h"
#include "Translation.h"

namespace TA_Base_App
{
    template<typename View, typename Model, typename UI, bool is_modal = false>
    struct AbstructQDialogTemplate : View, AbstructQDialog
    {
        typedef UI UI;
        typedef View View;
        typedef Model Model;

        AbstructQDialogTemplate(Model* model)
            : AbstructQDialog(is_modal),
              m_model(model)
        {
        }

        ~AbstructQDialogTemplate()
        {
            delete m_model;
        }

        UI ui;
        Model* m_model;
        Translation m_tr;
    };
}
