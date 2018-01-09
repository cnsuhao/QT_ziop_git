/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/3001_TIP/TA_BASE/transactive/bus/alarm/alarm_list_control/src/columns/ColumnAlarmType.h $
 * @author:  Karen Graham
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2009/12/29 14:17:48 $
 * Last modified by:  $Author: grace.koh $
 * 
 * This class provides all the information for the alarm type column.
 */

#if !defined(AFX_ColumnAlarmType_H__26C75118_4AC8_4594_BBE3_266FC53666A0__INCLUDED_)
#define AFX_ColumnAlarmType_H__26C75118_4AC8_4594_BBE3_266FC53666A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bus/alarm/alarm_list_control/src/columns/AbstractAlarmColumn.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnIdentifiers.h"

namespace TA_Base_Bus
{

    class ColumnAlarmOperatorType : public AbstractAlarmColumn
	{
		Q_DECLARE_TR_FUNCTIONS(ColumnAlarmOperatorType)
	public:

        
        /**
         * Constructor
         *
         * @param int - The width of the column
         * @param bool - Whether the column is currently enabled
         * @param Sort* - The ascending sort for the column. If the sort is NULL then the default sort will be used
         * @param Sort* - The descending sort for the column. If the sort is NULL then the default sort will be used
         * @param IAlarmAction* - The action to perform when the user double-clicks on the column. This can be NULL
         *                        signifying no action.
         */
        ColumnAlarmOperatorType(int width,
                         bool isEnabled,
                         SORT_COLUMN sortCol,
                         IAlarmAction* doubleClickAction);


        virtual ~ColumnAlarmOperatorType()
        {
        };


        /**
         * getColumnIdentifier
         *
         * @return long - A unique identifier for this column. This is defined in the file ColumnIdentifiers.h
         */
        virtual long getColumnIdentifier() const
        {
            return COL_OPERATORTYPE;
        }


        /**
         * getColumnHeaderText
         *
         * @return std::string - Returns the column header text
         */
		virtual TAUtfString getColumnHeaderText() const
        {
			return tr("T");
        }


        /**
         * getItemText
         *
         * @param AlarmDetailCorbaDef* - The alarm to retrieve information for
         * @param DatabaseCache - This object can be used to retrieve database information if necessary
         *
         * @return std::string - This returns the text to display in the column for the specified alarm.
         */
        //virtual const char* getItemText(TA_Base_Core::AlarmItem* displayItem);
		virtual const TAUtfString getItemText(unsigned long ulIndex);

    private:
        /**
         * operator= - Hidden as not used
         */
        ColumnAlarmOperatorType& operator=(const ColumnAlarmOperatorType& rhs);

        /**
         * Copy constructor - Hidden as not used
         */
        ColumnAlarmOperatorType(const ColumnAlarmOperatorType&);
 
	};
} //End of TA_Base_Bus


#endif // !defined(AFX_ColumnAlarmOperatorType_H__26C75118_4AC8_4594_BBE3_266FC53666A0__INCLUDED_)
