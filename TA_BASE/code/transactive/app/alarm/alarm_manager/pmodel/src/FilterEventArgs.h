/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP/TA_BASE/transactive/app/alarm/alarm_manager/pmodel/src/FilterEventArgs.h $
  * @author: Shiva
  * @version: $Revision:#1 $
  *
  * Last modification: $DateTime:  $
  * Last modified by:  $Author:shiva $
  * 
  *
  */

#ifndef FILTER_EVENT____ARGS_ALARM_MANAGER_1234567890_H
#define FILTER_EVENT____ARGS_ALARM_MANAGER_1234567890_H
#include "bus/generic_gui_pmod/src/CommandGateway.h"
#include "bus/alarm/alarm_list_control/pmodel/src/Filter.h"
#include "app\alarm\alarm_manager\pmodel\src\alarmmanagerconsts.h"

namespace TA_Base_App
{
	class FilterEventArgs: public TA_Base_Bus::EventArgs
	{
	public:
		FilterEventArgs(bool isDefault) 
			: TA_Base_Bus::EventArgs()
			, m_bIsDefault(isDefault)
			, m_filter(0)
		{			
		}

		~FilterEventArgs()
		{		
		}
		
		bool isDefault()
		{
			return m_bIsDefault;
		}

		TA_Base_Bus::Filter* getFilter()
		{
			return m_filter;
		}

		void setFilter(TA_Base_Bus::Filter* filter)
		{
			m_filter = filter;
		}
		
	private:
		TA_Base_Bus::Filter* m_filter;
		bool m_bIsDefault;
	};
}

#endif  // FILTER_EVENT____ARGS_ALARM_MANAGER_1234567890_H
