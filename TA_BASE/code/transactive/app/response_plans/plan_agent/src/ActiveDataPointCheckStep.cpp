/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/ActiveDataPointCheckStep.cpp $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  Implementation of .
  */

#include "app/response_plans/common/src/ProjectDefinition.h"

#if defined ( STEPTYPE_DATA_POINT_CHECK )

#include <memory>

#include "core/utilities/src/DebugUtil.h"
#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "bus/scada/common_library/src/CommonDefs.h"
#include "app/response_plans/plan_agent/src/IActivePlanFlow.h"
#include "app/response_plans/plan_agent/src/ActivePlanBulletinBoard.h"
#include "app/response_plans/plan_agent/src/ActiveDataPointCheckStep.h"
#include "app/response_plans/common/src/StringConstants.h"

// namespace alias for Corba structures/types

namespace TA_Base_App
{
	ActiveDataPointCheckStep::ActiveDataPointCheckStep( const TA_Base_Core::StepDetail& stepDetail,
        const TA_Base_Core::DataPointCheckParameters& params, IActivePlanFlow& flowControl ) : ActiveStep(stepDetail, flowControl),
    m_params(params),
    m_dataPoint(),
    m_ulDpKey( 0u )
	{
	}

	TA_Base_Core::EActiveStepState ActiveDataPointCheckStep::executeImpl( void )
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "Data Point Check step %lu", getStepNumber() );

		//TA_Base_Bus::DataPointCorbaDef::ClientDataPointState_var dataPointState;
		TA_Base_Core::EActiveStepState stepState = TA_Base_Core::EXECUTED_ACTIVE_STEP_STATE;

		try
		{
            // get named object of datapoint agent
            if ( m_dataPoint.getObjectName().empty() )
            {
                std::auto_ptr<TA_Base_Core::IEntityData> dpEntity(TA_Base_Core::EntityAccessFactory::getInstance().getEntity(m_params.dataPoint.in()));
                std::string dpAgentName = dpEntity->getAgentName();
                m_ulDpKey = dpEntity->getKey();
                m_dataPoint.setCorbaName(dpAgentName, dpAgentName + TA_Base_Bus::DP_ACCESS_INTERFACE);
            }

			TA_Base_Bus::ScadaCorbaTypes::UTag* dataPointState;
			CORBA_CALL_RETURN( dataPointState, m_dataPoint, getFieldValue, ( m_ulDpKey ) );
		
			bool success = false;
			// Do comparisons.

			switch( dataPointState->_d() )
			{
            case TA_Base_Bus::ScadaCorbaTypes::AnalogueTagType:
            //? why didn't deal with enum data type? case TA_Base_Bus::ScadaCorbaTypes::EnumTagType:
				{
					if( TA_Base_Core::NUMERIC_DATA_POINT != m_params.value._d() )
					{
                        m_remark = RemarkAttemptCompareDPwithNonfloatValue;
                        stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
                        LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
                            "Attempt to compare float data point with non-float value for data point: %s", m_params.dataPoint.in() );
					}
					else
					{
						success = test( m_params.value.numericValue().value, dataPointState->analogueTag().value, m_params.testType );
					}
				}
                break;

            case TA_Base_Bus::ScadaCorbaTypes::EnumTagType:
                {
                    if( TA_Base_Core::NUMERIC_DATA_POINT != m_params.value._d() )
                    {
                        m_remark = RemarkAttemptCompareDPwithNonfloatValue;
                        stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
                        LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
                            "Attempt to compare float data point with non-float value for data point: %s", m_params.dataPoint.in() );
                    }
                    else
                    {
                        success = test( static_cast<unsigned long>(m_params.value.numericValue().value),
                            dataPointState->enumTag().value, m_params.testType );
                    }
                }
                break;

			case TA_Base_Bus::ScadaCorbaTypes::BooleanTagType:
				{
					if( TA_Base_Core::BOOLEAN_DATA_POINT != m_params.value._d() )
					{
                        m_remark = RemarkAttemptCompareDPwithNonBoolValue;
                        stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
						LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
                            "Attempt to compare boolean data point with non-boolean value for data point: %s", m_params.dataPoint.in() );
					}
					else
					{
						success = test( m_params.value.booleanValue(), dataPointState->booleanTag().value, m_params.testType );
					}
				}
				break;

			case TA_Base_Bus::ScadaCorbaTypes::TextTagType:
				{
					if( TA_Base_Core::TEXT_DATA_POINT != m_params.value._d() )
					{
                        m_remark = RemarkAttemptCompareDPwithNonTextValue;
                        stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
						LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
                            "Attempt to compare text data point with non-text value for data point: %s", m_params.dataPoint.in() );
					}
					else
					{
						std::string lhs = m_params.value.textValue();
                        TA_Base_Bus::ScadaCorbaTypes::ByteSequence readSeq = dataPointState->textTag().value;
			            std::string rhs = "";
                        int length = readSeq.length();

			            for ( int i = 0; i < length; i++ )
			            {
				            rhs += static_cast<char>(readSeq[i]);
			            }

						success = test( lhs, rhs, m_params.testType );
					}
				}
				break;

			default:
				{
                    m_remark = RemarkInvalidDatapointValue;
                    stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
					LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Bad data point value: %s", m_params.dataPoint.in() );
				}
				break;
			}

            if ( stepState != TA_Base_Core::FAILED_ACTIVE_STEP_STATE )
            {
                m_flowControl.setNextStep( success ? m_params.successStep : m_params.failureStep );
            }
		}
		catch( const TA_Base_Core::ObjectResolutionException& e )
        {
            m_remark = RemarkInvalidDatapoint;

			std::ostringstream msg;
			msg << dumpContext() << ": " << e.what() << " for data point " << m_params.dataPoint.in() << ".";
			LOG_EXCEPTION_CATCH( SourceInfo, "TA_Base_Core::ObjectResolutionException", msg.str().c_str() );

			stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
		}
		catch( const TA_Base_Bus::ScadaCorbaTypes::NotWriteable&)
        {
            m_remark = RemarkInvalidDatapoint;

			std::ostringstream msg;
			msg << dumpContext() << ": No access to data point " << m_params.dataPoint.in() << ".";
			LOG_EXCEPTION_CATCH( SourceInfo, "nexus::DataPointDmIf::NoAccess", msg.str().c_str() );

			stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
		}
		catch( ... )
        {
            m_remark = RemarkInvalidDatapoint;

			std::ostringstream msg;
			msg << dumpContext() << ": No access to data point " << m_params.dataPoint.in() << ".";
			LOG_EXCEPTION_CATCH( SourceInfo, "Unknown exception", msg.str().c_str() );

			stepState = TA_Base_Core::FAILED_ACTIVE_STEP_STATE;
		}

		return stepState;
	}

	template<class T>
	bool ActiveDataPointCheckStep::test( const T& lhs, const T& rhs, TA_Base_Core::EDataPointTest testType )
	{
		switch( testType )
		{
			case TA_Base_Core::EQUAL_TO_TEST:
			{
				return( lhs == rhs );
			}
			break;

			case TA_Base_Core::NOT_EQUAL_TO_TEST:
			{
				return( lhs != rhs );
			}
			break;

			case TA_Base_Core::LESS_THAN_TEST:
			{
				return( lhs < rhs );
			}
			break;

			case TA_Base_Core::GREATER_THAN_TEST:
			{
				return( lhs > rhs );
			}
			break;

			case TA_Base_Core::LESS_THAN_OR_EQUAL_TO_TEST:
			{
				return( lhs <= rhs );
			}
			break;

			case TA_Base_Core::GREATER_THAN_OR_EQUAL_TO_TEST:
			{
				return( lhs >= rhs );
			}
			break;

			default:
			{
				return( false );
			}
			break;
		}
	}
} // TA_Base_App


#endif //  STEPTYPE_DATA_POINT_CHECK

