#include "app/response_plans/plan_manager/src/StdAfx.h"
#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "IGUIApplicationControllerHelper.h"

using namespace TA_Base_App;
using TA_Base_Bus::ITransActiveWinApp;

IGUIApplicationControllerHelper::IGUIApplicationControllerHelper(void)
{
}

IGUIApplicationControllerHelper::~IGUIApplicationControllerHelper(void)
{
}

TA_Base_Bus::IGUIApplicationController* TA_Base_App::IGUIApplicationControllerHelper::getAppController()
{
	return dynamic_cast<ITransActiveWinApp*>(AfxGetApp())->getApplicationController();
}
