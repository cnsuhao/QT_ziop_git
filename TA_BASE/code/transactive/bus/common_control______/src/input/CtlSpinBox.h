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
* The class implement customized CtlSpinBox widgets. 
*  
*
*/


#ifndef _CtlSpinBoxH_
#define _CtlSpinBoxH_

#include <QtDesigner/QtDesigner>
#include <QtWidgets/QWidget>
#include <QtWidgets/qspinbox.h>
 
#include "bus/common_control/src/common/ctlCommonEnhance.h"

QT_BEGIN_NAMESPACE
 
class  QDESIGNER_WIDGET_EXPORT CtlSpinBox : public QSpinBox 
{
    Q_OBJECT       
public:	 
	/** Constructor function
	*
	*  @param parent
	*/
	explicit CtlSpinBox(QWidget* parent = 0);
	/** Destructor  function
	*
	*/
	virtual ~CtlSpinBox(); 
	/**
	* setObjectName
	*
	* set object name and accessible Name
	*/
	void setObjectName(const QString &name);

	void setToolTip(std::string str);
	void setWhatsThis(std::string str);

	///This virtual function is used by the spin box whenever it needs to display the given value. 
	///The default implementation returns a string containing value printed in the standard way using QWidget::locale().toString(), 
	///but with the thousand separator removed unless setGroupSeparatorShown() is set. 	 
	std::string textFromValueEx(int value) const;

	///This virtual function is used by the spin box whenever it needs to interpret text entered by the user as a value.
	virtual int	valueFromText(const std::string &text) const;

    ///setValue() will emit valueChanged() if the new value is different from the old one. 
	///The value property has a second notifier signal which includes the spin box's prefix and suffix.
	void setValue(int val);
};
QT_END_NAMESPACE
 

#endif
