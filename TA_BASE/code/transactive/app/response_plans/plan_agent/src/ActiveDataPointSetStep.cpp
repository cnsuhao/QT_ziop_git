/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/ActiveDataPointSetStep.cpp $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  Implementation of .
  */

#include "app/response_plans/common/src/ProjectDefinition.h"

#if defined ( STEPTYPE_DATA_POINT_SET )

#include "core/utilities/src/DebugUtil.h"
#include "core/corba/src/CorbaUtil.h"
#include "core/threads/src/Thread.h"
#include "core/exceptions/src/ScadaProxyException.h"
#include "core/exceptions/IDL/src/CommonExceptionsCorbaDef.h"
#include "bus/scada/proxy_library/src/ScadaProxyFactory.h"
#include "bus/scada/proxy_library/src/IDataPointProxy.h"
#include "bus/scada/common_library/IDL/src/ScadaCorbaTypes.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"
#include "app/response_plans/plan_agent/src/AAMessageSender.h"
#include "app/response_plans/plan_agent/src/IActivePlanFlow.h"
#include "app/response_plans/plan_agent/src/ActivePlanBulletinBoard.h"
#include "app/response_plans/plan_agent/src/ActiveDataPointSetStep.h"
#include "app/response_plans/common/src/StringConstants.h"


namespace TA_Base_App
{
    const int DP_CONTROL_STATE_DELAY_TIME = 1500; // millisecond

    ActiveDataPointSetStep::ActiveDataPointSetStep( const TA_Base_Core::StepDetail& stepDetail,
        const TA_Base_Core::DataPointSetParameters& params, IActivePlanFlow& flowControl, bool bSubStep ) :
	ActiveStep(stepDetail, flowControl),
	m_params(params),
	m_semaphore(0),
	m_agentConnectedSem(0),
    m_pDataPoint(),
    m_bRccSucceed(false),
    m_bInControl(false)
	{
        m_bSubStep = bSubStep;
	}


	void ActiveDataPointSetStep::processEntityUpdateEvent(unsigned long entityKey , TA_Base_Bus::ScadaEntityUpdateType updateType)
	{
		if(updateType == TA_Base_Bus::AgentConnected)
		{
			m_agentConnectedSem.post();
		}

        LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "Received entity %lu update %d when %d control", entityKey, updateType, m_bInControl );

