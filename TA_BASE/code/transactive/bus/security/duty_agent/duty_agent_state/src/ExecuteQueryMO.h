/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/security/duty_agent/duty_agent_state/src/ExecuteQueryMO.h $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  The ACE Method Object that executes the executeQuery method.
  */

#ifndef EXECUTEQUERYMO_H_INCLUDED
#define EXECUTEQUERYMO_H_INCLUDED

namespace TA_Base_Bus
{
	class DutyPeerTask;
}

namespace TA_Base_Bus
{
	//Class declaration
	class ExecuteQueryMO : public DutyPeerTaskMO
	{
	// Public methods
	public:
		ExecuteQueryMO( DutyPeerTask& task, const ACE_Future<bool>& future,
			DutyState& state, IDutyQuery& query );

	protected:

		virtual void execute();

	private:

		ExecuteQueryMO& operator=( const ExecuteQueryMO& );
		ExecuteQueryMO( const ExecuteQueryMO& );

		DutyPeerTask& m_task;
		DutyState& m_state;
		IDutyQuery& m_query;
	}; 
} // TA_Base_Bus

#endif //EXECUTEQUERYMO_H_INCLUDED
