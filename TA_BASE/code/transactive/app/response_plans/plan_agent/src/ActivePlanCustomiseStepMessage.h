#ifndef ACTIVEPLANCUSTOMISESTEPMESSAGE_H_INCLUDED
#define ACTIVEPLANCUSTOMISESTEPMESSAGE_H_INCLUDED

/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/ActivePlanCustomiseStepMessage.h $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  <description>
  */

#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"
#include "app/response_plans/plan_agent/src/IActivePlanMessage.h"

namespace TA_Base_App
{
    class ActivePlan;

	//Class declaration
	class ActivePlanCustomiseStepMessage : public IActivePlanMessage
	{
	// Public methods
	public:
        ActivePlanCustomiseStepMessage( const TA_Base_Core::StepCustomisableDetail& detail );
        ActivePlanCustomiseStepMessage( const TA_Base_Core::CustomisationHistoryUpdate& historyUpdate );

		virtual void execute( ActivePlan& activePlan ) const;

	private:
		ActivePlanCustomiseStepMessage& operator=( const ActivePlanCustomiseStepMessage& );
		ActivePlanCustomiseStepMessage( const ActivePlanCustomiseStepMessage& );

        const TA_Base_Core::StepCustomisableDetail m_detail;
        const TA_Base_Core::CustomisationHistoryUpdate m_historyUpdate;
        bool m_bUpdateHistory;
	}; // ActivePlanCustomiseStepMessage
}; // TA_Base_App

#endif // ACTIVEPLANCUSTOMISESTEPMESSAGE_H_INCLUDED
