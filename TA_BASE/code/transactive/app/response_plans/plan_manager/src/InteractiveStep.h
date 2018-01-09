//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/InteractiveStep.h $
// @author:  Bart Golab
// @version: $Revision: #1 $
//
// Last modification: $DateTime: 2015/10/09 15:42:35 $
// Last modified by:  $Author: CM $
//
// <description>

#if !defined(AFX_INTERACTIVESTEP_H__711C3829_932C_4693_B233_9AF5C6DA25DD__INCLUDED_)
#define AFX_INTERACTIVESTEP_H__711C3829_932C_4693_B233_9AF5C6DA25DD__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"


namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // InteractiveStep

    class InteractiveStep
    {
    // Operations
    public:
        InteractiveStep(const TA_Base_Core::CurrentPlanStepDetail &stepDetail);
        virtual ~InteractiveStep();

        TA_Base_Core::CurrentPlanStepDetail getStepDetail() const;

        void launchAndSignal();
        std::string waitUntilLaunched(); // Returns launch error or empty string if launched OK.

    protected:
        virtual void launch() = 0;


    // Attributes
    private:
        TA_Base_Core::CurrentPlanStepDetail m_stepDetail;

        CEvent m_launchEvent;
        std::string m_launchError;
    };

} // namespace TA_Base_App

#endif // !defined(AFX_INTERACTIVESTEP_H__711C3829_932C_4693_B233_9AF5C6DA25DD__INCLUDED_)