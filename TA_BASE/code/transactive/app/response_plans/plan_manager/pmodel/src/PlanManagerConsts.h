#ifndef PLAN_MANAGER_PMODEL_CONSTS_H
#define PLAN_MANAGER_PMODEL_CONSTS_H

#include <vector>
#include "boost/shared_ptr.hpp"
//#include "bus/response_plans/active_plans_display/src/ActivePlansDisplayCommonDefs.h"
#include "PlanServiceConsts.h"

//#include "TreeNode.h";
//#include "RootNode.h";
//#include "PlanStep.h";
//#include "PlanStepParameter.h";
//#include "PlanInstance.h";

//namespace TA_Base_Bus
//{
//    typedef unsigned long TreeNodeId;
//    typedef unsigned long PlanInstanceNumber;
//    typedef unsigned long LocationKey;
//}

namespace TA_Base_App
{
	const unsigned int APP_NOTIFY_OPERATION_ERROR = 1;
	//const unsigned int APP_NOTIFY_OPERATION_ERROR = 1;

	#define WM_USER	0x0400
	#define TA_MSG_INIT_CORBA (WM_USER + 1001)

	#define PMODEL_DATA_CHANGE_SESSION_ID "session_id_change"
	#define PMODEL_DATA_INIT_CORBA "init_corba"

	#define PMODEL_PLAN_MANAGER_MAINFRAME "PlanManagerMainFrameModel"
	#define PMODEL_ALLPLANTREEFRAME	"AllPlanModel"
	#define PMODEL_APPROVEDPLANTREEFRAME	"ApprovedPlanModel"
	#define PMODEL_UNAPPROVEDPLANTREEFRAME	"UnapprovedPlanModel"
	#define PMODEL_ACTIVEPLANLISTFRAME	"ActivePlanModel"

	//#define MSG_ID_ACTIVE_PLAN_DETAIL_UPDATE "OnActivePlanDetailUpdate"
	//#define MSG_ID_PLAN_CONFIG_DATA_UPDATE "OnPlanConfigDataUpdate"
	//#define MSG_ID_PLAN_CUSTOMISATION_UPDATE "OnPlanCustomisationUpdate"
	//#define MSG_ID_RPARAM_EXECPLANID_UPDATE "OnRparamExecPlanIdUpdate"
	//#define MSG_ID_RPARAM_VIEWPLANID_UPDATE "OnRparamViewPlanIdUpdate"
	//#define MSG_ID_RPARAM_ASSOC_ALARM_UPDATE "OnRparamAssocAlarmIdUpdate"
	//#define MSG_ID_RPARAM_LOCKCATEGORY_UPDATE "OnRparamLockCategoryUpdate"
	//#define MSG_ID_RPARAM_SESSIONID_UPDATE "OnRparamSessionIdUpdate"

	//#define MSG_ID_PLAN_EDIT "OnUpdateTreeNodeCmd"
	//#define MSG_ID_PLAN_CONTROL "OnUpdateTreeNodeCmd"
	//#define MSG_ID_PLAN_APPROVE "OnUpdateTreeNodeCmd"
	//#define MSG_ID_PLAN_UNAPPROVE "OnUpdateTreeNodeCmd"
	//#define MSG_ID_PLAN_SCHEDULE "OnUpdateTreeNodeCmd"
	//#define MSG_ID_PLAN_NEW "OnUpdateTreeNodeCmd"
	//#define MSG_ID_PLAN_DELETE "OnUpdateTreeNodeCmd"
	//#define MSG_ID_PLAN_CUT "OnUpdateTreeNodeCmd"
	//#define MSG_ID_PLAN_COPY "OnUpdateTreeNodeCmd"
	//#define MSG_ID_PLAN_PASTE "OnUpdateTreeNodeCmd"
	//#define MSG_ID_CATEGORY_NEW "OnUpdateTreeNodeCmd"
	//#define MSG_ID_CATEGORY_DELETE "OnUpdateTreeNodeCmd"
	//#define MSG_ID_CATEGORY_RENAME "OnUpdateTreeNodeCmd"
	//#define MSG_ID_CATEGORY_OR_PLAN_DELETE "OnUpdateTreeNodeCmd"
	//#define MSG_ID_IMPORT_PLAN "OnUpdateTreeNodeCmd"
	//#define MSG_ID_EXPORT_PLAN "OnUpdateTreeNodeCmd"
	//#define MSG_ID_PLAN_VISIBLE "OnUpdateTreeNodeCmd"
	//#define MSG_ID_PLAN_INVISIBLE "OnUpdateTreeNodeCmd"

