/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP/TA_BASE/transactive/app/alarm/alarm_manager/pmodel/src/SeverityCreateEventArgs.h $
  * @author: Shiva
  * @version: $Revision:#1 $
  *
  * Last modification: $DateTime:  $
  * Last modified by:  $Author:shiva $
  * 
  *
  */

#ifndef SEVERITY_CREATE_EVENT_ARGS_ALARM_MANAGER_1234567890_H
#define SEVERITY_CREATE_EVENT_ARGS_ALARM_MANAGER_1234567890_H
#include "bus/generic_gui_pmod/src/CommandGateway.h"
#include "app\alarm\alarm_manager\pmodel\src\AlarmFilterService.h"
#include "app\alarm\alarm_manager\pmodel\src\alarmmanagerconsts.h"

namespace TA_Base_App
{
	
	class SeverityCreateEventArgs: public TA_Base_Bus::EventArgs
	{
	public:
		SeverityCreateEventArgs(std::list<SeverityItem> param1) 
			:TA_Base_Bus::EventArgs(CREATE_SEVERITY)
		{
			m_listDatas = param1;
		}

		~SeverityCreateEventArgs()
		{
			m_listDatas.clear();
		}

		std::list<SeverityItem> getListData()
		{
			return m_listDatas;
		}
		
	private:
		std::list<SeverityItem> m_listDatas;

	};
}

#endif  // SEVERITY_CREATE_EVENT_ARGS_ALARM_MANAGER_1234567890_H
