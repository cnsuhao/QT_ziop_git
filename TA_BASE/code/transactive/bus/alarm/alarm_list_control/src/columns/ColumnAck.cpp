/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/columns/ColumnAck.cpp $
 * @author:  Karen Graham
 * @version: $Revision: #3 $
 *
 * Last modification: $DateTime: 2016/11/22 14:12:39 $
 * Last modified by:  $Author: Noel $
 * 
 * This class provides all the information for the Acknowledge Column
 */

/*#include "bus/alarm/alarm_list_control/src/StdAfx.h"*/
#include "bus/alarm/alarm_list_control/src/columns/ColumnAck.h"
#include "bus/alarm/alarm_store_library/src/LogAlarmItem.h"
#include "core/utilities/src/DebugUtil.h"
 

using TA_Base_Core::DebugUtil;

namespace TA_Base_Bus
{
	TAUtfString ColumnAck::ALARM_ACK = tr("Yes");
	TAUtfString ColumnAck::ALARM_NOT_ACK = tr("No");

    ColumnAck::ColumnAck(int width,
                         bool isEnabled,
                         SORT_COLUMN sortCol,
                         IAlarmAction* doubleClickAction)
     : AbstractAlarmColumn(width, sortCol, isEnabled, doubleClickAction)
    {
    }

	const TAUtfString ColumnAck::getItemText(unsigned long ulIndex)
	{
		try
		{
			if (strcmp(m_model->getDisplayDataWrap()->getItemString(ulIndex, ALARM_ACKNOWLEDGE_BY).c_str(), "") != 0)
			{
				return ALARM_NOT_ACK;
			}
			else
			{
				return ALARM_ACK;
			}
		}
		catch(...)
		{ 
			LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "wrong");
		}
		return "";
    }


} //End of TA_Base_Bus

