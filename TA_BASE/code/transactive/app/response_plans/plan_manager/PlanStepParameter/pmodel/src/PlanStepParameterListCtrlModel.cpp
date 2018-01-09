#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/PlanStepParameterListCtrlModel.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/PlanStepParameterConstantsDef.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/PlanStepParameter.h"
#include "app/response_plans/plan_manager/PlanStepParameter/src/IGUIApplicationControllerHelper.h"

using namespace TA_Base_App;
using TA_Base_Bus::AbstractPModel;
using TA_Base_Bus::ITransActiveWinApp;

CPlanStepParameterListCtrlModel::CPlanStepParameterListCtrlModel(void)
    : AbstractPModel(IGUIApplicationControllerHelper::getAppController(), "")
{
}

CPlanStepParameterListCtrlModel::~CPlanStepParameterListCtrlModel(void)
{
}

void CPlanStepParameterListCtrlModel::addParameter(const std::string& parameterName,
                                                   const std::string& parameterValue,
                                                   PlanStepParameter* planStepParameter)
{
    m_currentAddedParameter = AddedParameterInfo(parameterName, parameterValue, planStepParameter);
    notifyViews(PARAMETER_ADD_NOTIFY);
}

CPlanStepParameterListCtrlModel::AddedParameterInfo CPlanStepParameterListCtrlModel::getCurrentAddedParameterInfo()
{
    return m_currentAddedParameter;
}

void CPlanStepParameterListCtrlModel::setCurrentOperationPlanStepParameter(PlanStepParameter* stepParameter)
{
    m_currentOperationStepParameter = stepParameter;
}

PlanStepParameter* CPlanStepParameterListCtrlModel::getCurrentOperationPlanStepParameter()
{
    return m_currentOperationStepParameter;
}

void TA_Base_App::CPlanStepParameterListCtrlModel::notifyErrorOccur(const char* errorType)
{
    notifyViews(errorType);
}
