/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/actions/ActionRelatedSchematic.cpp $
 * @author:  Karen Graham
 * @version: $Revision: #2 $
 *
 * Last modification: $DateTime: 2016/05/11 17:52:04 $
 * Last modified by:  $Author: Noel $
 * 
 * This class is for the action that displayed the schematic related to the alarm.
 */

#pragma warning(disable:4786)


#include "bus/alarm/alarm_list_control/src/actions/ActionRelatedSchematic.h"
#include "bus/alarm/alarm_list_control/src/AlarmRetriever.h"
// #include "bus/alarm/alarm_list_control/src/RightsChecker.h"
// #include "bus/alarm/alarm_list_control/src/DatabaseCache.h"

#include "bus/security/access_control/alarm_actions/src/AlarmActions.h"

 

#include "core/data_access_interface/entity_access/src/IEntityData.h"

#include "core/exceptions/src/AlarmListCtrlException.h"
#include "core/exceptions/src/AlarmActionException.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/ApplicationException.h"

#include "bus/alarm/alarm_list_control/src/ATSAlarmBridgeWrap.h"

#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/alarm/alarm_list_control/src/resource.h"

using TA_Base_Core::AlarmActionException;

namespace TA_Base_Bus
{
	 
 
 
	 
    ActionRelatedSchematic::ActionRelatedSchematic(bool isAvailableInContextMenu /*=true*/)
        : m_isAvailableInContextMenu(isAvailableInContextMenu)
    {
		 
    }
   
    ActionRelatedSchematic::~ActionRelatedSchematic()
    {
    }


    bool ActionRelatedSchematic::isAvailable(IAlarmRetriever& alarmRetriever)
    {
        
		return false;
    }


    void ActionRelatedSchematic::performAction(IAlarmRetriever& alarmRetriever)
    {
        FUNCTION_ENTRY("performAction");
		

        FUNCTION_EXIT;
    }
}
