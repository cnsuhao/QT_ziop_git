/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/NumericStepParameter.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  **/

#include "core/utilities/src/TAAssert.h"
//#include "bus/response_plans/active_plans_display/src/StringUtilities.h"
#include "app/response_plans/plan_manager/src/PlanManagerPModelConsts.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/NumericStepParameter.h"
//#include "PlanStepParameterNumericEditDlg.h"
#include "app/response_plans/plan_manager/src/PlanStep.h"
#include "app/response_plans/plan_manager/src/PlanNode.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/PlanStepParameterListCtrlModel.h"
#include <boost/lexical_cast.hpp>

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;


/////////////////////////////////////////////////////////////////////////////
// PlanStepParameter

NumericStepParameter::NumericStepParameter(PlanStep& parent, std::string name, unsigned long number, bool isHidden) :
    PlanStepParameter(parent, name),
    m_number(number),
    m_isHiddenValue(isHidden)
{
    FUNCTION_ENTRY("NumericStepParameter");
    FUNCTION_EXIT;
}


NumericStepParameter::NumericStepParameter(PlanStep& parent, const NumericStepParameter& parameter) :
    PlanStepParameter(parent, parameter.getName()),
    m_number(parameter.getNumericValue()),
    m_isHiddenValue(parameter.isHiddenValue())
{
    FUNCTION_ENTRY("NumericStepParameter");
    FUNCTION_EXIT;
}


NumericStepParameter::NumericStepParameter(const NumericStepParameter& parameter) :
    PlanStepParameter(*parameter.getParentStep(), parameter.getName()),
    m_number(parameter.getNumericValue()),
    m_isHiddenValue(parameter.isHiddenValue())
{
    FUNCTION_ENTRY("NumericStepParameter");
    FUNCTION_EXIT;
}


NumericStepParameter::~NumericStepParameter()
{
    FUNCTION_ENTRY("~NumericStepParameter");
    FUNCTION_EXIT;
}


void NumericStepParameter::addToParameterList(CPlanStepParameterListCtrlModel &listCtrl)
{
    FUNCTION_ENTRY("addToParameterList");

    listCtrl.addParameter(getName().c_str(), (isHiddenValue() ? "" : boost::lexical_cast<std::string>(m_number)), this);

    FUNCTION_EXIT;
}


/*
TA_Base_App::CPlanStepParameterEditDlg* NumericStepParameter::getEditDialog(bool canEdit)
{
    FUNCTION_ENTRY( "getEditDialog" );
    FUNCTION_EXIT;
    return new TA_Base_App::CPlanStepParameterNumericEditDlg(*this, canEdit);
}
*/


void NumericStepParameter::setValue(unsigned long number)
{
    FUNCTION_ENTRY("setValue");

    TA_ASSERT(getParentStep()->getParentPlan()->isCopiedPlan(), "Attempt to update the factory plan.");
    m_number = number;

    FUNCTION_EXIT;
}


void NumericStepParameter::hideValue(bool isHidden)
{
    FUNCTION_ENTRY("hideValue");

    m_isHiddenValue = isHidden;

    FUNCTION_EXIT;
}


