#include "bus/common_control/src/filedialog/CtlFileDilaog.h"

#include <qthread.h>
#include <QtWidgets/qapplication.h>
#include "bus/common_control/src/common/StringUtility.h"

QT_BEGIN_NAMESPACE

CtlFileDilaog::CtlFileDilaog(QWidget * parent, Qt::WindowFlags flags)
	: QFileDialog(parent, flags)
{
}

CtlFileDilaog::CtlFileDilaog(QWidget * parent /*= 0*/, const QString & caption/* = QString()*/, const QString & directory /*= QString()*/, const QString & filter /*= QString()*/)
	: QFileDialog(parent, caption, directory, filter)
{
}

CtlFileDilaog::CtlFileDilaog(QWidget * parent, const std::string & caption, const std::string & directory, const std::string & filter)
	: QFileDialog(parent, StringUtility::fromStdString(caption), StringUtility::fromStdString(directory), StringUtility::fromStdString(filter))
{

}

CtlFileDilaog::~CtlFileDilaog()
{
}

void CtlFileDilaog::showView(bool modal)
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

int CtlFileDilaog::doModal()
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
