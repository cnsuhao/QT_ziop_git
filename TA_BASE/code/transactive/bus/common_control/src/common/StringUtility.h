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
* The StringUtility class provide
*   1>functions to convert between QString to/from std::string .
*
*/


#ifndef _ctlQStringH_
#define _ctlQStringH_

#include <QString>

class QDESIGNER_WIDGET_EXPORT StringUtility
{
public:
    
	///convert QString to std::string
    static std::string toStdString(const QString qStr) 
	{
		const QByteArray asc = qStr.toUtf8(); return std::string(asc.constData(), asc.length());
	}

	///convert std::string to QString
	static QString fromStdString(const std::string &s)
	{
		QString qStr;
		return qStr.fromUtf8(s.data(), int(s.size()));	
	}
 
};

#endif