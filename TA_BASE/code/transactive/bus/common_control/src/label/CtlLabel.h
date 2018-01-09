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
* The class implement customized QLabel widgets.
*  
*
*/

#ifndef _CtlLabelH_
#define _CtlLabelH_

#include <QtDesigner/QtDesigner>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include "bus/common_control/src/common/ctlCommonEnhance.h"
QT_BEGIN_NAMESPACE
 
class QDESIGNER_WIDGET_EXPORT CtlLabel : public QLabel
{
	Q_OBJECT 
    
public:
	/** Constructor function
	*
	*  @param parent
	*/
	explicit CtlLabel(QWidget* parent = 0);
	/** Destructor  function
	*
	*/
	virtual  ~CtlLabel();

	// Customized method for Transactive

	//set button caption from QString
	void setText(const QString &text);

	//set button caption from std::string
	void setText(const std::string &text);

	//get multiple byte string of label caption 
	std::string textEx();

	//get selected text; If there is no selected text this property's value is an empty string.
	std::string	selectedTextEx() const;

	void setToolTip(std::string str);
	void setWhatsThis(std::string str);
	/**
	* setObjectName
	*
	* set object name and accessible Name
	*/
	void setObjectName(const QString &name);

public slots:

public:


};
QT_END_NAMESPACE
#endif
