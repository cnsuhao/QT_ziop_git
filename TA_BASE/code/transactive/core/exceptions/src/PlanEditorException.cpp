/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/exceptions/src/PlanEditorException.cpp $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  */
// PlanEditorException.cpp: implementation of the PlanEditorException class.
//
//////////////////////////////////////////////////////////////////////

#include "PlanEditorException.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace TA_Base_Core
{

	PlanEditorException::PlanEditorException()
	{

	}

	PlanEditorException::PlanEditorException(const std::string& msg) 
		: TransactiveException(msg)
	{

	}

	PlanEditorException::~PlanEditorException()
	{

	}
}

