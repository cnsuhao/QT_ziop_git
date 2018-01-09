/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/exceptions/src/FailoverException.cpp $
  * @author:  <Rob Ashcroft>
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * <description>
  */


// FailoverException.cpp: implementation of the FailoverException class.
//
//////////////////////////////////////////////////////////////////////

#include "FailoverException.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

namespace TA_Base_Core
{
    FailoverException::FailoverException() throw()
    {

    }

    FailoverException::FailoverException(const std::string& msg) throw() 
            : TA_Base_Core::TransactiveException(msg) , m_message(msg)
	        {

	        }

    FailoverException::~FailoverException() throw()
    {

    }

    const char* FailoverException::what() const throw()
    {
        return m_message.c_str();
    }
};
