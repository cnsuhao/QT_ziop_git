/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/exceptions/src/TcpSocketException.cpp $
  * @author:  J. Welton
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * Exception class thrown by the TcpSocket subsystem
  *
  */

#include "TcpSocketException.h"

namespace TA_Base_Core
{

    TcpSocketException::TcpSocketException() throw()
    {}

    TcpSocketException::TcpSocketException(const std::string& msg) 
        throw(): TransactiveException(msg)
    {}

    TcpSocketException::~TcpSocketException() throw()
    {}
}
