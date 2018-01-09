/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/exceptions/src/InterlockingException.cpp $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  */

#include "InterlockingException.h"


namespace TA_Base_Core
{
    InterlockingException::InterlockingException(const std::string& message) throw()
        : TransactiveException(message)
    {
    }

    InterlockingException::~InterlockingException() throw()
    {
    }
} // Closes TA_Base_Core
