/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/ActionThread.h $
 * @author:  Karen Graham
 * @version: $Revision: #2 $
 *
 * Last modification: $DateTime: 2016/05/11 17:52:04 $
 * Last modified by:  $Author: Noel $
 * 
 * This class performs an action in a separate thread to the main thread. This is to ensure the action
 * does not freeze the main application's update.
 */

#if !defined(AFX_ACTIONTHREAD_H__E16E2BFF_2ECB_4C32_A940_64B8DFC34A0C__INCLUDED_)
#define AFX_ACTIONTHREAD_H__E16E2BFF_2ECB_4C32_A940_64B8DFC34A0C__INCLUDED_
#include "core/synchronisation/src/ReEntrantThreadLockable.h"
#include "core/synchronisation/src/Semaphore.h"
#include "core/threads/src/Thread.h"
#include "bus/alarm/alarm_list_control/src/AlarmActionFactory.h"
#include "bus/alarm/alarm_list_control/src/AlarmRetriever.h"
namespace TA_Base_Bus
{ 
	//TODO:
	class ActionThread: public TA_Base_Core::Thread
    {
	public:
		ActionThread(AlarmActionFactory* actionfactory,AlarmRetriever* alarmRetriever);	        
		virtual ~ActionThread();		 
		//thread functions
		virtual void run() ; 	 
		virtual void terminate();
		//  action related
		void executeAction(unsigned long acitonID);
	private:
		bool  m_terminated;
		AlarmActionFactory* m_alarmActionFactory;
		AlarmRetriever* m_alarmRetriever;
		 /**
		 * semaphore for waiting task;
		 */
		TA_Base_Core::Semaphore m_waitSemaphore;
		unsigned long m_NextTask;
		TA_Base_Core::ReEntrantThreadLockable m_actionLock;
};

   
 
}
#endif // !defined(AFX_ACTIONTHREAD_H__E16E2BFF_2ECB_4C32_A940_64B8DFC34A0C__INCLUDED_)
