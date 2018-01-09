#include "InspectorpaneldlgModel.h"
#include "InspectorPanelConsts.h"
#include "bus/scada/proxy_library/src/DataPointProxySmartPtr.h"
#include "bus/scada/proxy_library/src/IDataPointProxy.h"
#include "bus/scada/proxy_library/src/DataNodeProxySmartPtr.h"
#include "bus/scada/proxy_library/src/IDataNodeProxy.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/RunParams.h"
#include "core/exceptions/src/ScadaProxyException.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"
#include "AccessControlHelper.h"


using TA_Base_Core::RunParams;
using TA_Base_Core::DebugUtil;
using namespace TA_Base_App;
using namespace TA_Base_Bus;
using namespace TA_Base_Core;

namespace TA_Base_App
{

	InspectorPanelDlgModel::InspectorPanelDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		:AbstractEquipmentPModel(appController, pmodelName)
	{
		m_displayLogic->unregisterObserver(this, DOMAIN_DATA_CHANGE_ENTITY_DATA);
		m_displayLogic->unregisterObserver(this, DOMAIN_DATA_CHANGE_PERTMITTION);
	}

	InspectorPanelDlgModel::~InspectorPanelDlgModel(void)
	{
		m_displayLogic->unregisterObserver(this);
	}

	std::string InspectorPanelDlgModel::getLabel()
	{
		return m_displayLogic->getDataNodeLabel();
	}


	std::string InspectorPanelDlgModel::getTitle()
	{
		std::string asset = "";
		std::string title = ""; //TA_Base_Core::RunParams::getInstance().get( RPARAM_APPNAME );

		asset = m_displayLogic->getDataNodeAssetName();
		if (!asset.empty())
		{
			title += " - ";
			title += asset;
		}
		return title;
	}

	bool	InspectorPanelDlgModel::haveAllDataPointsConfigured()
	{
		return m_displayLogic->haveAllDataPointsConfigured();
	}


	void InspectorPanelDlgModel::displayHelp()
	{
		TA_Base_Bus::HelpLauncher::getInstance().displayHelp(true);
	}

	std::string		InspectorPanelDlgModel::getRequestScreen()
	{
		return TA_Base_Core::RunParams::getInstance().get(SCREEN_RPARAM);
	}


	void InspectorPanelDlgModel::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{
		TA_ASSERT(_stricmp(updateType, DOMAIN_DATA_CHANGE_INIT_STATUS) == 0,"Invalid updateType");
		TA_ASSERT(args != NULL,"Invalid Args");

 		this->notifyViews(updateType, args);
	}
}


