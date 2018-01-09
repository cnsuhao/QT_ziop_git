#include "app/response_plans/plan_manager/src/StdAfx.h"
#include "app/response_plans/plan_manager/src/Resource.h"
#include "core/utilities/src/DebugUtil.h"
#include "PModelLabelMaker.h"
#include "HelperMacro.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;
using namespace TA_Base_Core;


/////////////////////////////////////////////////////////////////////////////
// CPModelLabelMaker

PModelLabelMaker::PModelLabelMaker()
{
    FUNCTION_ENTRY("PModelLabelMaker");
    FUNCTION_EXIT;
}


PModelLabelMaker::~PModelLabelMaker()
{
    FUNCTION_ENTRY("~PModelLabelMaker");
    FUNCTION_EXIT;
}


/////////////////////////////////////////////////////////////////////////////
// CPModelLabelMaker member functions

std::string PModelLabelMaker::getLabel(unsigned int labelResourceID, const char *defaultLabel)
{
    FUNCTION_ENTRY("getLabel");

    CString label;

    if(label.LoadString(labelResourceID) == 0)
    {
        label = defaultLabel;
    }

    FUNCTION_EXIT;
    return SCOPEDWTOM(LPCTSTR(label));
}


std::string PModelLabelMaker::getAllPlanTreeRootNodeLabel()
{
    FUNCTION_ENTRY("getAllPlanTreeRootNodeLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_TREE_ALLPLANS_ROOT_LABEL, "All Plans");
}


std::string PModelLabelMaker::getApprovedPlanTreeRootNodeLabel()
{
    FUNCTION_ENTRY("getApprovedPlanTreeRootNodeLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_TREE_APPROVEDPLANS_ROOT_LABEL, "Approved Plans");
}


std::string PModelLabelMaker::getUnapprovedPlanTreeRootNodeLabel()
{
    FUNCTION_ENTRY("getUnapprovedPlanTreeRootNodeLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_TREE_UNAPPROVEDPLANS_ROOT_LABEL, "Unapproved Plans");
}


std::string PModelLabelMaker::getPlanGeneralDetailTabLabel()
{
    FUNCTION_ENTRY("getPlanGeneralDetailTabLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_PLAN_GENERAL_DETAIL_TAB_LABEL, "General");
}


std::string PModelLabelMaker::getPlanStepDetailTabLabel()
{
    FUNCTION_ENTRY("getPlanStepDetailTabLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_PLAN_STEP_DETAIL_TAB_LABEL, "Steps");
}


std::string PModelLabelMaker::getActivePlanListPlanColumnLabel()
{
    FUNCTION_ENTRY("getActivePlanListPlanColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_PLAN_COLUMN, "Plan");
}


std::string PModelLabelMaker::getActivePlanListCategoryColumnLabel()
{
    FUNCTION_ENTRY("getActivePlanListCategoryColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_CATEGORY_COLUMN, "Category");
}


std::string PModelLabelMaker::getActivePlanListInstanceColumnLabel()
{
    FUNCTION_ENTRY("getActivePlanListInstanceColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_INSTANCE_COLUMN, "Instance");
}


std::string PModelLabelMaker::getActivePlanListOwnerColumnLabel()
{
    FUNCTION_ENTRY("getActivePlanListOwnerColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_OWNER_COLUMN, "Owner");
}


std::string PModelLabelMaker::getActivePlanListLocationColumnLabel()
{
    FUNCTION_ENTRY("getActivePlanListLocationColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_LOCATION_COLUMN, "Location");
}


std::string PModelLabelMaker::getActivePlanListActivationColumnLabel()
{
    FUNCTION_ENTRY("getActivePlanListActivationColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_ACTIVATION_COLUMN, "Activation");
}


std::string PModelLabelMaker::getActivePlanListStateColumnLabel()
{
    FUNCTION_ENTRY("getActivePlanListStateColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_STATE_COLUMN, "State");
}


std::string PModelLabelMaker::getActivePlanListStepColumnLabel()
{
    FUNCTION_ENTRY("getActivePlanListStepColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_STEP_COLUMN, "Step");
}


std::string PModelLabelMaker::getActivePlanListRemarksColumnLabel()
{
    FUNCTION_ENTRY("getActivePlanListRemarksColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_REMARKS_COLUMN, "Remarks");
}


std::string PModelLabelMaker::getStepListStepColumnLabel()
{
    FUNCTION_ENTRY("getStepListStepColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_LIST_STEP_COLUMN, "Step");
}


std::string PModelLabelMaker::getStepListNameColumnLabel()
{
    FUNCTION_ENTRY("getStepListNameColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_LIST_NAME_COLUMN, "Name");

}


std::string PModelLabelMaker::getStepListTypeColumnLabel()
{
    FUNCTION_ENTRY("getStepListTypeColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_LIST_TYPE_COLUMN, "Type");

}


std::string PModelLabelMaker::getStepListDescriptionColumnLabel()
{
    FUNCTION_ENTRY("getStepListDescriptionColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_LIST_DESCRIPTION_COLUMN, "Description");

}


std::string PModelLabelMaker::getStepListSkipColumnLabel()
{
    FUNCTION_ENTRY("getStepListSkipColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_LIST_SKIP_COLUMN, "Skip");

}


std::string PModelLabelMaker::getStepListIgnoreFailureColumnLabel()
{
    FUNCTION_ENTRY("getStepListIgnoreFailureColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_LIST_IGNORE_FAILURE_COLUMN, "Ignore Failure");

}

std::string PModelLabelMaker::getStepListNoWaitColumnLabel()
{
    return getLabel(IDS_STEP_LIST_NOWAIT_COLUMN, "No Wait");
}

std::string PModelLabelMaker::getStepListDelayColumnLabel()
{
    FUNCTION_ENTRY("getStepListDelayColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_LIST_DELAY_COLUMN, "Delay\n(mm:ss)");

}


std::string PModelLabelMaker::getStepParameterListNameColumnLabel()
{
    FUNCTION_ENTRY("getStepParameterListNameColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_PARAMETER_LIST_NAME_COLUMN, "Parameter Name");

}


std::string PModelLabelMaker::getStepParameterListValueColumnLabel()
{
    FUNCTION_ENTRY("getStepParameterListValueColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_PARAMETER_LIST_VALUE_COLUMN, "Parameter Value");
}


std::string PModelLabelMaker::getNodeListNameColumnLabel()
{
    FUNCTION_ENTRY("getNodeListNameColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_NODE_LIST_NAME_COLUMN, "Name");
}


std::string PModelLabelMaker::getNodeListTypeColumnLabel()
{
    FUNCTION_ENTRY("getNodeListTypeColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_NODE_LIST_TYPE_COLUMN, "Type");
}


std::string PModelLabelMaker::getNodeListStatusColumnLabel()
{
    FUNCTION_ENTRY("getNodeListStatusColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_NODE_LIST_STATUS_COLUMN, "Status");
}


std::string PModelLabelMaker::getTisMessageListTypeColumnLabel()
{
    FUNCTION_ENTRY("getTisMessageListTypeColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_TISMSG_LIST_TYPE_COLUMN, "Type");
}


std::string PModelLabelMaker::getTisMessageListIdColumnLabel()
{
    FUNCTION_ENTRY("getTisMessageListIdColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_TISMSG_LIST_ID_COLUMN, "ID");
}


std::string PModelLabelMaker::getTisMessageListMessageColumnLabel()
{
    FUNCTION_ENTRY("getTisMessageListMessageColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_TISMSG_LIST_MESSAGE_COLUMN, "Message");
}


std::string PModelLabelMaker::getTisMessageListPriorityColumnLabel()
{
    FUNCTION_ENTRY("getTisMessageListPriorityColumnLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_TISMSG_LIST_PRIORITY_COLUMN, "Priority");
}


std::string PModelLabelMaker::getYesNoLabel(bool affirmative)
{
    FUNCTION_ENTRY("getYesNoLabel");

    if(affirmative)
    {
        FUNCTION_EXIT;
        return getLabel(IDS_YES, "Yes");
    }

    FUNCTION_EXIT;
    return getLabel(IDS_NO, "No");
}


std::string PModelLabelMaker::getTrueFalseLabel(bool condition)
{
    FUNCTION_ENTRY("getTrueFalseLabel");

    if(condition)
    {
        FUNCTION_EXIT;
        return getLabel(IDS_TRUE, "True");
    }

    FUNCTION_EXIT;
    return getLabel(IDS_FALSE, "False");
}


std::string PModelLabelMaker::getNewActivePlanInstanceLabel()
{
    FUNCTION_ENTRY("getNewActivePlanInstanceLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_NEW_ACTIVE_PLAN_INSTANCE, "None");
}


std::string PModelLabelMaker::getRootNodeTypeLabel()
{
    FUNCTION_ENTRY("getRootNodeTypeLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_NODE_TYPE_ROOT, "Root");
}


std::string PModelLabelMaker::getCategoryNodeTypeLabel()
{
    FUNCTION_ENTRY("getCategoryNodeTypeLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_NODE_TYPE_CATEGORY, "Category");
}


std::string PModelLabelMaker::getPlanNodeTypeLabel()
{
    FUNCTION_ENTRY("getPlanNodeTypeLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_NODE_TYPE_PLAN, "Plan");
}


std::string PModelLabelMaker::getEmptyCategoryLabel()
{
    FUNCTION_ENTRY("getEmptyCategoryLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_CATEGORY_EMPTY, "Empty");
}


std::string PModelLabelMaker::getNonEmptyCategoryLabel()
{
    FUNCTION_ENTRY("getNonEmptyCategoryLabel");
    FUNCTION_EXIT;
    return getLabel(IDS_CATEGORY_NONEMPTY, "Populated");
}


std::string PModelLabelMaker::getCategoryHeadingPrefix()
{
    FUNCTION_ENTRY("getCategoryHeadingPrefix");
    FUNCTION_EXIT;
    return getLabel(IDS_CATEGORY_HEADING_PREFIX, "Category");
}


std::string PModelLabelMaker::getPlanHeadingPrefix()
{
    FUNCTION_ENTRY("getPlanHeadingPrefix");
    FUNCTION_EXIT;
    return getLabel(IDS_PLAN_HEADING_PREFIX, "Plan");
}


std::string PModelLabelMaker::getStepHeadingPrefix()
{
    FUNCTION_ENTRY("getStepHeadingPrefix");
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_HEADING_PREFIX, "Step");
}


std::string PModelLabelMaker::getDecisionStepDialogTitlePrefix()
{
    FUNCTION_ENTRY("getDecisionStepDialogTitlePrefix");
    FUNCTION_EXIT;
    return getLabel(IDS_DECISION_STEP_DIALOG_TITLE_PREFIX, "Decision");
}


std::string PModelLabelMaker::getLogEntryStepDialogTitlePrefix()
{
    FUNCTION_ENTRY("getLogEntryStepDialogTitlePrefix");
    FUNCTION_EXIT;
    return getLabel(IDS_LOGENTRY_STEP_DIALOG_TITLE_PREFIX, "Log");
}


std::string PModelLabelMaker::getPromptStepDialogTitlePrefix()
{
    FUNCTION_ENTRY("getPromptStepDialogTitlePrefix");
    FUNCTION_EXIT;
    return getLabel(IDS_PROMPT_STEP_DIALOG_TITLE_PREFIX, "Prompt");
}


std::string PModelLabelMaker::getRunPlanStepDialogTitlePrefix()
{
    FUNCTION_ENTRY("getRunPlanStepDialogTitlePrefix");
    FUNCTION_EXIT;
    return getLabel(IDS_RUN_PLAN_STEP_DIALOG_TITLE_PREFIX, "Run Plan");
}


std::string PModelLabelMaker::getLaunchGuiAppStepDialogTitlePrefix()
{
    FUNCTION_ENTRY("getLaunchGuiAppStepDialogTitlePrefix");
    FUNCTION_EXIT;
    return getLabel(IDS_LAUNCH_GUI_APP_STEP_DIALOG_TITLE_PREFIX, "Launch GUI Application");
}


std::string PModelLabelMaker::getLaunchSchematicStepDialogTitlePrefix()
{
    FUNCTION_ENTRY("getLaunchSchematicStepDialogTitlePrefix");
    FUNCTION_EXIT;
    return getLabel(IDS_LAUNCH_SCHEMATIC_STEP_DIALOG_TITLE_PREFIX, "Launch Schematic");
}


std::string PModelLabelMaker::getFailureSummaryStepDialogTitlePrefix()
{
    FUNCTION_ENTRY("getFailureSummaryStepDialogTitlePrefix");
    FUNCTION_EXIT;
    return getLabel(IDS_FAILURE_SUMMARY_STEP_DIALOG_TITLE_PREFIX, "Failed Step Summary");
}
