/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/PlanAccessFactory.cpp $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  Implementation of PlanAccessFactory.
  *  TODO: put hard coded column names and SQL in a config file or in the data
  *  access classes outlined in the DataAccessInterface usage guide.
  */

#include "core/utilities/src/DebugUtil.h"
#include "bus/response_plans/PlanDataAccess/src/PlanDataReader.h"
#include "app/response_plans/plan_agent/src/IActivePlanFlow.h"
#include "app/response_plans/plan_agent/src/ActiveStep.h"
#include "app/response_plans/plan_agent/src/ActiveStepsHeader.h"
#include "app/response_plans/plan_agent/src/PlanAccessFactory.h"

namespace TA_Base_App
{

	PlanAccessFactoryImpl::PlanAccessFactoryImpl()
	{
	}
	
    // Improve performance, load all steps in one sql
	void PlanAccessFactoryImpl::getActiveStepList( const unsigned long plan, IActivePlanFlow& flowControl, std::string& reason, std::list<ActiveStep*>& lstSteps ) const
	{
        LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Begin to load steps" );

		lstSteps.clear();

//  		TA_Base_Core::StepDetails_var stepDetails = TA_Base_Bus::PlanDataReader::instance()->getAllStepDetails( plan );
        TA_Base_Core::Steps_var varAllSteps = TA_Base_Bus::PlanDataReader::instance()->getAllSteps( plan );

		// Return an empty step list.
//         if( 0 == stepDetails->length() )
        if( 0 == varAllSteps->length() )
		{
			return;
		}

		TA_Base_Core::StepNumber stepIndex = 1;

        bool stepDefined = false;

		//TD20350 xinsong++
		int GRC3PrevStepIndex = -1; //the previous GRC3 step index 
		GRC3DataPointSetComplexStep * currentGRCType3Step = NULL;

// 		for( stepIndex = 1; stepIndex<= stepCount; stepIndex++ )
		for( stepIndex = 1; stepIndex<= varAllSteps->length(); stepIndex++ )
		{
// 			TA_Base_Core::StepParameters_var stepParameters;
            stepDefined = false;

// 			stepParameters = TA_Base_Bus::PlanDataReader::instance()->getStepParameters( plan, stepIndex, true );

//             switch( stepParameters->_d() )
            switch( varAllSteps[stepIndex-1].parameters._d() )
			{
				case TA_Base_Core::END_STEP:
				{
//                     lstSteps.push_back( new ActiveEndStep(stepDetails[stepIndex-1], flowControl) );
                    lstSteps.push_back( new ActiveEndStep(varAllSteps[stepIndex-1].detail, flowControl) );
                    stepDefined = true;

                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::END_STEP has not been defined.";
                    }
				}
				break;

				case TA_Base_Core::ABORT_STEP:
				{
                    #if defined ( STEPTYPE_ABORT )

//                     lstSteps.push_back( new ActiveAbortStep(stepDetails[stepIndex-1], flowControl) );
                    lstSteps.push_back( new ActiveAbortStep(varAllSteps[stepIndex-1].detail, flowControl) );
                    stepDefined = true;

				    #endif // STEPTYPE_ABORT

                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::ABORT_STEP has not been defined.";
                    }

                }
				break;

				case TA_Base_Core::ACTIVE_PLAN_CHECK_STEP:
				{
					#if defined ( STEPTYPE_ACTIVE_PLAN_CHECK )

//                     lstSteps.push_back( new ActiveActivePlanCheckStep( stepDetails[stepIndex-1], stepParameters->activePlanCheck(), flowControl) );
                    lstSteps.push_back( new ActiveActivePlanCheckStep( varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.activePlanCheck(), flowControl) );
                    stepDefined = true;

				    #endif //STEPTYPE_ACTIVE_PLAN_CHECK

                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::ACTIVE_PLAN_CHECK_STEP has not been defined.";
                    }
                }
				break;

