 
#include "bus/common_control/src/textedit/CtlPlainTextEdit.h"
#include "bus/common_control/src/common/StringUtility.h"
QT_BEGIN_NAMESPACE
CtlPlainTextEdit::CtlPlainTextEdit(QWidget* parent):  QPlainTextEdit(parent) 
{	
}

CtlPlainTextEdit::~CtlPlainTextEdit()
{
}
 
std::string	CtlPlainTextEdit::placeholderTextEx() const
{
	return  StringUtility::toStdString(this->placeholderText());
}

void CtlPlainTextEdit::setObjectName(const QString &name)
{
	QPlainTextEdit::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}

 
std::string	CtlPlainTextEdit::toPlainTextEx() const
{
	return  StringUtility::toStdString(this->toPlainText());	 
}

void CtlPlainTextEdit::setPlaceholderText(const std::string  & str)
{
	QPlainTextEdit::setPlaceholderText(StringUtility::fromStdString(str));
}




bool CtlPlainTextEdit::find(const std::string & exp, QTextDocument::FindFlags options)
{
	QString strExp = StringUtility::fromStdString(exp);
	return QPlainTextEdit::find(strExp, options);
}
std::string	CtlPlainTextEdit::documentTitleEx() const
{
	return StringUtility::toStdString(this->documentTitle());
}
void	CtlPlainTextEdit::setPlainText(const std::string & text)
{
	QString str=StringUtility::fromStdString(text);
	QPlainTextEdit::setPlainText(str);
}
void CtlPlainTextEdit::setPlainText(const QString & text)
{
	QPlainTextEdit::setPlainText(text);
}
void CtlPlainTextEdit::insertPlainText(const std::string &text)
{
	QString str = StringUtility::fromStdString(text);
	QPlainTextEdit::insertPlainText(str);
}
void CtlPlainTextEdit::appendPlainText(const std::string &text)
{
	QString str = StringUtility::fromStdString(text);
	QPlainTextEdit::appendPlainText(str);
}
void CtlPlainTextEdit::appendHtml(const std::string &html)
{
	QString str = StringUtility::fromStdString(html);
	QPlainTextEdit::appendHtml(str);
}
void CtlPlainTextEdit::insertPlainText(const QString &text)
{	 
	QPlainTextEdit::insertPlainText(text);
}
void CtlPlainTextEdit::appendPlainText(const QString &text)
{
	QPlainTextEdit::appendPlainText(text);
}
void CtlPlainTextEdit::appendHtml(const QString &html)
{
	QPlainTextEdit::appendHtml(html);
}

void CtlPlainTextEdit::setToolTip(std::string str)
{
	QPlainTextEdit::setToolTip(StringUtility::fromStdString(str));
}
void CtlPlainTextEdit::setWhatsThis(std::string str)
{
	QPlainTextEdit::setWhatsThis(StringUtility::fromStdString(str));
}
 
QT_END_NAMESPACE
 