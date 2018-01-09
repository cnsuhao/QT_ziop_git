 
#include "bus/common_control/src/textedit/CtlLineEdit.h"
#include "bus/common_control/src/common/StringUtility.h"
QT_BEGIN_NAMESPACE
CtlLineEdit::CtlLineEdit(QWidget* parent):  QLineEdit(parent) 
{
	QObject::connect(this, SIGNAL(textChanged(const QString &)), this, SLOT(textChangedSLOT(const QString &)));
	QObject::connect(this, SIGNAL(textEdited(const QString &)), this, SLOT(textEditedSLOT(const QString &)));
}

CtlLineEdit::~CtlLineEdit()
{
}

std::string CtlLineEdit::textEx()
{
	return  StringUtility::toStdString(CtlLineEdit::text());
}

void CtlLineEdit::setText(const std::string &text)
{
	QLineEdit::setText(StringUtility::fromStdString(text));
}

void CtlLineEdit::setText(const QString &text)
{
	QLineEdit::setText(text);
}

std::string	CtlLineEdit::displayTextEx() const
{
	return  StringUtility::toStdString(this->displayText());
}
std::string	CtlLineEdit::inputMaskEx() const
{
	return  StringUtility::toStdString(this->inputMask());
}
std::string	CtlLineEdit::placeholderTextEx() const
{
	return  StringUtility::toStdString(this->placeholderText());
}

std::string	CtlLineEdit::selectedTextEx() const
{
	return  StringUtility::toStdString(this->selectedText());
}

void	CtlLineEdit::setInputMask(const std::string &inputMask)
{
	QLineEdit::setInputMask(StringUtility::fromStdString(inputMask));
}
void	CtlLineEdit::setPlaceholderText(const std::string  & str)
{
	QLineEdit::setPlaceholderText(StringUtility::fromStdString(str));
}


void CtlLineEdit::setToolTip(std::string str)
{
	QLineEdit::setToolTip(StringUtility::fromStdString(str));
}
void CtlLineEdit::setWhatsThis(std::string str)
{
	QLineEdit::setWhatsThis(StringUtility::fromStdString(str));
}

void CtlLineEdit::textChangedSLOT(const QString &text)
{
	std::string str = StringUtility::toStdString(text);
	emit textChanged(str);
}
void CtlLineEdit::textEditedSLOT(const QString &text)
{
	std::string str = StringUtility::toStdString(text);
	emit textEdited(str);
}

void CtlLineEdit::setObjectName(const QString &name)
{
	QLineEdit::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}
QT_END_NAMESPACE
 