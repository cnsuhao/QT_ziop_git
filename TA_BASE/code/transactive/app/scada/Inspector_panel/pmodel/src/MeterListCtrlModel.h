#pragma  once

//#include "EquipmentProxy.h"
#include "bus\generic_gui_pmod\src\TransActiveViewModel.h"
#include "InspectorpanelAppController.h"
#include "AbstractEquipmentPModel.h"

namespace TA_Base_App
{
	struct MeterListCtrlItem
	{
		std::string		description;
		std::string		type;
		std::string		currentValue;
	};


	class MeterListCtrlModel:public AbstractEquipmentPModel
	{
	public:
		MeterListCtrlModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~MeterListCtrlModel(void);
	public:
		MeterItem				getMeterListCtrlItem(unsigned int iPoint);
		std::vector<MeterItem>	getAllMeterDisplayItem();
		bool					setMeteredCorrectionValue(unsigned int iPoint,double correction);
		double					getChildValueAsDouble(unsigned int iPoint);
		bool					checkValueAdjustable(unsigned int iPoint, const std::string& newValue,double& correction,double& adjustedValue);
		std::string				getChildDataPointDescription(unsigned int iPoint);
		std::string				getChildValueAsString(UINT iPoint);
		std::string				getDataNodeAssetName();
		std::string				getDataNodeDescription();
		std::string				getDataNodeLabel();
		bool					isMeterAdjustPermitted(unsigned int iPoint);

	protected:
		virtual void		onDataPointProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType);
		bool				isValid(unsigned int iPoint);
		
	private:
	};
}