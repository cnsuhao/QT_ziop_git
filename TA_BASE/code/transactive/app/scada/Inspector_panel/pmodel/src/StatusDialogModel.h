#pragma  once
#include "bus\generic_gui_pmod\src\TransActiveViewModel.h"
#include "InspectorPanelConsts.h"
//#include "EquipmentProxy.h"
#include "InspectorpanelAppController.h"
#include "AbstractEquipmentPModel.h"

namespace TA_Base_App
{
	class StatusDialogModel: public AbstractEquipmentPModel
	{
	public:
		StatusDialogModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		~StatusDialogModel(void);
	public:
		std::string getStatusInfo();
		void	updateDataNode(TA_Base_Bus::ScadaEntityUpdateType updateType);
		void	updateDataPoint(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType);
	private:
		const char * const		getQuality( TA_Base_Bus::EQualityStatus quality ) const;
	private:
		std::ostringstream				m_text;
	};
}