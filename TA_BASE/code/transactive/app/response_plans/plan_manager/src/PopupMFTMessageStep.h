#if !defined(AFX_DECISIONSTEP_H__37441840_28F6_42a1_B421_3680B445EAFA__INCLUDED_)
#define AFX_DECISIONSTEP_H__37441840_28F6_42a1_B421_3680B445EAFA__INCLUDED_
//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/PopupMFTMessageStep.h $
// @author:  Andy Parker
// @version: $Revision: #1 $
//
// Last modification: $DateTime: 2015/10/09 15:42:35 $
// Last modified by:  $Author: CM $
//
// <description>

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#if defined (STEPTYPE_POPUP_MFT_MESSAGE)

#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"
#include "PlanStep.h"
#include "PlanManagerCommonDefs.h"


namespace TA_Base_App
{
    class StepNumberStepParameter;
    class TextualStepParameter;
    class NumericStepParameter;
    class CPlanStepEditParameterPage;

    /////////////////////////////////////////////////////////////////////////////
    // PopupMFTMessageStep

    class PopupMFTMessageStep : public PlanStep
    {
        // Operations
    public:
        PopupMFTMessageStep(PlanNode* parent,const TA_Base_Core::StepDetail &stepDetail);
        PopupMFTMessageStep(PlanNode* parent,PlanStepId stepId);  // For new plan steps
        virtual ~PopupMFTMessageStep();

        virtual PlanStep* clone();
        virtual bool isStepBranching() const { return true; }
        virtual bool canIgnoreFailure() const { return true; }

        TextualStepParameter*     getMessageParameter() { return m_messageParameter;};
        StepNumberStepParameter*  getYesParameter() { return m_yesParameter; }
        StepNumberStepParameter*  getNoParameter() { return m_noParameter; };
        StepNumberStepParameter*  getPendingParameter() { return m_pendingParameter; };
        NumericStepParameter*     getOperatorProfile() {return m_operatorProfileNumber;};

    protected:
        PopupMFTMessageStep(const PopupMFTMessageStep &other);
        virtual void createPlanStepParameters(PlanStepParameterList &stepParameters);
        virtual void createPlanStepParameters(const TA_Base_Core::StepParameters &parameterSet,
            PlanStepParameterList &stepParameters);

        virtual void getUpdatedParameters(TA_Base_Core::Step& step);
        virtual CPlanStepEditParameterPage* getParameterEditPage();

    private:
        PopupMFTMessageStep &operator=(const PopupMFTMessageStep &other);


        // Attributes
    private:
        TextualStepParameter* m_messageParameter;
        StepNumberStepParameter* m_yesParameter;
        StepNumberStepParameter* m_noParameter;
        StepNumberStepParameter* m_pendingParameter;
        NumericStepParameter* m_operatorProfileNumber;
    };
}

#endif // defined (STEPTYPE_POPUP_MFT_MESSAGE)

#endif // defined (AFX_DECISIONSTEP_H__37441840_28F6_42a1_B421_3680B445EAFA__INCLUDED_)
