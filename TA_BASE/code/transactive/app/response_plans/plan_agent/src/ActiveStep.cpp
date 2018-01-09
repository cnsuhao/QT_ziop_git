/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/ActiveStep.cpp $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  Implementation of .
  */

#include <sstream>

#include "core/utilities/src/DebugUtil.h"
#include "core/exceptions/src/PlanAgentException.h"
#include "app/response_plans/plan_agent/src/AAMessageSender.h"
#include "app/response_plans/plan_agent/src/IActivePlanFlow.h"
#include "app/response_plans/plan_agent/src/ActivePlanBulletinBoard.h"
#include "app/response_plans/plan_agent/src/ActivePlanDelayedState.h"
#include "app/response_plans/plan_agent/src/ActivePlanExecutingState.h"
#include "app/response_plans/plan_agent/src/ActiveStep.h"
#include "app/response_plans/common/src/StringConstants.h"

// namespace alias for Corba structures/types
 
namespace TA_Base_App
{

	ActiveStep::ActiveStep( const TA_Base_Core::StepDetail& stepDetail, IActivePlanFlow& flowControl ) :
    m_detail( stepDetail ),
    m_flowControl( flowControl ),
    m_delayStartTime( 0u ), 
    m_delayCompleted( 0u ), 
    m_bGroupStep( false ),
	m_bSubStep( false )
	{
	}

    ActiveStep::~ActiveStep()
    {

    }

	void ActiveStep::customise( const TA_Base_Core::StepCustomisableDetail& newDetail )
	{
		std::ostringstream message;

		message << "ActiveStep::customise() : Plan[" << m_flowControl.getPlan().plan
			    << ":" << m_flowControl.getPlan().instance << "]" << " Step Number:" << getStepNumber();

		if ( !m_detail.skippable && newDetail.skip )
		{
			message << " Error: Skippable is not true so step cannot be skipped.";
			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, message.str().c_str() );
            TA_THROW ( TA_Base_Core::PlanAgentException (message.str(), TA_Base_Core::PAE_CANNOT_SKIP_UNSKIPPABLE));
		}

