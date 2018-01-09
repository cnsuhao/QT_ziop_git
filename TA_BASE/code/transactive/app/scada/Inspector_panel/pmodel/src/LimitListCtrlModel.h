#pragma  once

//#include "EquipmentProxy.h"
#include "bus\generic_gui_pmod\src\TransActiveViewModel.h"
#include "InspectorpanelAppController.h"
#include "AbstractEquipmentPModel.h"

namespace TA_Base_App
{
	class LimitListCtrlModel:public AbstractEquipmentPModel
	{
	public:
		LimitListCtrlModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~LimitListCtrlModel(void);
	public:
		std::vector<unsigned long>		getLimitIndex();
		std::string						getLimitInfoString(unsigned int iPoint);
		std::string						getChildDataPointDescription(UINT iPoint);
		bool							isLimitsAccessPermitted(unsigned int iPoint);
	protected:
		void							onDataPointProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType);
		bool							isValid(unsigned int iPoint);
		

	private:
	};
}