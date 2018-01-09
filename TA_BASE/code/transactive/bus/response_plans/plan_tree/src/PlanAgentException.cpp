/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/response_plans/plan_tree/src/PlanAgentException.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * Thrown by AgentPlanAccess when an error occurs while attempting to communicate with the Plan Agent.
  */

#include "bus\response_plans\plan_tree\src\PlanAgentException.h"


using namespace TA_Base_Bus;


PlanAgentException::PlanAgentException() throw()
{
}


PlanAgentException::PlanAgentException(const std::string& msg) throw() :
    TA_Base_Core::TransactiveException(msg)
{
}


PlanAgentException::~PlanAgentException() throw()
{
}
