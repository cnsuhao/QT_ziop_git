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
* The class implement customized QDoubleSpinBox widgets.
*  
*
*/


#ifndef _CtlDoubleSpinBoxH_
#define _CtlDoubleSpinBoxH_

#include <QtDesigner/QtDesigner>
#include <QtWidgets/QWidget>
#include <QtWidgets/QDoubleSpinBox>
 
#include "bus/common_control/src/common/ctlCommonEnhance.h"

QT_BEGIN_NAMESPACE
 
class  QDESIGNER_WIDGET_EXPORT CtlDoubleSpinBox : public QDoubleSpinBox 
{
    Q_OBJECT       
public:	 
	/** Constructor function
	*
	*  @param parent
	*/
	explicit CtlDoubleSpinBox(QWidget* parent = 0);
	/** Destructor  function
	*
	*/
	virtual ~CtlDoubleSpinBox(); 

	
	void setToolTip(std::string str);


	void setWhatsThis(std::string str);

	/**
	* setObjectName
	*
	* set object name and accessible Name
	*/
	void setObjectName(const QString &name);

	///This virtual function is used by the spin box whenever it needs to display the given value. 
	///The default implementation returns a string containing value printed using QWidget::locale().toString(value, QLatin1Char('f'), decimals()) 
	///and will remove the thousand separator unless setGroupSeparatorShown() is set. Reimplementations may return anything.
    ///Note: QDoubleSpinBox does not call this function for specialValueText() and that neither prefix() nor suffix() should be included in the return value.
	std::string textFromValueEx(double value) const;

	///This virtual function is used by the spin box whenever it needs to interpret text entered by the user as a value.
	///Subclasses that need to display spin box values in a non - numeric way need to reimplement this function.
	virtual double	valueFromText(const std::string &text) const;


	///setValue() will emit valueChanged() if the new value is different from the old one. The value property has a second notifier signal which includes the spin box's prefix and suffix.
    ///Note: The value will be rounded so it can be displayed with the current setting of decimals.
	void setValue(double val);


};
QT_END_NAMESPACE
 

#endif