				case TA_Base_Core::ASSIGN_CAMERA_TO_MONITOR_STEP:
				{
				    #if defined ( STEPTYPE_ASSIGN_CAMERA_TO_MONITOR )

//                     lstSteps.push_back( new ActiveAssignCameraToMonitorStep( stepDetails[stepIndex-1], stepParameters->cameraMonitor(), flowControl) );
                    lstSteps.push_back( new ActiveAssignCameraToMonitorStep( varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.cameraMonitor(), flowControl) );
                    stepDefined = true;

                    #endif //ASSIGN_CAMERA_TO_MONITOR_STEP

                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::ASSIGN_CAMERA_TO_MONITOR_STEP has not been defined.";
                    }
                }
				break;

				case TA_Base_Core::DATA_POINT_CHECK_STEP:
				{
					#if defined ( STEPTYPE_DATA_POINT_CHECK )

//                     lstSteps.push_back( new ActiveDataPointCheckStep(stepDetails[stepIndex-1], stepParameters->dataPointCheck(), flowControl) );
                    lstSteps.push_back( new ActiveDataPointCheckStep(varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.dataPointCheck(), flowControl) );
                    stepDefined = true;

				    #endif //STEPTYPE_DATA_POINT_CHECK

                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::DATA_POINT_CHECK_STEP has not been defined.";
                    }
                }
				break;

				case TA_Base_Core::DATA_POINT_SET_STEP:
				{
					#if defined ( STEPTYPE_DATA_POINT_SET )

					ActiveStep * pActiveStep = NULL;

                    TA_Base_Core::StepDetail& refDetail = varAllSteps[stepIndex-1].detail;
                    bool fGRCType3Step = ( !refDetail.skip && refDetail.ignoreFailure && refDetail.nowait ); 

//                     ActiveDataPointSetStep * pDataPointSetStep = new ActiveDataPointSetStep(stepDetails[stepIndex-1], stepParameters->dataPointSet(), flowControl, fGRCType3Step );
					ActiveDataPointSetStep * pDataPointSetStep = new ActiveDataPointSetStep(refDetail, varAllSteps[stepIndex-1].parameters.dataPointSet(), flowControl, fGRCType3Step );
					if ( fGRCType3Step )
					{
						if ( GRC3PrevStepIndex + 1 < stepIndex ) 
						{
//                             currentGRCType3Step = new GRC3DataPointSetComplexStep(stepDetails[stepIndex-1], flowControl);
                            currentGRCType3Step = new GRC3DataPointSetComplexStep(refDetail, flowControl);
                            pActiveStep = currentGRCType3Step;
						}
						else
						{
//                             pActiveStep = new ActiveNullStep(stepDetails[stepIndex-1], flowControl);
                            pActiveStep = new ActiveNullStep(refDetail, flowControl);
						}
						currentGRCType3Step->addChildStep(pDataPointSetStep);
						GRC3PrevStepIndex = stepIndex;
					}
					else
					{
						pActiveStep = pDataPointSetStep;
					}

                    lstSteps.push_back( pActiveStep );
                    stepDefined = true;
				    #endif //STEPTYPE_DATA_POINT_SET

                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::DATA_POINT_SET_STEP has not been defined.";
                    }
                }
				break;

				case TA_Base_Core::DECISION_STEP:
				{
					#if defined ( STEPTYPE_DECISION )
                    
//                     lstSteps.push_back( new ActiveDecisionStep(stepDetails[stepIndex-1], stepParameters->decision(), flowControl) );
                    lstSteps.push_back( new ActiveDecisionStep(varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.decision(), flowControl) );
                    stepDefined = true;

                    #endif //STEPTYPE_DECISION

                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::DECISION_STEP has not been defined.";
                    }
				}
				break;

				case TA_Base_Core::JUMP_STEP:
				{
					#if defined ( STEPTYPE_JUMP )

//                     lstSteps.push_back( new ActiveJumpStep(stepDetails[stepIndex-1], stepParameters->jump(), flowControl) );
                    lstSteps.push_back( new ActiveJumpStep(varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.jump(), flowControl) );
                    stepDefined = true;

                    #endif //STEPTYPE_JUMP

                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::JUMP_STEP has not been defined.";
                    }
				}
				break;

				case TA_Base_Core::LOG_STEP:
				{
					#if defined ( STEPTYPE_LOG )

//                     lstSteps.push_back( new ActiveLogStep(stepDetails[stepIndex-1], stepParameters->log(), flowControl) );
                    lstSteps.push_back( new ActiveLogStep(varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.log(), flowControl) );
                    stepDefined = true;

                    #endif //STEPTYPE_LOG

                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::LOG_STEP has not been defined.";
                    }
				}
				break;

				case TA_Base_Core::ACTIVATE_CAMERA_PRESET_STEP:
				{
				    #if defined ( STEPTYPE_ACTIVATE_CAMERA_PRESET )

//                     lstSteps.push_back( new ActiveAssignCameraToPresetStep( stepDetails[stepIndex-1], stepParameters->cameraPreset(), flowControl) );
                    lstSteps.push_back( new ActiveAssignCameraToPresetStep( varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.cameraPreset(), flowControl) );
                    stepDefined = true;

                    #endif //STEPTYPE_ACTIVATE_CAMERA_PRESET

                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::ACTIVATE_CAMERA_PRESET has not been defined.";
                    }
                }
				break;

				case TA_Base_Core::PROMPT_STEP:
				{
					#if defined ( STEPTYPE_PROMPT )

//                     lstSteps.push_back( new ActivePromptStep(stepDetails[stepIndex-1], stepParameters->prompt(), flowControl) );
                    lstSteps.push_back( new ActivePromptStep(varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.prompt(), flowControl) );
                    stepDefined = true;

                    #endif //STEPTYPE_PROMPT

                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::PROMPT_STEP has not been defined.";
                    }
				}
				break;

				case TA_Base_Core::RUN_PLAN_STEP:
				{
					#if defined ( STEPTYPE_RUN_PLAN )

//                     lstSteps.push_back( new ActiveRunPlanStep( stepDetails[stepIndex-1], stepParameters->runPlan(), flowControl) );
                    lstSteps.push_back( new ActiveRunPlanStep( varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.runPlan(), flowControl) );
                    stepDefined = true;

                    #endif //STEPTYPE_RUN_PLAN

                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::RUN_PLAN_STEP has not been defined.";
                    }
				}
				break;

				case TA_Base_Core::TERMINATE_PLAN_STEP:
				{
					#if defined ( STEPTYPE_TERMINATE_PLAN )

//                     lstSteps.push_back( new ActiveTerminatePlanStep( stepDetails[stepIndex-1], stepParameters->terminatePlan(), flowControl) );
                    lstSteps.push_back( new ActiveTerminatePlanStep( varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.terminatePlan(), flowControl) );
                    stepDefined = true;

                    #endif //STEPTYPE_TERMINATE_PLAN

                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::TERMINATE_PLAN_STEP has not been defined.";
                    }
				}
				break;

				case TA_Base_Core::VMS_SET_SCENE_STEP:
				{
                    #if defined ( STEPTYPE_VMS_SET_SCENE )
//                     lstSteps.push_back( new ActiveVmsSetSceneStep( stepDetails[stepIndex-1], stepParameters->vmsSetScene(), flowControl) );
                    lstSteps.push_back( new ActiveVmsSetSceneStep( varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.vmsSetScene(), flowControl) );
                    stepDefined = true;

                    #endif //STEPTYPE_VMS_SET_SCENE

                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::VMS_SET_SCENE_STEP has not been defined.";
                    }
				}
				break;

				case TA_Base_Core::LAUNCH_GUI_APP_STEP:
				{
                    #if defined ( STEPTYPE_LAUNCH_GUI_APP )
//                     lstSteps.push_back( new ActiveLaunchGuiAppStep( stepDetails[stepIndex-1], stepParameters->guiAppParameters(), flowControl) );
                    lstSteps.push_back( new ActiveLaunchGuiAppStep( varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.guiAppParameters(), flowControl) );
                    stepDefined = true;
                    #endif //STEPTYPE_LAUNCH_GUI_APP
                    
                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::LAUNCH_GUI_APP_STEP has not been defined.";
                    }
				}
				break;

                case TA_Base_Core::VMS_BLANKOUT_STEP:
                {
                    #if defined ( STEPTYPE_VMS_BLANKOUT )
//                     lstSteps.push_back( new ActiveVmsBlankoutStep( stepDetails[stepIndex-1], stepParameters->vmsBlankout(), flowControl) );
                    lstSteps.push_back( new ActiveVmsBlankoutStep( varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.vmsBlankout(), flowControl) );
                    stepDefined = true;
                    #endif //STEPTYPE_VMS_BLANKOUT
                    
                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::STEPTYPE_VMS_BLANKOUT has not been defined.";
                    }
                }
                break;

                case TA_Base_Core::VMS_RESTORE_SCHEDULE_STEP:
                {
                    #if defined ( STEPTYPE_VMS_RESTORE_SCHEDULE )
//                     lstSteps.push_back( new ActiveVmsRestoreScheduleStep( stepDetails[stepIndex-1], stepParameters->vmsRestoreSchedule(), flowControl) );
                    lstSteps.push_back( new ActiveVmsRestoreScheduleStep( varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.vmsRestoreSchedule(), flowControl) );
                    stepDefined = true;
                    #endif //STEPTYPE_VMS_RESTORE_SCHEDULE
                    
                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::VMS_RESTORE_SCHEDULE_STEP has not been defined.";
                    }

                }
                break;

                case TA_Base_Core::VIDEO_WALL_STEP:
                {
                    #if defined ( STEPTYPE_VIDEOWALL )
//                     lstSteps.push_back( new ActiveVideoWallStep( stepDetails[stepIndex-1], stepParameters->videoWallParams(), flowControl) );
                    lstSteps.push_back( new ActiveVideoWallStep( varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.videoWallParams(), flowControl) );
                    stepDefined = true;
                    #endif //STEPTYPE_VIDEOWALL
                    
                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::VIDEO_WALL_STEP has not been defined.";
                    }
                }
                break;

                case TA_Base_Core::STATION_PUBLIC_ADDRESS_STEP:
                {
                    #if defined ( STEPTYPE_STATION_PUBLIC_ADDRESS )
//                     lstSteps.push_back( new ActiveStationPaStep( stepDetails[stepIndex-1], stepParameters->stationPublicAddress(), flowControl) );
                    lstSteps.push_back( new ActiveStationPaStep( varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.stationPublicAddress(), flowControl) );
                    stepDefined = true;
                    #endif //STEPTYPE_STATION_PUBLIC_ADDRESS
                    
                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::STATION_PUBLIC_ADDRESS_STEP has not been defined.";
                    }
                }
                break;

                case TA_Base_Core::TRAIN_PUBLIC_ADDRESS_STEP:
                {
                    #if defined ( STEPTYPE_TRAIN_PUBLIC_ADDRESS )
//                     lstSteps.push_back( new ActiveTrainPaStep( stepDetails[stepIndex-1], stepParameters->trainPublicAddress(), flowControl) );
                    lstSteps.push_back( new ActiveTrainPaStep( varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.trainPublicAddress(), flowControl) );
                    stepDefined = true;
                    #endif //STEPTYPE_TRAIN_PUBLIC_ADDRESS
                    
                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::TRAIN_PUBLIC_ADDRESS_STEP has not been defined.";
                    }
                }
                break;

                case TA_Base_Core::SEND_SMS_MESSAGE_STEP:
                {
                    #if defined ( STEPTYPE_SEND_SMS_MESSAGE )
//                     lstSteps.push_back( new ActiveSendSmsMessageStep( stepDetails[stepIndex-1], stepParameters->sendSmsMessage(), flowControl ) );
                    lstSteps.push_back( new ActiveSendSmsMessageStep( varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.sendSmsMessage(), flowControl ) );
                    stepDefined = true;
                    #endif //STEPTYPE_SEND_SMS_MESSAGE

                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::SEND_SMS_MESSAGE_STEP has not been defined.";
                    }
                }
                break;

				case TA_Base_Core::LAUNCH_SCHEMATIC_STEP:
				{
                    #if defined ( STEPTYPE_LAUNCH_SCHEMATIC )
//                     lstSteps.push_back( new ActiveLaunchSchematicStep( stepDetails[stepIndex-1], stepParameters->launchSchematic(), flowControl) );
                    lstSteps.push_back( new ActiveLaunchSchematicStep( varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.launchSchematic(), flowControl) );
                    stepDefined = true;
                    #endif //STEPTYPE_LAUNCH_SCHEMATIC
                    
                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::LAUNCH_SCHEMATIC_STEP has not been defined.";
                    }
				}
				break;

				case TA_Base_Core::EVALUATE_EXPRESSION_STEP:
				{
					#if defined ( STEPTYPE_EVALUATE_EXPRESSION)
//                     lstSteps.push_back( new ActiveEvaluateExpressionStep( stepDetails[stepIndex-1], stepParameters->evaluateExpression(), flowControl) );
                    lstSteps.push_back( new ActiveEvaluateExpressionStep( varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.evaluateExpression(), flowControl) );
					stepDefined = true;
					#endif //STEPTYPE_EVALUATE_EXPRESSION
					
					if ( !stepDefined )
					{
						reason = "Error: TA_Base_Core::EVALUATE_EXPRESSION_STEP has not been defined.";
					}
				}
				break;

				case TA_Base_Core::STATION_TRAVELLER_INFO_SYSTEM_STEP:
				{
					#if defined ( STEPTYPE_STATION_TRAVELLER_INFO_SYSTEM )
//                     lstSteps.push_back( new ActiveStationTisStep(stepDetails[stepIndex-1], stepParameters->stationTravellerInfoSystem(), flowControl) );
                    lstSteps.push_back( new ActiveStationTisStep(varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.stationTravellerInfoSystem(), flowControl) );
                    stepDefined = true;
				    #endif //STEPTYPE_STATION_TRAVELLER_INFO_SYSTEM

                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::STATION_TRAVELLER_INFO_SYSTEM_STEP has not been defined.";
                    }
                }
				break;

				case TA_Base_Core::TRAIN_TRAVELLER_INFO_SYSTEM_STEP:
				{
					#if defined ( STEPTYPE_TRAIN_TRAVELLER_INFO_SYSTEM )
//                     lstSteps.push_back( new ActiveTrainTisStep(stepDetails[stepIndex-1], stepParameters->trainTravellerInfoSystem(), flowControl) );
                    lstSteps.push_back( new ActiveTrainTisStep(varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.trainTravellerInfoSystem(), flowControl) );
                    stepDefined = true;
				    #endif //STEPTYPE_TRAIN_TRAVELLER_INFO_SYSTEM

                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::TRAIN_TRAVELLER_INFO_SYSTEM_STEP has not been defined.";
                    }
                }
				break;
				// lizettejl++ Failure Step Summary (DP-changes)
				case TA_Base_Core::FAILURE_SUMMARY_STEP:
				{
					#if defined ( STEPTYPE_FAILURE_SUMMARY_STEP )
//                     lstSteps.push_back( new  ActiveFailureSummaryStep(stepDetails[stepIndex-1], stepParameters->failureSummary(), flowControl) );
                    lstSteps.push_back( new  ActiveFailureSummaryStep(varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.failureSummary(), flowControl) );
					stepDefined = true;
					#endif // STEPTYPE_FAILURE_SUMMARY_STEP 
					
					if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::FAILURE_SUMMARY_STEP has not been defined.";
                    }
				}
				break;
				//++lizettejl

                case TA_Base_Core::PLAN_CHECK_TIMER_STEP:
                {
                    #if defined (STEPTYPE_PLAN_CHECK_TIMER)
                        lstSteps.push_back(new ActivePlanCheckTimerStep(varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.planCheckTimer(), flowControl));
                        stepDefined = true;
                    #endif
                }
                break;

                case TA_Base_Core::PLAN_SET_TIMER_STEP:
                {
                    #if defined (STEPTYPE_PLAN_SET_TIMER)
                        lstSteps.push_back(new ActivePlanSetTimerStep(varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.planSetTimer(), flowControl));
                        stepDefined = true;
                    #endif
                }
                break;

                case TA_Base_Core::EXECUTE_SCRIPT_STEP:
                {
                #if defined (STEPTYPE_EXECUTE_SCRIPT)
                    lstSteps.push_back(new ActiveExecuteScriptStep(varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.executeScript(), flowControl));
                    stepDefined = true;
                #endif
                }
                break;

                case TA_Base_Core::DATA_POINT_TIME_CHECK_STEP:
                {
                #if defined (STEPTYPE_DATA_POINT_TIME_CHECK)
                    lstSteps.push_back(new ActiveDataPointTimeCheckStep(varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.dataPointTimeCheck(), flowControl));
                    stepDefined = true;
                #endif
                }
                break;

                case TA_Base_Core::POPUP_MFT_MESSAGE_STEP:
                {
                #if defined (STEPTYPE_POPUP_MFT_MESSAGE)
                    lstSteps.push_back(new ActivePopupMFTMessageStep(varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.popupMFTMessage(), flowControl));
                    stepDefined = true;
                #endif
                }
                break;

                case TA_Base_Core::SHUTDOWN_SYSTEM_STEP:
                {
                #if defined (STEPTYPE_SHUTDOWN_SYSTEM)
                        lstSteps.push_back(new ActiveShutdownSystemStep(varAllSteps[stepIndex-1].detail, varAllSteps[stepIndex-1].parameters.shutdownSystem(), flowControl));
                        stepDefined = true;
                #endif
                }
                break;

				case TA_Base_Core::UNDEFINED_STEP:
				{
					LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugError, "found undefined step" );
                    stepDefined = true;
                    if ( !stepDefined )
                    {
                        reason = "Error: TA_Base_Core::UNDEFINED_STEP has not been defined.";
                    }
				}
				break; 
			}

            if ( !stepDefined )
            {
                break;
            }
		}

        if ( stepDefined )
        {
            LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Loaded %lu steps", lstSteps.size() );
        }
        else
        {
		    LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Failed to load all steps, unusable steptype encountered: %s", reason.c_str() );
        }
	}

} // TA_Base_App
