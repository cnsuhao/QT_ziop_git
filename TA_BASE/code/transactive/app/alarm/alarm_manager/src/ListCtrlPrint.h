

#ifndef __PRINTER_JOB_H_INCLUDED__
#define __PRINTER_JOB_H_INCLUDED__

#include "core\threads\src\Thread.h"
#include <qstring.h>
#include <qfont.h>
#include <qRect.h>
#include <QObject>
#include <QtPrintSupport\qprinter.h>
#include "bus\unicode_common_lib\src\TAUnicodeString.h"
#include "bus\alarm\alarm_list_control\src\AlarmListCtrl.h"
namespace TA_Base_App
{
	class CListCtrlPrint
		: public QObject
		, public TA_Base_Core::Thread
	{
		Q_OBJECT
	public:
		CListCtrlPrint();
		~CListCtrlPrint();

		// Since this is a thread these are parameters to receive from the calling class.
		struct t_ParamsToReceive
		{
			TA_Base_Bus::TAUtfString title;
			TA_Base_Bus::TAUtfString csPageHead;
			TA_Base_Bus::TAUtfString csPage;
			AlarmListCtrl* list;
			QPrinter *printer;
			QTextDocument *document;
			// TO DO: Direct printing of the image instead of text
			std::vector<TA_Base_Bus::TAUtfString>* imageStringVector;

			t_ParamsToReceive() : title(""), csPageHead(""), csPage(""), list(NULL) {}
		};

		TA_Base_Bus::TAUtfString strTitle;
		
		AlarmListCtrl * pList;	// pointer of the list ctrl
		std::vector<TA_Base_Bus::TAUtfString>* pImageStringVector;

		
		TA_Base_Bus::TAUtfString csPageHead;	// text of header
		TA_Base_Bus::TAUtfString csPageFooter;	// text of footer
		TA_Base_Bus::TAUtfString csPage;	// format of page number / tot pages (ex. page %d of %d)

		TA_Base_Bus::TAUtfString csFtHead, csFtFoot, csFtPrint;	// fonts type names
		int     iFtHead, iFtFoot, iFtPrint;	// fonts sizes

		QSize szFoot;	// internal use

		void print(t_ParamsToReceive* params);

	protected:
		virtual void run();
		virtual void terminate();

	private:
		int getItemCountPerPage();

	private:
		TA_Base_Bus::TAUtfString m_strTitle;
		QFont m_font;
		QPrinter *m_printer;
		QTextDocument *m_document;
	};

}

#endif //__PRINTER_JOB_H_INCLUDED__