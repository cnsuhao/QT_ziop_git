/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/columns/ColumnSourceTimestamp.h $
 * @author:  Karen Graham
 * @version: $Revision: #3 $
 *
 * Last modification: $DateTime: 2016/11/22 14:12:39 $
 * Last modified by:  $Author: Noel $
 * 
 * This class provides all the information for the source timestamp column.
 */

#if !defined(AFX_ColumnSourceTimestamp_H__26C75118_4AC8_4594_BBE3_266FC53666A0__INCLUDED_)
#define AFX_ColumnSourceTimestamp_H__26C75118_4AC8_4594_BBE3_266FC53666A0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "bus/alarm/alarm_list_control/src/columns/AbstractAlarmColumn.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnIdentifiers.h"
#include "bus/common_control/src/common/TA_String.h"
namespace TA_Base_Bus
{

    class ColumnSourceTimestamp : public AbstractAlarmColumn
	{
		Q_DECLARE_TR_FUNCTIONS(ColumnSourceTimestamp)
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
        ColumnSourceTimestamp(int width,
                         bool isEnabled,
                         SORT_COLUMN sortCol,
                         IAlarmAction* doubleClickAction);


        virtual ~ColumnSourceTimestamp()
        {
        };


        /**
         * getColumnIdentifier
         *
         * @return long - A unique identifier for this column. This is defined in the file ColumnIdentifiers.h
         */
        virtual long getColumnIdentifier() const
        {
            return COL_SOURCETIMESTAMP;
        }


        /**
         * getColumnHeaderText
         *
         * @return std::string - Returns the column header text
         */
		virtual TAUtfString getColumnHeaderText() const
        {
			return tr("Date/Time");
        }


        /**
         * getItemText
         *
         * @param AlarmDetailCorbaDef* - The alarm to retrieve information for
         * @param DatabaseCache - This object can be used to retrieve database information if necessary
         *
         * @return std::string - This returns the text to display in the column for the specified alarm.
         */
		//++Noel
        //virtual const char* getItemText(TA_Base_Core::AlarmItem* displayItem);
		virtual const TAUtfString getItemText(unsigned long ulIndex);

        /**
         * setTimeFormat
		 *
		 * Sets the time format used by this column. This function asserts that the time
		 * format is not an empty string, but otherwise it is up to the client to ensure
		 * that the format passed is valid. The default format is "%d-%b-%y %H:%M:%S".
         *
         * @param const std::string& - The format string as per the strftime command
         */
        void setTimeFormat(const std::string& formatStr);


    private:
        

        ColumnSourceTimestamp& operator=(const ColumnSourceTimestamp& rhs);

        /**
         * Copy constructor - Hidden as not used
         */
        ColumnSourceTimestamp(const ColumnSourceTimestamp&);

		 
	};
} //End of TA_Base_Bus


#endif // !defined(AFX_ColumnSourceTimestamp_H__26C75118_4AC8_4594_BBE3_266FC53666A0__INCLUDED_)
