#pragma warning(disable:4250)

#include "core/utilities/src/TAAssert.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/PlanPathStepParameter.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/PlanStepParameterListCtrlModel.h"
#include "app/response_plans/plan_manager/src/PlanAgentAccess.h"
//#include "PlanStepParameterEditDlg.h"
//#include "PlanStepParameterPlanPathEditDlg.h"
#include "app/response_plans/plan_manager/PlanStep/src/PlanStep.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/PlanStepParameterConstantsDef.h"

using namespace TA_Base_App;

/////////////////////////////////////////////////////////////////////////////
// PlanStepParameter

PlanPathStepParameter::PlanPathStepParameter(PlanStep& parent, std::string name, TreeNodeId planNodeId) :
    PlanStepParameter(parent, name),
    m_planNodeId(planNodeId)
{
    FUNCTION_ENTRY("PlanPathStepParameter");
    FUNCTION_EXIT;
}


PlanPathStepParameter::PlanPathStepParameter(PlanStep& parent, PlanPathStepParameter& parameter) :
    PlanStepParameter(parent, parameter.getName()),
    m_planNodeId(parameter.getPlanNodeId())
{
    FUNCTION_ENTRY("PlanPathStepParameter");
    FUNCTION_EXIT;
}


PlanPathStepParameter::PlanPathStepParameter(PlanPathStepParameter& parameter) :
    PlanStepParameter(*parameter.getParentStep(), parameter.getName()),
    m_planNodeId(parameter.getPlanNodeId())
{
    FUNCTION_ENTRY("PlanPathStepParameter");
    FUNCTION_EXIT;
}


PlanPathStepParameter::~PlanPathStepParameter()
{
    FUNCTION_ENTRY("~PlanPathStepParameter");
    FUNCTION_EXIT;
}


void PlanPathStepParameter::addToParameterList(CPlanStepParameterListCtrlModel& listCtrlModel)
{
    FUNCTION_ENTRY("addToParameterList");

    std::string planPath;
    try
    {
        planPath = getPlanPath();
    }
    catch(TA_Base_Core::TransactiveException&)
    {
        // Couldn't retrieve plan path. Try adding the parameter with the plan path set
        // to an error message.
        listCtrlModel.notifyErrorOccur(PARAMETER_ADD_ERROR_NOTIFY);
        FUNCTION_EXIT;
        return;
    }
    listCtrlModel.addParameter(getName().c_str(), planPath.c_str(), this);

    FUNCTION_EXIT;
}


/*
TA_Base_App::CPlanStepParameterEditDlg* PlanPathStepParameter::getEditDialog(bool canEdit)
{
    FUNCTION_ENTRY( "getEditDialog" );
    FUNCTION_EXIT;
    return new TA_Base_App::CPlanStepParameterPlanPathEditDlg(*this,canEdit);
}
*/


void PlanPathStepParameter::setPlanNodeId(TreeNodeId planNodeId)
{
    FUNCTION_ENTRY("setPlanNodeId");

    //TA_ASSERT(getParentStep()->getParentPlan()->isCopiedPlan(),"Attempt to update the factory plan.");
    m_planNodeId = planNodeId;

    FUNCTION_EXIT;
}


std::string PlanPathStepParameter::getPlanPath()
{
    FUNCTION_ENTRY("getPlanPath");
    FUNCTION_EXIT;
    return PlanAgentAccess::getInstance().getNodePath(m_planNodeId);
}


