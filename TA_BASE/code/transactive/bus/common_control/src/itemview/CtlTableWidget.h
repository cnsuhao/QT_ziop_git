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
* The class implement customized QTableWidget widgets.
* It provides an item-based table view with a default model.
* If you want a table that uses your own data model you should use CtlTableView rather than this class
*
*/

#ifndef _CtlTableWidgetH_
#define _CtlTableWidgetH_

#include <QtDesigner/QtDesigner>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QHeaderView>
#include <QAbstractItemModel>
#include <QSortFilterProxyModel>
 
#include "bus/common_control/src/common/ctlCommonEnhance.h"
#include "bus/common_control/src/common/UICommonType.h"
QT_BEGIN_NAMESPACE


//CtlTableWidgetItem
//provide customized sorting algothrim for Number, DateTime
class QDESIGNER_WIDGET_EXPORT CtlTableWidgetItem : public QTableWidgetItem
{

public:
	explicit CtlTableWidgetItem(UISortType dataType = UISortType::UISTRING, int type = Type);
	explicit CtlTableWidgetItem(const QString &text, UISortType dataType = UISortType::UISTRING, int type = Type);
	explicit CtlTableWidgetItem(const QIcon &icon, const QString &text, UISortType dataType = UISortType::UISTRING, int type = Type);

	explicit CtlTableWidgetItem(const std::string &text, UISortType dataType = UISortType::UISTRING, int type = Type);
	explicit CtlTableWidgetItem(const QIcon &icon, const std::string &text, UISortType dataType = UISortType::UISTRING, int type = Type);

	void setText(const std::string &text);
	void setToolTip(const std::string &toolTip);
	void setWhatsThis(const std::string &whatsThis);

	bool operator< (const QTableWidgetItem &other) const;	

public:
	//data type for sorting
	UISortType m_dataType;
};


//CtlTableWidget 
//Provide : 1> sorting for number and datatime
//          2> sorting is enabled by default
//          3> section click event
class  QDESIGNER_WIDGET_EXPORT CtlTableWidget : public QTableWidget  
{
    Q_OBJECT   
public:	 
	/** Constructor function
	*
	*  @param parent
	*/
	explicit CtlTableWidget(QWidget* parent = 0);
	/** Destructor  function
	*
	*/
	virtual ~CtlTableWidget();
	/**
	* setObjectName
	*
	* set object name and accessible Name
	*/
	void setObjectName(const QString &name);
	/**
	*  sectionClickedEvent:
	* 	used to get a notification of sorting of header.
	* 	subclass can override the method for receiving notification.
	* @param column: index of column
	* @param order:  Ascending or Descending order
	*/
	virtual void sectionClickedEvent(int column, Qt::SortOrder order);

	///Finds items that matches the text using the given flags.
	QList<QTableWidgetItem *>	findItems(const QString &text, Qt::MatchFlags flags) const;
	QList<QTableWidgetItem *>	findItems(const std::string &text, Qt::MatchFlags flags) const;

	///Sets the vertical header labels using labels.
	void	setVerticalHeaderLabels(const QStringList &labels);
	void	setVerticalHeaderLabels(const STDStringList &labels);

    ///Sets the horizontal header labels using labels.
	void	setHorizontalHeaderLabels(const QStringList &labels);
	void	setHorizontalHeaderLabels(const STDStringList &labels);

	///Sets the current item to item.
	///Unless the selection mode is NoSelection, the item is also selected.
	void	setCurrentItem(CtlTableWidgetItem *item);
	void	setCurrentItem(CtlTableWidgetItem *item, QItemSelectionModel::SelectionFlags command);
    
	///Sets the item for the given row and column to item.
	void	setItem(int row, int column, CtlTableWidgetItem *item);

	///Sets the item prototype for the table to the specified item.
	///The table widget will use the item prototype clone function when it needs to create a new table item.
	///For example when the user is editing in an empty cell.
	///This is useful when you have a QTableWidgetItem subclass and want to make sure that QTableWidget creates instances of your subclass.
	void	setItemPrototype(const CtlTableWidgetItem *item);


public slots:
	void sortTableView(int column);
	 
private:
 
};
QT_END_NAMESPACE
 

#endif
