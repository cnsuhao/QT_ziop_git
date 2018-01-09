 
#include "bus/common_control/src/textedit/CtlTextEdit.h"
#include "bus/common_control/src/common/StringUtility.h"
QT_BEGIN_NAMESPACE
CtlTextEdit::CtlTextEdit(QWidget* parent):  QTextEdit(parent) 
{
	
}

CtlTextEdit::~CtlTextEdit()
{
}
void CtlTextEdit::setObjectName(const QString &name)
{
	QTextEdit::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}


std::string	CtlTextEdit::anchorAtEx(const QPoint & pos) const
{
	return StringUtility::toStdString(this->anchorAt(pos));
}
std::string	CtlTextEdit::documentTitleEx() const
{
	return StringUtility::toStdString(this->documentTitle());
}
std::string	CtlTextEdit::fontFamilyEx() const
{
	return StringUtility::toStdString(this->fontFamily());
}
void CtlTextEdit::setDocumentTitle(const std::string & title)
{
	QString str = StringUtility::fromStdString(title);
	QTextEdit::setDocumentTitle(str);
}
std::string	CtlTextEdit::toHtml() const
{
	
	
	QString str = QTextEdit::toHtml();
	return StringUtility::toStdString(str);
}
std::string	CtlTextEdit::toPlainText() const
{
	return StringUtility::toStdString(QTextEdit::toPlainText());
}
void	CtlTextEdit::setFontFamily(const std::string & fontFamily)
{
	QString str = StringUtility::fromStdString(fontFamily);
	QTextEdit::setFontFamily(str);
}
void CtlTextEdit::append(const std::string &text)
{
	QString str = StringUtility::fromStdString(text);
	QTextEdit::append(str);
}
void CtlTextEdit::append(const QString &text)
{
	QTextEdit::append(text);
}
void CtlTextEdit::insertHtml(const std::string &html)
{
	QString str = StringUtility::fromStdString(html);
	QTextEdit::insertHtml(str);
}
void CtlTextEdit::insertHtml(const QString &html)
{
	QTextEdit::insertHtml(html);
}
void CtlTextEdit::insertPlainText(const std::string &text)
{
	QString str = StringUtility::fromStdString(text);
	QTextEdit::insertPlainText(str);
}
void CtlTextEdit::insertPlainText(const QString &text)
{
	QTextEdit::insertPlainText(text);
}
void CtlTextEdit::setHtml(const std::string &html)
{
	QString str = StringUtility::fromStdString(html);
	QTextEdit::setHtml(str);
}
void CtlTextEdit::setHtml(const QString &html)
{
	QTextEdit::setHtml(html);
}
void CtlTextEdit::setPlainText(const std::string & text)
{
	QString str = StringUtility::fromStdString(text);
	QTextEdit::setPlainText(str);
}
void CtlTextEdit::setPlainText(const QString & text)
{
	QTextEdit::setPlainText(text);
}
void CtlTextEdit::setText(const std::string & text)
{
	QString str = StringUtility::fromStdString(text);
	QTextEdit::setText(str);
}
void CtlTextEdit::setText(const QString & text)
{
	QTextEdit::setText(text);
}

void CtlTextEdit::setToolTip(std::string str)
{
	QTextEdit::setToolTip(StringUtility::fromStdString(str));
}
void CtlTextEdit::setWhatsThis(std::string str)
{
	QTextEdit::setWhatsThis(StringUtility::fromStdString(str));
}
 
QT_END_NAMESPACE
 