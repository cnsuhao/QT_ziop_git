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
* The class implement customized QLineEdit widgets.
*  
*
*/


#ifndef _CtlLineEditH_
#define _CtlLineEditH_

#include <QtDesigner/QtDesigner>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLineEdit>
 
 
#include "bus/common_control/src/common/ctlCommonEnhance.h"

QT_BEGIN_NAMESPACE
 
class  QDESIGNER_WIDGET_EXPORT CtlLineEdit : public QLineEdit
{
    Q_OBJECT       
public:	 
	/** Constructor function
	*
	*  @param parent
	*/
	explicit CtlLineEdit(QWidget* parent = 0);
	/** Destructor  function
	*
	*/
	virtual ~CtlLineEdit(); 

	/**
	* setObjectName
	*
	* set object name and accessible Name
	*/
	void setObjectName(const QString &name);
	//Customized method for Transactive 

	///set button caption from QString
	void setText(const QString &text);
	 
	///get std::string of text 
	std::string textEx();

	///get std::string of displayText
	std::string	displayTextEx() const;

	///get std::string of inputmask
	std::string	inputMaskEx() const;

	//get std::string of PlaceHolderText
	std::string	placeholderTextEx() const;

	///get std::string of selectText
	std::string	selectedTextEx() const;

	///set std::string of inputmask
	void	setInputMask(const std::string &inputMask);

	///set std::string of PlaceHolderText
	void	setPlaceholderText(const std::string &);

	void setToolTip(std::string str);
	void setWhatsThis(std::string str);

public Q_SLOTS:
    ///set button caption from std::string
	void setText(const std::string &text);

Q_SIGNALS:
	///This signal is emitted whenever the text changes.The text argument is the new text.
	///	Unlike textEdited(), this signal is also emitted when the text is changed programmatically, for example, by calling setText().
	void textChanged(const std::string &);

	///This signal is emitted whenever the text is edited. The text argument is the new text.
	///Unlike textChanged(), this signal is not emitted when the text is changed programmatically, for example, by calling setText().
	void textEdited(const std::string &);

private Q_SLOTS:
    ///internal slot for implement these signals based on std::string
	void textChangedSLOT(const QString & text);
	void textEditedSLOT(const QString &text);


};
QT_END_NAMESPACE
 

#endif
