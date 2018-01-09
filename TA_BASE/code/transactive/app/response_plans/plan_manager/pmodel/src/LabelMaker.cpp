//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/response_plans/plan_manager/src/LabelMaker.cpp $
// @author:  Bart Golab
// @version: $Revision: #2 $
//
// Last modification: $DateTime: 2016/11/21 13:03:08 $
// Last modified by:  $Author: hoa.le $
//
// <description>


#include "core/utilities/src/DebugUtil.h"
#include "LabelMaker.h"
#include "StringDefn.h"

using namespace TA_Base_App;
using namespace TA_Base_Core;


/////////////////////////////////////////////////////////////////////////////
// CLabelMaker

LabelMaker::LabelMaker()
{
    FUNCTION_ENTRY( "LabelMaker" );
    FUNCTION_EXIT;
}


LabelMaker::~LabelMaker()
{
    FUNCTION_ENTRY( "~LabelMaker" );
    FUNCTION_EXIT;
}


/////////////////////////////////////////////////////////////////////////////
// CLabelMaker member functions

std::string LabelMaker::getLabel(const char *definedLabel, const char *defaultLabel)
{
    FUNCTION_ENTRY( "getLabel" );
	std::string label = definedLabel;
	if (label.empty())
		label = defaultLabel;
    FUNCTION_EXIT;
	return label;
}


