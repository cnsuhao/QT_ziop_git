/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/MessageBox.cpp $
* @author:  Bart Golab
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2017/05/09 13:33:55 $
* Last modified by:  $Author: hoa.le $
*
* This class implements custom MessageBox.
*/

#include "MessageBox.h"

#include "core/data_access_interface/PlanAgent/IDL/src/PlanAgentDataCorbaDef.h"
#include "core/data_access_interface/src/IOperator.h"
#include "core/data_access_interface/src/OperatorAccessFactory.h"
#include "core/exceptions/src/RightsException.h"
#include "core/exceptions/src/PlanStepDetailException.h"
#include "core/exceptions/src/PlanStepException.h"
#include "core/exceptions/src/PlanAgentException.h"
#include "core/exceptions/src/TransactiveException.h"
#include "core/exceptions/src/ObjectResolutionException.h"
#include "core/utilities/src/TAAssert.h"
#include "core/data_access_interface/src/OperatorAccessFactory.h"
#include "core/utilities/src/RunParams.h"
#include "bus/response_plans/plan_agent/IDL/src/IPlanAgentCorbaDef.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus\security\authentication_library\src\AuthenticationLibrary.h"
#include "app/response_plans/plan_agent/src/PlanNotFoundException.h"

#include "app/response_plans/plan_manager/pmodel/src/StringDefn.h"

using namespace TA_Base_App;
using namespace TA_Base_Core;

#define MessageBox MessageBox

/////////////////////////////////////////////////////////////////////////////
// MessageBox

MessageBox::MessageBox()
{
	FUNCTION_ENTRY("MessageBox");
	FUNCTION_EXIT;
}


MessageBox::~MessageBox()
{
	FUNCTION_ENTRY("~MessageBox");
	FUNCTION_EXIT;
}

void MessageBox::error(const char* actionStr, const TransactiveException& exception)
{
    FUNCTION_ENTRY( "error" );

	displayError(QString(actionStr), exception);

    FUNCTION_EXIT;
}


void MessageBox::error(const char* actionStr, const QString& actionParam, const TransactiveException& exception)
{
    FUNCTION_ENTRY( "error" );


	QString actionName = QString(actionStr).arg(actionParam);
	
	// Display messagebox warn the user that plan or category delete was unsuccessful so try again.
	if (actionStr == std::string(IDS_DELETE_PLAN) || actionStr == std::string(IDS_DELETE_CATEGORY))
	{
		// if PlanAgent failed to find the requested node
		const PlanAgentException* planAgentException = dynamic_cast<const PlanAgentException*>(&exception);
		if (planAgentException != NULL)
		{
			if (planAgentException->getErrorType() == PAE_DATA_ERROR)
			{
				TA_Base_Bus::TransActiveMessage userMsg;
				userMsg << actionName.toStdString();
				userMsg.showMsgBox(IDS_UE_250025);
			}
		}
	}
	else
	{
		displayError(actionName, exception);
	}    

    FUNCTION_EXIT;
}


void MessageBox::error(const char* actionStr)
{
    FUNCTION_ENTRY( "error" );

    // some generic "Cannot do x due to error" message.
    TA_Base_Bus::TransActiveMessage userMsg;
    QString actionName;
	userMsg << std::string(actionStr);
    userMsg.showMsgBox(IDS_UE_210100);

    FUNCTION_EXIT;
}


