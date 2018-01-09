#include "PlanService.h"

#include "core/synchronisation/src/ThreadGuard.h"
#include "core/utilities/src/DebugUtil.h"

//#include "core\data_access_interface\src\ProfileAccessFactory.h"
//#include "app/response_plans/plan_manager/src/ProfileConfigAccess.h"

#include "core\exceptions\src\DatabaseException.h"
#include "core\exceptions\src\DataException.h"

//#include "FloatingWindowThreadRegister.h"

#include "TreeNodeFactory.h"
#include "OperatorRights.h"
#include "PlanAgentAccess.h"
#include "PlanAgentUpdateProcessor.h"

#include "bus/response_plans/active_plans_display/pmodel/src/PlanAgentLocationAccess.h"
#include "core/data_access_interface/entity_access/src/PlanManagerEntityData.h"
#include "app/response_plans/plan_manager/pmodel/src/ProfileConfigAccess.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"
#include "core/exceptions/src/GenericGUIException.h"
#include "core/exceptions/src/UserSettingsException.h"
#include "bus/user_settings/src/SettingsMgr.h"

//#include "PlanBrowserService.h"
//#include "PlanTreeModel.h"


using TA_Base_Core::DebugUtil;
using TA_Base_Core::RunParams;
using TA_Base_Bus::SettingsMgr;

using namespace TA_Base_Bus;
using namespace TA_Base_Core;

namespace TA_Base_App
{
	// Define static.
	PlanService* PlanService::m_pInstance = NULL;
	TA_Base_Core::ReEntrantThreadLockable PlanService::m_singletonLock;

	PlanService& PlanService::getInstance()
	{
		// Make this threadsafe.
		TA_Base_Core::ThreadGuard guard(m_singletonLock);

		if (m_pInstance == NULL)
		{
			m_pInstance = new PlanService();
			if (m_pInstance == NULL)
			{
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugFatal,
					"New failed to allocate AdminManager");
				//exit(1);
			}
		}

