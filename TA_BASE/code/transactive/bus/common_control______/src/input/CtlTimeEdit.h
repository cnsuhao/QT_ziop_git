/**
* The source code in this file is the property of 
* ST Engineering Electronics (LSG) and is not for redistribution
* in any form.
*
* Source:   $File:   $
* @author:  Luo Huirong
* @version: $Revision: #1 $
*
* Last modification: $DateTime: 2015/11/20 16:15:14 $
* Last modified by:  $Author: Luo Huirong $
* 
* The class implement customized QTimeEdit widgets.
*  
*
*/


#ifndef _CtlTimeEditH_
#define _CtlTimeEditH_

#include <QtDesigner/QtDesigner>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTimeEdit>
 
#include "bus/common_control/src/common/ctlCommonEnhance.h"

QT_BEGIN_NAMESPACE
 
class  QDESIGNER_WIDGET_EXPORT CtlTimeEdit : public QTimeEdit 
{
    Q_OBJECT       
public:	 
	/** Constructor function
	*
	*  @param parent
	*/
	explicit CtlTimeEdit(QWidget* parent = 0);
	/** Destructor  function
	*
	*/
	virtual ~CtlTimeEdit(); 
	/**
	* setObjectName
	*
	* set object name and accessible Name
	*/
	void setObjectName(const QString &name);
	void setToolTip(std::string str);
	void setWhatsThis(std::string str);

	/**
	* setDateTime
	*
	* This property holds the QDateTime that is set in the QDateTimeEdit.
	* When setting this property the timespec of the QDateTimeEdit remains the same and the timespec of the new QDateTime is ignored.
	* By default, this property contains a date that refers to January 1, 2000 and a time of 00:00 : 00 and 0 milliseconds.
	*
	* @param dateTime: QDateTime
	*/
	void setDateTime(const QDateTime &dateTime);

	///set the display format
	void setDisplayFormat(const std::string &format);
};
QT_END_NAMESPACE
 

#endif
