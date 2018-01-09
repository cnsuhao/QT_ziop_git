/**
 * The source code in this file is the property of 
 * Ripple Systems Pty Ltd and is not for redistribution
 * in any form.
 *
 * Source: $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/exceptions/src/AuthenticationAgentException.cpp $
 * @author N. Jardine
 * @version $Revision: #1 $
 * Last modification: $DateTime: 2015/10/09 15:42:35 $
 * Last modified by:  $Author: CM $
 * 
 * This file outlines a simple exception class for use with
 * authentication code. It is derived from TransactiveException.
 * This exception is thrown when an unexpected error occurrs in 
 * in the Authentication Agent.
 */

#include "core/exceptions/src/AuthenticationAgentException.h"

namespace TA_Base_Core
{


	AuthenticationAgentException::AuthenticationAgentException(const char *msg) throw(): 
		  TransactiveException(msg)
	{
	}	

	AuthenticationAgentException::~AuthenticationAgentException() throw()
	{

	}

} // Closes TA_Base_Core
