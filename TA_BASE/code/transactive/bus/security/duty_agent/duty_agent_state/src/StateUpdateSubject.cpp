/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/security/duty_agent/duty_agent_state/src/StateUpdateSubject.cpp $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  Implements StateUpdateSubject.
  */

#include "bus/security/duty_agent/duty_agent_state/src/DutyAgentState.h"

namespace TA_Base_Bus
{
	void StateUpdateSubjectImpl::update( const DutyState& state )
	{
		FUNCTION_ENTRY("update");
		if( state.isEmpty() )
		{
			FUNCTION_EXIT;
			return;
		}
		
		notify(*this,state);
		FUNCTION_EXIT;
	}
}
