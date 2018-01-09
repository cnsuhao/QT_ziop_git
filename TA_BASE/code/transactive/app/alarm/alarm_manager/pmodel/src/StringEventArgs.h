/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP/TA_BASE/transactive/app/alarm/alarm_manager/pmodel/src/StringEventArgs.h $
  * @author: Shiva
  * @version: $Revision:#1 $
  *
  * Last modification: $DateTime:  $
  * Last modified by:  $Author:shiva $
  * 
  *
  */

#ifndef STRING_EVENT_ARGS_ALARM_MANAGER_1234567890_H
#define STRING_EVENT_ARGS_ALARM_MANAGER_1234567890_H
#include "bus/generic_gui_pmod/src/CommandGateway.h"

namespace TA_Base_App
{
	class StringEventArgs: public TA_Base_Bus::EventArgs
	{
	public:
		StringEventArgs(std::string param1):TA_Base_Bus::EventArgs()
		{
			m_param1 = param1;
		}

		~StringEventArgs()
		{

		}

		std::string getParam1()
		{
			return m_param1;
		}
		
	private:
		std::string m_param1;

	};
}

#endif  // STRING_EVENT_ARGS_ALARM_MANAGER_1234567890_H
