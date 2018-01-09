/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_agent/src/ThirdPartyLibException.cpp $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  Implementation of .
  */

#include "app/response_plans/plan_agent/src/ThirdPartyLibException.h"

// namespace alias for Corba structures/types
 
namespace TA_Base_App
{
	ThirdPartyLibException::ThirdPartyLibException() throw() : TransactiveException( "" )
	{
	}

	ThirdPartyLibException::ThirdPartyLibException(const std::string& msg) throw() : TransactiveException( msg )
	{
	}
	
	ThirdPartyLibException::~ThirdPartyLibException() throw()
	{
	}

} // TA_Base_App
