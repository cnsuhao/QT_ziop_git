/**
* The source code in this file is the property of
* ST Engineering Electronics (LSG) and is not for redistribution
* in any form.
*
* Source:   $File:   $
* @author:  Luo Huirong
* @version: $Revision: #1 $
*
* Last modification: $DateTime: 2016/11/25 13:15:14 $
* Last modified by:  $Author: Luo Huirong $
*
* The DataTimeUtility class provide 
*   1>functions to convert between QDateTime to/from std::string .
*   2>common datetime format strings
*  
*
*
*/



#pragma once


#ifndef _TA_UI_DateTimeUtility_
#define _TA_UI_DateTimeUtility_

#include <String>
#include <list>
 

#include "bus/common_control/src/common/ctlCommonEnhance.h"
#include "bus/common_control/src/common/UICommonType.h"
QT_BEGIN_NAMESPACE

//DateTimeUtility
//Convert datetime to/from displaying string
//
class QDESIGNER_WIDGET_EXPORT DateTimeUtility
{
public:

	///convert time_t to datetime string
	static std::string toDateTimeString(time_t t);

	///convert time_t to date string
	static std::string toDateString(time_t t);

	///convert time_t to time string
	static std::string toTimeString(time_t t);

	///convert QDateTime to datetime string
	static std::string toDateTimeString(QDateTime t);

	///convert QDateTime to date string
	static std::string toDateString(QDateTime t);

	///convert QDateTime to time string
	static std::string toTimeString(QDateTime t);

	///convert datetime string to time_t
	static time_t to_time_t(std::string t);

	///convert datetime string to time_t
	static time_t to_time_t(QString t);

	///convert datetime string to QDateTime
	static QDateTime toQDateTime(std::string t);

	///convert datetime string to QDateTime
	static QDateTime toQDateTime(QString t);

	///convert datetime string to toQDate
	static QDate toQDate(std::string t);

	///convert datetime string to toQDate
	static QDate toQDate(QString t);

	///convert datetime string to toQTime
	static QTime toQTime(std::string t);

	///convert datetime string to toQTime
	static QTime toQTime(QString t);

private:
	DateTimeUtility();
	~DateTimeUtility();

public:
	static const QString DateTimeFormat;
	static const QString DateFormat;
	static const QString TimeFormat;
};


QT_END_NAMESPACE


#endif