#include "InspectorpanelAppController.h"
#include "maintabcontrolmodel.h"
#include "core\utilities\src\RunParams.h"

namespace TA_Base_App
{
	MainTabControlModel::MainTabControlModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		:AbstractPModel(appController,pmodelName)
	{
		m_pRealAppController = dynamic_cast<InspectorpanelAppController*> (appController);
	}


	MainTabControlModel::~MainTabControlModel(void)
	{

	}

	std::vector<ETabNumbers> MainTabControlModel::getConfigureTabs()
	{
		std::vector<ETabNumbers> rtnVector;

		rtnVector.push_back(InformationTab);
		rtnVector.push_back(ControlTab);
		rtnVector.push_back(NotesTab);
		rtnVector.push_back(TagTab);
		rtnVector.push_back(InhibitTab);
		rtnVector.push_back(OverrideTab);
		rtnVector.push_back(LimitsTab);
		rtnVector.push_back(MeterTab);

		if ( TA_Base_Core::RunParams::getInstance().isSet("StatusTab") )
		{
			rtnVector.push_back(StatusTab);
		}
		return rtnVector;
	}
};

