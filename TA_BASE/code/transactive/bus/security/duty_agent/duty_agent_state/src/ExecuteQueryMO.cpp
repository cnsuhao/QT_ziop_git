/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/security/duty_agent/duty_agent_state/src/ExecuteQueryMO.cpp $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  Implementation of ExecuteQueryMO.
  */

#include "bus/security/duty_agent/duty_agent_state/src/DutyAgentState.h"

namespace TA_Base_Bus
{
	ExecuteQueryMO::ExecuteQueryMO( DutyPeerTask& task, const ACE_Future<bool>& future,
		DutyState& state, IDutyQuery& query ) : DutyPeerTaskMO(future),
		m_task(task),
		m_state(state),
		m_query(query)

	{
	}

	void ExecuteQueryMO::execute( void )
	{
		m_task.executeQueryImpl( m_state, m_query );
	}

} // TA_Base_Bus
