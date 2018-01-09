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
* The class implement customized QPushButton widgets.
*  
*/

#ifndef _CtlButtonH_
#define _CtlButtonH_

#include <QtDesigner/QtDesigner>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPushButton>
  
QT_BEGIN_NAMESPACE
 //Q_DECL_EXPORT
class  QDESIGNER_WIDGET_EXPORT CtlButton : public QPushButton  
{
    Q_OBJECT
    
   
public:	 

	/** Constructor function 
	*
	*  @param parent 
	*/
	explicit CtlButton(QWidget* parent = 0);
	/** Destructor  function
	*	 
	*/
	virtual ~CtlButton();

	/**
	* setObjectName
	*
	* set object name and accessible Name
	*/
	void setObjectName(const QString &name);
 
    //Customized method for Transactive 

	/** 
	* setText
	*
	* set button caption from QString
	*/
	void setText(const QString &text);

	/** 
	* setText
	*
	* set button caption from std::string
	*/
	void setText(const std::string &text);

	/** 
	* textEx
	*
	*This property holds the text shown on the button.
	*get multiple byte string of button caption 
	*/
	std::string textEx();
	 
	/** 
	* setToolTip
	*
	* set the widget's tooltip
	*/
	void setToolTip(std::string str);

	/** 
	*setWhatsThis
	*
	*set the widget's What's This help text
	*/
	void setWhatsThis(std::string str);
	
};
QT_END_NAMESPACE


#endif
