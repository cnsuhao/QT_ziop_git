/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/ActionThread.cpp $
 * @author:  Karen Graham
 * @version: $Revision: #3 $
 *
 * Last modification: $DateTime: 2016/11/22 14:12:39 $
 * Last modified by:  $Author: Noel $
 * 
 * This class performs an action in a separate thread to the main thread. This is to ensure the action
 * does not freeze the main application's update.
 */

/*#include "bus/alarm/alarm_list_control/src/stdafx.h"*/
#include "bus/alarm/alarm_list_control/src/ActionThread.h"
#include "bus/alarm/alarm_list_control/src/IAlarmAction.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionSuppressAlarm.h"
#include "bus/unicode_common_lib/src/TAStringTranslate.h"

#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include "core/exceptions/src/AlarmActionException.h"

#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"

using TA_Base_Core::AlarmActionException;

 

namespace TA_Base_Bus
{ 
	ActionThread::ActionThread(AlarmActionFactory* actionfactory,AlarmRetriever* alarmRetriever) : 
	   m_terminated(false), m_alarmActionFactory(actionfactory), m_alarmRetriever(alarmRetriever),m_NextTask(0)
    {
		start();
    }

    ActionThread::~ActionThread()
    {
		terminateAndWait();
    }

	void ActionThread::executeAction(unsigned long acitonID)
	{
		TA_Base_Core::ThreadGuard guard(m_actionLock);
		m_NextTask=acitonID;
		m_waitSemaphore.post();
	}

	void ActionThread::run()  	 
	{
       
	   while(!m_terminated)
	   {
		   unsigned long actionID;
		   m_waitSemaphore.wait();
		   {
			   TA_Base_Core::ThreadGuard guard(m_actionLock);
			   if (m_NextTask==0) continue;
			   actionID = m_NextTask;
			   m_NextTask=0;
		   }	   

		   try
		   {
			   //execute action
			   IAlarmAction* pAlarmAction = m_alarmActionFactory->getAlarmAction(actionID);
			   m_alarmRetriever->clearCache();
			   if (pAlarmAction != 0)
			   {
				   if (pAlarmAction->getActionControlId() == INHIBIT_ALARM)
				   {
					   ActionSuppressAlarm* suppressAction = dynamic_cast<ActionSuppressAlarm*> (pAlarmAction);
					   if (suppressAction == NULL)
					   {
						   LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "ActionSuppressAlarm not found! This should not happen!!!");
						   continue;
					   }
					   if (suppressAction->getSubActionControlId() == INHIBIT_ALARM_BY_TYPE)
					   {
						   suppressAction->performActionByType(*m_alarmRetriever);
					   }
					   else if (suppressAction->getSubActionControlId() == INHIBIT_ALARM_BY_DESCRIPTION)
					   {
						   suppressAction->performActionByDescription(*m_alarmRetriever);
					   }
					   else
					   {
						   pAlarmAction->performAction(*m_alarmRetriever);
					   }
				   }
				   else
				   {
					   pAlarmAction->performAction(*m_alarmRetriever);
				   }

				   
			   }
		   }
		   catch (const TA_Base_Core::AlarmActionException& ex)
		   {
			   TA_Base_Bus::TransActiveMessage userMsg;
			   std::string actionName;
			   unsigned int errorId;

			   switch (ex.getFailType())
			   {
			   case AlarmActionException::CANNOT_FIND_ALARM:
				   errorId = IDS_UE_040027;
				   break;
			   case AlarmActionException::CANNOT_UPDATE_ALARM_COMMENTS:
				   errorId = IDS_UE_040023;
				   actionName = TAStringTranslate("comment");
				   break;
			   case AlarmActionException::CANNOT_ACKNOWLEDGE_ALARMS:
				   errorId = IDS_UE_040023;
				   actionName = TAStringTranslate("acknowledge");
				   break;
			   case AlarmActionException::CANNOT_ACKNOWLEDGE_ALARM:
				   errorId = IDS_UE_040025;
				   break;
			   case AlarmActionException::CANNOT_ACKNOWLEDGE_AND_CLOSE_ALARMS:
				   errorId = IDS_UE_040023;
				   actionName = TAStringTranslate("acknowledge and close");
				   break;
			   case AlarmActionException::CANNOT_FIND_ASSOCIATED_PLAN:
				   errorId = IDS_UE_040008;
				   break;
			   case AlarmActionException::CANNOT_RUN_ASSOCIATED_PLAN:
				   errorId = IDS_UE_040024;
				   actionName = TAStringTranslate("run");
				   break;
			   case AlarmActionException::CANNOT_POSTPONE_PLAN:
				   errorId = IDS_UE_040024;
				   actionName = TAStringTranslate("postpone");
				   break;
			   case AlarmActionException::CANNOT_LAUNCH_SCHEMATIC:
				   errorId = IDS_UE_010002;
				   actionName = TAStringTranslate("schematic");
				   break;
			   case AlarmActionException::CANNOT_LAUNCH_REQUEST_MANAGER:
				   errorId = IDS_UE_010002;
				   actionName = TAStringTranslate("Service Request Manager");
				   break;
			   case AlarmActionException::CANNOT_LAUNCH_EVENT_VIEWER:
				   errorId = IDS_UE_010002;
				   actionName = TAStringTranslate("Event Viewer");
				   break;
			   case AlarmActionException::CANNOT_LAUNCH_INSPECTOR_PANEL:
				   errorId = IDS_UE_010002;
				   actionName = TAStringTranslate("Inspector Panel");
				   break;
			   case AlarmActionException::CANNOT_LAUNCH_TREND_VIEWER:
				   errorId = IDS_UE_010002;
				   actionName = TAStringTranslate("Trend Viewer");
				   break;
			   case AlarmActionException::CANNOT_LAUNCH_INCIDENT_MANAGER:
				   errorId = IDS_UE_010002;
				   actionName = TAStringTranslate("Incident Manager");
			   default:
				   break;
			   }

			   if (!actionName.empty())
			   {
				   userMsg << actionName;
			   }

			   userMsg.showMsgBox(errorId);
		   }
		   catch (const TA_Base_Core::TransactiveException& te)
		   {
			   LOG_EXCEPTION_CATCH(SourceInfo, "TA_Base_Core::TransActiveException", te.what());
		   }
		   catch (...)
		   {
			   std::ostringstream desc;
			   desc << "[ActionThread::performThreadedAction] Caught unknown exception actionID " << actionID;
			   LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", desc.str().c_str());
		   }
			


	   }

	}
	void ActionThread::terminate()
	{
		m_terminated=true;
		m_waitSemaphore.post();
	}
 

 

} // end of ActionThread
