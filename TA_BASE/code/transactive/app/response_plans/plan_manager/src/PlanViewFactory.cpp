/**
* The source code in this file is the property of
* ST Engineering Electronics (LSG) and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanViewFactory.cpp $
* @author:  Hoa Le
* @version: $Revision: #1 $
*
* Last modification: $DateTime: 2016/11/21 13:03:08 $
* Last modified by:  $Author: hoa.le $
*
* This class is for creating factory of Plan Manager view objects.
*/

#include "app/response_plans/plan_manager/src/PlanViewFactory.h"

#include "app/response_plans/plan_manager/src/PlanManagerFrame.h"
#include "app/response_plans/plan_manager/src/PlanTreeFrame.h"
#include "app/response_plans/plan_manager/src/ActivePlanListFrame.h"

#include "app/response_plans/plan_manager/src/PlanManagerCommonDefs.h"

namespace TA_Base_App
{
	PlanViewFactory::PlanViewFactory(TA_Base_Bus::ITransActiveApp* winApp) : m_winApp(winApp)
	{
	}

	TA_Base_Bus::ITransActiveView * PlanViewFactory::createView(const char * viewType)
	{
		TA_Base_Bus::ITransActiveView * viewRetVal = 0;

		if (viewType == VIEWTYPE_PLANMANAGER)
		{
			viewRetVal = new PlanManagerFrame(m_winApp);
			viewRetVal->setMainView(true);
		}
		else if (viewType == VIEWTYPE_ALLPLANTREEFRAME)
		{
			viewRetVal = new PlanTreeFrame(m_winApp, VIEWTYPE_ALLPLANTREEFRAME);
		}
		else if (viewType == VIEWTYPE_APPROVEDPLANTREEFRAME)
		{
			viewRetVal = new PlanTreeFrame(m_winApp, VIEWTYPE_APPROVEDPLANTREEFRAME);
		}
		else if (viewType == VIEWTYPE_UNAPPROVEDPLANTREEFRAME)
		{
			viewRetVal = new PlanTreeFrame(m_winApp, VIEWTYPE_UNAPPROVEDPLANTREEFRAME);
		}
		else if (viewType == VIEWTYPE_ACTIVEPLANLISTFRAME)
		{
			viewRetVal = new ActivePlanListFrame(m_winApp, VIEWTYPE_ACTIVEPLANLISTFRAME);
		}

		return viewRetVal;
	}
}