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
* The class implement customized QTableView widgets.
* If you don't need flexible table view, it is recommend to use CtlTableWidget
* The current implmentaiton only wrappered the QTableView, more functions will be trigged from developer requirement.
*
*/

#ifndef _CtlTableViewH_
#define _CtlTableViewH_

#include <QtDesigner/QtDesigner>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTableView>
#include <QtWidgets/QHeaderView>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
 
#include "bus/common_control/src/common/ctlCommonEnhance.h"

QT_BEGIN_NAMESPACE
 //Q_DECL_EXPORT
class  QDESIGNER_WIDGET_EXPORT CtlTableView : public QTableView
{
    Q_OBJECT   
public:
	/** Constructor function
	*
	*  @param parent
	*/
	explicit CtlTableView(QWidget* parent = 0);
	/** Destructor  function
	*
	*/
	virtual ~CtlTableView();
	/**
	* setObjectName
	*
	* set object name and accessible Name
	*/
	void setObjectName(const QString &name);
	/**
	* sectionClickedEvent:
	*    used to get a notification of sorting of header.
	*	subclass can override the method for receiving notification.
	* @param column: index of column
	* @param order:  Ascending or Descending order
	*/
	virtual void sectionClickedEvent(int column, Qt::SortOrder order);
public slots:
	void sortTableView(int column);
	 
private:
	 
};
QT_END_NAMESPACE
 

#endif
