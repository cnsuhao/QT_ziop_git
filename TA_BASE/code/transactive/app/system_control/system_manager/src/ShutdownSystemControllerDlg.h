/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/ShutdownSystemControllerDlg.h $
  * @author:  Dominique Beauregard
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * ShutdownSystemControllerDlg is associated with the dialog box
  * allowing operators to shutdown the System Controller currently
  * logged into.
  *
  */
#pragma once
#include "ui_ShutdownSystemControllerDlg.h"
#include "AbstructQDialogTemplate.h"
#include "app/system_control/system_manager/pmodel/src/IShutdownSystemControllerDlg.h"

namespace TA_Base_App
{
    struct ShutdownSystemControllerDlgModel;

    struct ShutdownSystemControllerDlg : AbstructQDialogTemplate<IShutdownSystemControllerDlg, ShutdownSystemControllerDlgModel, Ui::ShutdownSystemControllerDlg>
    {
        Q_OBJECT

    public:

        ShutdownSystemControllerDlg(ShutdownSystemControllerDlgModel* model);
        virtual void loadFromModel();
    };
}
