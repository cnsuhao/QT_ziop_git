/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/PlanDetail.cpp $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  Implementation of PlanAccessFactory.
  *  TODO: put hard coded column names and SQL in a config file or in the data
  *  access classes outlined in the DataAccessInterface usage guide.
  */

#include "ace/OS.h"

#include "core/utilities/src/DateFormat.h"
#include "bus/response_plans/PlanDataAccess/src/PlanDataReader.h"
#include "app/response_plans/plan_agent/src/PlanAgentUser.h"
#include "app/response_plans/plan_agent/src/PlanDetail.h"


namespace TA_Base_App
{

PlanDetail::PlanDetail( const PlanDetail& rhs ) :
m_detail( rhs.m_detail ),
m_timeOut( rhs.m_timeOut ),
m_stepStarted( rhs.m_stepStarted ),
m_session( rhs.m_session.c_str() ),
m_pmId( rhs.m_pmId.c_str() ),
m_alarmID( rhs.m_alarmID.c_str() ),
m_userKey( rhs.m_userKey ),
m_exeHistoryMap( rhs.m_exeHistoryMap ),
m_custHistoryMap( rhs.m_custHistoryMap ),
m_lastModified( rhs.m_lastModified ),
m_result( rhs.m_result )
{
}


PlanDetail::PlanDetail( const std::string& session, const std::string& pmId, const unsigned long userKey, const unsigned long updateId,
	const std::string& owner, const unsigned long planId, const int initialState, const std::string& remark, const ACE_Future<int>& result , 
	const bool manExec, const std::string& assocAlarmId ) :
m_detail(),
m_timeOut( 0 ),
m_stepStarted( 0 ),
m_session( session.c_str() ),
m_pmId( pmId.c_str() ),
m_alarmID( "" ),
m_userKey( userKey ),
m_exeHistoryMap(),
m_custHistoryMap(),
m_lastModified( ACE_OS::gettimeofday() ),
m_result( result )
{
        m_detail.plan.plan = planId;
		m_detail.plan.instance   = 0;
		m_detail.plan.triggerEntity = 0u;
		m_detail.plan.location = PlanAgentUser::getInstance()->getAgentLocationKey();
		m_detail.path = CORBA::string_dup( TA_Base_Bus::PlanDataReader::instance()->getNodePath(planId).c_str() );
		m_detail.owner = CORBA::string_dup( owner.c_str() );
		m_detail.activeUpdateId = updateId;
		m_detail.activePlanState = static_cast<TA_Base_Core::EActivePlanState>(initialState);
		m_detail.currentStepDetail.name = CORBA::string_dup( "undefined" );
		m_detail.currentStepDetail.position = 0;
		m_detail.currentStepDetail.state = TA_Base_Core::UNDEFINED_ACTIVE_STEP_STATE;
		m_detail.remark = CORBA::string_dup( remark.c_str() );
		m_detail.manuallyExecuted = manExec;
        m_detail.assocAlarmId = CORBA::string_dup(assocAlarmId.c_str());
        m_detail.timer = 0u;

		std::string tmpTime = getLocalTime();
		m_detail.timeOfLastStateTransition = CORBA::string_dup( tmpTime.c_str() );
		m_detail.timeOfInstantiation = CORBA::string_dup( tmpTime.c_str() );
}


PlanDetail::PlanDetail( const TA_Base_Core::ActivePlanDetail& detail ) :
m_detail(),
m_timeOut( 0 ),
m_stepStarted( 0 ),
m_session( "" ),
m_pmId( "" ),
m_alarmID( "" ),
m_userKey( 0 ),
m_exeHistoryMap(),
m_custHistoryMap(),
m_lastModified( ACE_OS::gettimeofday() ),
m_result( 0 )
{
    m_detail.plan.plan = detail.plan.plan;
    m_detail.plan.instance = detail.plan.instance;
    m_detail.plan.location = detail.plan.location;
    m_detail.plan.triggerEntity = detail.plan.triggerEntity;
    m_detail.path = CORBA::string_dup( detail.path );
    m_detail.owner = detail.owner;
    m_detail.activeUpdateId = detail.activeUpdateId;
    m_detail.activePlanState = detail.activePlanState;
    m_detail.currentStepDetail.name = CORBA::string_dup( detail.currentStepDetail.name );
    m_detail.currentStepDetail.position = detail.currentStepDetail.position;
    m_detail.manuallyExecuted = detail.manuallyExecuted;
    m_detail.assocAlarmId = CORBA::string_dup( detail.assocAlarmId );
    m_detail.timer = detail.timer;

	if ( TA_Base_Core::PAUSED_ACTIVE_PLAN_STATE == detail.activePlanState )
	{
		m_detail.currentStepDetail.state = TA_Base_Core::PAUSED_ACTIVE_STEP_STATE;
	}
	else
	{
		m_detail.currentStepDetail.state = detail.currentStepDetail.state;
	}

	m_detail.remark = CORBA::string_dup( detail.remark );
	m_detail.timeOfLastStateTransition = CORBA::string_dup( detail.timeOfLastStateTransition );
	m_detail.timeOfInstantiation = CORBA::string_dup( detail.timeOfInstantiation );
}

void PlanDetail::set( const TA_Base_Core::ActivePlanId& plan )
{
	m_detail.plan = plan;
}

bool PlanDetail::set( const int state )
{
	if( state != m_detail.activePlanState )
	{
		m_detail.activePlanState = static_cast<TA_Base_Core::EActivePlanState>(state);
		m_detail.timeOfLastStateTransition = CORBA::string_dup(getLocalTime().c_str());
		m_lastModified = ACE_OS::gettimeofday();
		if( isNonActive() )
		{
			m_result.set(state);
		}

		return true;
	}

	return false;
}

bool PlanDetail::isNonActive() const
{
    switch( m_detail.activePlanState )
    {
    case TA_Base_Core::COMPLETED_ACTIVE_PLAN_STATE:
    case TA_Base_Core::STOPPED_ACTIVE_PLAN_STATE:
    case TA_Base_Core::FAILED_ACTIVE_PLAN_STATE:
    case TA_Base_Core::ABORTED_ACTIVE_PLAN_STATE:
        return true;
        break;

    default:
        break;
    }

    return false;
}

bool PlanDetail::isActive() const
{
    switch( m_detail.activePlanState )
    {
    case TA_Base_Core::EXECUTING_ACTIVE_PLAN_STATE:
    case TA_Base_Core::PAUSED_ACTIVE_PLAN_STATE:
    case TA_Base_Core::PENDING_ACTIVE_PLAN_STATE:
    case TA_Base_Core::OPERATOR_PAUSED_ACTIVE_PLAN_STATE:
    case TA_Base_Core::DELAYED_ACTIVE_PLAN_STATE:
        return true;
        break;

    default:
        break;
    }

    return false;
}

std::string PlanDetail::getLocalTime( void )
{
	struct tm newtime;
	time_t long_time;

	time( &long_time );                // Get time as long integer.
	ACE_OS::localtime_r( &long_time, &newtime ); // Convert to local time.

	const size_t MAX_TIME_LENGTH(40);
	char localTime[MAX_TIME_LENGTH];

	// TD12474 ++
	TA_Base_Core::DateFormat dateFormat; 
	std::string dateFormatStr = dateFormat.getDateFormat(TA_Base_Core::DateFormat::DateTimeFormat4);

	::strftime(localTime, MAX_TIME_LENGTH, dateFormatStr.c_str(), &newtime); 
	// ++ TD12474 

	return localTime;
}

} // TA_Base_App
