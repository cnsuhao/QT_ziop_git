/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/event/event_viewer/src/CombinedEventItem.cpp $
 * @author:  Bradley Cleaver
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/09/03 14:11:12 $
 * Last modified by:  $Author: CM $
 * 
 * CombinedEventItem is an implementation of the DisplayItem abstract class. It contains the data for one row in the list, and 
 * defines the standard ordering operator (<) for list sorting.
 */

#ifdef __WIN32__
#pragma warning(disable:4786)
#endif

#include <sstream>

//#include "app/event/event_viewer/src/stdafx.h"
//#include "app/event/event_viewer/src/EventViewer.h"
#include "app/event/event_viewer/pmodel/src/CombinedEventItem.h"
#include "app/event/event_viewer/pmodel/src/CombinedEventFilter.h"
#include "app/event/event_viewer/pmodel/src/DatabaseAccess.h"
#include "core/data_access_interface/src/AlarmData.h"
//#include "core/data_access_interface/src/IAlarmData.h"
#include "core/utilities/src/RunParams.h"

// NOTE: This inclusion of the CombinedEventFilter is simply to gain access to the statically declared variables. While
// this does lead to a cross-dependency (as AuditEventFilter is dependant on this class), that is preferable to 
// hard coding string variables throughout the application
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/DateFormat.h" // TD12474 
#include "core/data_access_interface/src/ICombinedEventData.h"
#include <iomanip>

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
//#define new DEBUG_NEW
#endif

enum TA_Base_Core::EventCategory;
namespace
{
    const std::string DEFAULT_VALUE("");
	// Alarm Comments identifiers
    const std::string COMMENT_IDENTIFIER("C");
	// Alarm DSS Plan identifiers
	const std::string AUTO_PLAN_NOT_RUN_ID("A");
	const std::string SEMI_AUTO_PLAN_NOT_RUN_ID("S");
	const std::string PLAN_RUN_ID("R");
	const std::string PLAN_POSTPONED_ID("X");
	const std::string NO_PLAN("");
	// Alarm Avalanche Status identifiers
	const std::string AVL_HEAD_ID("H");
	const std::string AVL_CHILD_ID("C");
	// Alarm Mms Status identifiers
	const std::string MMS_AUTO_ID("A");
	const std::string MMS_SEMI_ID("S");
	const std::string MMS_JR_SENT_ID("R");
	const std::string MMS_FAILED_ID("A");  // change from "D", this might be deleted in future.
	const std::string MMS_INHIBIT_ID("I");

}

bool CombinedEventItem::is_set_millisec = TA_Base_Core::RunParams::getInstance().isSet( EventConstants::RPARAM_MILLISECOND.c_str());
std::string CombinedEventItem::dateFormatStr = TA_Base_Core::DateFormat().getDateFormat(TA_Base_Core::DateFormat::DateTimeFormat4);
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
CombinedEventItem::CombinedEventItem():DisplayItem()
{

}

