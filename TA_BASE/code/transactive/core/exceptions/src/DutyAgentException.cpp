/**
  *  The source code in this file is the property of
  *  Ripple Systems and is not for redistribution
  *  in any form.
  *
  *  Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/exceptions/src/DutyAgentException.cpp $
  *  @author:  Gregg Kirkpatrick
  *  @version: $Revision: #1 $
  *
  *  Last modification: $DateTime: 2015/10/09 15:42:35 $
  *  Last modified by:  $Author: CM $
  *
  *  Implementation of .
  */

#include "DutyAgentException.h"

namespace TA_Base_Core
{
		DutyAgentException::DutyAgentException() throw()
		{
		}

		DutyAgentException::DutyAgentException(const std::string& msg) throw() :
			m_msg(msg)
		{
		}
		
		const char* DutyAgentException::what() const throw()
		{
			return( m_msg.c_str() );
		}
		
		DutyAgentException::~DutyAgentException() throw()
		{
		}

} // TA_Base_Core
