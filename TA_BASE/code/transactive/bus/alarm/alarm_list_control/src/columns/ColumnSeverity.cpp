/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/columns/ColumnSeverity.cpp $
 * @author:  Karen Graham
 * @version: $Revision: #4 $
 *
 * Last modification: $DateTime: 2016/11/23 18:37:10 $
 * Last modified by:  $Author: Noel $
 * 
 * This class provides all the information for the severity column.
 */

/*#include "bus/alarm/alarm_list_control/src/StdAfx.h"*/
#include "bus/alarm/alarm_list_control/src/columns/ColumnSeverity.h"
/*#include "bus/alarm/alarm_list_control/src/DatabaseCache.h"*/
#include "core/utilities/src/DebugUtil.h"
#include "bus/alarm/alarm_store_library/src/LogAlarmItem.h"

using TA_Base_Core::DebugUtil;
namespace TA_Base_Bus
{
    ColumnSeverity::ColumnSeverity(int width,
                         bool isEnabled,
                         SORT_COLUMN sortCol,
                         IAlarmAction* doubleClickAction)
     : AbstractAlarmColumn(width, sortCol, isEnabled, doubleClickAction)
    {
    }

	const TAUtfString ColumnSeverity::getItemText(unsigned long ulIndex)
	{
		TAUtfString strSeverity;
		try
		{
			int nSeverity = m_model->getDisplayDataWrap()->getItemNumber(ulIndex, ALARM_SEVERITY);
			if (nSeverity != 0)
			{
				strSeverity = TAUtfString::number(nSeverity);
			}
		
		}
		catch(...)
		{ 
			LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "wrong");			
		}
		return strSeverity;
    }


} //End of TA_Base_Bus

