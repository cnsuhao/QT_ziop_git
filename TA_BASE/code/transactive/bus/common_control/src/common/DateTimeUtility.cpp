#include "DateTimeUtility.h"
#include "StringUtility.h"

QT_BEGIN_NAMESPACE


const QString DateTimeUtility::DateTimeFormat = "dd/MM/yyyy hh::mm::ss";
const QString DateTimeUtility::DateFormat = "dd/MM/yyyy";
const QString DateTimeUtility::TimeFormat = "hh::mm::ss";

DateTimeUtility::DateTimeUtility()
{
}


DateTimeUtility::~DateTimeUtility()
{
}


std::string DateTimeUtility::toDateTimeString(time_t t)
{
	QDateTime qdt=QDateTime::fromTime_t(t);

	QString str = qdt.toString(DateTimeFormat);
	return StringUtility::toStdString(str);
}

std::string DateTimeUtility::toDateString(time_t t)
{
	QDateTime qdt = QDateTime::fromTime_t(t);
	QString str = qdt.toString(DateFormat);
	return StringUtility::toStdString(str);
}
std::string DateTimeUtility::toTimeString(time_t t)
{
	QDateTime qdt = QDateTime::fromTime_t(t);
	QString str = qdt.toString(TimeFormat);
	return StringUtility::toStdString(str);
}

std::string DateTimeUtility::toDateTimeString(QDateTime t)
{	 
	QString str = t.toString(DateTimeFormat);
	return StringUtility::toStdString(str);
}
std::string DateTimeUtility::toDateString(QDateTime t)
{
	QString str = t.toString(DateFormat);
	return StringUtility::toStdString(str);
}

std::string DateTimeUtility::toTimeString(QDateTime t)
{
	QString str = t.toString(TimeFormat);
	return StringUtility::toStdString(str);
}

time_t DateTimeUtility::to_time_t(std::string t)
{
	QString str = StringUtility::fromStdString(t);
	QDateTime qdt = QDateTime::fromString(str, DateTimeFormat);
	return qdt.toTime_t();
}

time_t DateTimeUtility::to_time_t(QString t)
{	 
	QDateTime qdt = QDateTime::fromString(t, DateTimeFormat);
	return qdt.toTime_t();
}


QDateTime DateTimeUtility::toQDateTime(std::string t)
{
	QString str = StringUtility::fromStdString(t);
	return QDateTime::fromString(str, DateTimeFormat);
}

 
QDateTime DateTimeUtility::toQDateTime(QString t)
{
	return QDateTime::fromString(t, DateTimeFormat);
}
 

QDate DateTimeUtility::toQDate(std::string t)
{
	QString str = StringUtility::fromStdString(t);
	return QDate::fromString(str, DateFormat);
}

QDate DateTimeUtility::toQDate(QString t)
{
	return QDate::fromString(t, DateFormat);
}


QTime DateTimeUtility::toQTime(std::string t)
{
	QString str = StringUtility::fromStdString(t);
	return QTime::fromString(str, TimeFormat);
}

QTime DateTimeUtility::toQTime(QString t)
{
	return QTime::fromString(t, TimeFormat);
}


QT_END_NAMESPACE


 