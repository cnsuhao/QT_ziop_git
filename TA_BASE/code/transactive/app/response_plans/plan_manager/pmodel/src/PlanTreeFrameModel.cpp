#include "PlanTreeFrameModel.h"

#include "PlanService.h"
#include "app/response_plans/plan_manager/pmodel/src/PlanManagerConsts.h"

namespace TA_Base_App
{
	void PlanTreeFrameModel::registerSettings()
	{
		FUNCTION_ENTRY("registerSettings");

		PlanService::getInstance().registerPlanTreeSettings();

		FUNCTION_EXIT;
	}


	void PlanTreeFrameModel::deregisterSettings()
	{
		FUNCTION_ENTRY("deregisterSettings");

		PlanService::getInstance().deregisterPlanTreeSettings();

		FUNCTION_EXIT;
	}
}