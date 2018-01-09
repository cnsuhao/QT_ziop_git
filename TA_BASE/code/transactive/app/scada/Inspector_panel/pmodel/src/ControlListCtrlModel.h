#pragma once
#include "bus\generic_gui_pmod\src\TransActiveViewModel.h"
//#include "equipmentproxy.h"
#include "InspectorpanelAppController.h"
#include "AbstractEquipmentPModel.h"
namespace TA_Base_App
{
	struct ControlListCtrlItem
	{
		std::string		description;
		std::string		type;
		std::string		state;
		std::string		newValue;
		std::string		units;

	};

	class ControlListCtrlModel:public AbstractEquipmentPModel
	{
	public:
		ControlListCtrlModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~ControlListCtrlModel(void);
	public:
		ControlItem			getControlListCtrlItem(unsigned int iPoint);
		std::vector<ControlItem>		getAllControlDisplayItem();
		int					updateControlState(std::vector<std::string>& parameters);
		bool				isControlPermit(unsigned int iPoint);
		void				setValue(unsigned int iPoint, const std::string& strValue);
		bool				isOkToSendControl(unsigned int iPoint, const std::string& strValue);
		bool				isValid(unsigned int iPoint);

		std::string			getEquipmentDescription();

		EPModelCellType		getPMCellTypeFromPoint(unsigned int iPoint);
		std::vector<std::string> getCellValues(unsigned int iPoint);
		//void				updateControlState();
	protected:
		virtual	void		onInitStatusUpdate (EquipmentInitStatus status);
		virtual void		onDataPointProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType);
		virtual void		onPertmissionUpdate();

	private:
		std::string			addControlStateToStatusLine ( TA_Base_Bus::DataPointProxySmartPtr & datapoint, TA_Base_Bus::EDataPointControlState state ) /*wenching++ (TD14179) */;
		std::string			getControlStatusString ( TA_Base_Bus::EDataPointControlState state );
		
//		void				postStatusLine(const std::string& statusLine);
		
	private:
		std::map<unsigned int, bool>	m_setValueActionAllowed;
	};
}