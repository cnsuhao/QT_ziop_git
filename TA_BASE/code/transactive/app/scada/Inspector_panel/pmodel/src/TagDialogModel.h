#pragma  once
#include "bus\generic_gui_pmod\src\TransActiveViewModel.h"
#include "InspectorPanelConsts.h"
//#include "EquipmentProxy.h"
#include "AbstractEquipmentPModel.h"


namespace TA_Base_App
{
	/*enum InhibitType
	{
		scanInhibit				= 0,
		alarmInhibit			= 1,
		mmsInhibit				= 2,
		controlInhibit			= 3
	};*/

	class TagDialogModel:public AbstractEquipmentPModel
	{
	public:
		TagDialogModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		~TagDialogModel(void);
	public:
		bool		setTagState(const ETagState tagState, const EOutputInhibitState outputInhibitState,	const EInputInhibitState inputInhibitState,
								const std::string& ptwId, const std::string& tagComment);
		bool	removeTagState();

		TA_Base_Bus::ETagState		getDataNodeTaggingState();
		TA_Base_Bus::InhibitState   getDataNodeTagInhibitState();
		std::string					getDataNodePtwId();
		std::string					getDataNodeTagComment();
		bool						isProfileAccessAllowed(bool buttonChecked, InhibitType type);
		bool						isActionPermitted(AccessControlActions action);

		std::string					getDataNodeAssetName();
		std::string					getDataNodeDescription();
		bool						hasInformationPoint();
		bool						hasControlPoint();
	protected:
		virtual void				onDataNodeProxyUpdate(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType);
		
	private:
	};
}