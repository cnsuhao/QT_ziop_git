// DutyChangeNoteDlg.cpp : implementation file
// For TD19075

#include "StdAfx.h"
#include "controlstation.h"
#include "DutyChangeNoteDlg.h"


DutyChangeNoteDlg::DutyChangeNoteDlg()
{
    ui.setupUi(this);
    connect(ui.m_detailsButton, SIGNAL(clicked(bool)), this, SLOT(OnDetail()));
    connect(ui.m_closeButton, SIGNAL(clicked(bool)), this, SLOT(OnClose()));
}


void DutyChangeNoteDlg::OnDetail()
{
    CsDialog::accept();
}

void DutyChangeNoteDlg::OnClose()
{
    CsDialog::reject();
}
