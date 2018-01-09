#ifndef DYDLL_H
#define DYDLL_H

#include "dydll_global.h"
#include "bus/generic_gui_common/DyDLL/GeneratedFiles/ui_f.h"

class myDialog2 : public QDialog
{
	Q_OBJECT

public:
	myDialog2(QWidget *parent = 0, Qt::WFlags flags = 0);
	virtual ~myDialog2(){};
private:
	Ui::Dialog ui;
public slots:
		void clicktest();

};


class DYDLL_EXPORT DyDLL
{
public:
	DyDLL();
	void show();
	~DyDLL();

private:

};

#endif // DYDLL_H
