/**
* The source code in this file is the property of 
* ST Engineering Electronics (LSG) and is not for redistribution
* in any form.
*
* Source:   $File:   $
* @author:  Luo Huirong
* @version: $Revision: #1 $
*
* Last modification: $DateTime: 2016/11/20 13:15:14 $
* Last modified by:  $Author: Luo Huirong $
* 
* The class implement customized QListView.
* If you don't need flexible list view, it is recommend to use CtlListWidget
* The current implmentaiton only wrappered the QListView, more functions will be trigged from developer requirement.
*/

#ifndef _CtlListViewH_
#define _CtlListViewH_

#include <QtDesigner/QtDesigner>
#include <QtWidgets/QWidget>
#include <QtWidgets/QListView>
#include <QtWidgets/QHeaderView>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
 
#include "bus/common_control/src/common/ctlCommonEnhance.h"

QT_BEGIN_NAMESPACE
 //Q_DECL_EXPORT
class  QDESIGNER_WIDGET_EXPORT CtlListView : public QListView 
{
    Q_OBJECT   
public:
	/** Constructor function
	*
	*  @param parent
	*/
	explicit CtlListView(QWidget* parent = 0);
	/** Destructor  function
	*
	*/
	virtual ~CtlListView();

	/**
	* setObjectName
	*
	* set object name and accessible Name
	*/
	void setObjectName(const QString &name);
    
};
QT_END_NAMESPACE
 

#endif
