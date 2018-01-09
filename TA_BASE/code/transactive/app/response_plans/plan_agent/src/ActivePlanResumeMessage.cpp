/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/ActivePlanResumeMessage.cpp $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  Implementation of .
  */

#include "app/response_plans/plan_agent/src/ActivePlan.h"
#include "app/response_plans/plan_agent/src/ActivePlanResumeMessage.h"

namespace TA_Base_App
{
	ActivePlanResumeMessage::ActivePlanResumeMessage()
	{
	}
	
	void ActivePlanResumeMessage::execute( ActivePlan& activePlan ) const
	{
		activePlan.resume();
	}

} // TA_Base_App
