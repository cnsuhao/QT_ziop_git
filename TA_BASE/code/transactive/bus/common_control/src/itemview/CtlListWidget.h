/**
* The source code in this file is the property of 
* ST Engineering Electronics (LSG) and is not for redistribution
* in any form.
*
* Source:   $File:   $
* @author:  Luo Huirong
* @version: $Revision: #1 $
*
* Last modification: $DateTime: 2016/11/20 16:15:14 $
* Last modified by:  $Author: Luo Huirong $
* 
* The class implement customized QListWidget.
* It is a convenience class that provides a list view similar to the one supplied by QListView, but with a classic item - based interface for adding and removing items.
* CtlListWidget uses an internal model to manage each QListWidgetItem in the list.
* For a more flexible list view widget, use the CtlListView class with a standard model.
*
*/

#ifndef _CtlListWidgetH_
#define _CtlListWidgetH_

#include <QtDesigner/QtDesigner>
#include <QtWidgets/QWidget>
#include <QtWidgets/QListWidget> 
#include <QtDesigner/QDesignerExportWidget> 
 
#include "bus/common_control/src/common/UICommonType.h"
#include "bus/common_control/src/common/ctlCommonEnhance.h"


QT_BEGIN_NAMESPACE
 
///CtlListWidgetItem
///provide customized sorting algothrim for Number, DateTime and string
class QDESIGNER_WIDGET_EXPORT CtlListWidgetItem : public QListWidgetItem
{

public:
	explicit CtlListWidgetItem(QListWidget *view = 0, UISortType dataType = UISortType::UISTRING, int type = Type);
	explicit CtlListWidgetItem(const QString &text, UISortType dataType = UISortType::UISTRING, QListWidget *view = 0, int type = Type);
	explicit CtlListWidgetItem(const QIcon &icon, const QString &text, UISortType dataType = UISortType::UISTRING, QListWidget *view = 0, int type = Type);

	explicit CtlListWidgetItem(const std::string &text, UISortType dataType = UISortType::UISTRING,  QListWidget *view = 0, int type = Type);
	explicit CtlListWidgetItem(const QIcon &icon, const std::string &text, UISortType dataType = UISortType::UISTRING, QListWidget *view = 0, int type = Type);

	void setText(const std::string &text);
	void setToolTip(const std::string &toolTip);
	void setWhatsThis(const std::string &whatsThis);

	bool operator< (const QListWidgetItem &other) const;

public:
	///data type for sorting
	UISortType m_dataType;
};


class  QDESIGNER_WIDGET_EXPORT  CtlListWidget : public QListWidget
{
    Q_OBJECT   
public:
	/** Constructor function
	*
	*  @param parent
	*/
	explicit CtlListWidget(QWidget* parent = 0);
	/** Destructor  function
	*
	*/
	virtual ~CtlListWidget();
	/**
	* setObjectName
	*
	* set object name and accessible Name
	*/
	void setObjectName(const QString &name);
	///Inserts an item with the text label at the end of the list widget.
	void	addItem(const QString & label);
	void	addItem(const std::string & label);

	///Inserts items with the text labels at the end of the list widget.
	void	addItems(const QStringList & labels);
 	void	addItems(const STDStringList & labels);	

	///Finds items with the text that matches the string text using the given flags.
	QList<QListWidgetItem *>	findItems(const std::string & text, Qt::MatchFlags flags) const;
	QList<QListWidgetItem *>	findItems(const QString & text, Qt::MatchFlags flags) const;
   
	///Inserts an item with the text label in the list widget at the position given by row.
	void	insertItem(int row, const QString & label);
	void	insertItem(int row, const std::string & label);

	///Inserts items from the list of labels into the list, starting at the given row.
	void	insertItems(int row, const QStringList & labels);
	void	insertItems(int row, const STDStringList & labels);
	 
};
QT_END_NAMESPACE
 

#endif
