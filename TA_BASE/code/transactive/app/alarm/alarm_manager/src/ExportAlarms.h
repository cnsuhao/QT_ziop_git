

#ifndef __EXPORT_ALARMS_H_INCLUDED__
#define __EXPORT_ALARMS_H_INCLUDED__

#include "bus/common_control/src/filedialog/CtlFileDilaog.h"
#include "core/threads/src/Thread.h"
#include "bus\alarm\alarm_list_control\src\AlarmListCtrl.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"
#include <QObject>

namespace TA_Base_App
{

	class CExportAlarms 
		: public QObject
		, public TA_Base_Core::Thread
	{
		Q_DECLARE_TR_FUNCTIONS(CExportAlarms)
		
	public:
		struct t_ParamsToReceive
		{
			TA_Base_Bus::AlarmListCtrl* alarmList;
			CtlFileDilaog* fileDlg;
			TA_Base_Bus::TAUtfString defaultExportPath;
			std::vector<TA_Base_Bus::TAUtfString>* pImageStringVector;
		};

		CExportAlarms() : m_alarmList(NULL), EXPORTALARMS_LOCALPATH("c:\\") {};

		virtual ~CExportAlarms(){};

		void saveNow(t_ParamsToReceive* params);

	protected:
		virtual void run();
		virtual void terminate();
		void convertText(std::string& text);
	private:
		bool m_terminate;
		TA_Base_Bus::AlarmListCtrl* m_alarmList;
		CtlFileDilaog* m_fileDlg;
		TA_Base_Bus::TAUtfString m_defaultExportPath;
		std::vector<TA_Base_Bus::TAUtfString>* m_pImageStringVector;
		std::string EXPORTALARMS_LOCALPATH;
	};

}

#endif //__EXPORT_ALARMS_H_INCLUDED__