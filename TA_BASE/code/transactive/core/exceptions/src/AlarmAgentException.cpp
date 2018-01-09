/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/exceptions/src/AlarmAgentException.cpp $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  */

#include "AlarmAgentException.h"

namespace TA_Base_Core
{

    AlarmAgentException::AlarmAgentException() throw()
    {}

    AlarmAgentException::AlarmAgentException(const std::string& msg) 
        throw(): TransactiveException(msg)
    {}

    AlarmAgentException::~AlarmAgentException() throw()
    {}
}
