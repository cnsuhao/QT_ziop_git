/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/ActivePlanDecisionStepMessage.cpp $
  *  @author:  Mahaveer Pareek
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  <description>
  */
#include "app/response_plans/common/src/ProjectDefinition.h"

#if defined ( STEPTYPE_DECISION )

#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"
#include "app/response_plans/plan_agent/src/ActivePlan.h"
#include "app/response_plans/plan_agent/src/ActiveDecisionStep.h"
#include "app/response_plans/plan_agent/src/ActivePlanBulletinBoard.h"
#include "app/response_plans/plan_agent/src/ActivePlanDecisionStepMessage.h"
#include "app/response_plans/common/src/StringConstants.h"

namespace TA_Base_App
{
    ActivePlanDecisionStepMessage::ActivePlanDecisionStepMessage(const int status) : 
    m_status (status)
    {
    }

    void ActivePlanDecisionStepMessage::execute ( ActivePlan& activePlan ) const
    {
        ActiveStep* activeStep = activePlan.getCurrentStep();        
        TA_Base_Core::StepNumber stepNumber = activeStep->getStepNumber();

        ActiveDecisionStep* decisionStep = dynamic_cast <ActiveDecisionStep *>(activeStep);

        if ( decisionStep == NULL )
        {
            //Step failed
            activePlan.setInteractiveStepFailed();
            activeStep->processStepExecutionFailure();
            return;
        }

		//DP-changes
	    //TA_Base_Core::StepNumber nextStepNumber = decisionStep->getNextDecisionStep(m_status);
 		switch(m_status)
		{
		case TA_Base_Core::PSCS_PAUSE_RESPONSE:
			{				
 				//pause active plan instance
				// send active plan update, indicating the plan has paused
				ActivePlanBulletinBoard::instance()->update( activePlan.getPlan(), stepNumber, TA_Base_Core::PAUSED_ACTIVE_STEP_STATE, RemarkPlanPausedByOperator);
			}
			break;
		case TA_Base_Core::PSCS_TIMEOUT:
			{
				ActivePlanBulletinBoard::instance()->update( activePlan.getPlan(), stepNumber, TA_Base_Core::PAUSED_ACTIVE_STEP_STATE, RemarkPlanTimeout);				 
			}
			break;
		default:
			{
                activePlan.resume();

				TA_Base_Core::StepNumber nextStepNumber = decisionStep->getNextDecisionStep(m_status);
				if ( nextStepNumber == 0)
				{
					//Step failed
 					activePlan.setInteractiveStepFailed();
                    activeStep->processStepExecutionFailure(RemarkPlanPaused);
					return;
				}

				ActivePlanBulletinBoard::instance()->update( activePlan.getPlan(), stepNumber, TA_Base_Core::EXECUTED_ACTIVE_STEP_STATE, RemarkPlanStepExecuted );

				activePlan.setNextStep(nextStepNumber);
				activePlan.setInteractiveStep(false);
				break;
			}
		}	
		//++lizettejl
    }
}

#endif //STEPTYPE_DECISION