void MessageBox::displayError(const QString& actionName, const TransactiveException& exception)
{
    FUNCTION_ENTRY( "displayError" );

    // determine which type of error it is and display the error accordingly
    const RightsException* rightsException = dynamic_cast<const RightsException*>(&exception);
    if (rightsException)
    {
        displayPermissionError(actionName, *rightsException);

        FUNCTION_EXIT;
        return;
    }

    const PlanNotFoundException* planNotFoundException = dynamic_cast<const PlanNotFoundException*>(&exception);
    if (planNotFoundException)
    {
        displayPlanNotFoundError(actionName, *planNotFoundException);

        FUNCTION_EXIT;
        return;
    }

    const PlanStepException* planStepException = dynamic_cast<const PlanStepException*>(&exception);
    if (planStepException)
    {
        displayPlanStepError(actionName, *planStepException);

        FUNCTION_EXIT;
        return;
    }

    const PlanStepDetailException* planStepDetailException = dynamic_cast<const PlanStepDetailException*>(&exception);
    if (planStepDetailException)
    {
        displayPlanStepDetailError(actionName, *planStepDetailException);

        FUNCTION_EXIT;
        return;
    }

    const PlanAgentException* planAgentException = dynamic_cast<const PlanAgentException*>(&exception);
    if (planAgentException)
    {
        displayPlanAgentError(actionName, *planAgentException);

        FUNCTION_EXIT;
        return;
    }

     const ObjectResolutionException* objectResolutionException = dynamic_cast<const ObjectResolutionException*>(&exception);
    if (objectResolutionException)
    {
        TA_Base_Bus::TransActiveMessage userMsg;
        userMsg << actionName.toStdString();
        userMsg.showMsgBox(IDS_UE_210011);

        FUNCTION_EXIT;
        return;
    }

    // otherwise, it's not any of above, so just display with some generic error message.
    TA_Base_Bus::TransActiveMessage userMsg;
	userMsg << actionName.toStdString();
    userMsg.showMsgBox(IDS_UE_210100);

    FUNCTION_EXIT;
}


void MessageBox::displayPermissionError(const QString& actionName, const RightsException& /*exception*/)
{
    FUNCTION_ENTRY( "displayPermissionError" );

    TA_Base_Bus::TransActiveMessage userMsg;
    std::string userName;
    std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
    TA_Base_Bus::AuthenticationLibrary authLibrary(true);
    TA_Base_Bus::SessionInfo sessionInfo = authLibrary.getSessionInfo(sessionId, sessionId);
    std::auto_ptr<TA_Base_Core::IOperator> iOperator(TA_Base_Core::OperatorAccessFactory::getInstance().getOperator(sessionInfo.UserId, false));
    userName = iOperator->getName();
    userMsg << userName;
	userMsg << actionName.toStdString();
    userMsg.showMsgBox(IDS_UE_010021);

    FUNCTION_EXIT;
}


void MessageBox::displayPlanNotFoundError(const QString& actionName, const PlanNotFoundException& exception)
{
    FUNCTION_ENTRY( "displayPlanNotFoundError" );

    unsigned long messageId;

    switch (exception.getErrorType())
    {
        case PNFE_FAIL_FETCH_ACTIVE_LIST_STEP:
            messageId = IDS_UE_210047;
            break;
        case PNFE_INSTANCE_NOT_IN_BULLETIN_BOARD:
            messageId = IDS_UE_210048;
            break;
        case PNFE_INSTANCE_NOT_FOUND:
            messageId = IDS_UE_210049;
            break;
        case PNFE_NODE_NOT_EXIST:
        default:
            messageId = IDS_UE_210050;
            break;
    }

    TA_Base_Bus::TransActiveMessage userMsg;
	userMsg << actionName.toStdString() << exception.getPlanId() << exception.getInstanceId();
    userMsg.showMsgBox(messageId);

    FUNCTION_EXIT;
}


