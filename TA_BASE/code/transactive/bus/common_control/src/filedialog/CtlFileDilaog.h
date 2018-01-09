
#ifndef __CTRL_FILE_DIALOG_H_INCLUDED__
#define __CTRL_FILE_DIALOG_H_INCLUDED__
#include <QtCore>
#include <QtDesigner/QtDesigner>
#include <QtWidgets/qfiledialog.h>

//#pragma once
QT_BEGIN_NAMESPACE

class QDESIGNER_WIDGET_EXPORT CtlFileDilaog : public QFileDialog
{
	Q_OBJECT
public:
	explicit CtlFileDilaog(QWidget * parent, Qt::WindowFlags flags);
	explicit CtlFileDilaog(QWidget * parent = 0, const QString & caption = QString(), const QString & directory = QString(), const QString & filter = QString());
	explicit CtlFileDilaog(QWidget * parent, const std::string & caption, const std::string & directory, const std::string & filter);

	~CtlFileDilaog();

	virtual void showView(bool modal);
	virtual int doModal();
};

QT_END_NAMESPACE


#endif //__CTRL_FILE_DIALOG_H_INCLUDED__