//Finished //TODO: add c_str() to the temporary return string.
CombinedEventItem::CombinedEventItem( TA_Base_Core::ICombinedEventData* const eventData )
{
    TA_ASSERT( NULL != eventData, "CombinedEventItem constructor provided with NULL data." );

	if( eventData->getEventCategoryType() == TA_Base_Core::ATSCategory)
	{
		m_eventCategory = TA_Base_Core::ATSCategory;
	}else 
	{
		m_eventCategory = TA_Base_Core::ISCSCategory;
	}

	m_key = eventData->getKey();

	m_atsEventID = eventData->getATSEventID().c_str();
	

    // Convert the creation time into a string in the required format.
	// TD #11252
	// Format the event date as "dd-Mon-yy hh:mm:ss"
	// eg. "28-Jul-05 10:23:40"

	// TD12474 ++
	//std::string timePart(CTime(eventData->getCreateTime().time).Format("%d-%b-%Y %H:%M:%S"));

// 	TA_Base_Core::DateFormat dateFormat; 
// 	std::string dateFormatStr = dateFormat.getDateFormat(TA_Base_Core::DateFormat::DateTimeFormat4);
	char formatStr [80];
	struct tm * timeInfo;
	time_t ttt = eventData->getCreateTime().time;
	timeInfo = localtime(&ttt);
	strftime (formatStr,80,dateFormatStr.c_str(),timeInfo);
	std::string timePart(formatStr);
	// ++ TD12474

    std::stringstream fullString;
    fullString << timePart;
	strftime (formatStr,80,"%Y%m%d%H%M%S",timeInfo);
	m_strCreateTime = formatStr;
    //m_strCreateTime = CTime(eventData->getCreateTime().time).Format("%Y%m%d%H%M%S");
	
	// wenguang++
	// Change the millisecond setting to the RUNPARAM
	/*if (TA_Base_Core::RunParams::getInstance().isSet( EventConstants::RPARAM_MILLISECOND.c_str()))*/
	if(is_set_millisec)
	{
		fullString << "." << std::setfill('0') << std::setw(3) << eventData->getCreateTime().millitm;
	}

    // Insert the derived time string into the data
    m_columnData.insert(std::map<std::string,std::string>::value_type
        (EVDisplayStrRes::GetInstance()->COLUMN_DATETIME,
         fullString.str()) );
    m_createTime = eventData->getCreateTime();

    // Insert the severity name.
    m_columnData.insert(std::map<std::string,std::string>::value_type
        (EVDisplayStrRes::GetInstance()->COLUMN_SEVERITY,
          eventData->getAlarmSeverityName().c_str() ) );

    // Insert the asset name.
    m_columnData.insert(std::map<std::string,std::string>::value_type
        (EVDisplayStrRes::GetInstance()->COLUMN_ASSET,
         eventData->getAssetName().c_str() ) );

	// Insert the alarm status.
	DatabaseAccess& data=DatabaseAccess::getInstance();
	std::map<std::string, unsigned long> alarmStateMap = data.getAlarmStatesMap_StringKey();
	std::string eventTypeName = eventData->getEventTypeName().c_str();
	if(alarmStateMap.find(eventTypeName) != alarmStateMap.end() )
	{
		m_columnData.insert(std::map<std::string,std::string>::value_type
			(EVDisplayStrRes::GetInstance()->COLUMN_ALARM_STATUS,eventTypeName ) );
	}else
	{
		m_columnData.insert(std::map<std::string,std::string>::value_type
			(EVDisplayStrRes::GetInstance()->COLUMN_ALARM_STATUS,"" ) );
	}


    // Insert the description.

	/////////////////////////////////////////////////////////////////////
	//CL-15090 modified by: zhangjunsheng 2008/08/06 14:00:00
	
	std::string strTem = eventData->getDescription().c_str();
	std::string strDescription("JobRequestSubmitted from ");
	std::string strValue("");
	std::string cstrTem("JobRequestSubmitted");
	if (cstrTem.compare(strTem.substr(0,19)) == 0 )
	{
		strDescription += eventData->getAssetName().c_str();
		m_columnData.insert(std::map<std::string,std::string>::value_type
			(EVDisplayStrRes::GetInstance()->COLUMN_DESCRIPTION,
			strDescription ) );
		m_columnData.insert(std::map<std::string,std::string>::value_type
			(EVDisplayStrRes::GetInstance()->COLUMN_VALUE,
			strValue ) );
	}

	else
	{
		m_columnData.insert(std::map<std::string,std::string>::value_type
			(EVDisplayStrRes::GetInstance()->COLUMN_DESCRIPTION,
			eventData->getDescription().c_str() ) );
		
		// Insert the value.
		std::string valueStr(eventData->getValue().c_str());
		//TD12415
		// Change the format 20.0(0C) to correct format of degree Celsius
		// using ALT+0176
		//CString degreeString = valueStr.c_str();
		std::size_t pos = valueStr.find("?C");
		if (pos != std::string::npos)
		{
			valueStr.replace(pos,1,"?");  ///TODO Check
		}
		//valueStr.assign(degreeString);
		
		//TD12415
		
		m_columnData.insert(std::map<std::string,std::string>::value_type
			(EVDisplayStrRes::GetInstance()->COLUMN_VALUE,
			valueStr ) );
	}
	
    /*m_columnData.insert(std::map<std::string,std::string>::value_type
        (EVDisplayStrRes::GetInstance()->COLUMN_DESCRIPTION,
        eventData->getDescription() ) );

    // Insert the value.
    std::string valueStr(eventData->getValue());
	//TD12415
	// Change the format 20.0(0C) to correct format of degree Celsius
	// using ALT+0176
	CString degreeString = valueStr.c_str();

	if (degreeString.Find("?C", 0) >= 0)
	{
		degreeString.Replace(_T('?'), _T('?));
	}
	valueStr.assign(degreeString);

	//TD12415

    m_columnData.insert(std::map<std::string,std::string>::value_type
        (EVDisplayStrRes::GetInstance()->COLUMN_VALUE,
         valueStr ) );*/
    //CL-15090
	/////////////////////////////////////////////////////////////////////

	// Insert the "T" column value.
	m_columnData.insert(std::map<std::string,std::string>::value_type
		(EVDisplayStrRes::GetInstance()->COLUMN_T,
		eventData->getOperationMode().c_str() ) );

    // Insert the MMS state.
    m_columnData.insert(std::map<std::string,std::string>::value_type
        (EVDisplayStrRes::GetInstance()->COLUMN_MMS,
         eventData->getAlarmMmsState().c_str() ) );

    // Insert the DSS state.
    m_columnData.insert(std::map<std::string,std::string>::value_type
        (EVDisplayStrRes::GetInstance()->COLUMN_DSS,
         eventData->getAlarmDssState().c_str() ) );
    
    // Insert the AVL state.
    m_columnData.insert(std::map<std::string,std::string>::value_type
        (EVDisplayStrRes::GetInstance()->COLUMN_AVL,
         eventData->getAlarmAvlState().c_str() ) );

    // Insert the operator name.
    m_columnData.insert(std::map<std::string,std::string>::value_type
        (EVDisplayStrRes::GetInstance()->COLUMN_OPERATOR,
         eventData->getOperatorName().c_str() ) );

    // Insert the comment.
    m_columnData.insert(std::map<std::string,std::string>::value_type
        (EVDisplayStrRes::GetInstance()->COLUMN_COMMENT,
        eventData->getAlarmComment().c_str() ) );

    // Insert the alarmid.
    m_columnData.insert(std::map<std::string,std::string>::value_type
        (EVDisplayStrRes::GetInstance()->HIDDEN_COLUMN_ALARMID,
        eventData->getAlarmId().c_str() ) );

    // Insert the ack status.
    m_columnData.insert(std::map<std::string,std::string>::value_type
        (EVDisplayStrRes::GetInstance()->HIDDEN_COLUMN_ACKED,
        eventData->getAlarmAck().c_str() ) );

    // Insert the close status.
    m_columnData.insert(std::map<std::string,std::string>::value_type
        (EVDisplayStrRes::GetInstance()->HIDDEN_COLUMN_STATUS,
         eventData->getAlarmStatus().c_str() ) );

// 	m_columnData.insert(std::map<std::string,std::string>::value_type
// 		(EVDisplayStrRes::GetInstance()->HIDDEN_COLUMN_SELECTED,
// 		"N" ) );

	//this redundant column is just for sorting:
	string assetName =( m_columnData.find(EVDisplayStrRes::GetInstance()->COLUMN_ASSET))->second.c_str();
	string desc = ( m_columnData.find(EVDisplayStrRes::GetInstance()->COLUMN_DESCRIPTION))->second.c_str();
    string operatorName = (m_columnData.find(EVDisplayStrRes::GetInstance()->COLUMN_OPERATOR))->second.c_str();
	string value = (m_columnData.find(EVDisplayStrRes::GetInstance()->COLUMN_VALUE))->second.c_str();
	std::transform(assetName.begin(), assetName.end(), assetName.begin(),toupper);
	std::transform(desc.begin(), desc.end(), desc.begin(),toupper);
	std::transform(operatorName.begin(), operatorName.end(), operatorName.begin(),toupper);
	std::transform(value.begin(), value.end(), value.begin(),toupper);
	m_columnData.insert(std::map<std::string,std::string>::value_type
		(CombinedEventFilter::COLUMN_ASSET_UPPER,assetName ) );
	m_columnData.insert(std::map<std::string,std::string>::value_type
		(CombinedEventFilter::COLUMN_DESCRIPTION_UPPER,desc ) );
	m_columnData.insert(std::map<std::string,std::string>::value_type
		(CombinedEventFilter::COLUMN_OPERATOR_UPPER,operatorName ) );
	m_columnData.insert(std::map<std::string,std::string>::value_type
		(CombinedEventFilter::COLUMN_VALUE_UPPER,value ) );
}