		return *m_pInstance;
	}

	PlanService::PlanService()
		: m_planManagerServant(NULL)
		, m_execPlanIdChanged(false)
		, m_viewPlanIdChanged(false)
		, m_assocAlarmIdChanged(false)
	{
		FUNCTION_ENTRY("Constructor");
		FUNCTION_EXIT;
	}

	PlanService::~PlanService()
	{
		FUNCTION_ENTRY("Destructor");
		if (m_planManagerServant != NULL)
		{
			m_planManagerServant->deactivateAndDeleteServant();
			delete m_planManagerServant;
			m_planManagerServant = NULL;
		}
		FUNCTION_EXIT;
	}

	void PlanService::registerRunParam()
	{
		// Register for --view-plan-id runtime parameter updates."ViewPlanId"
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, RPARAM_VIEWPLANID);
		// Register for --exec-plan-id runtime parameter updates.
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, RPARAM_EXECPLANID);
		// Register for --session-id runtime parameter updates (TD 10849).
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, RPARAM_ASSOC_ALARMID);
	}

	std::string PlanService::getRunParam(const char* name)
	{
		return RunParams::getInstance().get(name);
	}

	void PlanService::setRunRaram(const char* name, const char* value)
	{
		RunParams::getInstance().set(name, value);
	}

	void PlanService::onRunParamChange(const std::string &name, const std::string &value)
	{
		FUNCTION_ENTRY("onRunParamChange");

		if (name == RPARAM_EXECPLANID)
		{
			// ExecPlanId update ultimately results in a Plan Controller being opened. However,
			// this needs to be postponed until the Plan Manager window has been positioned
			// by the Control Station so that Plan Controller displays on top of Plan Manager.
			// The ExecPlanId change is processed in setPosition().

			m_execPlanIdChanged = true;
		}
		else if (name == RPARAM_VIEWPLANID)
		{
			m_viewPlanIdChanged = true;
		}
		else if (name == RPARAM_SESSIONID)
		{
			// SessionId update will cause the active plans to fail if they require interactive
			// steps or if a plan is paused (TD 10849).
			notifyObservers(PMODEL_DATA_CHANGE_SESSION_ID, NULL);
		}
		else if (name == RPARAM_ASSOC_ALARMID)
		{
			// as per ExecPlanId , the AssocAlarmId will result in Plan Controller being opened
			// which needs to be postponed
			// AssocAlarmId is processed in setPosition

			m_assocAlarmIdChanged = true;
		}

		FUNCTION_EXIT;
	}

	void PlanService::setMainViewPosition()
	{
		FUNCTION_ENTRY("setPosition");

		//AbstractDocumentGUI::setPosition(posFlag, alignFlag, objectDim, boundaryDim);

		// The Plan Manager window has been moved. A preceeding ExecPlanId parameter update can
		// now be forwarded for processing. The resulting Plan Controller should display on top
		// of Plan Manager.
		if (m_execPlanIdChanged)
		{
			RunParams::getInstance().set(RPARAM_FWDEXECPLANID, RunParams::getInstance().get(RPARAM_EXECPLANID).c_str());

			m_execPlanIdChanged = false;
		}

		if (m_viewPlanIdChanged)
		{
			RunParams::getInstance().set(RPARAM_FWDVIEWPLANID, RunParams::getInstance().get(RPARAM_VIEWPLANID).c_str());

			m_viewPlanIdChanged = false;
		}

		// similar for Assoc Alarm
		if (m_assocAlarmIdChanged)
		{
			RunParams::getInstance().set(RPARAM_FWDASSOCALARMID, RunParams::getInstance().get(RPARAM_ASSOC_ALARMID).c_str());
			m_assocAlarmIdChanged = false;
		}

		FUNCTION_EXIT;
	}


	void PlanService::setupGUIEntity()
	{
		FUNCTION_ENTRY("setupGUIEntity");

		TA_Base_Core::IEntityData* planManagerEntityData =
			TA_Base_Core::EntityAccessFactory::getInstance().getEntity("PlanManager");
		TA_Base_Core::PlanManagerEntityData *pmEntityData = dynamic_cast<TA_Base_Core::PlanManagerEntityData *>(planManagerEntityData);
		if (!pmEntityData)
		{
			TA_THROW(GenericGUIException("Entity configuration is not of type Plan Manager", GenericGUIException::ENTITY_CONFIGURATION_INVALID));
		}

		// Set the root directory of the flowchart as a run param so that we could access it
		// in other parts of the app
		RunParams::getInstance().set(RPARAM_FLOWCHARTDIR, pmEntityData->getFlowchartDirectory().c_str());
		RunParams::getInstance().set(RPARAM_REPORTTEMPLATE, pmEntityData->getReportTemplate().c_str());
		RunParams::getInstance().set(RPARAM_MAXCATEGORY, pmEntityData->getMaxCategoryDepth().c_str());

		//SEPTODODEL: Already set AbstractGUIAppController::processApplicationName( from AbstractGUIAppController::initController(
		//TA_Base_Core::RunParams::getInstance().set(RPARAM_APPNAME, "Plan Manager"); 

		FUNCTION_EXIT;
	}

	void PlanService::entityChanged(IEntityData* guiEntity)
	{
		FUNCTION_ENTRY("entityChanged");

		PlanManagerEntityData *pmEntityData = dynamic_cast<PlanManagerEntityData *> (guiEntity);
		TA_ASSERT(pmEntityData != NULL, "The GUI Entity cannot be converted into Plan Manager entity");
		RunParams::getInstance().set(RPARAM_FLOWCHARTDIR, pmEntityData->getFlowchartDirectory().c_str());

		// TD 13265
		RunParams::getInstance().set(RPARAM_REPORTTEMPLATE, pmEntityData->getReportTemplate().c_str());

		FUNCTION_EXIT;
	}

	std::string PlanService::checkLocations()
	{
		delete m_planManagerServant;
		int iSize = activatePManagerServant();
		std::string strError;
		for (int i = 0; i < iSize; i++)
		{
			strError += registerStationAgent(i);
			strError += " ";
		}

		return strError;
	}

	bool PlanService::isStepServantActive()
	{
		return m_planManagerServant == NULL ? false : true;
	}

	int PlanService::activatePManagerServant()
	{
		m_planManagerServant = new PlanManagerCorbaDefImpl();
		return m_planManagerServant->toActivate();
		//return 0; //tmp
	}

	std::string PlanService::registerStationAgent(int iIndex)
	{
		std::string strLocation;
		//m_planManagerServant->init(iIndex, strLocation);
		return strLocation;
	}

	std::string PlanService::getDefaultPlanAgentLocationName()
	{
		return PlanAgentLocationAccess::getInstance().getDefaultPlanAgentLocationName();
	}

	void PlanService::cleanUp()
	{
		//FloatingWindowThreadRegister::destroyInstance();
		TreeNodeFactory::destroyInstance();
		PlanAgentAccess::destroyInstance();
		PlanAgentUpdateProcessor::destroyInstance();
		OperatorRights::destroyInstance();
		ProfileConfigAccess::close();
	}

}