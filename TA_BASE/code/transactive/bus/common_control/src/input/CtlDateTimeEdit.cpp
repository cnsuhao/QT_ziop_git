 
#include "CtlDateTimeEdit.h"
#include "bus/common_control/src/common/StringUtility.h"
#include "bus/common_control/src/common/DateTimeUtility.h"
QT_BEGIN_NAMESPACE
CtlDateTimeEdit::CtlDateTimeEdit(QWidget* parent):  QDateTimeEdit(parent)//, ctlCommonEnhance(this)
{
	//set display format, so that all application can use same datetime format
	QDateTimeEdit::setDisplayFormat(DateTimeUtility::DateTimeFormat);
	QDateTimeEdit::setDateTime(QDateTime::currentDateTime());
	//enable calendar popup by default
	this->setCalendarPopup(true);
}

CtlDateTimeEdit::~CtlDateTimeEdit()
{
}


void CtlDateTimeEdit::setObjectName(const QString &name)
{
	QDateTimeEdit::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}
void CtlDateTimeEdit::setToolTip(std::string str)
{
	QDateTimeEdit::setToolTip(StringUtility::fromStdString(str));
}
void CtlDateTimeEdit::setWhatsThis(std::string str)
{
	QDateTimeEdit::setWhatsThis(StringUtility::fromStdString(str));
}


void CtlDateTimeEdit::setDateTime(const QDateTime &dateTime)
{
	QDateTimeEdit::setDateTime(dateTime);
}

void CtlDateTimeEdit::setDisplayFormat(const std::string &format)
{
	QDateTimeEdit::setDisplayFormat(StringUtility::fromStdString(format));
}
QT_END_NAMESPACE
 