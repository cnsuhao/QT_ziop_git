#pragma once

#include "bus\generic_gui_pmod\src\TransActiveViewModel.h"
//#include "equipmentproxy.h"
#include "InspectorpanelAppController.h"
#include "AbstractEquipmentPModel.h"

namespace TA_Base_App
{
	struct InformationListCtrlItem
	{
		std::string		description;
		std::string		shortName;
		std::string		valueString;
		std::string		timestamp;
		std::string		qualityString;

	};


	class InformationListCtrlModel:public AbstractEquipmentPModel
	{
	public:
		InformationListCtrlModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~InformationListCtrlModel(void);
	protected:
		//virtual	void		onInitStatusUpdate (EquipmentInitStatus status);
		virtual void		onDataPointProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType);
	public:
		InformationItem					getInformationItem(unsigned int iPoint);
		std::vector<InformationItem>	getAllInfomationDisplayItem();
	private:
		bool isValid(unsigned int iPoit);
	};
}