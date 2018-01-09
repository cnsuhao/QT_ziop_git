//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/ExecutableLogEntryStep.h $
// @author:  Bart Golab
// @version: $Revision: #1 $
//
// Last modification: $DateTime: 2015/10/09 15:42:35 $
// Last modified by:  $Author: CM $
//
// <description>

#if !defined(AFX_EXECUTABLELOGENTRYSTEP_H__B1B0E068_806C_4D2D_B0A7_B69D79592D4E__INCLUDED_)
#define AFX_EXECUTABLELOGENTRYSTEP_H__B1B0E068_806C_4D2D_B0A7_B69D79592D4E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InteractiveStep.h"


namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // ExecutableLogEntryStep

    class ExecutableLogEntryStep : public InteractiveStep
    {
    // Operations
    public:
        ExecutableLogEntryStep(const TA_Base_Core::CurrentPlanStepDetail& stepDetail);
        virtual ~ExecutableLogEntryStep();

    protected:
        virtual void launch();
    };

} // namespace TA_Base_App

#endif // !defined(AFX_EXECUTABLELOGENTRYSTEP_H__B1B0E068_806C_4D2D_B0A7_B69D79592D4E__INCLUDED_)
