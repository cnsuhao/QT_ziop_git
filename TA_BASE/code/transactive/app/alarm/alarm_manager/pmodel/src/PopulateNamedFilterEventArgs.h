#pragma once

#include "bus\generic_gui_pmod\src\pmodeldatatype.h"
#include "app\alarm\alarm_manager\pmodel\src\alarmmanagerconsts.h"
#include <string>

namespace TA_Base_App
{
	class PopulateNamedFilterEventArgs
		: public TA_Base_Bus::EventArgs
	{
	public:
		PopulateNamedFilterEventArgs(const std::string& filterName)
			: TA_Base_Bus::EventArgs(POPULATE_NAMED_FILTER)
			, m_filterName(filterName)
		{}

		~PopulateNamedFilterEventArgs() {}

		std::string getFiltername()
		{
			return m_filterName;
		}

	private:
		std::string m_filterName;

	};

}

