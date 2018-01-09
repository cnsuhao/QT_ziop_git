#pragma once

#include "bus\generic_gui_pmod\src\pmodeldatatype.h"
#include "app\alarm\alarm_manager\pmodel\src\alarmmanagerconsts.h"
#include <string>

namespace TA_Base_App
{
	enum FilterType
	{
		DEFAULT = 0,
		NAMED,
		USERDEF
	};

	class FilterChangeEventArgs
		: public TA_Base_Bus::EventArgs
	{
	public:
		FilterChangeEventArgs(FilterType filterType, const std::string& filterName)
			: TA_Base_Bus::EventArgs(FILTER_CHANGE)
			, m_filterType(filterType)
			, m_filterName(filterName)
		{}

		~FilterChangeEventArgs() {}

		FilterType getFilterType()
		{
			return m_filterType;
		}

		std::string getFiltername()
		{
			return m_filterName;
		}

	private:
		FilterType m_filterType;
		std::string m_filterName;
	};

}

