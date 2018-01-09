//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanManagerAppController.cpp $
// @author:  Bart Golab
// @version: $Revision: #7 $
//
// Last modification: $DateTime: 2013/03/20 14:16:56 $
// Last modified by:  $Author: qi.huang $
//
// <description>

#include "PlanManagerAppController.h"
#include "app/response_plans/plan_manager/pmodel/src/PlanManagerConsts.h"

#include "PlanManagerFrameModel.h"
#include "PlanTreeModel.h"
#include "ActivePlanModel.h"

#include "PlanService.h"
#include "bus/application_types/src/apptypes.h"

using namespace TA_Base_Bus;
using namespace TA_Base_Core;

namespace TA_Base_App
{
	/////////////////////////////////////////////////////////////////////////////
	// PlanManagerAppController

	PlanManagerAppController::PlanManagerAppController(ITransActiveApp * pWinApp, const char * applicationName)
		: AbstractGUIAppController(pWinApp, applicationName)
	{
		FUNCTION_ENTRY("PlanManagerAppController");
		FUNCTION_EXIT;
	}

	PlanManagerAppController::~PlanManagerAppController()
	{
		FUNCTION_ENTRY("~PlanManagerAppController");
		FUNCTION_EXIT;
	}

	/////////////////////////////////////////////////////////////////////////////
	// IGUIApplication operations

	unsigned long PlanManagerAppController::getApplicationType()
	{
		FUNCTION_ENTRY("getApplicationType");
		FUNCTION_EXIT;
		return PLAN_MANAGER_GUI_APPTYPE;
	}

	TA_Base_Bus::ITransActivePModel * TA_Base_App::PlanManagerAppController::createPModel(const std::string & pModelName)
	{
		//SEPTODO
		ITransActivePModel * pModel = NULL;
		if (pModelName == PMODEL_PLAN_MANAGER_MAINFRAME)
		{
			pModel = new PlanManagerFrameModel(this, pModelName.c_str());
		}
		else if (pModelName == PMODEL_ALLPLANTREEFRAME)
		{
			pModel = new PlanTreeModel(this, pModelName.c_str());
		}
		else if (pModelName == PMODEL_APPROVEDPLANTREEFRAME)
		{
			pModel = new PlanTreeModel(this, pModelName.c_str());
		}
		else if (pModelName == PMODEL_UNAPPROVEDPLANTREEFRAME)
		{
			pModel = new PlanTreeModel(this, pModelName.c_str());
		}
		else if (pModelName == PMODEL_ACTIVEPLANLISTFRAME)
		{
			pModel = new ActivePlanModel(this, pModelName.c_str());
		}

		return pModel;
	}

	void PlanManagerAppController::prepareAppControllerRun()
	{
		getWinApp()->getMainView()->postMessage(TA_MSG_INIT_CORBA, 0, 0);
		PlanService::getInstance().setupGUIEntity();

		//QTTODO:
	}

	void PlanManagerAppController::processWorkflowDuringAppInit()
	{
		//SEPTODO: RunParam settings
	}

	// TD 11846 Fix: Changed setPosition's signature to follow AbstractGUIApplication's setPosition's signature.
	void PlanManagerAppController::setMainViewPosition(const unsigned long posFlag, const unsigned long alignFlag, const RECT& objectDim, const RECT& boundaryDim)
	{
		FUNCTION_ENTRY("setPosition");

		//AbstractDocumentGUI::setPosition(posFlag, alignFlag, objectDim, boundaryDim);

		// The Plan Manager window has been moved. A preceeding ExecPlanId parameter update can
		// now be forwarded for processing. The resulting Plan Controller should display on top
		// of Plan Manager.
		PlanService::getInstance().setMainViewPosition();

		FUNCTION_EXIT;
	}

	void PlanManagerAppController::entityChanged(const std::vector<std::string>& changes)
	{
		FUNCTION_ENTRY("entityChanged");

		PlanService::getInstance().entityChanged(getEntity());

		FUNCTION_EXIT;
	}

	///////////////////////////////////////////////////////////////////////////////
	//// RunParamUser operations

	void PlanManagerAppController::onRunParamChange(const std::string& name, const std::string& value)
	{
		FUNCTION_ENTRY("onRunParamChange");

		PlanService::getInstance().onRunParamChange(name, value);

		FUNCTION_EXIT;
	}
}