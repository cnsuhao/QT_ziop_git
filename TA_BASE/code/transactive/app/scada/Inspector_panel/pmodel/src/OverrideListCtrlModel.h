#pragma  once

//#include "EquipmentProxy.h"
#include "bus\generic_gui_pmod\src\TransActiveViewModel.h"
#include "InspectorpanelAppController.h"
#include "AbstractEquipmentPModel.h"

namespace TA_Base_App
{
	struct OverrideListCtrlItem
	{
		std::string		description;
		std::string		type;
		bool			isOverride;
		std::string		overrideValue;
		std::string		currentValue;

		OverrideListCtrlItem();
	};


	class OverrideListCtrlModel:public AbstractEquipmentPModel
	{
	public:
		OverrideListCtrlModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~OverrideListCtrlModel(void);
	public:
		OverrideItem			getOverrideItem(unsigned int iPoint);
		std::vector<OverrideItem> getAllOverrideItem();
		bool					isNotWritableDp(unsigned int iPoint);
		bool					isDpDiaplayable(unsigned int iPoint);
		bool					isOverrideColumnEnable(unsigned int iPoint);
		std::string				getDataPointDescription(unsigned int iPoint);

		bool					isOkToOverride(unsigned int iPoint);
		bool					isOkToRemoveOverride(unsigned int iPoint);

		bool					setManuallyOverrideValue (unsigned int iPoint, std::string& strValue);
		bool					removeOverride(unsigned int iPoint);
		bool					isChildOverrideEnabled(unsigned int iPoint);
		std::string				getEquipmentDescription();
		EPModelCellType			getCellTypeFromPoint(unsigned int iPoint);
		std::vector<std::string> getCellValues(unsigned int iPoint);



	protected:
		virtual void		onDataPointProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType);
		bool				isValid(unsigned int iPoint);
	private:
	};
}