void MessageBox::displayPlanStepError(const QString& actionName, const PlanStepException& exception)
{
    FUNCTION_ENTRY( "displayPlanStepError" );

    QString resourceStr;

    switch (exception.getStepType())
    {
        case END_STEP:
            resourceStr = IDS_STEP_TYPE_END;
            break;
        case ABORT_STEP:
            resourceStr = IDS_STEP_TYPE_ABORT;
            break;
        case ACTIVATE_CAMERA_PRESET_STEP:
            resourceStr = IDS_STEP_TYPE_ACTIVATE_CAMERA_PRESET;
            break;
        case ACTIVE_PLAN_CHECK_STEP:
            resourceStr = IDS_STEP_TYPE_ACTIVE_PLAN_CHECK;
            break;
        case ASSIGN_CAMERA_TO_MONITOR_STEP:
            resourceStr = IDS_STEP_TYPE_ASSIGN_CAMERA_TO_MONITOR;
            break;
        case DATA_POINT_CHECK_STEP:
            resourceStr = IDS_STEP_TYPE_DATA_POINT_CHECK;
            break;
        case DATA_POINT_SET_STEP:
            resourceStr = IDS_STEP_TYPE_DATA_POINT_SET;
            break;
        case DECISION_STEP:
            resourceStr = IDS_STEP_TYPE_DECISION;
            break;
        case JUMP_STEP:
            resourceStr = IDS_STEP_TYPE_JUMP;
            break;
        case LAUNCH_GUI_APP_STEP:
            resourceStr = IDS_STEP_TYPE_LAUNCH_GUI_APP;
            break;
        case LAUNCH_SCHEMATIC_STEP:
            resourceStr = IDS_STEP_TYPE_LAUNCH_SCHEMATIC;
            break;
        case LOG_STEP:
            resourceStr = IDS_STEP_TYPE_LOG;
            break;
        case PROMPT_STEP:
            resourceStr = IDS_STEP_TYPE_PROMPT;
            break;
        case RUN_PLAN_STEP:
            resourceStr = IDS_STEP_TYPE_RUN_PLAN;
            break;
        case SEND_SMS_MESSAGE_STEP:
            resourceStr = IDS_STEP_TYPE_SEND_SMS_MESSAGE;
            break;
        case STATION_TRAVELLER_INFO_SYSTEM_STEP:
            resourceStr = IDS_STEP_TYPE_STATION_TRAVELLER_INFO_SYSTEM;
            break;
        case TERMINATE_PLAN_STEP:
            resourceStr = IDS_STEP_TYPE_TERMINATE_PLAN;
            break;
          case STATION_PUBLIC_ADDRESS_STEP:
            resourceStr = IDS_STEP_TYPE_STATION_PUBLIC_ADDRESS;
            break;
          case TRAIN_PUBLIC_ADDRESS_STEP:
            resourceStr = IDS_STEP_TYPE_TRAIN_PUBLIC_ADDRESS;
            break;
        case TRAIN_TRAVELLER_INFO_SYSTEM_STEP:
            resourceStr = IDS_STEP_TYPE_TRAIN_TRAVELLER_INFO_SYSTEM;
            break;
          case VMS_RESTORE_SCHEDULE_STEP:
            resourceStr = IDS_STEP_TYPE_VMS_RESTORE_SCHEDULE;
            break;
        case VIDEO_WALL_STEP:
            resourceStr = IDS_STEP_TYPE_VIDEO_WALL;
            break;
        case VMS_SET_SCENE_STEP:
            resourceStr = IDS_STEP_TYPE_VMS_SET_SCENE;
            break;
        case VMS_BLANKOUT_STEP:
            resourceStr = IDS_STEP_TYPE_VMS_BLANKOUT;
            break;
        // lizettejl++ (DP-Changes)
        case FAILURE_SUMMARY_STEP:
            resourceStr = IDS_STEP_TYPE_FAILURE_SUMMARY;
            break;
        // ++lizettejl
        case PLAN_CHECK_TIMER_STEP:
            resourceStr = IDS_STEP_TYPE_PLAN_CHECK_TIMER;
            break;
        case PLAN_SET_TIMER_STEP:
            resourceStr = IDS_STEP_TYPE_PLAN_SET_TIMER;
            break;
        case  DATA_POINT_TIME_CHECK_STEP:
            resourceStr = IDS_STEP_TYPE_DATA_POINT_TIME_CHECK;
            break;
                /*
        case MITSIM_GENERATE_INCIDENT_IMAGES_STEP:
            resourceStr = IDS_STEP_TYPE_MITSIM_GENERATE_INCIDENT_IMAGES;
            break;
        case GENERATE_REPORT_STEP:
            resourceStr = IDS_STEP_TYPE_GENERATE_REPORT;
            break;
        case LIVE_PROPERTY_CHECK_STEP:
            resourceStr = IDS_STEP_TYPE_LIVE_PROPERTY_CHECK;
            break;
        case LAUNCH_MAP_VIEWER_STEP:
            resourceStr = IDS_STEP_TYPE_LAUNCH_MAP_VIEWER;
            break;
        case START_RECORDING_STEP:
            resourceStr = IDS_STEP_TYPE_START_RECORDING;
            break;
        case STOP_RECORDING_STEP:
            resourceStr = IDS_STEP_TYPE_STOP_RECORDING;
            break;
        case ASSIGN_VIDEO_INPUT_TO_RECORDING_UNIT_STEP:
            resourceStr = IDS_STEP_TYPE_ASSIGN_VIDEO_INPUT_TO_RECORDING_UNIT;
            break;
        case CLEAR_VIDEO_WALL_STEP:
            resourceStr = IDS_STEP_TYPE_CLEAR_VIDEO_WALL;
            break;
            */
        case UNDEFINED_STEP:
        default:
            resourceStr = IDS_STEP_TYPE_UNDEFINED;
            break;
    }

    TA_Base_Bus::TransActiveMessage userMsg;
    if (resourceStr == QString(IDS_STEP_TYPE_UNDEFINED))
    {
        userMsg.showMsgBox(IDS_UE_210084);

        FUNCTION_EXIT;
        return;
    }

	userMsg << resourceStr.toStdString();
    userMsg.showMsgBox(IDS_UE_210082);

    FUNCTION_EXIT;
}


