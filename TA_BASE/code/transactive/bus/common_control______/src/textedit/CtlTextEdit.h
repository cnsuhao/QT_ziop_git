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
* The class implement customized QTextEdit widgets.
*  
*
*/


#ifndef _CtlTextEditH_
#define _CtlTextEditH_

#include <QtDesigner/QtDesigner>
#include <QtWidgets/QWidget>
#include <QtWidgets/QTextEdit>
 
 
#include "bus/common_control/src/common/ctlCommonEnhance.h"

QT_BEGIN_NAMESPACE
 
class  QDESIGNER_WIDGET_EXPORT CtlTextEdit : public QTextEdit  
{
    Q_OBJECT       
public:	 
	/** Constructor function
	*
	*  @param parent
	*/
	explicit CtlTextEdit(QWidget* parent = 0);
	/** Destructor  function
	*
	*/
	virtual ~CtlTextEdit(); 
	/**
	* setObjectName
	*
	* set object name and accessible Name
	*/
	void setObjectName(const QString &name);
	///Returns the reference of the anchor at position pos, or an empty string if no anchor exists at that point.
	std::string	anchorAtEx(const QPoint & pos) const;

	///This property holds the title of the document parsed from the text.
	std::string	documentTitleEx() const;
	void	setDocumentTitle(const std::string & title);

	///Returns the font family of the current format.
	std::string	fontFamilyEx() const;
	///Sets the font family of the current format to fontFamily.
	void	setFontFamily(const std::string & fontFamily);

	///toHtml() returns the text of the text edit as html.
	///setHtml() changes the text of the text edit.
	///Any previous text is removed and the undo / redo history is cleared.
	///The input text is interpreted as rich text in html format.
	std::string	toHtml() const;
	void setHtml(const std::string &html);
	void setHtml(const QString &html);

	///This property gets and sets the text editor's contents as plain text. 
	///Previous contents are removed and undo/redo history is reset when the property is set.
	///If the text edit has another content type, it will not be replaced by plain text if you call toPlainText().
	///The only exception to this is the non - break space, nbsp; , that will be converted into standard space.
	std::string	toPlainText() const;
	///Changes the text of the text edit to the string text. Any previous text is removed.
	void setPlainText(const std::string & text);
	void setPlainText(const QString & text);


	///Appends a new paragraph with text to the end of the text edit.
    ///Note: The new paragraph appended will have the same character format and block format as the current paragraph, 
	///determined by the position of the cursor.
	void append(const std::string &text);
	void append(const QString &text);

	///Convenience slot that inserts text which is assumed to be of html formatting at the current cursor position.
	void insertHtml(const std::string &text);
	void insertHtml(const QString &text);

	///Convenience slot that inserts text at the current cursor position.
	void insertPlainText(const std::string &text);
	void insertPlainText(const QString &text);

	///Sets the text edit's text. The text can be plain text or HTML and the text edit will try to guess the right format.
	///Use setHtml() or setPlainText() directly to avoid text edit's guessing.
	void setText(const std::string & text);
	void setText(const QString & text);	

	void setToolTip(std::string str);
	void setWhatsThis(std::string str);
};
QT_END_NAMESPACE
 

#endif
