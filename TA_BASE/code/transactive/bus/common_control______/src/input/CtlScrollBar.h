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
* The class implement customized QScrollBar widgets.
*  
*
*/


#ifndef _CtlScrollBarH_
#define _CtlScrollBarH_

#include <QtDesigner/QtDesigner>
#include <QtWidgets/QWidget>
#include <QtWidgets/QScrollBar>
 
#include "bus/common_control/src/common/ctlCommonEnhance.h"

QT_BEGIN_NAMESPACE
 
class  QDESIGNER_WIDGET_EXPORT CtlScrollBar : public QScrollBar 
{
    Q_OBJECT       
public:	 
	/** Constructor function
	*
	*  @param parent
	*/
	explicit CtlScrollBar(QWidget* parent = 0);
	/** Constructor function
	*
	*  @param Orientation
	*  @param parent
	*/
	explicit CtlScrollBar(Qt::Orientation o, QWidget *parent = 0);
	/** Destructor  function
	*
	*/
	virtual ~CtlScrollBar(); 

	/**
	* setObjectName
	*
	* set object name and accessible Name
	*/
	void setObjectName(const QString &name);
	void setToolTip(std::string str);
	void setWhatsThis(std::string str);
};
QT_END_NAMESPACE
 

#endif
