/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP/TA_BASE/transactive/app/alarm/alarm_manager/pmodel/src/StringEventArgs.h $
* @author: Noel R. Tajanlangit
* @version: $Revision:#1 $
*
* Last modification: $DateTime:  $
* Last modified by:  $Author:Noel $
*
*
*/

#ifndef __FILTER_DIALOG_BUTTON_STATE_UPDATE_H_INCLUDED__
#define __FILTER_DIALOG_BUTTON_STATE_UPDATE_H_INCLUDED__
#include "bus/generic_gui_pmod/src/CommandGateway.h"
#include "app\alarm\alarm_manager\pmodel\src\alarmmanagerconsts.h"

namespace TA_Base_App
{
	class FilterDlgBtnStateEventArgs : public TA_Base_Bus::EventArgs
	{
	public:
		FilterDlgBtnStateEventArgs() 
			: TA_Base_Bus::EventArgs(CREATE_SEVERITY)
		{}

		FilterDlgBtnStateEventArgs(T_ButtonStateMap buttonStateMap)
			: TA_Base_Bus::EventArgs(CREATE_SEVERITY)
		{
			m_buttonStateMap = buttonStateMap;
		}

		~FilterDlgBtnStateEventArgs() {}

		void addButtonUpdate(FilterDlgButton button, bool enabled)
		{
			m_buttonStateMap[button] = enabled;
		}

		T_ButtonStateMap getButtonStateMap()
			{ return m_buttonStateMap; }

	private:
		T_ButtonStateMap m_buttonStateMap;
	};
}

#endif  // __FILTER_DIALOG_BUTTON_STATE_UPDATE_H_INCLUDED__
