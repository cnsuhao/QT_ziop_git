/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/actions/ActionRelatedTrend.h $
 * @author:  Karen Graham
 * @version: $Revision: #3 $
 *
 * Last modification: $DateTime: 2016/11/22 14:12:39 $
 * Last modified by:  $Author: Noel $
 * 
 * This class is for the action that displays the trend viewer with related information
 */

#ifndef ACTION_RELATED_TREND_H_FRI_20_02_04
#define ACTION_RELATED_TREND_H_FRI_20_02_04

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include <string>

#include "bus/alarm/alarm_list_control/src/IAlarmAction.h"


namespace TA_Base_Bus
{
    class ActionRelatedTrend : public virtual IAlarmAction
    {
		Q_DECLARE_TR_FUNCTIONS(ActionRelatedTrend)
    public:

        /**
         * Constructor
         *
         * @param bool - This indicates if this action should be available in the alarm list context
         *               menu. This will default to true and the action will be added to the menu.
         */
        ActionRelatedTrend(bool isAvailableInContextMenu = true);


        /**
         * Destructor
         */
        virtual ~ActionRelatedTrend();


        /**
         * isAvailableInContextMenu
         *
         * This should indicate whether this action is available in the alarm list context menu
         *
         * @return bool - true if this action should be in the context menu. False otherwise
         */
        virtual bool isAvailableInContextMenu() const
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
        virtual QString getMenuEntry() const
        {
            return tr("Related &Trend...");
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
        virtual unsigned int getActionControlId() const
        {
            return RELATED_TREND;
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
		virtual void preformPreAction();

    private:
        // Hidden as not required
        ActionRelatedTrend(ActionRelatedTrend&);
        ActionRelatedTrend& operator=(const ActionRelatedTrend&);

		unsigned long getTrendViewerKey();

		bool m_isAvailableInContextMenu;

		unsigned long m_trendViewerKey;
    };

}
#endif // ACTION_RELATED_TREND_H_FRI_20_02_04
