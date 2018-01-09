#include "bus/common_control/src/printdialog/CtlPrintDialog.h"
#include <qthread.h>
#include <QtWidgets/qapplication.h>

QT_BEGIN_NAMESPACE

CtlPrintDialog::CtlPrintDialog(QWidget * parent /*= 0*/)
	: QPrintDialog(parent)
{
}

CtlPrintDialog::CtlPrintDialog(QPrinter * printer, QWidget * parent /*=0*/)
	: QPrintDialog(printer, parent)
{
}

CtlPrintDialog::~CtlPrintDialog()
{
}

void CtlPrintDialog::showView(bool modal)
{
	if (modal)
	{
		//this->exec();
		if (QThread::currentThread() == QApplication::instance()->thread())
		{
			this->exec();
		}
		else
		{
			QMetaObject::invokeMethod(this, "exec", Qt::BlockingQueuedConnection);
		}

	}
	else
	{
		//this->show();
		if (QThread::currentThread() == QApplication::instance()->thread())
		{
			this->show();
		}
		else
		{
			QMetaObject::invokeMethod(this, "exec", Qt::BlockingQueuedConnection);
		}
	}
}

int CtlPrintDialog::doModal()
{
	int dialogRes = -1;
	if (QThread::currentThread() == QApplication::instance()->thread())
	{
		dialogRes = this->exec();
	}
	else
	{
		QMetaObject::invokeMethod(this, "exec", Qt::BlockingQueuedConnection, Q_RETURN_ARG(int, dialogRes));
	}

	return dialogRes;
}

QT_END_NAMESPACE