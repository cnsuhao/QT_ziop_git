#pragma once

#include "bus\generic_gui_pmod\src\pmodeldatatype.h"
#include "app\alarm\alarm_manager\pmodel\src\alarmmanagerconsts.h"
#include "bus\alarm\alarm_list_control\pmodel\src\Filter.h"

namespace TA_Base_App
{
	class PopulateFilterSelEventArgs
		: public TA_Base_Bus::EventArgs
	{
	public:
		PopulateFilterSelEventArgs(TA_Base_Bus::Filter* objFilter)
			: TA_Base_Bus::EventArgs(POPULATE_FILTER_SELECTION)
			, m_objFilter(objFilter)
		{}

		~PopulateFilterSelEventArgs() {}

		TA_Base_Bus::Filter* getFilterObject()
		{
			return m_objFilter;
		}

	private:
		TA_Base_Bus::Filter* m_objFilter;

	};

}

