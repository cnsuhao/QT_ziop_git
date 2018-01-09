/**
  * The source code in this file is the property of 
  * Ripple Systems Pty Ltd and is not for redistribution
  * in any form.
  *
  * Source: $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/exceptions/src/CtaRuntimeEntityParameterNotFoundException.cpp $
  * @author K. Graham
  * @version $Revision: #1 $
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * Exception to be thrown from CtaRuntime when entity configuration cannot be found
  */

#include "CtaRuntimeEntityParameterNotFoundException.h"


namespace TA_Base_Core
{
    CtaRuntimeEntityParameterNotFoundException::CtaRuntimeEntityParameterNotFoundException(const std::string& msg)
        throw(): TransactiveException(msg)
    {
        // Empty
    }


    CtaRuntimeEntityParameterNotFoundException::~CtaRuntimeEntityParameterNotFoundException() throw()
    {
        // Empty
    }
}