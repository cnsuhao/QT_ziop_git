/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/columns/ColumnSourceTimestamp.cpp $
 * @author:  Karen Graham
 * @version: $Revision: #4 $
 *
 * Last modification: $DateTime: 2016/11/22 14:12:39 $
 * Last modified by:  $Author: Noel $
 * 
 * This class provides all the information for the source timestamp column.
 */
/*#include "bus/alarm/alarm_list_control/src/StdAfx.h"*/
#include "bus/alarm/alarm_list_control/src/columns/ColumnSourceTimestamp.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/RunParams.h"

#include "bus/alarm/alarm_common/src/AlarmConstants.h"

#include <sys/timeb.h>
#include "core/utilities/src/DateFormat.h" // TD12474 
#include "core/utilities/src/DebugUtil.h"
#include "bus/alarm/alarm_store_library/src/LogAlarmItem.h"
#include "bus/common_control/src/common/TA_String.h"
#include "bus/common_control/src/common/DateTimeUtility.h"
#include <QDateTime>

using TA_Base_Core::DebugUtil;
using TA_Base_Core::RunParams;

namespace TA_Base_Bus
{
    ColumnSourceTimestamp::ColumnSourceTimestamp(int width,
                         bool isEnabled,
                         SORT_COLUMN sortCol,
                         IAlarmAction* doubleClickAction)
     : AbstractAlarmColumn(width, sortCol, isEnabled, doubleClickAction)
	   //m_formatStr("%d-%b-%y %H:%M:%S") // TD12474
    {
		 
    }

	 

	const TAUtfString ColumnSourceTimestamp::getItemText(unsigned long ulIndex)
	{
		unsigned long tm = 0;
		unsigned long mi = 0;
		try
		{
			tm = m_model->getDisplayDataWrap()->getItemNumber(ulIndex, TRANSACTIVETIME_TIME);
            //mi = m_model->getDisplayDataWrap()->getItemNumber(ulIndex, TRANSACTIVETIME_MILI);
		}
		catch(...)
		{ 
 			LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "wrong");

			return "";
		}

		if (tm == 0 ) return "";
		
	    return DateTimeUtility::toDateTimeString(tm);	
    }


} //End of TA_Base_Bus

