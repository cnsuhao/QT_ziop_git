#ifndef IDUTYAGENTCOMMAND_H_INCLUDED
#define IDUTYAGENTCOMMAND_H_INCLUDED

/**
 * The source code in this file is the property of
 * Ripple Systems and is not for redistribution in any form.
 * 
 * Source:    $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/security/duty_agent/src/IDutyAgentCommand.h $
 * @author:   Gregg Kirkpatrick
 * @version:  $Revision: #1 $
 *
 * Last modification: $DateTime: 2015/10/09 15:42:35 $
 * Last modified by:  $Author: CM $
 *
 * The base interface for a command pattern to enable the safe execution of
 * all duty agent functions.
 */

namespace TA_Base_App
{
    class IDutyAgentCommand
    {
    public:

		virtual ~IDutyAgentCommand(){}

		bool rightsOk( TA_Base_Bus::RightsLibrary& rightsLib, unsigned long resource,
			std::string& reason ) const;

		virtual void execute() = 0;

	protected:

		IDutyAgentCommand( const TA_Base_Bus::DutyState& state );

		virtual unsigned long getActionKey() const{ return(ULONG_MAX); }

	private:

		const TA_Base_Bus::DutyState& m_state;
    };
}

#endif // IDUTYAGENTCOMMAND_H_INCLUDED
