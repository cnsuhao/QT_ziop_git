/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/exceptions/src/UtilitiesException.cpp $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  */
// UtilitiesException.cpp: implementation of the UtilitiesException class.
//
//////////////////////////////////////////////////////////////////////

#include "UtilitiesException.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace TA_Base_Core
{

	UtilitiesException::UtilitiesException() throw()
	{

	}

	UtilitiesException::UtilitiesException(const std::string& msg) 
		throw(): TransactiveException(msg)
	{

	}

	UtilitiesException::~UtilitiesException() throw()
	{

	}
}
