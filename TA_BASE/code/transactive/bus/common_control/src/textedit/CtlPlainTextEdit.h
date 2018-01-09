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
* The class implement customized QPlainTextEdit widgets.
*  
*
*/


#ifndef _CtlPlainTextEditH_
#define _CtlPlainTextEditH_

#include <QtDesigner/QtDesigner>
#include <QtWidgets/QWidget>
#include <QtWidgets/QPlainTextEdit>
 
 
#include "bus/common_control/src/common/ctlCommonEnhance.h"

QT_BEGIN_NAMESPACE
 
class  QDESIGNER_WIDGET_EXPORT CtlPlainTextEdit : public QPlainTextEdit  
{
    Q_OBJECT       
public:	 
	/** Constructor function
	*
	*  @param parent
	*/
	explicit CtlPlainTextEdit(QWidget* parent = 0);
	/** Destructor  function
	*
	*/
	virtual ~CtlPlainTextEdit(); 
	/**
	* setObjectName
	*
	* set object name and accessible Name
	*/
	void setObjectName(const QString &name);
	///This property gets and sets the plain text editor's contents. 
	///The previous contents are removed and undo/redo history is reset when this property is set.
	std::string	toPlainTextEx() const;
	void setPlainText(const std::string & text);
	void setPlainText(const QString & text);
	 
	///This property holds the editor placeholder text.
	///Setting this property makes the editor display a grayed - out placeholder text as long as the document() is empty.
	std::string	placeholderTextEx() const;	 
	void	setPlaceholderText(const std::string &);

	///This property holds the title of the document parsed from the text.
	std::string	documentTitleEx() const;

	///Finds the next occurrence of the string, exp, using the given options. 
	///Returns true if exp was found and changes the cursor to select the match; otherwise returns false.
	bool find(const std::string & exp, QTextDocument::FindFlags options = 0);
	
	///Convenience slot that inserts text at the current cursor position.
	void insertPlainText(const std::string &text);
	void insertPlainText(const QString &text);

	///Appends a new paragraph with text to the end of the text edit.
	void appendPlainText(const std::string &text);
	void appendPlainText(const QString &text);

	///Appends a new paragraph with html to the end of the text edit.
	void appendHtml(const std::string &html);	
	void appendHtml(const QString &html);

	void setToolTip(std::string str);
	void setWhatsThis(std::string str);
};
QT_END_NAMESPACE
 

#endif
