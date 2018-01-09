/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/system_control/system_manager/src/SystemControllerDlg.h $
  * @author:  Karen Graham
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * This is the dialog used to allow the user to enter the System Controller host name and
  * port number.
  */
#pragma once
#include "ui_SystemControllerDlg.h"
#include "AbstructQDialogTemplate.h"
#include "app/system_control/system_manager/pmodel/src/ISystemControllerDlg.h"

namespace TA_Base_App
{
    struct SystemControllerDlgModel;

    struct SystemControllerDlg : AbstructQDialogTemplate<ISystemControllerDlg, SystemControllerDlgModel, Ui::SystemControllerDlg>
    {
        Q_OBJECT

    public:

        SystemControllerDlg(SystemControllerDlgModel* model);
        virtual void setupSignalSlotConnection();
        virtual void loadFromModel();

    private slots:

        void onPortChanged(const QString& text);
        void onHostnameIndexChanged(int index);
    };
}
