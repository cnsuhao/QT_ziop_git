//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanManagerApp.cpp $
* @author:  Bart Golab
* @version: $Revision: #3 $
*
* Last modification: $DateTime: 2016/11/21 13:03:08 $
* Last modified by:  $Author: hoa.le $
*
* Application class for Plan Manager.
*/

#include "bus/generic_gui_view/src/TransActiveMessage.h"

#include "app/response_plans/plan_manager/src/PlanManagerCommonDefs.h"

#include "app/response_plans/plan_manager/pmodel/src/PlanManagerAppController.h"

#include "app/response_plans/plan_manager/src/PlanManagerApp.h"
#include "app/response_plans/plan_manager/src/PlanViewFactory.h"

using TA_Base_Bus::TransActiveWinApp;

using namespace TA_Base_App;

/////////////////////////////////////////////////////////////////////////////
// PlanManagerApp construction/destruction

PlanManagerApp::PlanManagerApp(int argc, char *argv[])
: TransActiveWinApp(argc, argv, TA_Base_Bus::MVT_MDI_FRAME, true)
{
	FUNCTION_ENTRY("PlanManagerApp");

	// Create factory object to manage view objects
	PlanViewFactory* pVf = new PlanViewFactory(this);
	this->getViewManager()->setViewFactory(pVf);

	FUNCTION_EXIT;
}

/////////////////////////////////////////////////////////////////////////////
// PlanManagerApp initialization

TA_Base_Bus::IApplicationController * TA_Base_App::PlanManagerApp::createApplicationController()
{
	return new TA_Base_App::PlanManagerAppController(this, "Plan Manager");
}

TA_Base_Bus::ITransActiveView* PlanManagerApp::createMainView()
{
	return this->getViewManager()->getView(VIEWTYPE_PLANMANAGER, "main");
}