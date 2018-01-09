/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/ExclusiveDutyDialog.h $
  * @author:  Azenith Arcilla
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * A dialog which lists the exclusive duties of the current session
  *
  */
#pragma once
#include "ui_ExclusiveDutyDialog.h"
#include "CsDialog.h"

class ExclusiveDutyDialog : public CsDialog
{
    Q_OBJECT

public:

    ExclusiveDutyDialog(QString subsystemsAtLocations);    // standard constructor

public slots:

    void onInitDialog();
    virtual void OnOK();
    virtual void OnCancel();

private:

    QString m_detail;
    Ui::ExclusiveDutyDialog ui;
};
