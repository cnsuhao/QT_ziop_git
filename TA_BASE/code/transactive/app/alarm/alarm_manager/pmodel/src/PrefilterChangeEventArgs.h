/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP/TA_BASE/transactive/app/alarm/alarm_manager/pmodel/src/SystemStatusEventArgs.h $
* @author: Shiva
* @version: $Revision:#1 $
*
* Last modification: $DateTime:  $
* Last modified by:  $Author:shiva $
*
*
*/

#ifndef __PREFILTER_CHANGE_EVENT_ARGS_H_INCLUDED__
#define __PREFILTER_CHANGE_EVENT_ARGS_H_INCLUDED__
#include "bus/generic_gui_pmod/src/CommandGateway.h"
#include "bus\alarm\alarm_list_control\pmodel\src\Filter.h"
#include "app\alarm\alarm_manager\pmodel\src\alarmmanagerconsts.h"

#include <vector>
namespace TA_Base_App
{
	enum SYSTEM_STATE
	{
		NORMAL_MODE = 0,
		DEGRADED_MODE
	};

	class PrefilterChangeEventArgs : public TA_Base_Bus::EventArgs
	{
	public:
		PrefilterChangeEventArgs(SYSTEM_STATE state, const std::string& sessionID,
			TA_Base_Bus::Filter* prefilter, std::vector<unsigned long>& locations)
			: TA_Base_Bus::EventArgs()
			, m_state(state)
			, m_sessionID(sessionID)
			, m_preFilter(prefilter)
		{
			m_locations = locations;
		}

		~PrefilterChangeEventArgs()
		{
			if (m_preFilter != NULL)
			{
				delete m_preFilter;
				m_preFilter = 0;
			}
		}

		SYSTEM_STATE getSystemState()
		{
			return m_state;
		}

		std::string getSessionID() { return m_sessionID; }

		TA_Base_Bus::Filter* getPreFilter() { return m_preFilter; }

		std::vector<unsigned long> getLocations() { return m_locations; }
	private:
		SYSTEM_STATE m_state;
		std::string m_sessionID;
		TA_Base_Bus::Filter* m_preFilter;
		std::vector<unsigned long> m_locations;
	};
}

#endif  // __PREFILTER_CHANGE_EVENT_ARGS_H_INCLUDED__
