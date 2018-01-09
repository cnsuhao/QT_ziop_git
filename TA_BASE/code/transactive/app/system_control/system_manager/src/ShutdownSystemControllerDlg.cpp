/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/ShutdownSystemControllerDlg.cpp $
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
#include "ShutdownSystemControllerDlg.h"
#include "app/system_control/system_manager/pmodel/src/ShutdownSystemControllerDlgModel.h"
#include <QPushButton>


namespace TA_Base_App
{

    ShutdownSystemControllerDlg::ShutdownSystemControllerDlg(ShutdownSystemControllerDlgModel* model)
        : AbstructQDialogTemplate(model)
    {
        setupDialog(this);
    }


    void ShutdownSystemControllerDlg::loadFromModel()
    {
        ui.hostname->setText(m_model->ui.hostname.c_str());
    }
}
