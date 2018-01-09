#ifndef ACTIVE_PLAN_PAUSE_FOR_OPERATOR_MESSAGE_HEADER_INCLUDED
#define ACTIVE_PLAN_PAUSE_FOR_OPERATOR_MESSAGE_HEADER_INCLUDED

/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/ActivePlanPauseForOperatorMessage.h $
  *  @author:  Mahaveer Pareek
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  <description>
  */

#include "app/response_plans/plan_agent/src/IActivePlanMessage.h"

namespace TA_Base_App
{
    class ActivePlan;

	//Class declaration
	class ActivePlanPauseForOperatorMessage : public IActivePlanMessage
	{
	// Public methods
	public:
		ActivePlanPauseForOperatorMessage(){}

		virtual void execute( ActivePlan& activePlan ) const;
	
	private:
		ActivePlanPauseForOperatorMessage& operator=( const ActivePlanPauseForOperatorMessage& );
		ActivePlanPauseForOperatorMessage( const ActivePlanPauseForOperatorMessage& );

	}; // ActivePlanPauseMessage
}; // TA_Base_App

#endif // ACTIVE_PLAN_PAUSE_FOR_OPERATOR_MESSAGE_HEADER_INCLUDED
