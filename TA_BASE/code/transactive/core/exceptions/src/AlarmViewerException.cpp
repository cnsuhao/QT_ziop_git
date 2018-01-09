/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/exceptions/src/AlarmViewerException.cpp $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  */
// AlarmViewerException.cpp: implementation of the AlarmViewerException class.
//
//////////////////////////////////////////////////////////////////////

#include "AlarmViewerException.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace TA_Base_Core
{
	AlarmViewerException::AlarmViewerException()
	{

	}

	AlarmViewerException::AlarmViewerException(const std::string& msg) 
		: TransactiveException(msg)
	{

	}	

	AlarmViewerException::~AlarmViewerException()
	{

	}
}
