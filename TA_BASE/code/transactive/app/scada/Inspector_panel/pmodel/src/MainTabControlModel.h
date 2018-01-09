#pragma  once
#include "bus\generic_gui_pmod\src\TransActiveViewModel.h"
#include "InspectorPanelConsts.h"


namespace TA_Base_App
{
	class MainTabControlModel: public TA_Base_Bus::AbstractPModel
	{
	public:
		MainTabControlModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		~MainTabControlModel(void);
	public:
		std::vector<ETabNumbers>		getConfigureTabs();
	public:

	private:
		InspectorpanelAppController* m_pRealAppController;
	};
}