/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/utilities/src/MatrixException.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * Thrown by Matrix when an attempt is made to access a nonexisting element.
  *
  */

#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/MatrixException.h"


namespace TA_Base_Core
{
    MatrixException::MatrixException() throw()
    {
    }


    MatrixException::MatrixException(const std::string& msg) throw() :
        TransactiveException(msg)
    {
    }


    MatrixException::~MatrixException() throw()
    {
    }
}
