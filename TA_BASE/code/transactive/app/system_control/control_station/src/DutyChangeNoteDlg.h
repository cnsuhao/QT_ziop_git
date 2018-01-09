#pragma once
#include "ui_DutyChangeNoteDlg.h"
#include "CsDialog.h"

class DutyChangeNoteDlg : public CsDialog
{
    Q_OBJECT

public:

    DutyChangeNoteDlg();

public slots:

    void OnDetail();
    void OnClose();

private:

    Ui::DutyChangeNoteDlg ui;
};
