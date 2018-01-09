#ifndef __PLAN_MANAGER_PMODEL_CONSTS_H
#define __PLAN_MANAGER_PMODEL_CONSTS_H

#include <vector>
#include "bus/response_plans/active_plans_display/src/ActivePlansDisplayCommonDefs.h"

namespace TA_Base_Bus
{
    typedef unsigned long TreeNodeId;
    typedef unsigned long PlanInstanceNumber;
    typedef unsigned long LocationKey;
}

namespace TA_Base_App
{

#define PLAN_TREE_VIEW_PMODEL "PlanTreeViewPModel"
#define PLAN_LIST_VIEW_PMODEL "PlanListViewPModel"
#define ALL_PLAN_TREE_PMODEL "AllPlanTreePModel"
#define APPROVED_PLAN_TREE_PMODEL "ApprovedPlanTreePModel"
#define UNAPPROVED_PLAN_TREE_PMODEL "UnapprovedPlanTreePModel"

#define MSG_ID_ACTIVE_PLAN_DETAIL_UPDATE "OnActivePlanDetailUpdate"
#define MSG_ID_PLAN_CONFIG_DATA_UPDATE "OnPlanConfigDataUpdate"
#define MSG_ID_PLAN_CUSTOMISATION_UPDATE "OnPlanCustomisationUpdate"
#define MSG_ID_RPARAM_EXECPLANID_UPDATE "OnRparamExecPlanIdUpdate"
#define MSG_ID_RPARAM_VIEWPLANID_UPDATE "OnRparamViewPlanIdUpdate"
#define MSG_ID_RPARAM_ASSOC_ALARM_UPDATE "OnRparamAssocAlarmIdUpdate"
#define MSG_ID_RPARAM_LOCKCATEGORY_UPDATE "OnRparamLockCategoryUpdate"
#define MSG_ID_RPARAM_SESSIONID_UPDATE "OnRparamSessionIdUpdate"

#define MSG_ID_PLAN_EDIT "OnUpdateTreeNodeCmd"
#define MSG_ID_PLAN_CONTROL "OnUpdateTreeNodeCmd"
#define MSG_ID_PLAN_APPROVE "OnUpdateTreeNodeCmd"
#define MSG_ID_PLAN_UNAPPROVE "OnUpdateTreeNodeCmd"
#define MSG_ID_PLAN_SCHEDULE "OnUpdateTreeNodeCmd"
#define MSG_ID_PLAN_NEW "OnUpdateTreeNodeCmd"
#define MSG_ID_PLAN_DELETE "OnUpdateTreeNodeCmd"
#define MSG_ID_PLAN_CUT "OnUpdateTreeNodeCmd"
#define MSG_ID_PLAN_COPY "OnUpdateTreeNodeCmd"
#define MSG_ID_PLAN_PASTE "OnUpdateTreeNodeCmd"
#define MSG_ID_CATEGORY_NEW "OnUpdateTreeNodeCmd"
#define MSG_ID_CATEGORY_DELETE "OnUpdateTreeNodeCmd"
#define MSG_ID_CATEGORY_RENAME "OnUpdateTreeNodeCmd"
#define MSG_ID_CATEGORY_OR_PLAN_DELETE "OnUpdateTreeNodeCmd"
#define MSG_ID_IMPORT_PLAN "OnUpdateTreeNodeCmd"
#define MSG_ID_EXPORT_PLAN "OnUpdateTreeNodeCmd"
#define MSG_ID_PLAN_VISIBLE "OnUpdateTreeNodeCmd"
#define MSG_ID_PLAN_INVISIBLE "OnUpdateTreeNodeCmd"

#define CMD_ID_PLAN_EDIT "OnEditPlan"
#define CMD_ID_PLAN_CONTROL "OnControlPlan"
#define CMD_ID_PLAN_APPROVE "OnApprovePlan"
#define CMD_ID_PLAN_UNAPPROVE "OnUnapprovePlan"
#define CMD_ID_PLAN_SCHEDULE "OnSchedulePlan"
#define CMD_ID_PLAN_NEW "OnNewPlan"
#define CMD_ID_PLAN_DELETE "OnDeletePlan"
#define CMD_ID_PLAN_CUT "OnCutPlan"
#define CMD_ID_PLAN_COPY "OnCopyPlan"
#define CMD_ID_PLAN_PASTE "OnPastePlan"
#define CMD_ID_CATEGORY_NEW "OnNewCategory"
#define CMD_ID_CATEGORY_DELETE "OnDeleteCategory"
#define CMD_ID_CATEGORY_OR_PLAN_DELETE "OnDeleteCategoryOrPlan"
#define CMD_ID_STEP_EDIT "OnEditStep"
#define CMD_ID_PLAN_VISIBLE "OnMakePlanVisible"
#define CMD_ID_PLAN_INVISIBLE "OnMakePlanInvisible"
#define CMD_ID_IMPORT_PLAN "OnImportPlan"
#define CMD_ID_EXPORT_PLAN "OnExportPlan"

using TA_Base_Bus::TreeNodeId;
using TA_Base_Bus::PlanInstanceNumber;

// Forwards
class TreeNode;
class RootNode;
class PlanStep;
class PlanStepParameter;
class PlanInstance;

// Types
typedef unsigned long PlanStepId;
typedef unsigned long PlanStepNumber;
typedef unsigned long LocationKey;

typedef std::vector<TreeNodeId> TreeNodeIdList;
typedef std::vector<TreeNode *> TreeNodeList;
typedef std::vector<RootNode *> RootNodeList;
typedef std::vector<PlanStep *> PlanStepList;
typedef std::vector<PlanStepParameter *> PlanStepParameterList;
typedef std::vector<PlanInstance *> PlanInstanceList;

// Constants
const unsigned int MIN_INTERACTIVE_STEP_TIMEOUT = 10;   // seconds
const unsigned int MAX_INTERACTIVE_STEP_TIMEOUT = 300;  // seconds
const unsigned int MIN_STEP_DELAY_MINUTE = 0;
const unsigned int MAX_STEP_DELAY_MINUTE = 59;
const unsigned int MIN_STEP_DELAY_SECOND = 0;
const unsigned int MAX_STEP_DELAY_SECOND = 59;
const unsigned int MIN_PA_MESSAGE_INTERVAL = 0;//seconds
const unsigned int MAX_PA_MESSAGE_INTERVAL = 10;//seconds
const unsigned int REREGISTRATION_TIMEOUT_SECONDS = 30;//seconds
const char *const STEP_DELAY_EDIT_MASK = "[0-5][0-9]:[0-5][0-9]";  // mm:ss (00:00 -> 59:59)
const char *const RPARAM_LOCKCATEGORY = "LockCategory";
const char *const RPARAM_FWDEXECPLANID = "FwdExecPlanId";
const char *const RPARAM_FWDVIEWPLANID = "FwdViewPlanId";
const char* const RPARAM_FWDASSOCALARMID = "FwsAssocAlarmId";
const char *const RPARAM_FLOWCHARTDIR = "FlowchartDir";
const char *const RPARAM_REPORTTEMPLATE = "ReportTemplate";
const char *const RPARAM_MAXCATEGORY = "MaxCategoryDepth";

static const std::string PA_STATION_MESSAGE_TYPE = "PA Station Message";
static const std::string PA_TRAIN_MESSAGE_TYPE    = "Train Messages";
static const std::string NO_MESSAGE = "No message selected";

// Plan tree control item images, as per IDB_TREE_NODE_IMAGE_LIST_BMP
typedef enum
{
    TNIT_CLOSED_FOLDER = 0,
    TNIT_OPEN_FOLDER,
    TNIT_APPROVED_PLAN,
    TNIT_UNAPPROVED_PLAN,
    TNIT_ROOT
}
ETreeNodeImageType;

// Plan step list item images, as per IDB_STEP_EXEC_STATE_IMAGE_LIST_BMP
typedef enum
{
    PSIT_NEXT_STEP = 0,
    PSIT_PAUSED_STEP,
    PSIT_EXECUTED_STEP,
    PSIT_FAILED_STEP,
    PSIT_SKIPPED_STEP,
    PSIT_UNEXECUTED_STEP,
    PSIT_DELAYED_STEP
}
EPlanStepImageType;

struct PlanInstanceId
{
    PlanInstanceId(PlanInstanceNumber instance, LocationKey location) :
        m_instance(instance), m_location(location)
    {
    }

    bool operator==(const PlanInstanceId &other) const
    {
        return (m_instance == other.m_instance && m_location == other.m_location);
    }

    bool operator<(const PlanInstanceId &other) const
    {
        if (m_location < other.m_location)
        {
            return true;
        }

        if (m_location > other.m_location)
        {
            return false;
        }

        return (m_instance < other.m_instance);
    }

    bool operator>(const PlanInstanceId &other) const
    {
        return !(*this == other || *this < other);
    }

    PlanInstanceNumber m_instance;
    LocationKey m_location;
};

typedef enum { STEP_NOT_USE = 0, STEP_USE} EStepUseType;
}

#endif