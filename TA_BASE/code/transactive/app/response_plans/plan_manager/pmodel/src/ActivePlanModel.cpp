/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/ActivePlanModel.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #3 $
  *
  * Last modification: $DateTime: 2016/11/21 13:03:08 $
  * Last modified by:  $Author: hoa.le $
  *
  * <description>
  *
  **/

//#include "StdAfx.h"
//#include "Resource.h"
#include "ActivePlanModel.h"
#include "PlanAgentUpdateProcessor.h"
#include "OperatorRights.h"

#include "PlanService.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;


/////////////////////////////////////////////////////////////////////////////
// ActivePlanModel

ActivePlanModel::ActivePlanModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
: TA_Base_Bus::AbstractPModel(appController, pmodelName)
{
    FUNCTION_ENTRY( "ActivePlanModel" );
    FUNCTION_EXIT;
}


ActivePlanModel::~ActivePlanModel()
{
	FUNCTION_ENTRY("~ActivePlanModel");
	FUNCTION_EXIT;
}


void ActivePlanModel::registerSettings()
{
    FUNCTION_ENTRY( "registerSettings" );

    // Register for active plan updates.
	registerPlanSettings();

    FUNCTION_EXIT;
}


void ActivePlanModel::deregisterSettings()
{
    FUNCTION_ENTRY( "deregisterSettings" );

	deregisterPlanSettings();

    FUNCTION_EXIT;
}


