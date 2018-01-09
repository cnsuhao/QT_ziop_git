//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/ExecutableLaunchGuiAppStep.h $
// @author:  Bart Golab
// @version: $Revision: #1 $
//
// Last modification: $DateTime: 2015/10/09 15:42:35 $
// Last modified by:  $Author: CM $
//
// <description>

#if !defined(AFX_EXECUTABLELAUNCHGUIAPPSTEP_H__C0331656_7DB2_4721_B24D_BAF5543FB7DA__INCLUDED_)
#define AFX_EXECUTABLELAUNCHGUIAPPSTEP_H__C0331656_7DB2_4721_B24D_BAF5543FB7DA__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "InteractiveStep.h"


namespace TA_Base_App
{
    /////////////////////////////////////////////////////////////////////////////
    // ExecutableLaunchGuiAppStep

    class ExecutableLaunchGuiAppStep : public InteractiveStep
    {
    // Operations
    public:
        ExecutableLaunchGuiAppStep(const TA_Base_Core::CurrentPlanStepDetail& stepDetail);
        virtual ~ExecutableLaunchGuiAppStep();

    protected:
        virtual void launch();
    };

} // namespace TA_Base_App

#endif // !defined(AFX_EXECUTABLELAUNCHGUIAPPSTEP_H__C0331656_7DB2_4721_B24D_BAF5543FB7DA__INCLUDED_)
