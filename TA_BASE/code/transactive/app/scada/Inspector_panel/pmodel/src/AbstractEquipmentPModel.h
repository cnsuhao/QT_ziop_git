#pragma  once


#include "inspectorpanelconsts.h"
#include "bus\generic_gui_pmod\src\TransActiveViewModel.h"
#include "InspectorpanelAppController.h"
#include "Equipment.h"
//#include "EquipmentProxy.h"
#include "DisplayLogic.h"
#include "OutputLogic.h"

namespace TA_Base_App
{
	typedef enum 
	{
		PMCT_NOTEDITABLE,
		PMCT_TEXT,
		PMCT_NUMBER,
		PMCT_SELECTION,
		PMCT_DECISION,
		PMCT_CHECKBOX,
		PMCT_CHECKBOX_DISABLED,
		PMCT_DATE,
		PMCT_TIME,
		PMCT_MASK,
		PMCT_BUTTON
	} EPModelCellType;


	class AbstractEquipmentPModel: public TA_Base_Bus::AbstractPModel
	{
	public:
		AbstractEquipmentPModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~AbstractEquipmentPModel();
	public:

		unsigned long			getDataNodeEntityKey();
		EDataPointStatusColor	getDataPointStatusColor(unsigned int iPoint);
		std::string				getDisplayValueWithUnit(unsigned int iPoint);
		std::string				getChildDataPointDescription(UINT iPoint);
		void					postStatusLine(const std::string& statusLine);
		void					postStatusLine(int codeId, const std::vector<std::string>	&parameters);
		void			initialize(DisplayLogic* displayLogic, OutputLogic* outputLogic);
		void			cleanUp();
	private:
		//
		virtual void notifyUpdate(const char * updateType,TA_Base_Bus::EventArgs * args);
		virtual void onDataNodeProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType);
		virtual void onDataPointProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType);
		virtual void onInitStatusUpdate (EquipmentInitStatus status);
		virtual void onPertmissionUpdate();
		



	protected:
		InspectorpanelAppController*	m_pRealAppController;
		//CEquipment*						m_pEquipment;
		//EquipmentProxy*					m_pEquipmentProxy;

		DisplayLogic*					m_displayLogic;
		OutputLogic*					m_outputLogic;
	};
}