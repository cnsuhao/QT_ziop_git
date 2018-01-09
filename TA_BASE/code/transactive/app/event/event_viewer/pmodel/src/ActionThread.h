/**
* The source code in this file is the property of 
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: $
* @author:  Karen Graham
* @version: $Revision: #1 $
*
* Last modification: $DateTime:  $
* Last modified by:  $Author:  $
* 
* This class performs an action in a separate thread to the main thread. This is to ensure the action
* does not freeze the main application's update.
* 
*/

#if !defined(ACTIONTHREAD_H__E16E2BFF_2ECB_4C32_A940_64B8DFC34A0C__INCLUDED_)
#define ACTIONTHREAD_H__E16E2BFF_2ECB_4C32_A940_64B8DFC34A0C__INCLUDED_

#include "app/event/event_viewer/pmodel/src/Filter.h"
#include "app/event/event_viewer/pmodel/src/IEventAction.h"

#include "core/threads/src/Thread.h"

namespace TA_Base_App
{
	class ActionThread :  public TA_Base_Core::Thread
	{
	public:
		ActionThread(Filter* filter,IEventAction* action);
		virtual ~ActionThread();
		virtual void run();		
		/* This will try to terminate the thread */
		virtual void terminate();	
	private:
		IEventAction* m_action;
		Filter* m_filter;
		//bool m_terminated;
	};
}


#endif // !defined(ACTIONTHREAD_H__E16E2BFF_2ECB_4C32_A940_64B8DFC34A0C__INCLUDED_)