void MessageBox::displayPlanStepDetailError(const QString& actionName, const PlanStepDetailException& exception)
{
    FUNCTION_ENTRY( "displayPlanStepDetailError" );

    QString resourceStr;

    switch (exception.getDetailType())
    {
        case PSDT_NAME:
            resourceStr = IDS_STEP_LIST_NAME_COLUMN;
            break;
        case PSDT_TYPE:
            resourceStr = IDS_STEP_LIST_TYPE_COLUMN;
            break;
        case PSDT_SKIP:
            resourceStr = IDS_STEP_LIST_SKIP_COLUMN;
            break;
        case PSDT_POSITION:
            resourceStr = IDS_STEP_PARAMETER_POSITION;
            break;
        case PSDT_PLAN_PATH:
            resourceStr = IDS_STEP_PARAMETER_PLAN_PATH;
            break;
        case PSDT_YES:
            resourceStr = IDS_STEP_PARAMETER_YES_STEP;
            break;
        case PSDT_NO:
            resourceStr = IDS_STEP_PARAMETER_NO_STEP;
            break;
        case PSDT_DATA_POINT_NAME:
            resourceStr = IDS_STEP_PARAMETER_DATA_POINT_NAME;
            break;
        case PSDT_DATA_POINT_VALUE:
            resourceStr = IDS_STEP_PARAMETER_DATA_POINT_NEW_VALUE;
            break;
        case PSDT_DATA_POINT_TEST_TYPE:
            resourceStr = IDS_STEP_PARAMETER_DATA_POINT_TEST_TYPE;
            break;
        case PSDT_SUCCESS:
            resourceStr = IDS_STEP_PARAMETER_SUCCESS_STEP;
            break;
        case PSDT_FAILURE:
            resourceStr = IDS_STEP_PARAMETER_FAILURE_STEP;
            break;
        case PSDT_DECISION_MESSAGE:
            resourceStr = IDS_STEP_PARAMETER_DECISION_MESSAGE;
            break;
        case PSDT_JUMP:
            resourceStr = IDS_STEP_PARAMETER_JUMP_STEP;
            break;
        case PSDT_PROMPT_MESSAGE:
            resourceStr = IDS_STEP_PARAMETER_PROMPT_MESSAGE;
            break;
        case PSDT_APPLICATION:
            resourceStr = IDS_STEP_PARAMETER_APPLICATION;
            break;
        case PSDT_WIDTH:
            resourceStr = IDS_STEP_PARAMETER_WIDTH;
            break;
        case PSDT_HEIGHT:
            resourceStr = IDS_STEP_PARAMETER_HEIGHT;
            break;
        case PSDT_CAMERA_NAME:
            resourceStr = IDS_STEP_PARAMETER_CAMERA_NAME;
            break;
        case PSDT_MONITOR_NAME:
            resourceStr = IDS_STEP_PARAMETER_MONITOR_NAME;
            break;
        case PSDT_CAMERA_PRESET:
            resourceStr = IDS_STEP_PARAMETER_PRESET_NAME;
            break;
        case PSDT_VMS_SIGN:
            resourceStr = IDS_STEP_PARAMETER_VMS_NAME;
            break;
        case PSDT_VMS_SCENE:
            resourceStr = IDS_STEP_PARAMETER_VMS_SCENE;
            break;
        case PSDT_WALLBOARD:
            resourceStr = IDS_STEP_PARAMETER_WALLBOARD_NAME;
            break;
        case PSDT_VIDEO_LAYOUT:
            resourceStr = IDS_STEP_PARAMETER_LAYOUT_NAME;
            break;
        case PSDT_SMS_RECIPIENT:
            resourceStr = IDS_STEP_PARAMETER_SMS_RECIPIENT;
            break;
        case PSDT_SMS_MESSAGE:
            resourceStr = IDS_STEP_PARAMETER_SMS_MESSAGE;
            break;
        case PSDT_SCHEMATIC_NAME:
            resourceStr = IDS_STEP_PARAMETER_SCHEMATIC;
            break;
        case PSDT_SCREEN_NUMBER:
            resourceStr = IDS_STEP_PARAMETER_SCREEN;
            break;
          case PSDT_EXPRESSION_VALUE:
               resourceStr = IDS_STEP_PARAMETER_EVAL_EXP;
                break;
        case PSDT_STATION_NAME:
            resourceStr = IDS_STEP_PARAMETER_STATION;
            break;
        case PSDT_STATION_MESSAGE:
            resourceStr = IDS_STEP_PARAMETER_PA_MESSAGE;
            break;
        case PSDT_STATION_ZONE:
            resourceStr = IDS_STEP_PARAMETER_PA_ZONE;
            break;
        case PSDT_TRAIN_ID:
            resourceStr = IDS_STEP_PARAMETER_TRAIN;
            break;
        case PSDT_TIS_MESSAGE:
            resourceStr = IDS_STEP_PARAMETER_TIS_MESSAGE;
            break;
        case PSDT_TIS_PRIORITY:
            resourceStr = IDS_STEP_PARAMETER_TIS_PRIORITY;
            break;
        case PSDT_TIS_DURATION:
            resourceStr = IDS_STEP_PARAMETER_TIS_DURATION;
            break;
        // lizettejl++ (DP-changes)
        case PSDT_YES_LABEL:
            resourceStr = IDS_STEP_PARAMETER_YES_LABEL;
            break;
        case PSDT_NO_LABEL:
            resourceStr = IDS_STEP_PARAMETER_NO_LABEL;
            break;
        case PSDT_PAUSE_LABEL:
            resourceStr = IDS_STEP_PARAMETER_PAUSE_LABEL;
            break;
        case PSDT_FAILURE_SUMMARY:
            resourceStr = IDS_STEP_PARAMETER_FAILURE_CATEGORY;
            break;
        // ++lizettejl
        case PSDT_TIME:
            resourceStr = IDS_STEP_PARAMETER_TIME_SPAN;
            break;
        case PSDT_PROFILE:
            resourceStr = IDS_STEP_PARAMETER_PROFILE;
            break;
        default:
            TA_ASSERT(false, "Unrecognised step detail");
    }

    QString errorStr;

    switch (exception.getErrorType())
    {
        case PSDE_UNSPECIFIED:
			errorStr = IDS_UNSPECIFIED;
            break;
        case PSDE_INVALID:
        default:
			errorStr = IDS_INVALID;
            break;
    }

    TA_Base_Bus::TransActiveMessage userMsg;
	userMsg << actionName.toStdString() << exception.getStepNumber() << resourceStr.toStdString() << errorStr.toStdString();
    userMsg.showMsgBox(IDS_UE_210051);

    FUNCTION_EXIT;
}


