/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/core/alarm/src/AlarmConstants.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * <description>
  *
  */
#if !defined(ALARM_CONSTANTS_H)
#define ALARM_CONSTANTS_H

#include <sys/timeb.h>

namespace TA_Base_Core
{
    namespace AlarmConstants
    {        
        const timeb defaultTime = {0,0,-1,-1};
    }
}

#endif // !defined(ALARM_CONSTANTS_H)
