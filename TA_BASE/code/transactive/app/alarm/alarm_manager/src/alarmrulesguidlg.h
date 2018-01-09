#ifndef ALARMRULESGUIDLG_H
#define ALARMRULESGUIDLG_H

#include <QWidget>
#include "app\alarm\alarm_manager\GeneratedFiles\ui_alarmrulesguidlg.h"

class alarmRulesGUIDlg : public QWidget
{
	Q_OBJECT

public:
	alarmRulesGUIDlg(QWidget *parent = 0);
	~alarmRulesGUIDlg();

private:
	Ui::alarmRulesGUIDlg ui;
};

#endif // ALARMRULESGUIDLG_H