		m_detail.skip  = newDetail.skip;
		m_detail.delay = newDetail.delay;
        m_detail.ignoreFailure = newDetail.ignoreFailure;
		m_detail.nowait = newDetail.nowait;
	}

	TA_Base_Core::StepNumber ActiveStep::getStepNumber( void ) const
	{
		return m_detail.position;
	}

	void ActiveStep::load( bool fNeedUpdateBulletinBoard )
	{
        if ( m_remark.empty() )
        {
			m_remark = RemarkPlanStarted;
        }

		if ( fNeedUpdateBulletinBoard )
		{
			ActivePlanBulletinBoard::instance()->update( m_flowControl.getPlan(), getStepNumber(), m_detail.name.in(), m_remark );
		}
	}

    void ActiveStep::processStepExecutionFailure( const std::string& failureReason )
    {
        if ( !failureReason.empty() )
        {
            m_remark = failureReason;
        }

        const std::string strPlanName = ActivePlanBulletinBoard::instance()->getActivePlanDetail(m_flowControl.getPlan()).path.in();
        const unsigned long ulInstanceNum = m_flowControl.getPlan().instance;
        const unsigned long ulStepNum = getStepNumber();

		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Step %d of Plan %s instance %d failed - %s", 
            ulStepNum, strPlanName.c_str(), ulInstanceNum, m_remark.c_str() );

        std::string alarmId = "";
        std::string strSession = ActivePlanBulletinBoard::instance()->getSession(m_flowControl.getPlan());

        bool isGRCPlan = ActivePlanBulletinBoard::instance()->isGRCPlan(m_flowControl.getPlan());
        //GRC plans
        if (isGRCPlan)
        {
            if (ActivePlanBulletinBoard::instance()->isManualPlan(m_flowControl.getPlan()))
            {
                AAMessageSender::instance()->submitAuditManualGrcFailed( strSession, strPlanName, m_remark, ulInstanceNum, ulStepNum );	
            }
            else//automatic GRC
            {
                AAMessageSender::instance()->submitAuditAutoGrcFailed( strSession, strPlanName, m_remark, ulInstanceNum, ulStepNum );	
            }
        }
        //For GRC, alarm is not needed
        else 
        {
            if (ActivePlanBulletinBoard::instance()->isManualPlan(m_flowControl.getPlan()))
            {
                AAMessageSender::instance()->submitAuditStepExecutedFailure( strSession, strPlanName, m_remark, ulInstanceNum, ulStepNum );
            }
            else
            {
                alarmId = AAMessageSender::instance()->raiseAlarmStepExecutedFailure( strPlanName, m_remark, ulInstanceNum, ulStepNum );
                ActivePlanBulletinBoard::instance()->setAssociatedAlarmId (m_flowControl.getPlan(), alarmId.c_str ());
            }
        }

		m_flowControl.setInteractiveStep(false);

		if ( m_detail.ignoreFailure )
		{
            // TD8788: Where a step failure does NOT cause the plan to pause,
            // the associated alarm remains open until it is manually closed
            // by the operator. ActivePlan is not given the alarm ID.

            m_flowControl.setPlanStepFailed();
		    ActivePlanBulletinBoard::instance()->update( m_flowControl.getPlan(), getStepNumber(), TA_Base_Core::FAILED_ACTIVE_STEP_STATE, m_remark );

            if ( !m_bGroupStep && !m_bSubStep )
			{
                m_flowControl.setNextStep();
			}
        }
        else
        {
            // TD8788: Pass on the alarm ID to allow the alarm to be closed
            // when the plan is resumed or stopped.
            m_flowControl.pausePlan( m_remark, alarmId );
            ActivePlanBulletinBoard::instance()->update( m_flowControl.getPlan(), getStepNumber(), TA_Base_Core::PAUSED_ACTIVE_STEP_STATE, m_remark);
        }
    }

	void ActiveStep::execute( void )
	{
		if( m_detail.skippable && m_detail.skip )
		{
			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "%s: skip.", dumpContext().c_str() );

            load();

            if ( m_remark.empty() )
            {
				m_remark = RemarkPlanStepSkipped;
            }

			ActivePlanBulletinBoard::instance()->update( m_flowControl.getPlan(), getStepNumber(), TA_Base_Core::SKIPPED_ACTIVE_STEP_STATE, m_remark );
            m_flowControl.setNextStep();    //MP: skip to the next step

			return;
		}

        try
        {
            if (m_detail.delay > 0)
            {
                // Get the current state only delay is enable
                int state = ActivePlanBulletinBoard::instance()->getState( m_flowControl.getPlan() );

                if ( state == TA_Base_Core::EXECUTING_ACTIVE_PLAN_STATE )
                {
                    load(!m_bGroupStep);

                    // Check if we are partway through a delay.
                    if (m_delayCompleted != 0)
                    {
                        // Part way through a delay. Update the start time so that the total delay is correct.
                        m_delayStartTime = time(NULL) - m_delayCompleted;
                    }
                    else
                    {
                        // Keep a timestamp of the time the delay started.
                        m_delayStartTime = time(NULL);
                    }

                    // Update the remark to indicate we're in a delay
                    std::ostringstream remark;
                    remark << "In a " << m_detail.delay << " second delay";
                    m_remark = remark.str();

				    if ( !m_bGroupStep )
				    {
					    ActivePlanBulletinBoard::instance()->update( m_flowControl.getPlan(), TA_Base_Core::DELAYED_ACTIVE_PLAN_STATE, m_remark);

					    ActivePlanBulletinBoard::instance()->update( m_flowControl.getPlan(), getStepNumber(), TA_Base_Core::DELAYED_ACTIVE_STEP_STATE, m_remark);
				    }

                    // Update the state of the plan to delayed.
                    m_flowControl.changeState(ActivePlanDelayedState::instance());
                    state = TA_Base_Core::DELAYED_ACTIVE_PLAN_STATE;
                }

                if (state == TA_Base_Core::DELAYED_ACTIVE_PLAN_STATE)
                {
                    if ( 0 == m_delayStartTime )
                    {
                        // The state is restored from monitor mode, simply re-wait
                        m_delayStartTime = time(NULL);
                        return;
                    }

                    // Check that the delay hasn't completed yet.
                    time_t currentTime = time(NULL);
                    if (m_delayStartTime + static_cast<time_t>(m_detail.delay) > currentTime)
                    {
                        // Delay not complete. Update the amount of the delay completed and return.
                        m_delayCompleted = time(NULL) - m_delayStartTime;
                        return;
                    }

                    // TD10841: Set active plan state back to executing after the delay has completed.
                    m_flowControl.changeState(ActivePlanExecutingState::instance());
                    state = TA_Base_Core::EXECUTING_ACTIVE_PLAN_STATE;
                }
            }
            else if (m_detail.delay == 0)
            {
                //Introduce a short finite delay if the delay is zero
			    ACE_Time_Value tv( 0, 50000 ); // 50ms
			    ACE_OS::sleep( tv );
            }
        }
        catch ( ... )
        {
            // Do nothing as plan state was not found, let the processing 
            // be default
        }

        // Update the remark in case it currently indicates that we're in a delay.
		m_remark = RemarkExecutingStep;

        load( !m_bGroupStep );

        // Delay is finished. Reset the amount of the delay completed.
        m_delayCompleted = 0;

		if ( !m_bGroupStep )
		{
			ActivePlanBulletinBoard::instance()->update( m_flowControl.getPlan(), TA_Base_Core::EXECUTING_ACTIVE_PLAN_STATE, m_remark);
		}

		TA_Base_Core::EActiveStepState stepState = TA_Base_Core::UNDEFINED_ACTIVE_STEP_STATE;

		try
		{
			stepState = executeImpl();
		}
		catch( ... )
		{
			LOG_EXCEPTION_CATCH( SourceInfo, "unknown", dumpContext().c_str() );
			stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
		}

		//TD20350
		if( TA_Base_Core::FAILED_ACTIVE_STEP_STATE == stepState  )
		{
			//should not process GRCType3ParentStep failure
			if ( !m_bGroupStep )
			{
				processStepExecutionFailure("");
			}
		}
		else
		{
			if ( m_remark.empty() )
            {
                m_remark = RemarkPlanStepExecuted;
            }

			if ( !m_bGroupStep )
			{
				ActivePlanBulletinBoard::instance()->update( m_flowControl.getPlan(), getStepNumber(), stepState, m_remark );
			}
		}
	}

	std::string ActiveStep::dumpContext( void ) const
	{
		std::ostringstream context;
		context << "[P:" << m_flowControl.getPlan().plan << ",I:" << m_flowControl.getPlan().instance << ",S:" << getStepNumber() << "]";

		return( context.str() );
	}

    TA_Base_Core::StepId ActiveStep::getStepId( void ) const
    {
        return m_detail.step;
    }

} // TA_Base_App
