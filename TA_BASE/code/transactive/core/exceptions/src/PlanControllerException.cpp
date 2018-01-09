/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/exceptions/src/PlanControllerException.cpp $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  */
// PlanControllerException.cpp: implementation of the PlanControllerException class.
//
//////////////////////////////////////////////////////////////////////

#include "PlanControllerException.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace TA_Base_Core
{

	PlanControllerException::PlanControllerException()
	{

	}

	PlanControllerException::PlanControllerException(const std::string& msg) 
		: TransactiveException(msg)
	{

	}

	PlanControllerException::~PlanControllerException()
	{

	}
}
