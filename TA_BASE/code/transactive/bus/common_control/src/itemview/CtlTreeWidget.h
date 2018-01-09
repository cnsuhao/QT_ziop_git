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
* The class implement customized QTreeWidget.
*  
*
*/

#ifndef _CtlTreeWidgetH_
#define _CtlTreeWidgetH_

#include <QtDesigner/QtDesigner>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTreeWidget> 
#include <QtDesigner/QDesignerExportWidget> 
 
#include "bus/common_control/src/common/UICommonType.h"
#include "bus/common_control/src/common/ctlCommonEnhance.h"


QT_BEGIN_NAMESPACE
 //Q_DECL_EXPORT
class  QDESIGNER_WIDGET_EXPORT  CtlTreeWidget : public QTreeWidget
{
    Q_OBJECT   
public:
	/** Constructor function
	*
	*  @param parent
	*/
	explicit CtlTreeWidget(QWidget* parent = 0);
	/** Destructor  function
	*
	*/
	virtual ~CtlTreeWidget();
	/**
	* setObjectName
	*
	* set object name and accessible Name
	*/
	void setObjectName(const QString &name);

	///Returns a list of items that match the given text, using the given flags, in the given column.
	QList<QTreeWidgetItem *>	findItems(const std::string & text, Qt::MatchFlags flags) const;
	QList<QTreeWidgetItem *>	findItems(const QString & text, Qt::MatchFlags flags) const;
   
	///Adds a column in the header for each item in the labels list, and sets the label for each column.
	///Note that setHeaderLabels() won't remove existing columns.
	void	setHeaderLabel(const QString & label);
	void	setHeaderLabel(const std::string & label);

	///Adds a column in the header for each item in the labels list, and sets the label for each column.
	///Note that setHeaderLabels() won't remove existing columns.
	void	setHeaderLabels(const QStringList & labels);
	void	setHeaderLabels(const STDStringList & labels);
	 
};
QT_END_NAMESPACE
 

#endif
