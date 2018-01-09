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
* The class allows you to include several custom widgets in one single library. 
*
*
*/


#include "bus/common_control/src/button/CtlButtonPlugin.h"
#include "bus/common_control/src/label/CtlLabelPlugin.h"

#include <QtDesigner/QDesignerCustomWidgetCollectionInterface>
#include <QtCore/qplugin.h>
#include <QtCore/qdebug.h>
#include <QtGui/QIcon>

#ifndef _CtlCommonControlH_
#define _CtlCommonControlH_
 
QT_BEGIN_NAMESPACE
class CommonControl: public QObject, public QDesignerCustomWidgetCollectionInterface
{
    Q_OBJECT
    Q_INTERFACES(QDesignerCustomWidgetCollectionInterface)

 
   #if QT_VERSION >= QT_VERSION_CHECK(5, 0, 0)
	  Q_PLUGIN_METADATA(IID "TransActive Common Control" FILE "")
  #endif
public:
	/** Constructor function
	*
	*  @param parent
	*/
	explicit CommonControl(QObject *parent = 0);

	/**
	* customWidgets
	*
	* return collection of common control plugin objects
	*/

    virtual QList<QDesignerCustomWidgetInterface*> customWidgets() const;

private:
    QList<QDesignerCustomWidgetInterface*> m_plugins;
};

QT_END_NAMESPACE
 
#endif

 

 