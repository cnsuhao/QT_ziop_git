/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/columns/ColumnEntityName.cpp $
 * @author:  Karen Graham
 * @version: $Revision: #4 $
 *
 * Last modification: $DateTime: 2016/11/23 18:37:10 $
 * Last modified by:  $Author: Noel $
 * 
 * This class provides all the information for the entity name column.
 */

/*#include "bus/alarm/alarm_list_control/src/StdAfx.h*/
#include "bus/alarm/alarm_list_control/src/columns/ColumnEntityName.h"

namespace TA_Base_Bus
{
    ColumnEntityName::ColumnEntityName(int width,
                         bool isEnabled,
                         SORT_COLUMN sortCol,
                         IAlarmAction* doubleClickAction)
     : AbstractAlarmColumn(width, sortCol, isEnabled, doubleClickAction)
	{
    }


	const TAUtfString ColumnEntityName::getItemText(unsigned long ulIndex)
    {
		TAUtfString strEntityName = "";
		
		try
		{
			strEntityName = TAUtfString::createFromMultitByteString(m_model->getDisplayDataWrap()->getItemString(ulIndex, ENTITY_NAME));
		}
		catch(...)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "wrong");			
		}

        return strEntityName;
    }


} //End of TA_Base_Bus
