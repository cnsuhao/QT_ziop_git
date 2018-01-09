/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/bus/alarm/alarm_list_control/src/PreConflictAckDlg.h $
* @author:  Ripple
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2012/02/06 16:15:14 $
* Last modified by:  $Author: haijun.li $
*
*/

#ifndef PRECONFLICKDIALOG_H
#define PRECONFLICKDIALOG_H

#include "bus/alarm/alarm_list_control/src/IAlarmRetriever.h"


#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"

#include "core\utilities\src\RunParams.h"

#include "core/alarm/IDL/src/AlarmMessageCorbaDef.h"
#include "bus/alarm/alarm_List_Control/GeneratedFiles/ui_preconflickdialog.h"

struct ConflictingTrains
{
	int offendingTrain;
	std::string offendingTrainString;
	int owningTrain;
	std::string owningTrainString;
};

namespace TA_Base_Bus
{
	class PreConflickDialog : public AbstractDialog
	{
		Q_OBJECT

	public:
		PreConflickDialog(TA_Base_Bus::ITransActiveApp* app);
		~PreConflickDialog();
		 
		//ITransActiveView interface
		virtual std::string  getMyPModelName(void);
		virtual void setupPModel(void);
		// For dynamic language switching
		virtual void translateUI();

		virtual void OnInitDialog();
		
		void setAlarmDetail(TA_Base_Bus::AlarmDataItem* details);
	public slots:
		void OnOK();
		void OnCancel();

	private: //Methods

	private: //Methods
		Ui::PreConflickDialog ui;

		TA_Base_Bus::AlarmDataItem* m_details;
		int m_train1;
		int m_train2;
	};
}



#endif // PRECONFLICKDIALOG_H
