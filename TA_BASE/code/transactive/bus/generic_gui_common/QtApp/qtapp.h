#ifndef QTAPP_H
#define QTAPP_H

#include <QtGui/QMainWindow>
#include <QtGui/QDialog>
#include "ui_qtapp.h"
#include "ui_NewForm.h"
class QtApp : public QMainWindow
{
	Q_OBJECT

public:
	QtApp(QWidget *parent = 0, Qt::WFlags flags = 0);
	~QtApp();
public slots:
		void destroyedSlot(QObject * obj);
		void finishedSlot(int a);
private:
		 virtual void closeEvent(QCloseEvent *e);
private:
	Ui::QtAppClass ui;
};


class myDialog : public QDialog
{
	Q_OBJECT

public:
	myDialog(QWidget *parent = 0, Qt::WFlags flags = 0): QDialog(parent,flags)
	{
		ui.setupUi(this);
		
 
	};
	~myDialog(){};
private:
	Ui::NewForm ui;
};

#endif // QTAPP_H