	//#define CMD_ID_PLAN_EDIT "OnEditPlan"
	//#define CMD_ID_PLAN_CONTROL "OnControlPlan"
	//#define CMD_ID_PLAN_APPROVE "OnApprovePlan"
	//#define CMD_ID_PLAN_UNAPPROVE "OnUnapprovePlan"
	//#define CMD_ID_PLAN_SCHEDULE "OnSchedulePlan"
	//#define CMD_ID_PLAN_NEW "OnNewPlan"
	//#define CMD_ID_PLAN_DELETE "OnDeletePlan"
	//#define CMD_ID_PLAN_CUT "OnCutPlan"
	//#define CMD_ID_PLAN_COPY "OnCopyPlan"
	//#define CMD_ID_PLAN_PASTE "OnPastePlan"
	//#define CMD_ID_CATEGORY_NEW "OnNewCategory"
	//#define CMD_ID_CATEGORY_DELETE "OnDeleteCategory"
	//#define CMD_ID_CATEGORY_OR_PLAN_DELETE "OnDeleteCategoryOrPlan"
	//#define CMD_ID_STEP_EDIT "OnEditStep"
	//#define CMD_ID_PLAN_VISIBLE "OnMakePlanVisible"
	//#define CMD_ID_PLAN_INVISIBLE "OnMakePlanInvisible"
	//#define CMD_ID_IMPORT_PLAN "OnImportPlan"
	//#define CMD_ID_EXPORT_PLAN "OnExportPlan"

	//using TA_Base_Bus::TreeNodeId;
	//using TA_Base_Bus::PlanInstanceNumber;

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
	typedef unsigned long TreeNodeId;
	typedef unsigned long PlanInstanceNumber;

	typedef std::vector<TreeNodeId> TreeNodeIdList;
	typedef std::vector<TreeNode *> TreeNodeList;
	typedef std::vector<RootNode *> RootNodeList;
	typedef std::vector<PlanStep *> PlanStepList;
	typedef std::vector<PlanStepParameter *> PlanStepParameterList;
	typedef boost::shared_ptr<PlanInstance> PlanInstanceSharedPtr;
	typedef std::vector< PlanInstanceSharedPtr > PlanInstanceList;

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
	const char* const STEP_DELAY_EDIT_MASK = "[0-5][0-9]:[0-5][0-9]";  // mm:ss (00:00 -> 59:59)


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

	static const std::string ICON_CLOSED_FOLDER = ":/icons/treeNodeFolderClose.png";
	static const std::string ICON_OPEN_FOLDER = ":/icons/treeNodeFolderOpen.png";
	static const std::string ICON_APPROVED_PLAN = ":/icons/treeNodeTick.png";
	static const std::string ICON_UNAPPROVED_PLAN = ":/icons/treeNodeCross.png";
	static const std::string ICON_ROOT = ":/icons/treeNodeRoot.png";

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

    // Case insensitive character comparison function
    struct CaseInsensitiveCompare
    {
        bool operator()(const char& ch1, const char& ch2) const
        {
            return ::tolower(ch1) < ::tolower(ch2);
        }
    };


    struct PlanInstanceId
    {
        PlanInstanceId(PlanInstanceNumber instance, LocationKey location) :
            m_instance(instance), m_location(location)
        {
        }

        bool operator!=(const PlanInstanceId &other) const
        {
            return (m_instance != other.m_instance || m_location != other.m_location);
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
    const unsigned long MAX_MESSAGEBOX_STYLE = 6;

	const int APP_NOTIFY_ERROR_PLANAGENT_LOCATIONS = 1;
	const int APP_NOTIFY_ERROR_REGISTER_PLANMANAGER = 2;
}

#endif