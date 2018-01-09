/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/exceptions/src/AlarmCacheCapacityExceededException.cpp $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  */

#include "AlarmCacheCapacityExceededException.h"

namespace TA_Base_Core
{

    AlarmCacheCapacityExceededException::AlarmCacheCapacityExceededException() throw()
    {}

    AlarmCacheCapacityExceededException::AlarmCacheCapacityExceededException(const std::string& msg) 
        throw(): TransactiveException(msg)
    {}

    AlarmCacheCapacityExceededException::~AlarmCacheCapacityExceededException() throw()
    {}
}
