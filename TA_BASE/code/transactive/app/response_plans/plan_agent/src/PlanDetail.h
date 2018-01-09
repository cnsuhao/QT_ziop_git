#ifndef PLANDETAIL_H_INCLUDED
#define PLANDETAIL_H_INCLUDED

/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/PlanDetail.h $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  A thread-safe Singleton factory for converting database data into
  *  application data and vice-versa.
  */

#include <string>

#include "ace/Time_Value.h"
#include "ace/Future.h"

#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"

namespace TA_Base_App
{

typedef std::map<unsigned long, TA_Base_Core::ActiveStepDetail> ExeHistoryMap;
typedef std::map<unsigned long, TA_Base_Core::ActiveStepDetail>::iterator ExeHistoryMapIterator;
typedef std::map<unsigned long, TA_Base_Core::ActiveStepDetail>::const_iterator ConstExeHistoryMapIterator;

typedef std::map<unsigned long, TA_Base_Core::StepCustomisableDetail> CustHistoryMap;
typedef std::map<unsigned long, TA_Base_Core::StepCustomisableDetail>::iterator CustHistoryMapIterator;
typedef std::map<unsigned long, TA_Base_Core::StepCustomisableDetail>::const_iterator ConstCustHistoryMapIterator;

class PlanDetail
{
public:
    PlanDetail( const PlanDetail& );
	PlanDetail( const std::string& session, const std::string& pmId, const unsigned long userKey, const unsigned long updateId,
		const std::string& owner, const unsigned long planId, const int initialState, const std::string& remark, const ACE_Future<int>& result , 
		const bool manExecuted, const std::string& assocAlarmId );
	PlanDetail( const TA_Base_Core::ActivePlanDetail& detail );

	void set( const TA_Base_Core::ActivePlanId& plan );
	bool set( const int state );

	TA_Base_Core::ActivePlanDetail m_detail;

	//the following are to help rectify TD 7183
	//total time-out for all steps in an interactive plan
	//to pause a plan in case connection is lost
	ACE_Time_Value m_timeOut;
	//time a step was started
	ACE_Time_Value m_stepStarted;
	//note m_timeOut and m_stepStarted are reset 
	//for each step in a plan

	std::string m_session;
    std::string m_pmId;
    //alarm ID of the alarm raised by this Plan
    std::string m_alarmID;

	unsigned long m_userKey;

	ExeHistoryMap m_exeHistoryMap;
	CustHistoryMap m_custHistoryMap;

	ACE_Time_Value  m_lastModified;
	ACE_Future<int> m_result;

	bool isNonActive() const;
    bool isActive () const;

    std::string getLocalTime();


private:
    PlanDetail();
	PlanDetail& operator=( const PlanDetail& );
};

} // TA_Base_App

#endif //PLANDETAIL_H_INCLUDED
