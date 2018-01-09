

#ifndef __PRINTER_JOB_H_INCLUDED__
#define __PRINTER_JOB_H_INCLUDED__

#include "core\threads\src\Thread.h"
#include <qstring.h>
#include <qfont.h>
#include "bus\unicode_common_lib\src\TAUnicodeString.h"
namespace TA_Base_App
{
	class CPrinterJob 
		: public TA_Base_Core::Thread
	{
	public:
		CPrinterJob();
		~CPrinterJob();

		virtual void run();

		virtual void terminate();

	private:
		TA_Base_Bus::TAUtfString m_strTitle;
		QFont m_font;

		bool m_bTerminate;

	};

}

#endif //__PRINTER_JOB_H_INCLUDED__