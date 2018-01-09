/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/ActivePlanMessageException.cpp $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  Implementation of .
  */

#include "app/response_plans/plan_agent/src/ActivePlanMessageException.h"

namespace TA_Base_App
{
    ActivePlanMessageException::ActivePlanMessageException() throw() : TransactiveException( "" )
	{
	}

	ActivePlanMessageException::ActivePlanMessageException(const std::string& msg) throw() : TransactiveException( msg )
	{
	}

	ActivePlanMessageException::~ActivePlanMessageException() throw()
	{
	}

} // TA_Base_App
