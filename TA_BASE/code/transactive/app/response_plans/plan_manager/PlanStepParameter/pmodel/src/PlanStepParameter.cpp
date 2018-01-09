//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanStepParameter.cpp $
// @author:  Bart Golab
// @version: $Revision: #1 $
//
// Last modification: $DateTime: 2013/09/03 14:11:12 $
// Last modified by:  $Author: CM $
//
// <description>

#include "core/utilities/src/TAAssert.h"
//#include "PlanStepParameterEditDlg.h"
#include "app/response_plans/plan_manager/src/PlanManagerPModelConsts.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/PlanStepParameter.h"
#include "app/response_plans/plan_manager/src/PlanStep.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/PlanStepParameterListCtrlModel.h"
#include "app/response_plans/plan_manager/PlanStepParameter/pmodel/src/PlanStepParameterConstantsDef.h"
#include "app/response_plans/plan_manager/PlanStepParameter/src/IGUIApplicationControllerHelper.h"

using namespace TA_Base_App;
using TA_Base_Bus::AbstractPModel;
using TA_Base_Bus::ITransActiveWinApp;

/////////////////////////////////////////////////////////////////////////////
// PlanStepParameter

PlanStepParameter::PlanStepParameter(PlanStep& parent, std::string name)
    : AbstractPModel(IGUIApplicationControllerHelper::getAppController(), ""),
      m_name(name),
      m_parent(parent)
{
    FUNCTION_ENTRY("PlanStepParameter");
    FUNCTION_EXIT;
}


PlanStepParameter::~PlanStepParameter()
{
    FUNCTION_ENTRY("~PlanStepParameter");
    FUNCTION_EXIT;
}


const std::string& PlanStepParameter::getName() const
{
    FUNCTION_ENTRY("getName");
    FUNCTION_EXIT;
    return m_name;
}


void PlanStepParameter::editParameter()
{
    FUNCTION_ENTRY("editParameter");

    /*
    CPlanStepParameterEditDlg* dlg = getEditDialog(true);
    TA_ASSERT(dlg!=NULL,"No dialog defined for step parameter editing.");
    dlg->DoModal();
    delete dlg;
    */
    notifyViews(EDIT_PARAMETER_NOTIFY);

    FUNCTION_EXIT;
}


void PlanStepParameter::viewParameter()
{
    FUNCTION_ENTRY("viewParameter");

    /*
    CPlanStepParameterEditDlg* dlg = getEditDialog(false);
    TA_ASSERT(dlg!=NULL,"No dialog defined for step parameter viewing.");
    dlg->DoModal();
    delete dlg;
    */
    notifyViews(VIEW_PARAMETER_NOTIFY);

    FUNCTION_EXIT;
}


