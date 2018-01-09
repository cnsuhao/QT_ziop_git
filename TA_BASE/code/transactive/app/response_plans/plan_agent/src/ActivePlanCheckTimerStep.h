#if defined ( STEPTYPE_PLAN_CHECK_TIMER )

#ifndef ActivePlanCheckTimerStep_H_INCLUDED
#define ActivePlanCheckTimerStep_H_INCLUDED
/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/ActivePlanCheckTimerStep.h $
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
    class ActivePlanCheckTimerStep: public ActiveStep
    {
    public:
        ActivePlanCheckTimerStep( const TA_Base_Core::StepDetail& stepDetail,
            const TA_Base_Core::PlanCheckTimerParameters& params, IActivePlanFlow& flowControl );

    protected:
        virtual TA_Base_Core::EActiveStepState executeImpl( void );

    private:
        ActivePlanCheckTimerStep& operator=( const ActivePlanCheckTimerStep& );
        ActivePlanCheckTimerStep( const ActivePlanCheckTimerStep& );

    private:
        const TA_Base_Core::PlanCheckTimerParameters m_params;
    };
}


#endif // ActivePlanCheckTimerStep_H_INCLUDED
#endif // STEPTYPE_PLAN_CHECK_TIMER