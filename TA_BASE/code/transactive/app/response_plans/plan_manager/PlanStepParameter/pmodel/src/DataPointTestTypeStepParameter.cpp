/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/DataPointTestTypeStepParameter.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  **/

#if defined (STEPTYPE_DATA_POINT_CHECK)

#include "core/utilities/src/TAAssert.h"
#include "DataPointTestTypeStepParameter.h"
//#include "PlanStepParameterEditDlg.h"
#include "app/response_plans/plan_manager/src/PlanManagerPModelConsts.h"
#include "app/response_plans/plan_manager/src/PlanStep.h"
#include "app/response_plans/plan_manager/src/PlanNode.h"
#include "PlanStepParameterListCtrlModel.h"
#include "app/response_plans/plan_manager/src/Utilities.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;


/////////////////////////////////////////////////////////////////////////////
// PlanStepParameter

DataPointTestTypeStepParameter::DataPointTestTypeStepParameter(PlanStep& parent, std::string name, TA_Base_Core::EDataPointTest testType) :
    PlanStepParameter(parent, name),
    m_testType(testType)
{
    FUNCTION_ENTRY("DataPointTestTypeStepParameter");
    FUNCTION_EXIT;
}


DataPointTestTypeStepParameter::DataPointTestTypeStepParameter(PlanStep& parent, const DataPointTestTypeStepParameter& parameter) :
    PlanStepParameter(parent, parameter.getName()),
    m_testType(parameter.getDataPointTestType())
{
    FUNCTION_ENTRY("DataPointTestTypeStepParameter");
    FUNCTION_EXIT;
}


DataPointTestTypeStepParameter::DataPointTestTypeStepParameter(const DataPointTestTypeStepParameter& parameter) :
    PlanStepParameter(*parameter.getParentStep(), parameter.getName()),
    m_testType(parameter.getDataPointTestType())
{
    FUNCTION_ENTRY("DataPointTestTypeStepParameter");
    FUNCTION_EXIT;
}


DataPointTestTypeStepParameter::~DataPointTestTypeStepParameter()
{
    FUNCTION_ENTRY("~DataPointTestTypeStepParameter");
    FUNCTION_EXIT;
}


void DataPointTestTypeStepParameter::addToParameterList(CPlanStepParameterListCtrlModel &listCtrl)
{
    FUNCTION_ENTRY("addToParameterList");

    std::string testTypeStr = (m_testType == TA_Base_Core::UNDEFINED_TEST ? "" : Utilities::convertToString(m_testType));

    listCtrl.addParameter(getName().c_str(), testTypeStr.c_str(), this);

    FUNCTION_EXIT;
}


void DataPointTestTypeStepParameter::setDataPointTestType(TA_Base_Core::EDataPointTest testType)
{
    FUNCTION_ENTRY("setDataPointTestType");

    TA_ASSERT(getParentStep()->getParentPlan()->isCopiedPlan(), "Attempt to update the factory plan.");
    m_testType = testType;

    FUNCTION_EXIT;
}


/*
TA_Base_App::CPlanStepParameterEditDlg* DataPointTestTypeStepParameter::getEditDialog(bool canEdit)
{
    FUNCTION_ENTRY( "getEditDialog" );
    FUNCTION_EXIT;
    return new TA_Base_App::CPlanStepParameterEditDlg(*this,canEdit);
}
*/


#endif // defined (STEPTYPE_DATA_POINT_CHECK)