CombinedEventItem::~CombinedEventItem()
{
}
void CombinedEventItem::setEventCategory(  TA_Base_Core::EventCategory ec )
{
	m_eventCategory= ec;
}

TA_Base_Core::EventCategory CombinedEventItem::getEventCategory()
{
	return m_eventCategory;
}

unsigned long CombinedEventItem::getKey()
{
	return m_key;
}

std::string CombinedEventItem::getATSEventID()
{
	return m_atsEventID;
}

void CombinedEventItem::setColumnData (string columnName, string value)
{
	std::map<std::string,std::string>::iterator iter = m_columnData.find(columnName);

	if (iter != m_columnData.end())
	{
		iter->second = value;
	}
}

const std::string CombinedEventItem::getColumnDataForCompare(const std::string& columnName)
{
	std::map<std::string,std::string>::iterator iter;

	if( EVDisplayStrRes::GetInstance()->COLUMN_DESCRIPTION == columnName)
	{
		iter = m_columnData.find(CombinedEventFilter::COLUMN_DESCRIPTION_UPPER);
		if(iter!=m_columnData.end())
			return iter->second.c_str();
		else return "";
	}
	if( EVDisplayStrRes::GetInstance()->COLUMN_ASSET == columnName)
	{
		iter = m_columnData.find(CombinedEventFilter::COLUMN_ASSET_UPPER);
		if(iter!=m_columnData.end())
			return iter->second.c_str();
		else return "";
	}
	if( EVDisplayStrRes::GetInstance()->COLUMN_OPERATOR == columnName)
	{
		iter = m_columnData.find(CombinedEventFilter::COLUMN_OPERATOR_UPPER);
		if(iter!=m_columnData.end())
			return iter->second.c_str();
		else return "";
	}
	if( EVDisplayStrRes::GetInstance()->COLUMN_VALUE == columnName)
	{
		iter = m_columnData.find(CombinedEventFilter::COLUMN_VALUE_UPPER);
		if(iter!=m_columnData.end())
			return iter->second.c_str();
		else return "";
	}
	else return getColumnData(columnName);
}
const std::string CombinedEventItem::getColumnData(const std::string& columnName)
{
    // Try to find the specified column in the data available for this item
	std::map<std::string,std::string>::iterator iter = m_columnData.find(columnName);

    if (iter != m_columnData.end())
    {
        // If the comment column is requested then we only want to display a 'C'.
        if ( ( EVDisplayStrRes::GetInstance()->COLUMN_COMMENT == columnName ) && ( 0 < iter->second.size() ) )
        {
            return COMMENT_IDENTIFIER.c_str();
        }
		else if ( ( EVDisplayStrRes::GetInstance()->COLUMN_MMS == columnName ) && ( 0 < iter->second.size() ) )
		{
			// this is the MMS status column
			 return getMmsStatusIdentifier(iter->second);
		}
		else if ( ( EVDisplayStrRes::GetInstance()->COLUMN_DSS == columnName ) && ( 0 < iter->second.size() ) )
		{
			// this is the DSS Plan column
			return getDssPlanStatusIdentifier(iter->second);
		}
		else if ( ( EVDisplayStrRes::GetInstance()->COLUMN_AVL == columnName ) && ( 0 < iter->second.size() ) )
		{
			// this is the AVL status column
			return getAvlStatusIdentifier(iter->second);
		}
		else if ( ( EVDisplayStrRes::GetInstance()->COLUMN_SEVERITY == columnName ) && ( 0 < iter->second.size() ) )
		{
			// this is the severity column
			return getSevIdentifier(iter->second);
		}

        // If it is found, return the string stored there
        return iter->second.c_str();
    }
    else
    {
        // If not return an empty string
        return DEFAULT_VALUE.c_str();
    }
}

