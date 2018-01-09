#pragma once
#include "ui_OverRideExclusiveDutyDlg.h"
#include "CsDialog.h"

class OverRideExclusiveDuty : public CsDialog
{
    Q_OBJECT

public:

    OverRideExclusiveDuty(QString subsystemsAtLocations);

    void onInitDialog();

public slots:

    virtual void OnCancel();
    virtual void OnOK();

private:

    QString m_detail;
    Ui::OverRideExclusiveDutyDlg ui;
};
