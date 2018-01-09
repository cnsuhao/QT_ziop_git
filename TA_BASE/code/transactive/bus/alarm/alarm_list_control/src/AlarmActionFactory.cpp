/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/AlarmActionFactory.cpp $
 * @author:  Karen Graham
 * @version: $Revision: #2 $
 *
 * Last modification: $DateTime: 2016/05/11 17:52:04 $
 * Last modified by:  $Author: Noel $
 * 
 * This factory holds all the alarm actions that can be performed by this instance
 * of the alarm list. The main application needs to add the actions it wants to be
 * available from the list context menu. This class is not intended to be dynamic. All
 * actions should be added at the start and 
 */

#pragma warning(disable:4786)

#include "bus/alarm/alarm_list_control/src/AlarmActionFactory.h"
#include "bus/alarm/alarm_list_control/src/IAlarmAction.h"
#include "bus/alarm/alarm_list_control/src/IAlarmRetriever.h"

namespace TA_Base_Bus
{
    AlarmActionFactory::AlarmActionFactory(const AlarmActionList& actionList)
        : m_alarmActions(actionList)
    {
    }


    AlarmActionFactory::~AlarmActionFactory()
    {
        try
        {
            for(AlarmActionList::iterator iter = m_alarmActions.begin(); iter != m_alarmActions.end(); ++iter)
            {
                 if (*iter != NULL)
                 {
                     delete *iter;
                     *iter = NULL;
                 }
            }
        }
        catch( ... )
        {
        }
    }

	IAlarmAction* AlarmActionFactory::getAlarmAction(const unsigned int actionID)
	{
		IAlarmAction* pRetVal = 0;

		for (AlarmActionList::iterator iter = m_alarmActions.begin(); iter != m_alarmActions.end(); ++iter)
		{
			IAlarmAction* pTemp = *iter;
			if (pTemp->getActionControlId() == actionID)
			{
				pRetVal = pTemp;
				break;
			}
		}

		return pRetVal;
	}

} //End of TA_Base_Bus

