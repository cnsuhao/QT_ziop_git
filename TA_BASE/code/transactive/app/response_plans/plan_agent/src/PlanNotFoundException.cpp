/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/PlanNotFoundException.cpp $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  Implementation of .
  */

#include "app/response_plans/plan_agent/src/PlanNotFoundException.h"

// namespace alias for Corba structures/types
 
namespace TA_Base_App
{
    PlanNotFoundException::PlanNotFoundException() throw() : TransactiveException( "Plan not found" ),
    m_errorCode(0),
    m_planId(0),
    m_instanceId(0)
	{
	}

	PlanNotFoundException::PlanNotFoundException(const std::string& msg, const int error, 
        const unsigned long planId , const unsigned long instanceId ) throw() : TransactiveException( msg ),
    m_errorCode(error),
    m_planId(planId),
    m_instanceId(instanceId)
	{
	}

	PlanNotFoundException::~PlanNotFoundException() throw()
	{
	}

} // TA_Base_App
