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
* The class implement customized QComboBox widgets.
*  
*
*/


#ifndef _CtlComboBoxH_
#define _CtlComboBoxH_

#include <QtDesigner/QtDesigner>
#include <QtWidgets/QWidget>
#include <QtWidgets/QComboBox>
#include "bus/common_control/src/common/UICommonType.h"
#include "bus/common_control/src/common/ctlCommonEnhance.h"

QT_BEGIN_NAMESPACE
 
class  QDESIGNER_WIDGET_EXPORT CtlComboBox : public QComboBox 
{
    Q_OBJECT       
public:	 
	/** Constructor function
	*
	*  @param parent
	*/
	explicit CtlComboBox(QWidget* parent = 0);
	/** Destructor  function
	*
	*/
	virtual ~CtlComboBox( ); 
	/**
	* setObjectName
	*
	* set object name and accessible Name
	*/
	void setObjectName(const QString &name);
	//Customized method for Transactive 
	void setToolTip(std::string str);
	void setWhatsThis(std::string str);

	///Adds an item to the combobox with the given text, and containing the specified userData (stored in the Qt::UserRole).
	///The item is appended to the list of existing items
	void    addItem(const std::string & text, const QVariant & userData = QVariant());

	///Adds an item to the combobox with the given icon and text, and containing the specified userData (stored in the Qt::UserRole). 
	///The item is appended to the list of existing items.
	void	addItem(const QIcon & icon, const std::string & text, const QVariant & userData = QVariant());

	///Adds each of the strings in the given texts to the combobox. Each item is appended to the list of existing items in turn.
	void	addItems(const STDStringList & texts);

	///If the combo box is editable, the current text is the value displayed by the line edit.Otherwise, 
	///it is the value of the current item or an empty string if the combo box is empty or no current item is set
	std::string    currentTextEx();

	///Returns the text for the given index in the combobox.
	std::string 	itemTextEx(int index) const;

	///Sets the text for the item on the given index in the combobox.
	void	setItemText(int index, const std::string & text);

	///Returns the index of the item containing the given text; otherwise returns -1.
	///The flags specify how the items in the combobox are searched.
	int	findText(const std::string & text, Qt::MatchFlags flags = static_cast<Qt::MatchFlags> (Qt::MatchExactly | Qt::MatchCaseSensitive)) const;

	///Inserts the text and userData (stored in the Qt::UserRole) into the combobox at the given index.
	///If the index is equal to or higher than the total number of items, the new item is appended to the list of existing items.
	///If the index is zero or negative, the new item is prepended to the list of existing items.
	void	insertItem(int index, const std::string & text, const QVariant & userData = QVariant());

	///Inserts the icon, text and userData (stored in the Qt::UserRole) into the combobox at the given index.
	///If the index is equal to or higher than the total number of items, the new item is appended to the list of existing items.
	///If the index is zero or negative, the new item is prepended to the list of existing items.
	void	insertItem(int index, const QIcon & icon, const std::string & text, const QVariant & userData = QVariant());

	///Inserts the strings from the list into the combobox as separate items, starting at the index specified.
    ///If the index is equal to or higher than the total number of items, the new item is appended to the list of existing items.
	///If the index is zero or negative, the new item is prepended to the list of existing items.
	void	insertItems(int index, const STDStringList & list);
    
public Q_SLOTS:
    void setEditText(const std::string &text);
	 
Q_SIGNALS:
	///Sets the text in the combobox's text edit.
	void editTextChanged(const std::string &);

	///This signal is sent when the user chooses an item in the combobox. The item's text is passed. Note that this signal is sent even when the choice is not changed. 
	///If you need to know when the choice actually changes, use signal currentIndexChanged().
	void activated(const std::string &);

	///This signal is sent whenever the currentIndex in the combobox changes either through user interaction or programmatically. The item's text is passed..
	void currentIndexChanged(const std::string &);
    
	///This signal is sent whenever currentText changes.The new value is passed as text.
	///This function was introduced in Qt 5.0.
	void currentTextChanged(const std::string &);

private Q_SLOTS:

     //internal slot for implement these signals based on std::string
     void activatedSLOT(const QString & text);
	 void currentIndexChangedSLOT(const QString & text);
	 void currentTextChangedSLOT(const QString &text);
	 void editTextChangedSLOT(const QString &);
};
QT_END_NAMESPACE
 

#endif