std::string LabelMaker::getAllPlanTreeRootNodeLabel()
{
    FUNCTION_ENTRY( "getAllPlanTreeRootNodeLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_TREE_ALLPLANS_ROOT_LABEL, "All Plans");
}


std::string LabelMaker::getApprovedPlanTreeRootNodeLabel()
{
    FUNCTION_ENTRY( "getApprovedPlanTreeRootNodeLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_TREE_APPROVEDPLANS_ROOT_LABEL, "Approved Plans");
}


std::string LabelMaker::getUnapprovedPlanTreeRootNodeLabel()
{
    FUNCTION_ENTRY( "getUnapprovedPlanTreeRootNodeLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_TREE_UNAPPROVEDPLANS_ROOT_LABEL, "Unapproved Plans");
}


std::string LabelMaker::getPlanGeneralDetailTabLabel()
{
    FUNCTION_ENTRY( "getPlanGeneralDetailTabLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_PLAN_GENERAL_DETAIL_TAB_LABEL, "General");
}


std::string LabelMaker::getPlanStepDetailTabLabel()
{
    FUNCTION_ENTRY( "getPlanStepDetailTabLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_PLAN_STEP_DETAIL_TAB_LABEL, "Steps");
}


std::string LabelMaker::getActivePlanListPlanColumnLabel()
{
    FUNCTION_ENTRY( "getActivePlanListPlanColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_PLAN_COLUMN, "Plan");
}


std::string LabelMaker::getActivePlanListCategoryColumnLabel()
{
    FUNCTION_ENTRY( "getActivePlanListCategoryColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_CATEGORY_COLUMN, "Category");
}


std::string LabelMaker::getActivePlanListInstanceColumnLabel()
{
    FUNCTION_ENTRY( "getActivePlanListInstanceColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_INSTANCE_COLUMN, "Instance");
}


std::string LabelMaker::getActivePlanListOwnerColumnLabel()
{
    FUNCTION_ENTRY( "getActivePlanListOwnerColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_OWNER_COLUMN, "Owner");
}


std::string LabelMaker::getActivePlanListLocationColumnLabel()
{
    FUNCTION_ENTRY( "getActivePlanListLocationColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_LOCATION_COLUMN, "Location");
}


std::string LabelMaker::getActivePlanListActivationColumnLabel()
{
    FUNCTION_ENTRY( "getActivePlanListActivationColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_ACTIVATION_COLUMN, "Activation");
}


std::string LabelMaker::getActivePlanListStateColumnLabel()
{
    FUNCTION_ENTRY( "getActivePlanListStateColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_STATE_COLUMN, "State");
}


std::string LabelMaker::getActivePlanListStepColumnLabel()
{
    FUNCTION_ENTRY( "getActivePlanListStepColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_STEP_COLUMN, "Step");
}


std::string LabelMaker::getActivePlanListRemarksColumnLabel()
{
    FUNCTION_ENTRY( "getActivePlanListRemarksColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_ACTIVE_PLAN_LIST_REMARKS_COLUMN, "Remarks");
}


std::string LabelMaker::getStepListStepColumnLabel()
{
    FUNCTION_ENTRY( "getStepListStepColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_LIST_STEP_COLUMN, "Step");
}


std::string LabelMaker::getStepListNameColumnLabel()
{
    FUNCTION_ENTRY( "getStepListNameColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_LIST_NAME_COLUMN, "Name");

}


std::string LabelMaker::getStepListTypeColumnLabel()
{
    FUNCTION_ENTRY( "getStepListTypeColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_LIST_TYPE_COLUMN, "Type");

}


std::string LabelMaker::getStepListDescriptionColumnLabel()
{
    FUNCTION_ENTRY( "getStepListDescriptionColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_LIST_DESCRIPTION_COLUMN, "Description");

}


std::string LabelMaker::getStepListSkipColumnLabel()
{
    FUNCTION_ENTRY( "getStepListSkipColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_LIST_SKIP_COLUMN, "Skip");

}


std::string LabelMaker::getStepListIgnoreFailureColumnLabel()
{
    FUNCTION_ENTRY( "getStepListIgnoreFailureColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_LIST_IGNORE_FAILURE_COLUMN, "Ignore Failure");

}

std::string LabelMaker::getStepListNoWaitColumnLabel()
{
    return getLabel(IDS_STEP_LIST_NOWAIT_COLUMN, "No Wait");
}

std::string LabelMaker::getStepListDelayColumnLabel()
{
    FUNCTION_ENTRY( "getStepListDelayColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_LIST_DELAY_COLUMN, "Delay\n(mm:ss)");

}


std::string LabelMaker::getStepParameterListNameColumnLabel()
{
    FUNCTION_ENTRY( "getStepParameterListNameColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_PARAMETER_LIST_NAME_COLUMN, "Parameter Name");

}


std::string LabelMaker::getStepParameterListValueColumnLabel()
{
    FUNCTION_ENTRY( "getStepParameterListValueColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_PARAMETER_LIST_VALUE_COLUMN, "Parameter Value");
}

// Reference from StringDefn.h instead
//std::string LabelMaker::getNodeListNameColumnLabel()
//{
//    FUNCTION_ENTRY( "getNodeListNameColumnLabel" );
//    FUNCTION_EXIT;
//    return getLabel(IDS_NODE_LIST_NAME_COLUMN, "Name");
//}
//
//
//std::string LabelMaker::getNodeListTypeColumnLabel()
//{
//    FUNCTION_ENTRY( "getNodeListTypeColumnLabel" );
//    FUNCTION_EXIT;
//    return getLabel(IDS_NODE_LIST_TYPE_COLUMN, "Type");
//}
//
//
//std::string LabelMaker::getNodeListStatusColumnLabel()
//{
//    FUNCTION_ENTRY( "getNodeListStatusColumnLabel" );
//    FUNCTION_EXIT;
//    return getLabel(IDS_NODE_LIST_STATUS_COLUMN, "Status");
//}


std::string LabelMaker::getTisMessageListTypeColumnLabel()
{
    FUNCTION_ENTRY( "getTisMessageListTypeColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_TISMSG_LIST_TYPE_COLUMN, "Type");
}


std::string LabelMaker::getTisMessageListIdColumnLabel()
{
    FUNCTION_ENTRY( "getTisMessageListIdColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_TISMSG_LIST_ID_COLUMN, "ID");
}


std::string LabelMaker::getTisMessageListMessageColumnLabel()
{
    FUNCTION_ENTRY( "getTisMessageListMessageColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_TISMSG_LIST_MESSAGE_COLUMN, "Message");
}


std::string LabelMaker::getTisMessageListPriorityColumnLabel()
{
    FUNCTION_ENTRY( "getTisMessageListPriorityColumnLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_TISMSG_LIST_PRIORITY_COLUMN, "Priority");
}


std::string LabelMaker::getYesNoLabel(bool affirmative)
{
    FUNCTION_ENTRY( "getYesNoLabel" );

    if (affirmative)
    {
        FUNCTION_EXIT;
        return getLabel(IDS_YES, "Yes");
    }

    FUNCTION_EXIT;
    return getLabel(IDS_NO, "No");
}


std::string LabelMaker::getTrueFalseLabel(bool condition)
{
    FUNCTION_ENTRY( "getTrueFalseLabel" );

    if (condition)
    {
        FUNCTION_EXIT;
        return getLabel(IDS_TRUE, "True");
    }

    FUNCTION_EXIT;
    return getLabel(IDS_FALSE, "False");
}


std::string LabelMaker::getNewActivePlanInstanceLabel()
{
    FUNCTION_ENTRY( "getNewActivePlanInstanceLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_NEW_ACTIVE_PLAN_INSTANCE, "None");
}


std::string LabelMaker::getRootNodeTypeLabel()
{
    FUNCTION_ENTRY( "getRootNodeTypeLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_NODE_TYPE_ROOT, "Root");
}


std::string LabelMaker::getCategoryNodeTypeLabel()
{
    FUNCTION_ENTRY( "getCategoryNodeTypeLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_NODE_TYPE_CATEGORY, "Category");
}


std::string LabelMaker::getPlanNodeTypeLabel()
{
    FUNCTION_ENTRY( "getPlanNodeTypeLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_NODE_TYPE_PLAN, "Plan");
}


std::string LabelMaker::getEmptyCategoryLabel()
{
    FUNCTION_ENTRY( "getEmptyCategoryLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_CATEGORY_EMPTY, "Empty");
}


std::string LabelMaker::getNonEmptyCategoryLabel()
{
    FUNCTION_ENTRY( "getNonEmptyCategoryLabel" );
    FUNCTION_EXIT;
    return getLabel(IDS_CATEGORY_NONEMPTY, "Populated");
}


std::string LabelMaker::getCategoryHeadingPrefix()
{
    FUNCTION_ENTRY( "getCategoryHeadingPrefix" );
    FUNCTION_EXIT;
    return getLabel(IDS_CATEGORY_HEADING_PREFIX, "Category");
}


std::string LabelMaker::getPlanHeadingPrefix()
{
    FUNCTION_ENTRY( "getPlanHeadingPrefix" );
    FUNCTION_EXIT;
    return getLabel(IDS_PLAN_HEADING_PREFIX, "Plan");
}


std::string LabelMaker::getStepHeadingPrefix()
{
    FUNCTION_ENTRY( "getStepHeadingPrefix" );
    FUNCTION_EXIT;
    return getLabel(IDS_STEP_HEADING_PREFIX, "Step");
}


std::string LabelMaker::getDecisionStepDialogTitlePrefix()
{
    FUNCTION_ENTRY( "getDecisionStepDialogTitlePrefix" );
    FUNCTION_EXIT;
    return getLabel(IDS_DECISION_STEP_DIALOG_TITLE_PREFIX, "Decision");
}


std::string LabelMaker::getLogEntryStepDialogTitlePrefix()
{
    FUNCTION_ENTRY( "getLogEntryStepDialogTitlePrefix" );
    FUNCTION_EXIT;
    return getLabel(IDS_LOGENTRY_STEP_DIALOG_TITLE_PREFIX, "Log");
}


std::string LabelMaker::getPromptStepDialogTitlePrefix()
{
    FUNCTION_ENTRY( "getPromptStepDialogTitlePrefix" );
    FUNCTION_EXIT;
    return getLabel(IDS_PROMPT_STEP_DIALOG_TITLE_PREFIX, "Prompt");
}


std::string LabelMaker::getRunPlanStepDialogTitlePrefix()
{
    FUNCTION_ENTRY( "getRunPlanStepDialogTitlePrefix" );
    FUNCTION_EXIT;
    return getLabel(IDS_RUN_PLAN_STEP_DIALOG_TITLE_PREFIX, "Run Plan");
}


std::string LabelMaker::getLaunchGuiAppStepDialogTitlePrefix()
{
    FUNCTION_ENTRY( "getLaunchGuiAppStepDialogTitlePrefix" );
    FUNCTION_EXIT;
    return getLabel(IDS_LAUNCH_GUI_APP_STEP_DIALOG_TITLE_PREFIX, "Launch GUI Application");
}


std::string LabelMaker::getLaunchSchematicStepDialogTitlePrefix()
{
    FUNCTION_ENTRY( "getLaunchSchematicStepDialogTitlePrefix" );
    FUNCTION_EXIT;
    return getLabel(IDS_LAUNCH_SCHEMATIC_STEP_DIALOG_TITLE_PREFIX, "Launch Schematic");
}


std::string LabelMaker::getFailureSummaryStepDialogTitlePrefix()
{
    FUNCTION_ENTRY( "getFailureSummaryStepDialogTitlePrefix" );
    FUNCTION_EXIT;
    return getLabel(IDS_FAILURE_SUMMARY_STEP_DIALOG_TITLE_PREFIX, "Failed Step Summary");
}


