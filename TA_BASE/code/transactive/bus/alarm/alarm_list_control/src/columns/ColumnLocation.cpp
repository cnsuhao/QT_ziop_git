/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/columns/ColumnLocation.cpp $
 * @author:  Karen Graham
 * @version: $Revision: #4 $
 *
 * Last modification: $DateTime: 2016/11/23 18:37:10 $
 * Last modified by:  $Author: Noel $
 * 
 * This class provides all the information for the location column.
 */

/*#include "bus/alarm/alarm_list_control/src/StdAfx.h"*/
#include "bus/alarm/alarm_list_control/src/columns/ColumnLocation.h"
/*#include "bus/alarm/alarm_list_control/src/DatabaseCache.h"*/
#include "core/data_access_interface/src/ILocation.h"
#include "core/data_access_interface/src/LocationAccessFactory.h"
#include "core/utilities/src/DebugUtil.h"

using namespace TA_Base_Core;

namespace TA_Base_Bus
{
    ColumnLocation::ColumnLocation(int width,
                         bool isEnabled,
                         SORT_COLUMN sortCol,
                         IAlarmAction* doubleClickAction)
     : AbstractAlarmColumn(width, sortCol, isEnabled, doubleClickAction)
    {
    }


	const TAUtfString ColumnLocation::getItemText(unsigned long ulIndex)
    {
		TAUtfString strLocation;
		try
		{
			unsigned long ulLocation = m_model->getDisplayDataWrap()->getItemNumber(ulIndex, LOCATION_KEY);
			if (ulLocation != 0)
			{
				strLocation = TAUtfString::number(ulLocation);
				//UI component should not access database, it should be implemented in PModel.
				//If need, the conversion should be done in PModel
				/*TA_Base_Core::ILocation* pLocationData = TA_Base_Core::LocationAccessFactory::getInstance().getLocationByKey(ulLocation);
			
				if (NULL != pLocationData)
				{
					m_strLocation = pLocationData->getName();
				}*/
			}
		}
		catch(...)
		{ 
			LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "wrong");			
		}

		return strLocation;
    }


} //End of TA_Base_Bus

