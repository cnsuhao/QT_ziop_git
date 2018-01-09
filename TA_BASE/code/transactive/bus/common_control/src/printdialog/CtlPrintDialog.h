
#ifndef __CTRL_PRINT_DIALOG_H_INCLUDED__
#define __CTRL_PRINT_DIALOG_H_INCLUDED__
#include <QtCore>
#include <QtDesigner/QtDesigner>
#include <QtPrintSupport\QPrintDialog.h>

//#pragma once


QT_BEGIN_NAMESPACE

class QDESIGNER_WIDGET_EXPORT CtlPrintDialog : public QPrintDialog
{
	Q_OBJECT
public:
	explicit CtlPrintDialog(QWidget * parent = 0);
	explicit CtlPrintDialog(QPrinter * printer, QWidget * parent = 0);
	
	~CtlPrintDialog();

	virtual void showView(bool modal);

	virtual int doModal();
};

QT_END_NAMESPACE


#endif //__CTRL_PRINT_DIALOG_H_INCLUDED__