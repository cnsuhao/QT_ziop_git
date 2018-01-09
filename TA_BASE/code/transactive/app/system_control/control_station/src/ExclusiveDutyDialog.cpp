/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/control_station/src/ExclusiveDutyDialog.cpp $
  * @author:  Azenith Arcilla
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * A dialog which lists the exclusive duties of the current session
  *
  */

// ExclusiveDutyDialog.cpp : implementation file
//

#include "StdAfx.h"
#include "controlstation.h"
#include "ExclusiveDutyDialog.h"
#include "app/system_control/control_station/src/SessionManager.h"

/////////////////////////////////////////////////////////////////////////////
// ExclusiveDutyDialog dialog

ExclusiveDutyDialog::ExclusiveDutyDialog(QString subsystemsAtLocations)
    : m_detail(subsystemsAtLocations)
{
    ui.setupUi(this);

    connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(OnOK()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(OnCancel()));
}

void ExclusiveDutyDialog::OnOK()
{
    CsDialog::accept();
}

void ExclusiveDutyDialog::OnCancel()
{
    CsDialog::reject();
}

void ExclusiveDutyDialog::onInitDialog()
{
    //// disable the 'x' button for now
    //CMenu* pSysMenu = GetSystemMenu(FALSE);

    //if (pSysMenu != NULL)
    //{
    //    pSysMenu->EnableMenuItem(SC_CLOSE, MF_GRAYED);
    //}

    // format the content
    ui.m_textCtrl->setText(m_detail);
    //m_textCtrl.SetBackgroundColor(FALSE, GetSysColor(COLOR_3DFACE));
}
