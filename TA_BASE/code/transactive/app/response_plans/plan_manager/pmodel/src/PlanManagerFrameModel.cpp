//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanManagerFrameModel.cpp $
// @author:  Bart Golab
// @version: $Revision: #6 $
//
// Last modification: $DateTime: 2013/04/29 17:39:48 $
// Last modified by:  $Author: huangjian $
//
// <description>
#include "PlanManagerFrameModel.h"
#include "PlanService.h"
#include "app/response_plans/plan_manager/pmodel/src/PlanManagerConsts.h"

#include "OperatorRights.h"


using namespace TA_Base_Bus;
using namespace TA_Base_Core;

namespace TA_Base_App
{
	PlanManagerFrameModel::PlanManagerFrameModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
		:TA_Base_Bus::AbstractPModel(appController, pmodelName)
	{
		FUNCTION_ENTRY( "PlanManagerFrameModel" );

		//TODO: need to get appGUI
		//TA_ASSERT(appGUI != NULL, "the IGUIApplication is NULL!");
		//// TODO: add member initialization code here
		//m_appGUI = appGUI;

		FUNCTION_EXIT;
	}

	PlanManagerFrameModel::~PlanManagerFrameModel()
	{
		FUNCTION_ENTRY( "~PlanManagerFrameModel" );
		FUNCTION_EXIT;
	}

	void PlanManagerFrameModel::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{
		//if (_stricmp(updateType, PMODEL_DATA_CHANGE_SESSION_ID) == 0 ||
		//	_stricmp(updateType, PMODEL_DATA_CHANGE_SESSION_ID) == 0)
		//{
			notifyViews(updateType, args);
		//}

	}

	// sessionIdChanged Remove?
	//void PlanManagerFrameModel::sessionIdChanged()
	//{
	//	notifyViews(PMODEL_DATA_CHANGE_SESSION_ID, NULL);
	//}

	std::string PlanManagerFrameModel::checkLocations()
	{
		return PlanService::getInstance().checkLocations();
	}

	bool PlanManagerFrameModel::isStepServantActive()
	{
		return PlanService::getInstance().isStepServantActive();
	}

	int PlanManagerFrameModel::activatePManagerServant()
	{
		return PlanService::getInstance().activatePManagerServant();
	}

	std::string PlanManagerFrameModel::registerStationAgent(int iIndex)
	{
		return PlanService::getInstance().registerStationAgent(iIndex);
	}

	void PlanManagerFrameModel::registerRunParam()
	{
		PlanService::getInstance().registerRunParam();
	}

	std::string PlanManagerFrameModel::getRunParam(const char* name)
	{
		return PlanService::getInstance().getRunParam(name);
	}

	void PlanManagerFrameModel::setRunRaram(const char* name, const char* value)
	{
		PlanService::getInstance().setRunRaram(name, value);
	}

	std::string PlanManagerFrameModel::getDefaultPlanAgentLocationName()
	{
		return PlanService::getInstance().getDefaultPlanAgentLocationName();
	}
	 

	void PlanManagerFrameModel::setBaseTitle(std::string strTitle)
	{
		m_strBaseTitle = strTitle;	
	}

	std::string PlanManagerFrameModel::getTitle()
	{
		return m_strBaseTitle;
	}

	std::string PlanManagerFrameModel::updateTitle(std::string strTitle)
	{
		return m_strBaseTitle + " - " + strTitle;
	}

	void PlanManagerFrameModel::cleanUp()
	{
		PlanService::getInstance().cleanUp();
	}

	void PlanManagerFrameModel::sendErrorMsgToUser(unsigned int msgId, const unsigned int msgType, const std::string param1, const std::string param2)
	{
		TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(msgId));
		dataContextPtr->pushParamValue(param1);
		dataContextPtr->pushParamValue(param2);

		TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, msgType));
		m_appController->getWinApp()->requestUserReply(eventArgs);
	}

	bool PlanManagerFrameModel::canEditCategories()
	{
		return OperatorRights::getInstance().canEditCategories();
	}

	bool PlanManagerFrameModel::canEditPlans()
	{
		return OperatorRights::getInstance().canEditPlans();
	}

	bool PlanManagerFrameModel::canApprovePlans()
	{
		return OperatorRights::getInstance().canApprovePlans();
	}

	bool PlanManagerFrameModel::canCustomisePlans()
	{
		return OperatorRights::getInstance().canCustomisePlans();
	}

	bool PlanManagerFrameModel::canControlPlans()
	{
		return OperatorRights::getInstance().canControlPlans();
	}

	bool PlanManagerFrameModel::canSchedulePlans()
	{
		return OperatorRights::getInstance().canSchedulePlans();
	}
}