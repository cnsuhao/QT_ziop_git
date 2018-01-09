#include "qtapp.h"
 
#include <QMessageBox>

QtApp::QtApp(QWidget *parent, Qt::WFlags flags)
	: QMainWindow(parent, flags)
{
	ui.setupUi(this);
	 
    connect(this, SIGNAL(destroyed()), this, SLOT(destroyedSlot()));
	 connect(this, SIGNAL(finished()), this, SLOT(finishedSlot()));
}

QtApp::~QtApp()
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, tr("~QtApp()"),
		"~QtApp",
		QMessageBox::Yes | QMessageBox::No );
}
void QtApp::closeEvent(QCloseEvent *e)
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, tr("closeevent()"),
		"MESSAGE1",
		QMessageBox::Yes | QMessageBox::No );
	// e->accetp();
	//  e->ignore();

}

void QtApp::destroyedSlot(QObject * obj )
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, tr("destroyed()"),
		"destroyed",
		QMessageBox::Yes | QMessageBox::No );
}


void QtApp::finishedSlot(int a)
{
	QMessageBox::StandardButton reply;
	reply = QMessageBox::question(this, tr("finishedSlot()"),
		"finishedSlot",
		QMessageBox::Yes | QMessageBox::No );
}