// These functions are to convert the various mms, dss and avl status from string stored in database
// to defined identifier (ie. letter) for display
const std::string CombinedEventItem::getMmsStatusIdentifier(const std::string mmsStatus)
{
	//check the given string with the static const used in the alarm data
	//as mms status is a number under alarm data, there is need to convert the string to number
	int status = atoi(mmsStatus.c_str());
	
	switch (status)
	{
		case TA_Base_Core::IAlarmData::TYPE_SEMI:
			return MMS_SEMI_ID.c_str();
		case TA_Base_Core::IAlarmData::TYPE_AUTO:
			return MMS_AUTO_ID.c_str();
		case TA_Base_Core::IAlarmData::TYPE_ALM_SENT:
			return MMS_AUTO_ID.c_str();
		case TA_Base_Core::IAlarmData::TYPE_JR_SENT:
			return MMS_JR_SENT_ID.c_str();
		case TA_Base_Core::IAlarmData::TYPE_INHIBIT:
			return MMS_INHIBIT_ID.c_str();
		case TA_Base_Core::IAlarmData::TYPE_FAILED:
			return MMS_FAILED_ID.c_str();
		default:
			return DEFAULT_VALUE.c_str();
	}
}

const std::string CombinedEventItem::getDssPlanStatusIdentifier(const std::string dssStatus)
{
	if ( 0 == dssStatus.compare(TA_Base_Core::AlarmData::AUTO_PLAN_NOT_RUN) )
	{
		return AUTO_PLAN_NOT_RUN_ID.c_str();
	}
	else if ( 0 == dssStatus.compare(TA_Base_Core::AlarmData::SEMIAUTO_PLAN_NOT_RUN) )
	{
		return SEMI_AUTO_PLAN_NOT_RUN_ID.c_str();
	}
	else if ( 0 == dssStatus.compare(TA_Base_Core::AlarmData::PLAN_RUN) )
	{
		return PLAN_RUN_ID.c_str();
	}
	else if ( 0 == dssStatus.compare(TA_Base_Core::AlarmData::PLAN_POSTPONED) )
	{
		return PLAN_POSTPONED_ID.c_str();
	}
	else
	{
		return NO_PLAN.c_str();
	}
}

