#include "dydll.h"

DyDLL::DyDLL()
{

}

DyDLL::~DyDLL()
{

}

void DyDLL::show()
{
  myDialog2*  m=new myDialog2();
  m->show();
}

myDialog2::myDialog2(QWidget *parent, Qt::WFlags flags)
: QDialog(parent, flags)
{
	ui.setupUi(this);
	QString str="~QtApp()";

	this->setWindowTitle(tr(str.toStdString().c_str()));
	connect (ui.btnTest,SIGNAL(clicked()),this,SLOT(clicktest()));
	 
}

void myDialog2::clicktest()
{
    QString str=ui.txtedit->toPlainText();
	this->setWindowTitle(tr(str.toStdString().c_str()));
	QString s1 ="输入中文";
	ui.txtedit->setPlainText(s1);
	 
}