/**
* The source code in this file is the property of 
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File:  $
* @author:  
* @version: $Revision:  $
*
* Last modification: $DateTime:  $
* Last modified by:  $Author:  $
* 
* This class performs an action in a separate thread to the main thread. This is to ensure the action
* does not freeze the main application's update.
*
*/

#include "app/event/event_viewer/pmodel/src/ActionThread.h"

#include "core/utilities/src/DebugUtil.h"

using TA_Base_Core::DebugUtil;

namespace TA_Base_App
{

	ActionThread::ActionThread(Filter* filter,IEventAction* action)
	{	
		m_filter = filter;
		m_action = action;
		//m_terminated = false;
		start();
	}

	ActionThread::~ActionThread(){

	}

	void ActionThread::run()
	{
		if ( ( NULL != m_action ) || ( NULL != m_filter ) )
		{			
			try
			{
				// Now execute the action.
				m_action->executeAction( m_filter );
			}
			catch( ... )
			{
				// Actions are responsible for handling all exceptions they can generate.
				// If we get here just log it.
				LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "Action execution threw an unexpected exception." );
			}
		}		
	}
	
	void ActionThread::terminate()
	{
	}

};