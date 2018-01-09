#ifndef EQUIPMENT_ITEM_UPDATE_EVENT_H
#define EQUIPMENT_ITEM_UPDATE_EVENT_H

#include "bus/generic_gui_pmod/src/PModelDataType.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/CustomDefines.h"

namespace TA_Base_App
{
	class EquipmentItemUpdateEvent : public TA_Base_Bus::EventArgs
	{
	public:
		EquipmentItemUpdateEvent(unsigned long entitykey, unsigned long colFlags, bool isFiltered)
			: EventArgs(EDSV::EVENT_ARG_ITEM_UPDATE),
			m_isFiltered(isFiltered),
			m_entityKey(entitykey),
			m_colUpdateFlags(colFlags)
		{

		}

		~EquipmentItemUpdateEvent(){};

		bool isFiltered()
		{
			return m_isFiltered;
		}

		unsigned long getEntityKey()
		{
			return m_entityKey;
		}

		unsigned long getColUpdateFlags()
		{
			return m_colUpdateFlags;
		}
	private:
		bool				m_isFiltered;
		unsigned long		m_entityKey;
		unsigned long		m_colUpdateFlags;
	};

	struct EquipmentItemInfo
	{
		EquipmentItemInfo(){};
		int	m_icon[ESL_COLUMN_MAX];
		std::string m_text[ESL_COLUMN_MAX];
	};
}

#endif