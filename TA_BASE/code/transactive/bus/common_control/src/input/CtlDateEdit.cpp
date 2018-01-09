 
#include "CtlDateEdit.h"
#include "bus/common_control/src/common/StringUtility.h"
#include "bus/common_control/src/common/DateTimeUtility.h"

QT_BEGIN_NAMESPACE
CtlDateEdit::CtlDateEdit(QWidget* parent):  QDateEdit(parent) 
{
    //set display format, so that all application can use same datetime format
	QDateEdit::setDisplayFormat(DateTimeUtility::DateFormat);
	//enable calendar popup by default
	QDateEdit::setDate(QDateTime::currentDateTime().date());
	this->setCalendarPopup(true);
}

CtlDateEdit::~CtlDateEdit()
{
}

void CtlDateEdit::setObjectName(const QString &name)
{
	QDateEdit::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}
 
void CtlDateEdit::setToolTip(std::string str)
{
	QDateEdit::setToolTip(StringUtility::fromStdString(str));
}
void CtlDateEdit::setWhatsThis(std::string str)
{
	QDateEdit::setWhatsThis(StringUtility::fromStdString(str));
}

void CtlDateEdit::setDateTime(const QDateTime &dateTime)
{
	QDateEdit::setDateTime(dateTime);	 
}
 
void CtlDateEdit::setDisplayFormat(const std::string &format)
{
	QDateEdit::setDisplayFormat(StringUtility::fromStdString(format));
}

QT_END_NAMESPACE
 