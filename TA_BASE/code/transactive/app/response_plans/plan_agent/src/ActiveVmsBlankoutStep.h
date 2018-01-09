#if defined ( STEPTYPE_VMS_BLANKOUT )

#ifndef ACTIVE_VMS_BLANK_OUT_STEP_HEADER_INCLUDED
#define ACTIVE_VMS_BLANK_OUT_STEP_HEADER_INCLUDED

/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/ActiveVmsBlankoutStep.h $
  *  @author:  Mahaveer Pareek
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  <description>
  */

#include "app/response_plans/plan_agent/src/ActiveStep.h"

namespace TA_Base_App
{
	//Class declaration
	class ActiveVmsBlankoutStep : public ActiveStep
	{
	// Public methods
	public:
		ActiveVmsBlankoutStep( const TA_Base_Core::StepDetail& stepDetail,
            const TA_Base_Core::VmsBlankoutParameters& params, IActivePlanFlow& flowControl );
		
	protected:
		virtual TA_Base_Core::EActiveStepState executeImpl( void );

	private:
		ActiveVmsBlankoutStep& operator=( const ActiveVmsBlankoutStep& );
		ActiveVmsBlankoutStep( const ActiveVmsBlankoutStep& );

		const TA_Base_Core::VmsBlankoutParameters m_params;		
	}; // ActiveVmsBlankoutStep
}; // TA_Base_App

#endif // ACTIVE_VMS_BLANK_OUT_STEP_HEADER_INCLUDED

#endif //  STEPTYPE_VMS_BLANKOUT
