/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/actions/ActionDisplayComment.cpp $
 * @author:  Karen Graham
 * @version: $Revision: #5 $
 *
 * Last modification: $DateTime: 2016/11/22 14:12:39 $
 * Last modified by:  $Author: Noel $
 * 
 * This class is for the action that displays a read-only alarm comment
 */

#pragma warning(disable:4786)
 
#include "bus/alarm/alarm_list_control/src/actions/ActionDisplayComment.h"
#include "bus/alarm/alarm_list_control/src/AlarmRetriever.h"
#include "bus/alarm/alarm_list_control/src/ConstDefine.h"
#include "bus/alarm/alarm_list_control/src/AlarmListViewFactory.h"

#include "core/exceptions/src/AlarmListCtrlException.h"
#include "core/exceptions/src/AlarmActionException.h"

#include "bus/generic_gui_view/src/TransActiveMessage.h"

using TA_Base_Core::AlarmActionException;

namespace TA_Base_Bus
{
    ActionDisplayComment::ActionDisplayComment(bool isAvailableInContextMenu /*=true*/)
        : m_isAvailableInContextMenu(isAvailableInContextMenu)
    {
	 
    }

    
    bool ActionDisplayComment::isAvailable(IAlarmRetriever& alarmRetriever)
    {
		// Display comment is available if at least one alarm is selected
        std::vector<std::string>& selectedAlarms = alarmRetriever.getSelectedAlarms();
        return (selectedAlarms.size() >= 1);
    }


    void ActionDisplayComment::performAction(IAlarmRetriever& alarmRetriever)
    {
        FUNCTION_ENTRY("performAction");   

		TAUtfString comment = "";

		std::vector<std::string> selectedAlarms = alarmRetriever.getSelectedAlarms();
		if (selectedAlarms.size() == 1)
		{
			AlarmDataItem * pTemp = alarmRetriever.getAlarmItem(selectedAlarms[0]);
			if (pTemp == NULL) return;

			std::auto_ptr<TA_Base_Bus::AlarmDataItem> pAlarm(pTemp);
			comment = pAlarm->alarmComments;
		}
		else if (selectedAlarms.size() > 1)
		{
			// TD 8705: When multiple alarms have been selected and 'Comment' is selected, indicate
			// that comments cannot be viewed simultaneously, but can be created, or deleted.
			// When operator attempts to edit comments of multiple alarms "the edit alarm comment dialog is displayed
			comment = tr("Comments cannot be viewed simultaneously, as multiple Alarms are selected. \r\n");
			comment += tr("If you wish to delete comment of selected alarms, click Delete button.\r\n");
			comment += tr("If you wish to replace existing/create new comment for selected alarms, click Edit button.");
		}
		else // A case that the selected alarm is zero or retriever is faulty
		{
			comment = tr("No alarm is selected or system is not working fine.");
		}


		ITransActiveView* view=m_model->getViewManager()->getView(ViewType_Alarm_ViewComment,"");
		if (view!=NULL)
		{
			CommentDialog* pCommentDlg = dynamic_cast<CommentDialog*>(view);
			pCommentDlg->setAlarmRetriever(&alarmRetriever);
			pCommentDlg->setComment(comment);
			//pCommentDlg->showView(true);
			view->doModal();
		}
        // By default the comment is empty. This applies in the case when either there is no
		// comment for the single selected alarm, of if multiple alarms are selected.


		FUNCTION_EXIT;
    }
}
