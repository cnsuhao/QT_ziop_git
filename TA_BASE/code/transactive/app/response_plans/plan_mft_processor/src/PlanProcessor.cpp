/**
  * The source code in this file is the property of Ripple Systems and is not for redistribution in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_mft_processor/src/PlanProcessor.cpp $
  * @author:  Huang Jian
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  */

#include "app/response_plans/plan_mft_processor/src/PlanProcessor.h"
#include "app/response_plans/plan_mft_processor/src/ActivePlanAgency.h"

namespace TA_Base_App
{

    PlanProcessor::PlanProcessor()
    {
    }

    PlanProcessor::~PlanProcessor()
    {
    }

    void PlanProcessor::init( TA_Base_App::IDllMgrCallBack* callback )
    {
        ActivePlanAgency::instance()->init();
    }

    void PlanProcessor::uninit()
    {
        ActivePlanAgency::instance()->uninit();

        ActivePlanAgency::close();
    }

    std::string PlanProcessor::getDLLName() const
    {
        return "TA_Plan_Mft_Step_Message.dll";
    }

}