/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/GuiAppTypeStepParameter.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  **/

#if defined(_MSC_VER)
#pragma warning(disable:4786)
#endif // defined (_MSC_VER)

#if defined (STEPTYPE_LAUNCH_GUI_APP)

#include <sstream>
#include "core/utilities/src/DebugUtil.h"
#include "core/data_access_interface/src/IGui.h"
#include "core/data_access_interface/src/GuiAccessFactory.h"
#include "core/exceptions/src/TransactiveException.h"
#include "app/response_plans/plan_manager/src/PlanManagerPModelConsts.h"
#include "app/response_plans/plan_manager/src/PlanStep.h"
#include "app/response_plans/plan_manager/src/PlanNode.h"
#include "PlanStepParameterListCtrlModel.h"
#include "app/response_plans/plan_manager/src/Resource.h"
#include "GuiAppTypeStepParameter.h"
//#include "MessageBox.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;


/////////////////////////////////////////////////////////////////////////////
// PlanStepParameter

GuiAppTypeStepParameter::GuiAppTypeStepParameter(PlanStep& parent, std::string name, unsigned long appType, bool isHidden) :
    NumericStepParameter(parent, name, appType, isHidden)
{
    FUNCTION_ENTRY("GuiAppTypeStepParameter");
    FUNCTION_EXIT;
}


GuiAppTypeStepParameter::GuiAppTypeStepParameter(PlanStep& parent, const GuiAppTypeStepParameter& parameter) :
    NumericStepParameter(parent, parameter)
{
    FUNCTION_ENTRY("GuiAppTypeStepParameter");
    FUNCTION_EXIT;
}


GuiAppTypeStepParameter::GuiAppTypeStepParameter(const GuiAppTypeStepParameter& parameter) :
    NumericStepParameter(parameter)
{
    FUNCTION_ENTRY("GuiAppTypeStepParameter");
    FUNCTION_EXIT;
}


GuiAppTypeStepParameter::~GuiAppTypeStepParameter()
{
    FUNCTION_ENTRY("~GuiAppTypeStepParameter");
    FUNCTION_EXIT;
}


void GuiAppTypeStepParameter::addToParameterList(CPlanStepParameterListCtrlModel &listCtrl)
{
    FUNCTION_ENTRY("addToParameterList");

    // Convert application key to application name before inserting into the parameter list
    listCtrl.addParameter(getName().c_str(), (isHiddenValue() ? "" : getApplicationName().c_str()), this);

    FUNCTION_EXIT;
}


std::string GuiAppTypeStepParameter::getApplicationName()
{
    FUNCTION_ENTRY("getApplicationName");

    try
    {
        TA_Base_Core::IGui *gui = TA_Base_Core::GuiAccessFactory::getInstance().getGui(getNumericValue());

        FUNCTION_EXIT;
        return gui->getFullName();
    }
    catch(TA_Base_Core::TransactiveException& ex)
    {
        //MessageBox::error(IDS_RETRIEVE_APPLICATION_NAME, ex);
    }

    FUNCTION_EXIT;
    return "Undefined application type";
}


#endif // defined (STEPTYPE_LAUNCH_GUI_APP)
