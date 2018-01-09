#pragma once

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"
#include "core/utilities/src/RunParams.h"
#include "Equipment.h"
#include "InspectorpanelAppController.h"
//#include "equipmentproxy.h"
#include "AbstractEquipmentPModel.h"

namespace TA_Base_App
{
	class InspectorPanelDlgModel:public AbstractEquipmentPModel
	{
	public:
		InspectorPanelDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~InspectorPanelDlgModel(void);
		
		std::string		getLabel();
		std::string		getTitle();
		std::string		getRequestScreen();
		void			displayHelp();
		bool			haveAllDataPointsConfigured();

		void notifyUpdate(const char * updateType,TA_Base_Bus::EventArgs * args/* =NULL */);
	public:
		
	private:
	};

}