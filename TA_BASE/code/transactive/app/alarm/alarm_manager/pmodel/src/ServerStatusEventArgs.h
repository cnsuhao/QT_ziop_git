/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP/TA_BASE/transactive/app/alarm/alarm_manager/pmodel/src/ServerStatusEventArgs.h $
* @author: Shiva
* @version: $Revision:#1 $
*
* Last modification: $DateTime:  $
* Last modified by:  $Author:shiva $
*
*
*/

#ifndef __SERVER_STATUS_EVENT_ARGS_H_INCLUDED__
#define __SERVER_STATUS_EVENT_ARGS_H_INCLUDED__
#include "bus/generic_gui_pmod/src/CommandGateway.h"
#include "app\alarm\alarm_manager\pmodel\src\alarmmanagerconsts.h"

#include <vector>
namespace TA_Base_App
{
	enum SEVER_STATE
	{
		SERVER_UP = 0,
		SERVER_DOWN
	};

	class ServerStatusEventArgs : public TA_Base_Bus::EventArgs
	{
	public:
		ServerStatusEventArgs(SEVER_STATE state)
			: TA_Base_Bus::EventArgs()
			, m_state(state)
		{
		}

		~ServerStatusEventArgs()
		{
		}

		SEVER_STATE getServerState()
		{
			return m_state;
		}

	private:
		SEVER_STATE m_state;
	};
}

#endif  // __SERVER_STATUS_EVENT_ARGS_H_INCLUDED__
