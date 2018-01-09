/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/columns/ColumnSubsystem.cpp $
 * @author:  Karen Graham
 * @version: $Revision: #4 $
 *
 * Last modification: $DateTime: 2016/11/23 18:37:10 $
 * Last modified by:  $Author: Noel $
 * 
 * This class provides all the information for the subsystem column.
 */

/*#include "bus/alarm/alarm_list_control/src/StdAfx.h"*/
#include "bus/alarm/alarm_list_control/src/columns/ColumnSubsystem.h"
/*#include "bus/alarm/alarm_list_control/src/DatabaseCache.h"*/
#include "core/data_access_interface/src/SubsystemAccessFactory.h"
#include "core/data_access_interface/src/ISubsystem.h"

using namespace TA_Base_Core;

namespace TA_Base_Bus
{
    ColumnSubsystem::ColumnSubsystem(int width,
                         bool isEnabled,
                         SORT_COLUMN sortCol,
                         IAlarmAction* doubleClickAction)
     : AbstractAlarmColumn(width, sortCol, isEnabled, doubleClickAction)
    {
    }


	const TAUtfString ColumnSubsystem::getItemText(unsigned long ulIndex)
    {
		TAUtfString subSystem = "";
		try
		{
			unsigned long ulSubSystem = m_model->getDisplayDataWrap()->getItemNumber(ulIndex, SUBSYSTEM_KEY);
			//UI component should not access database, it should be implemented in PModel.
			//If need, the conversion should be done in PModel
			if (ulSubSystem != 0)
			{
				subSystem = TAUtfString::number(ulSubSystem);
			}
		}
		catch(...)
		{ 
			LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "wrong");			
		}
		return subSystem;

    }


} //End of TA_Base_Bus

