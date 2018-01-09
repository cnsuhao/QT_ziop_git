/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/response_plans/plan_manager/src/PopupMFTMessageStep.cpp $
  * @author:  Andy Parker
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2012/02/06 16:15:14 $
  * Last modified by:  $Author: haijun.li $
  *
  * <description>
  *
  **/

#include "StdAfx.h"

#if defined (STEPTYPE_POPUP_MFT_MESSAGE)

#include "core/utilities/src/TAAssert.h"
#include "resource.h"
#include "PopupMFTMessageStep.h"
#include "StepNumberStepParameter.h"
#include "NumericStepParameter.h"
#include "TextualStepParameter.h"
#include "PlanStepEditPopupMFTMessagePage.h"
#include "core/exceptions/src/PlanStepException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;


/////////////////////////////////////////////////////////////////////////////
// PlanStep

PopupMFTMessageStep::PopupMFTMessageStep(PlanNode* parent,const TA_Base_Core::StepDetail &stepDetail) :
    PlanStep(parent,stepDetail),
    m_messageParameter (NULL),
    m_yesParameter(NULL),
    m_noParameter(NULL),
    m_pendingParameter(NULL),
    m_operatorProfileNumber(NULL)
{
    FUNCTION_ENTRY( "PopupMFTMessageStep" );
    FUNCTION_EXIT;
}


PopupMFTMessageStep::PopupMFTMessageStep(PlanNode* parent,PlanStepId stepId) :
    PlanStep(parent,stepId,TA_Base_Core::POPUP_MFT_MESSAGE_STEP),
    m_messageParameter (NULL),
    m_yesParameter(NULL),
    m_noParameter(NULL),
    m_pendingParameter(NULL),
    m_operatorProfileNumber(NULL)
{
    FUNCTION_ENTRY( "PopupMFTMessageStep" );
    FUNCTION_EXIT;
}


PopupMFTMessageStep::PopupMFTMessageStep(const PopupMFTMessageStep& planStep) :
    PlanStep(planStep),
    m_messageParameter (NULL),
    m_yesParameter(NULL),
    m_noParameter(NULL),
    m_pendingParameter(NULL),
    m_operatorProfileNumber(NULL)
{
    FUNCTION_ENTRY( "PopupMFTMessageStep" );

    m_messageParameter = new TextualStepParameter(*this, *planStep.m_messageParameter);
    m_yesParameter = new StepNumberStepParameter(*this, *planStep.m_yesParameter);
    m_noParameter = new StepNumberStepParameter(*this, *planStep.m_noParameter);
    m_pendingParameter = new StepNumberStepParameter(*this, *planStep.m_pendingParameter);
    m_operatorProfileNumber = new NumericStepParameter(*this, *planStep.m_operatorProfileNumber);

    addPlanStepParameter(*m_messageParameter);
    addPlanStepParameter(*m_yesParameter);
    addPlanStepParameter(*m_noParameter);
    addPlanStepParameter(*m_pendingParameter);
    addPlanStepParameter(*m_operatorProfileNumber);

    FUNCTION_EXIT;
}


PopupMFTMessageStep::~PopupMFTMessageStep()
{
    FUNCTION_ENTRY( "~PopupMFTMessageStep" );

    // The parameters are deleted by PlanStep.

    FUNCTION_EXIT;
}


PlanStep* PopupMFTMessageStep::clone()
{
    FUNCTION_ENTRY( "clone" );

    // Ensure parameters are loaded
    PlanStepParameterList stepParameters;
    getPlanStepParameters(stepParameters);

    FUNCTION_EXIT;
    return new PopupMFTMessageStep(*this);
}


