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
* The class implement customized QGroupBox widgets.
*  
*
*/

#ifndef _CtlGroupBoxH_
#define _CtlGroupBoxH_

#include <QtDesigner/QtDesigner>
#include <QtWidgets/QWidget>
#include <QtWidgets/QGroupBox>
 
#include "bus/common_control/src/common/ctlCommonEnhance.h"
QT_BEGIN_NAMESPACE
 //Q_DECL_EXPORT
class  QDESIGNER_WIDGET_EXPORT CtlGroupBox : public QGroupBox 
{
    Q_OBJECT
    
   
public:
	/** Constructor function
	*
	*  @param parent
	*/
	explicit CtlGroupBox(QWidget* parent = 0);
	/** Destructor  function
	*
	*/
	virtual ~CtlGroupBox();
 
	//Customized method for Transactive 

	///set button caption from QString
	void setTitle(const QString &text);

	///set button caption from std::string
	void setTitle(const std::string &text);

	///get multiple byte string of button caption 
	std::string titleEx();

	void setToolTip(std::string str);
	void setWhatsThis(std::string str);
	/**
	* setObjectName
	*
	* set object name and accessible Name
	*/
	void setObjectName(const QString &name);
};
QT_END_NAMESPACE
 

#endif
