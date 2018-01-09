/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/actions/ActionAcknowledgeAllAlarms.h $
 * @author:  Karen Graham
 * @version: $Revision: #3 $
 *
 * Last modification: $DateTime: 2016/11/22 14:12:39 $
 * Last modified by:  $Author: Noel $
 * 
 * This class is for the action that acknowledges all visible alarms
 */

#ifndef ACTION_ACKNOWLEDGE_ALL_ALARMS_H_SAT_21_02_04
#define ACTION_ACKNOWLEDGE_ALL_ALARMS_H_SAT_21_02_04

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <string>

#include "bus/alarm/alarm_list_control/src/IAlarmAction.h"

namespace TA_Base_Core
{
    class AlarmModificationHelper;
}


namespace TA_Base_Bus
{
    class ActionAcknowledgeAllAlarms : public virtual IAlarmAction
    {
		Q_DECLARE_TR_FUNCTIONS(ActionAcknowledgeAllAlarms)

    public:

        /**
         * Constructor
         *
         * @param bool - This indicates if this action should be available in the alarm list context
         *               menu. This will default to true and the action will be added to the menu.
         */
        ActionAcknowledgeAllAlarms( bool isAvailableInContextMenu = true);


        /**
         * Destructor
         */
        virtual ~ActionAcknowledgeAllAlarms();


        /**
         * isAvailableInContextMenu
         *
         * This should indicate whether this action is available in the alarm list context menu
         *
         * @return bool - true if this action should be in the context menu. False otherwise
         */
        inline virtual bool isAvailableInContextMenu() const
        {
            return m_isAvailableInContextMenu;
        }


        /**
         * getMenuEntry
         *
         * This will determine the entry that should appear in any menus that display this action
         *
         * @return string - The menu entry including & for the shortcut letter
         */
		inline virtual QString getMenuEntry() const
        {
            return tr("Acknowledge All");
        };


        /**
         * getActionControlId
         *
         * This should return the id to use for the menu item. This id will be used to determine what
         * item the user has selected from the context menu.
         *
         * @return unsigned int - A control id. This should not be the same as any other control id and must be within
         *                        the range 4000 - 4999.
         */
        inline virtual unsigned int getActionControlId() const
        {
            return ACKNOWLEDGE_ALL;
        };


        /**
         * isAvailable
         *
         * This is called when the application/list needs to determine if an action is available at
         * the current time. This is based on the user's access rights as well as which alarms are
         * currently selected.
         *
         * @param IAlarmRetriever& - The object to use to find details and perform actions.
         *
         * @return bool - true if the action is available to the user, false if it is not.
         */
        virtual bool isAvailable(IAlarmRetriever& alarmRetriever);


        /** 
         * performAction
         *
         * This is called when this action needs to be performed. This should only be called once checking
         * that the action isAvailable().
         *
         * @param IAlarmRetriever& - The object to use to find details and perform actions.
         *
         * @exception AlarmActionException - This should be thrown if any error is encountered. The message
         *                                   in the exception should be set to a suitable error message that
         *                                   can be displayed directly to the user.
         */
        virtual void performAction(IAlarmRetriever& alarmRetriever);


    private:
        // Hidden as not required
        ActionAcknowledgeAllAlarms(ActionAcknowledgeAllAlarms&);
        ActionAcknowledgeAllAlarms& operator=(const ActionAcknowledgeAllAlarms&);

		TA_Base_Core::AlarmModificationHelper& m_alarmHelper;

        bool m_isAvailableInContextMenu;
     
		unsigned long m_cctvTrainPreConflictAlarmTypeKey;
		unsigned long m_trainCctvCameraAlarmTypeKey;
    };

}
#endif // ACTION_ACKNOWLEDGE_ALARM_H_THU_19_02_04
