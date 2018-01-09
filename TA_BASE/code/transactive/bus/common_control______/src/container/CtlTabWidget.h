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
* The class implement customized QTabWidget widgets.
*  
*
*/

#ifndef _CtlTabWidgetH_
#define _CtlTabWidgetH_

#include <QtDesigner/QtDesigner>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTabWidget> 
#include <QtDesigner/QDesignerExportWidget> 
 
#include "bus/common_control/src/common/UICommonType.h"
#include "bus/common_control/src/common/ctlCommonEnhance.h"


QT_BEGIN_NAMESPACE
 //Q_DECL_EXPORT
class  QDESIGNER_WIDGET_EXPORT  CtlTabWidget : public QTabWidget
{
    Q_OBJECT   
public:
	 
	explicit CtlTabWidget(QWidget* parent = 0);
	virtual ~CtlTabWidget();

	///Adds a tab with the given page and label to the tab widget, and returns the index of the tab in the tab bar.
	///If the tab's label contains an ampersand, the letter following the ampersand is used as a shortcut for the tab, e.g. 
	///if the label is "Bro&wse" then Alt+W becomes a shortcut which will move the focus to this tab.
	///Note: If you call addTab() after show(), the layout system will try to adjust to the changes in its widgets hierarchy and may cause flicker.
	///To prevent this, you can set the QWidget::updatesEnabled property to false prior to changes; 
	///remember to set the property to true when the changes are done, making the widget receive paint events again.
	int addTab(QWidget *page, const std::string &label);
	int addTab(QWidget *page, const QIcon &icon, const std::string &label);

	///Inserts a tab with the given label and page into the tab widget at the specified index, and returns the index of the inserted tab in the tab bar.
	///If index is out of range, the tab is simply appended. Otherwise it is inserted at the specified position.
	///If the QTabWidget was empty before this function is called, the new page becomes the current page.
	///Inserting a new tab at an index less than or equal to the current index will increment the current index, but keep the current page.
	int insertTab(int index, QWidget *page, const std::string  &label);
	int insertTab(int index, QWidget *page, const QIcon &icon, const std::string  &label);

	///Defines a new label for the page at position index's tab.
	void setTabText(int index, const std::string  &label);

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
