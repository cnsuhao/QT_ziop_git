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
* It help to append addtional functions to common control .
* TODO: need requirement.
*
*
*/



#ifndef _ctlCommonEnhance_H_
#define _ctlCommonEnhance_H_

#include <QtCore>
#include <QtDesigner/QtDesigner>
 

QT_BEGIN_NAMESPACE
class QDESIGNER_WIDGET_EXPORT ctlCommonEnhance
{
    
public:
	/** Constructor function
	*
	*  @param parent
	*/
    ctlCommonEnhance(QWidget* control);

	/** Destructor  function
	*
	*/
	virtual ~ctlCommonEnhance();
	 
    QString ControlID();
	virtual void setControlID(QString controlID);
private:
	QString m_controlID;
	QWidget*  m_control;
     
};

QT_END_NAMESPACE
 
#endif _ctlCommonEnhance_H_
  

 