const std::string CombinedEventItem::getAvlStatusIdentifier(const std::string avlStatus)
{
	//check the given string with the static const used in the alarm data
	//as avl status is a number under alarm data, there is need to convert the string to number
	int status = atoi(avlStatus.c_str());
	
	switch (status)
	{
		case TA_Base_Core::IAlarmData::HEAD_AVL:
			return AVL_HEAD_ID.c_str();
		case TA_Base_Core::IAlarmData::CHILD_AVL:
			return AVL_CHILD_ID.c_str();
		default:
			return DEFAULT_VALUE.c_str();
	}
}

const std::string CombinedEventItem::getSevIdentifier(const std::string severityName)
{
	//check the given string with the static const used in the alarm data
	//as avl status is a number under alarm data, there is need to convert the string to number
	int sev = atoi(severityName.c_str());

	if (sev == 0)
	{
		return DEFAULT_VALUE.c_str();
	}else
	{
		return severityName.c_str();
	}
}


//Finished //TODO: delete "&" from return type
const std::string CombinedEventItem::getCompleteColumnData( const std::string& columnName )
{
    // Try to find the specified column in the data available for this item
    std::map<std::string,std::string>::iterator iter = m_columnData.find(columnName);

    if (iter != m_columnData.end())
    {
        // If it is found, return the string stored there
        return iter->second.c_str();
    }
    else
    {
        // If not return an empty string
        return DEFAULT_VALUE.c_str();
    }
}

