/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/columns/ColumnAlarmType.cpp $
 * @author:  Karen Graham
 * @version: $Revision: #4 $
 *
 * Last modification: $DateTime: 2016/11/23 18:37:10 $
 * Last modified by:  $Author: Noel $
 * 
 * This class provides all the information for the alarm type column.
 */

/*#include "bus/alarm/alarm_list_control/src/StdAfx.h"*/
#include "bus/alarm/alarm_list_control/src/columns/ColumnAlarmType.h"
#include "core/data_access_interface/src/IAlarmTypeData.h"
#include "core/data_access_interface/src/AlarmTypeAccessFactory.h"
#include "core/utilities/src/DebugUtil.h"

using TA_Base_Core::DebugUtil;
namespace TA_Base_Bus
{
    ColumnAlarmType::ColumnAlarmType(int width,
                         bool isEnabled,
                         SORT_COLUMN sortCol,
                         IAlarmAction* doubleClickAction)
     : AbstractAlarmColumn(width, sortCol, isEnabled, doubleClickAction)
	{
    }


	const TAUtfString ColumnAlarmType::getItemText(unsigned long ulIndex)
    {
		unsigned long ulMessageTypeKey = 0;

		try
		{
			TAUtfString temp = TAUtfString::number(m_model->getDisplayDataWrap()->getItemNumber(ulIndex, MESSAGE_TYPE_KEY));
			//ulMessageTypeKey = m_model->getDisplayDataWrap()->getItemNumber(ulIndex, MESSAGE_TYPE_KEY);
		    //This class should be removed, due to there is no requirement for displaying alarm type.
			return temp;
		}
		catch(...)
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "wrong");
		}

		return "";
    }


} //End of TA_Base_Bus

