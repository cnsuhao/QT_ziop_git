/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/security/duty_agent/duty_agent_state/src/SnapshotQuery.h $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  Gets a snapshot of the current state.
  */

#if !defined(SNAPSHOTQUERY_H_INCLUDED)
#define SNAPSHOTQUERY_H_INCLUDED

namespace TA_Base_Bus
{
	class SnapshotQuery : public IDutyQuery
	{

	public:
		SnapshotQuery();

		virtual std::auto_ptr<IDutyQuery> clone() const;

		virtual void execute(DutyState& state, DASType::DutyAgentPeer* peer );

		virtual void execute(DutyState& state, const DASType::DbKey& region, const DASType::DutySet& dutySet);
		virtual void execute(DutyState& state, const DASType::DbKey& subsystem,
			const DASType::DutySet& dutySet, const SubsystemState& subState );

		virtual bool isValidToCheckSubsystems();

		virtual const DASType::SubsystemSet& getSubsystems() const;
		virtual const DASType::RegionSet&    getRegions()    const;

	private:
		DASType::SubsystemSet m_subsystems;
		DASType::RegionSet    m_regions;
	};
}

#endif // !defined(SNAPSHOTQUERY_H_INCLUDED)
