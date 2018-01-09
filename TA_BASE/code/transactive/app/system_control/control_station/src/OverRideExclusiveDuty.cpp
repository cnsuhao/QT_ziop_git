// OverRideExclusiveDuty.cpp : implementation file
// added for TD19409

#include "StdAfx.h"
#include "controlstation.h"
#include "OverRideExclusiveDuty.h"
#include "app/system_control/control_station/src/SessionManager.h"

/////////////////////////////////////////////////////////////////////////////
// OverRideExclusiveDuty dialog


OverRideExclusiveDuty::OverRideExclusiveDuty(QString subsystemsAtLocations)
    : m_detail(subsystemsAtLocations)
{
    ui.setupUi(this);

    connect(ui.buttonBox, SIGNAL(accepted()), this, SLOT(OnOK()));
    connect(ui.buttonBox, SIGNAL(rejected()), this, SLOT(OnCancel()));

    onInitDialog();
}


void OverRideExclusiveDuty::OnCancel()
{
    CsDialog::reject();
}

void OverRideExclusiveDuty::OnOK()
{
    CsDialog::accept();
}

void OverRideExclusiveDuty::onInitDialog()
{
    ui.m_textEdit->setText(m_detail);
}
