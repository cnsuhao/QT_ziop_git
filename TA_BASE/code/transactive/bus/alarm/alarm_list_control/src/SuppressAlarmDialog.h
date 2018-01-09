#ifndef SUPPRESSALARMDIALOG_H
#define SUPPRESSALARMDIALOG_H

#include "bus/alarm/alarm_list_control/src/IAlarmRetriever.h"
#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"
#include "bus\unicode_common_lib\src\TAUnicodeString.h"
#include "core\utilities\src\RunParams.h"
#include "core/alarm/IDL/src/AlarmMessageCorbaDef.h"
#include "bus\unicode_common_lib\src\TAUnicodeString.h"
#include "ui_SuppressAlarmDialog.h"

namespace TA_Base_Bus
{
	// Forward declaration
	class SuppressAlarmDlgPModel;

	class SuppressAlarmDialog : public AbstractDialog
	{
		Q_OBJECT
	public:
		SuppressAlarmDialog(TA_Base_Bus::ITransActiveApp* app);
		~SuppressAlarmDialog();
		
		virtual std::string  getMyPModelName(void);
		virtual void setupPModel(void);
		// For dynamic language switching
		virtual void translateUI();

		bool wasSuccessful();

		void inhibitAlarmByType();
		void inhibitAlarmByDescription();
		void setAlarmDetails(boost::shared_ptr<TA_Base_Bus::AlarmDataItem> alarm, TAUtfString createdBySessionKey, unsigned long createdByProfileKey, TAUtfString createdByOperatorName);
		
		void setResult(boost::shared_ptr<bool> bResult) 
			{ m_bResult = bResult; };

	public slots: 
		void OnCancel();
		void OnOK();
	protected:
		virtual void initUIControl();
	private:
		void inhibitAlarm();

	private:
		Ui::SuppressAlarmDialog ui;
		SuppressAlarmDlgPModel* m_pTheRealPModel;
		boost::shared_ptr<bool> m_bResult;
	};

}


#endif // SUPPRESSALARMDIALOG_H