		if(( updateType == TA_Base_Bus::ControlStateUpdate ) && m_bInControl)
		{
			TA_Base_Bus::EDataPointControlState dst = m_pDataPoint->getControlState();

			if(TA_Base_Bus::conditionalSuccess == dst || TA_Base_Bus::success == dst)
			{
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Datapoint Set response: success");

                m_bRccSucceed = true;

                m_semaphore.post();

                const std::string strPlanName = ActivePlanBulletinBoard::instance()->getActivePlanDetail(m_flowControl.getPlan()).path.in();
                const unsigned long ulInstanceNum = m_flowControl.getPlan().instance;
				AAMessageSender::instance()->submitAuditManualGrcSucceed(strPlanName, ulInstanceNum);
			}
			else if(TA_Base_Bus::LCC_failed == dst)
			{
                LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Datapoint Set response: LCC_failed");

                m_semaphore.post();

                const std::string strEquipDesc = m_pDataPoint->getDescription();
                const std::string strLcc = m_pDataPoint->getLaunchingConditionResultString();
				AAMessageSender::instance()->submitAuditManualGrcBadLaunch( strEquipDesc, strLcc );
			}
			else if(TA_Base_Bus::RCC_failed == dst)
			{
                LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Datapoint Set response: RCC_failed");

                m_semaphore.post();

                const std::string strEquipDesc = m_pDataPoint->getDescription();
                const std::string strRcc = m_pDataPoint->getLaunchingConditionResultString();
				AAMessageSender::instance()->submitAuditManualGrcBadReturn( strEquipDesc, strRcc );
			}
			else if(TA_Base_Bus::failed == dst || TA_Base_Bus::comms_failed == dst)
			{
                LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Datapoint Set response: failed / comms_failed");

                m_semaphore.post();

                const std::string strPlanName = ActivePlanBulletinBoard::instance()->getActivePlanDetail(m_flowControl.getPlan()).path.in();
                const unsigned long ulInstanceNum = m_flowControl.getPlan().instance;
                AAMessageSender::instance()->submitAuditManualGrcFailed( "", strPlanName, "", ulInstanceNum, 0u );
			}
		}
	}


	TA_Base_Core::EActiveStepState ActiveDataPointSetStep::executeImpl( void )
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "Data Point Set step %lu", getStepNumber() );

		TA_Base_Core::EActiveStepState stepState = TA_Base_Core::EXECUTED_ACTIVE_STEP_STATE;

        m_remark = RemarkPlanStepExecuted;    

        std::auto_ptr<TA_Base_Core::IEntityData> dpEntity(NULL);
        try
        {
            dpEntity.reset(TA_Base_Core::EntityAccessFactory::getInstance().getEntity(m_params.dataPoint.in()));
        }
        catch (...) 
        {
            m_remark = RemarkInvalidDatapoint;

            std::ostringstream msg;
            msg << dumpContext() << ": " << " for data point " << m_params.dataPoint.in() << ".";
            LOG_EXCEPTION_CATCH( SourceInfo, "Unknown Exception", msg.str().c_str() );

            return TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
        }

		unsigned long action = TA_Base_Bus::aca_DATA_POINT_WRITE;
		std::string session = ActivePlanBulletinBoard::instance()->getSession(m_flowControl.getPlan());
		std::string reason = "";

		if ( !ActivePlanBulletinBoard::instance()->isActionPermitted(session, dpEntity->getKey(), action, reason) )
		{
			m_remark = RemarkAccessDataPointDenied;

            std::ostringstream msg;
            msg << dumpContext() << "Write Access to Data Point " << m_params.dataPoint.in() << " not allowed.";
			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugWarn, msg.str().c_str() );

			return TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
		}

        int millsec = -1;

		try
		{
            boost::shared_ptr<TA_Base_Core::DataPointEntityData> ptrEntity( dynamic_cast<TA_Base_Core::DataPointEntityData*>(dpEntity.release()) );
			millsec = ptrEntity->getABETimeoutPeriodSeconds() * 1000;

			TA_Base_Bus::ScadaProxyFactory::getInstance().createDataPointProxy( ptrEntity,*this, m_pDataPoint );

			if(!m_agentConnectedSem.timedWait(5000))
			{
				LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "DataPoint is not config valid");
				return TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
			}

			std::string tmp;
			std::stringstream str;
			if(m_pDataPoint->isDerived())
			{
				tmp = m_pDataPoint->getDerivedStateAsString( static_cast<unsigned long>(m_params.newValue.numericValue().value) );
			}
			else if(m_pDataPoint->isText())
			{
				tmp = m_params.newValue.textValue();
			}
			else if(m_pDataPoint->isNumber())
			{
				str << m_params.newValue.numericValue().value;
				tmp = str.str();
			}
			else if(m_pDataPoint->isBoolean())
			{
				tmp = m_pDataPoint->getBooleanLabel(m_params.newValue.booleanValue());
			}
			else
			{
				LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Unknow DataPoint Type");
				return TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
			}

			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
                "Datapoin <%s> setValue: value=%s, sessionId=%s, semaphore timeout = %d",
				m_params.dataPoint.in(), tmp.c_str(), session.c_str(), millsec );

            // Clear all signal of m_semaphore, only wait for m_pDataPoint->setValue response.
            while ( m_semaphore.tryWait() )
            {
                // Do nothing
            }

            m_bRccSucceed = false;
            m_bInControl = true;

            long tryCount = 0;
            while( tryCount++ < 100 )
            {
                try
                {
                    m_pDataPoint->setValue(tmp, session);
                    break;
                }
                catch (TA_Base_Core::ScadaProxyException &e)
                {
                    if ( tryCount >= 100 )
                    {
                        LOG_EXCEPTION_CATCH( SourceInfo, "TA_Base_Core::ScadaProxyException", e.what() );

                        return TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
                    }

                    TA_Base_Core::Thread::sleep( 500 );
			    }
		    }
        }
        catch( std::exception& expWhat )
        {
            std::ostringstream msg;
            msg << dumpContext() << ": " << expWhat.what() << " for data point " << m_params.dataPoint.in() << ".";
            LOG_EXCEPTION_CATCH( SourceInfo, typeid(expWhat).name(), msg.str().c_str() );

            stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
        }
        catch( TA_Base_Core::BadParameterException& e)
        {
            std::ostringstream msg;
            msg << dumpContext() << ": " << e.reason.in() << " for data point " << m_params.dataPoint.in() << ".";
            LOG_EXCEPTION_CATCH( SourceInfo, "TA_Base_Core::BadParameterException", msg.str().c_str() );

            stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
        }
        catch( TA_Base_Core::OperationModeException& e)
        {
            std::ostringstream msg;
            msg << dumpContext() << ": " << e.reason.in() << " for data point " << m_params.dataPoint.in() << ".";
            LOG_EXCEPTION_CATCH( SourceInfo, "TA_Base_Core::OperationModeException", msg.str().c_str() );

            stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
        }
        catch( TA_Base_Bus::ScadaCorbaTypes::NotWriteable& e)
        {
            std::ostringstream msg;
            msg << dumpContext() << ": " << e.reason.in() << " for data point " << m_params.dataPoint.in() << ".";
            LOG_EXCEPTION_CATCH( SourceInfo, "TA_Base_Bus::ScadaCorbaTypes::NotWriteable", msg.str().c_str() );

            stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
        }
        catch( TA_Base_Core::NotAvailableException& e)
        {
            std::ostringstream msg;
            msg << dumpContext() << ": " << e.reason.in() << " for data point " << m_params.dataPoint.in() << ".";
            LOG_EXCEPTION_CATCH( SourceInfo, "TA_Base_Core::NotAvailableException", msg.str().c_str() );

            stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
        }
        catch( TA_Base_Core::AccessDeniedException& e)
        {
            std::ostringstream msg;
            msg << dumpContext() << ": " << e.reason.in() << " for data point " << m_params.dataPoint.in() << ".";
            LOG_EXCEPTION_CATCH( SourceInfo, "TA_Base_Core::AccessDeniedException", msg.str().c_str() );

            stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
        }
        catch( CORBA::Exception& e )
        {
            std::ostringstream msg;
            msg << dumpContext() << ": " << TA_Base_Core::CorbaUtil::getInstance().exceptionToString(e) << " for data point " << m_params.dataPoint.in() << ".";
            LOG_EXCEPTION_CATCH( SourceInfo, "CORBA::Exception", msg.str().c_str() );

            stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
        }
        catch( ... )
        {
            m_remark = RemarkWriteDataPointFailed;

            std::ostringstream msg;
            msg << dumpContext() << ": " << " for data point " << m_params.dataPoint.in() << ".";
            LOG_EXCEPTION_CATCH( SourceInfo, "Unknown Exception", msg.str().c_str() );

            stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
        }

		//only wait when there are RCC checking
        if (millsec !=0 )
		{
			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Waiting for Entity Update Event: millsec=%d", millsec );
			if( !m_semaphore.timedWait(millsec + DP_CONTROL_STATE_DELAY_TIME))
			{
				LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Wait for set datapoint time out");
				stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
			}
			else if( !m_bRccSucceed )
			{
                stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
			}
		}

        m_bInControl = false;

		if( stepState != TA_Base_Core::FAILED_ACTIVE_STEP_STATE )
		{
			m_remark = RemarkStepExecutionSucceed;

			ActivePlanBulletinBoard::instance()->update( m_flowControl.getPlan(), getStepNumber(),
                TA_Base_Core::EXECUTED_ACTIVE_STEP_STATE, m_remark );
		}
        else
        {
            return stepState;
        }

		if ( m_bSubStep )
		{
			m_execResult.set( stepState != TA_Base_Core::FAILED_ACTIVE_STEP_STATE );
		}
		else
		{
			if( stepState != TA_Base_Core::FAILED_ACTIVE_STEP_STATE )
			{
				LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
                    "DataPoint<%s> setValue successfully, setNextStep to flowControl", m_params.dataPoint.in() );
				m_flowControl.setNextStep();
			}
		}

        return stepState;
	}

    void ActiveDataPointSetStep::setExecResult( ACE_Future<bool> & execResult )
    {
        m_execResult = execResult;
    }

} // TA_Base_App


#endif //  STEPTYPE_DATA_POINT_SET