void MessageBox::displayPlanAgentError(const QString& actionName, const PlanAgentException& exception)
{
    FUNCTION_ENTRY( "displayPlanAgentError" );

    TA_Base_Bus::TransActiveMessage userMsg;

    if (!actionName.isEmpty())
    {
		userMsg << actionName.toStdString();
    }

    unsigned long resourceId = 0;

    switch (exception.getErrorType())
    {
        case PAE_EDIT_LOCKED_BY_OPERATOR:
            {
                try
                {
                    IOperator* op = OperatorAccessFactory::getInstance().getOperator(exception.getItemId(), false);
                    if (op != NULL)
                    {
                        userMsg << op->getName();
                    }
                    resourceId = IDS_UE_210052;
                }
                catch (...)
                {
                    // if there's any error, then just
                    resourceId = IDS_UE_210052;
                }
            }
            break;
        case PAE_CANNOT_RELEASE_EDIT_LOCK:
            resourceId = IDS_UE_210053;
            break;
        case PAE_NO_PLAN_NAME:
            resourceId = IDS_UE_210054;
            break;
          case PAE_INVALID_PLAN_NAME:
              resourceId = IDS_UE_210121;
              break;
        case PAE_NAME_NOT_UNIQUE:
            resourceId = IDS_UE_210055;
            break;
        case PAE_CANNOT_SAVE_APPROVED_PLAN:
            resourceId = IDS_UE_210056;
            break;
        case PAE_CANNOT_DELETE_APPROVED_PLAN:
            resourceId = IDS_UE_210057;
            break;
        case PAE_NOT_APPROVED:
            resourceId = IDS_UE_210058;
            break;
        case PAE_CANNOT_SKIP_UNSKIPPABLE:
            resourceId = IDS_UE_210059;
            break;
        case PAE_NOT_PLAN:
            resourceId = IDS_UE_210061;
            break;
        case PAE_NOT_CATEGORY:
            resourceId = IDS_UE_210062;
            break;
        case PAE_PARENT_NOT_CATEGORY:
            resourceId = IDS_UE_210063;
            break;
        case PAE_EXCEED_MAX_CATAGORY_LEVEL:
            resourceId = IDS_UE_210064;
            break;
        case PAE_CANNOT_DELETE_NON_EMPTY_CATAGORY:
            resourceId = IDS_UE_210065;
            break;
        case PAE_PLAN_HAS_ACTIVE_INSTANCE:
            resourceId = IDS_UE_210066;
            break;
        case PAE_CANNOT_CREATE_PLAN:
            resourceId = IDS_UE_210067;
            break;
        case PAE_DATABASE_ERROR:
            resourceId = IDS_UE_010001;
            break;
        case PAE_DATA_ERROR:
            resourceId = IDS_UE_010028;
            break;
        case PAE_WRONG_STATE_TO_CUSTOMISE_ACTIVE_PLAN:
            resourceId = IDS_UE_210069;
            break;
        case PAE_NO_RESOURCE:
            resourceId = IDS_UE_210070;
            break;
        case PAE_STEP_TYPE_AND_PARAMETER_MISMATCH:
            resourceId = IDS_UE_210071;
            break;
          case PAE_PLAN_UNAPPROVE_ERROR:
              resourceId = IDS_UE_210126;
              break;
          case PAE_PLAN_EXEC_ERROR:
              resourceId = IDS_UE_210126;
              break;
          case PAE_PLAN_STOP_ERROR:
              resourceId = IDS_UE_210125;
              break;
          case PAE_PLAN_RESUME_ERROR:
              resourceId = IDS_UE_210124;
              break;
          case PAE_PLAN_PAUSE_ERROR:
              resourceId = IDS_UE_210123;
              break;
        case PAE_INITIALISATION_FAILURE:    // this is agent initialisation problem, so probably shouldn't affect us
        case PAE_UNKNOWN_ERROR:
        default:
            resourceId = IDS_UE_250001;
            break;
    }

    userMsg.showMsgBox(resourceId);

    FUNCTION_EXIT;
}