void PopupMFTMessageStep::createPlanStepParameters(PlanStepParameterList &stepParameters)
{
    FUNCTION_ENTRY( "createPlanStepParameters" );

    delete m_messageParameter;
    delete m_yesParameter;
    delete m_noParameter;
    delete m_pendingParameter;
    delete m_operatorProfileNumber;

    m_messageParameter = new TextualStepParameter(*this, getParameterName(IDS_STEP_PARAMETER_DECISION_MESSAGE), "");
    m_yesParameter = new StepNumberStepParameter(*this, getParameterName(IDS_STEP_PARAMETER_YES_STEP), 0);
    m_noParameter = new StepNumberStepParameter(*this, getParameterName(IDS_STEP_PARAMETER_NO_STEP), 0);
    m_pendingParameter = new StepNumberStepParameter(*this, getParameterName(IDS_STEP_PARAMETER_PENDING_STEP), 0);
    m_operatorProfileNumber = new NumericStepParameter(*this, getParameterName(IDS_STEP_PARAMETER_PROFILE_NAME), 0);

    stepParameters.clear();
    stepParameters.push_back(m_messageParameter);
    stepParameters.push_back(m_yesParameter);
    stepParameters.push_back(m_noParameter);
    stepParameters.push_back(m_pendingParameter);
    stepParameters.push_back(m_operatorProfileNumber);

    FUNCTION_EXIT;
}


void PopupMFTMessageStep::createPlanStepParameters(const TA_Base_Core::StepParameters &parameterSet,
                                                  PlanStepParameterList &stepParameters)
{
    FUNCTION_ENTRY( "createPlanStepParameters" );

    if (parameterSet._d() != TA_Base_Core::POPUP_MFT_MESSAGE_STEP)
    {
        std::ostringstream errorMessage;
        errorMessage << "Invalid parameter set type (" << parameterSet._d() << ") for popup MFT message step.";

        TA_THROW(TA_Base_Core::PlanStepException(errorMessage.str(), TA_Base_Core::POPUP_MFT_MESSAGE_STEP));
    }
    delete m_messageParameter;
    delete m_yesParameter;
    delete m_noParameter;
    delete m_pendingParameter;
    delete m_operatorProfileNumber;

    m_messageParameter = new TextualStepParameter(*this, getParameterName(IDS_STEP_PARAMETER_DECISION_MESSAGE), parameterSet.popupMFTMessage().decisionMessage.in());
    m_yesParameter = new StepNumberStepParameter(*this, getParameterName(IDS_STEP_PARAMETER_YES_STEP), parameterSet.popupMFTMessage().yesStep);
    m_noParameter = new StepNumberStepParameter(*this, getParameterName(IDS_STEP_PARAMETER_NO_STEP), parameterSet.popupMFTMessage().noStep);
    m_pendingParameter = new StepNumberStepParameter(*this, getParameterName(IDS_STEP_PARAMETER_PENDING_STEP), parameterSet.popupMFTMessage().pendingStep);
    m_operatorProfileNumber = new NumericStepParameter(*this, getParameterName(IDS_STEP_PARAMETER_PROFILE_NAME), parameterSet.popupMFTMessage().operatorProfileNumber);

    stepParameters.clear();
    stepParameters.push_back(m_messageParameter);
    stepParameters.push_back(m_yesParameter);
    stepParameters.push_back(m_noParameter);
    stepParameters.push_back(m_pendingParameter);
    stepParameters.push_back(m_operatorProfileNumber);

    FUNCTION_EXIT;
}


void PopupMFTMessageStep::getUpdatedParameters(TA_Base_Core::Step& step)
{
    FUNCTION_ENTRY( "getUpdatedParameters" );

    TA_Base_Core::PopupMFTMessageParameters  parameter;

    parameter.decisionMessage = CORBA::string_dup(m_messageParameter->getMessageText().c_str());
    parameter.yesStep = m_yesParameter->getStepId();
    parameter.noStep = m_noParameter->getStepId();
    parameter.pendingStep = m_pendingParameter->getStepId();
    parameter.operatorProfileNumber = m_operatorProfileNumber->getNumericValue();

    step.parameters.popupMFTMessage(parameter);

    FUNCTION_EXIT;
}


CPlanStepEditParameterPage* PopupMFTMessageStep::getParameterEditPage()
{
    FUNCTION_ENTRY( "getParameterEditPage" );
    FUNCTION_EXIT;
    return new CPlanStepEditPopupMFTMessagePage(this);
}


#endif // defined (STEPTYPE_POPUP_MFT_MESSAGE)