// must be "strict weak ordering"!
// Strict: pred (X, X) is always false.
// Weak: If ! pred (X, Y) && !pred (Y, X), X==Y.
// Ordering: If pred (X, Y) && pred (Y, Z), then pred (X, Z).
bool CombinedEventItem::compareTo(DisplayItem* rhs, string sortColumn, bool sortAscending)
{
	bool ISCSSortingPriority = EventHandlerManager::getIsISCSSortingPriority();
	// Note: shouldn't add any log in this function, otherwise will make CPU usage very high! Because this func will be called VERY frequently.
	//LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "Come into CombinedEventItem::compareTo()");

	// These are both string values, so use std::string.compare(), and store the result, where
	// result > 0 --> this item is lexicographically greater than the rhs item
	// result = 0 --> this item is lexicographically equal to the rhs item
	// result < 0 --> this item is lexicographically less than the rhs item
	int sortValue = 0;
	if ( EVDisplayStrRes::GetInstance()->COLUMN_DATETIME == sortColumn )
	{

		sortValue = m_createTime.time - (rhs->getCreateTime()).time;


		if (0 == sortValue)
		{
			// If the sort value was 0, it means the two times were the same, so want to sort on the milliseconds.
			sortValue = m_createTime.millitm - (rhs->getCreateTime()).millitm;
		}
		if (0== sortValue)
		{
			// if the time is totally equal, compair the key
			if ( m_eventCategory == rhs->getEventCategory() )
			{
				sortValue = m_key - rhs->getKey();				
			}
			else // the EventCategory is different
			{
				if(m_eventCategory == TA_Base_Core::ATSCategory)
				{
					if (ISCSSortingPriority)
					{
						sortValue = -1;
					}
					else
					{
						sortValue = 1;
					}
				}else
				{
					if (ISCSSortingPriority)
					{
						sortValue = 1;
					}
					else
					{
						sortValue = -1;
					}
				}

			}
		} 
	}
	else
	{
		//std::string lhsData(getColumnData(sortColumn));
		//std::string rhsData(rhs->getColumnData(sortColumn));
		//std::transform(lhsData.begin(), lhsData.end(), lhsData.begin(),toupper);
		//std::transform(rhsData.begin(), rhsData.end(), rhsData.begin(),toupper);
		//sortValue = lhsData.compare(rhsData);
		sortValue=getColumnDataForCompare(sortColumn).compare(rhs->getColumnDataForCompare(sortColumn));

		if( 0 == sortValue ){
			//if the compare result is equal, go on comparing based on timestamp, and by the same sort direction. Added by xiangmei.
			sortValue = m_createTime.time - (rhs->getCreateTime()).time;
			if (0 == sortValue)
			{
				// If the sort value was 0, it means the two times were the same, so want to sort on the milliseconds.
				sortValue = m_createTime.millitm - (rhs->getCreateTime()).millitm;
			}
			if (0== sortValue)
			{
				// if the time is totally equal, compair the key
				if ( m_eventCategory == rhs->getEventCategory() )
				{
					sortValue = m_key - rhs->getKey();
				}
				else // the EventCategory is different
				{
					if(m_eventCategory == TA_Base_Core::ATSCategory)
					{
						if (ISCSSortingPriority)
						{
							sortValue = -1;
						}
						else
						{
							sortValue = 1;
						}
					}else
					{
						if (ISCSSortingPriority)
						{
							sortValue = 1;
						}
						else
						{
							sortValue = -1;
						}
					}
				}
			} 
		}
	}

	
	//LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "sortValue = %d, going to Come Out from compareTo.", sortValue);
	// Now compare the sort values to determine what we're doing.
	if (sortAscending)
	{
		// If the sort method is ascending, a result less than 0 is true (i.e. lower item first)
		return 0 > sortValue ? true : false;

	}
	else
	{
		// If the sort method is decending, a result greater than 0 is true (i.e. lower item last).
		return 0 < sortValue ? true : false;
	}
}



std::string CombinedEventItem::getCreateTimeStr()
{
	return m_strCreateTime;
}

timeb CombinedEventItem::getCreateTime()
{
	return m_createTime;
}
