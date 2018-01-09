 
#include "CtlTimeEdit.h"
#include "bus/common_control/src/common/StringUtility.h"
#include "bus/common_control/src/common/DateTimeUtility.h"

 

QT_BEGIN_NAMESPACE
CtlTimeEdit::CtlTimeEdit(QWidget* parent):  QTimeEdit(parent) 
{
	QTimeEdit::setDisplayFormat(DateTimeUtility::TimeFormat);
	QTimeEdit::setTime(QDateTime::currentDateTime().time());
	//this->setButtonSymbols( ButtonSymbols::UpDownArrows);
	//enable calendar popup by default
	this->setCalendarPopup(true);
}

CtlTimeEdit::~CtlTimeEdit()
{
}

void CtlTimeEdit::setObjectName(const QString &name)
{
	QTimeEdit::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}

void CtlTimeEdit::setToolTip(std::string str)
{
	QTimeEdit::setToolTip(StringUtility::fromStdString(str));
}
void CtlTimeEdit::setWhatsThis(std::string str)
{
	QTimeEdit::setWhatsThis(StringUtility::fromStdString(str));
}

void CtlTimeEdit::setDateTime(const QDateTime &dateTime)
{
	QTimeEdit::setDateTime(dateTime);
}

void CtlTimeEdit::setDisplayFormat(const std::string &format)
{
	QTimeEdit::setDisplayFormat(StringUtility::fromStdString(format));
}
QT_END_NAMESPACE
 