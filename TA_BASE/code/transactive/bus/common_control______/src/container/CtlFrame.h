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
* The class implement customized QButton widgets.
*  
*
*/

#ifndef _CtlButtonH_
#define _CtlButtonH_

#include <QtDesigner/QtDesigner>
#include <QWidget>
#include <QPushButton>
#include "bus/common_control/src/common/ta_String.h"
#include "bus/common_control/src/common/ctlCommonEnhance.h"
QT_BEGIN_NAMESPACE
 //Q_DECL_EXPORT
class  QDESIGNER_WIDGET_EXPORT CtlButton : public QPushButton ,public ctlCommonEnhance
{
    Q_OBJECT
    
   
public:
	CtlButton(CtlButton *widget, QDesignerFormEditorInterface *core, QObject *parent);
	CtlButton(QWidget* parent=0);
	virtual ~CtlButton();
    virtual void setControlID(TA_String controlID);

 
};
QT_END_NAMESPACE
 

#endif
