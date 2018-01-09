 

#include "bus/common_control/src/itemview/CtlListWidget.h"
#include "bus/common_control/src/common/StringUtility.h"
#include "bus/common_control/src/common/DateTimeUtility.h"

QT_BEGIN_NAMESPACE
 
CtlListWidgetItem::CtlListWidgetItem(QListWidget *view , UISortType dataType , int type )
	:QListWidgetItem(view, type), m_dataType(dataType)
{

}
CtlListWidgetItem::CtlListWidgetItem(const QString &text, UISortType dataType, QListWidget *view, int type)
	:QListWidgetItem(text,view, type), m_dataType(dataType)
{

}
CtlListWidgetItem::CtlListWidgetItem(const QIcon &icon, const QString &text, UISortType dataType, QListWidget *view , int type)
	: QListWidgetItem(icon, text, view, type), m_dataType(dataType)
{

}
CtlListWidgetItem::CtlListWidgetItem(const std::string &text, UISortType dataType, QListWidget *view, int type)
	: QListWidgetItem(StringUtility::fromStdString( text), view, type), m_dataType(dataType)
{

}
CtlListWidgetItem::CtlListWidgetItem(const QIcon &icon, const std::string &text, UISortType dataType, QListWidget *view, int type)
	: QListWidgetItem(icon, StringUtility::fromStdString(text), view, type), m_dataType(dataType)
{

}





void CtlListWidgetItem::setText(const std::string &text)
{
	QListWidgetItem::setText(StringUtility::fromStdString(text));
}
void CtlListWidgetItem::setToolTip(const std::string &toolTip)
{
	QListWidgetItem::setToolTip(StringUtility::fromStdString(toolTip));
}

void CtlListWidgetItem::setWhatsThis(const std::string &whatsThis)
{
	QListWidgetItem::setWhatsThis(StringUtility::fromStdString(whatsThis));
}

bool CtlListWidgetItem::operator< (const QListWidgetItem &other) const
{
	switch (m_dataType)
	{
	case UISortType::UINUMBER:
		if (this->text().toInt() < other.text().toInt())
			return true;
		else
			return false;
		break;
	case UISortType::UIDATETIME:
	{
		QDateTime dt = DateTimeUtility::toQDateTime(this->text());
		QDateTime dtOther = DateTimeUtility::toQDateTime(other.text());
		if (dt < dtOther)
			return true;
		else
			return false;
		break;
	}
	case UISortType::UIDATE:
	{
		QDate dt = DateTimeUtility::toQDate(this->text());
		QDate dtOther = DateTimeUtility::toQDate(other.text());
		if (dt < dtOther)
			return true;
		else
			return false;
		break;
	}
	default:
		if (this->text().toUpper() < other.text().toUpper())
			return true;
		else
			return false;
	}
	return false;
}
 



CtlListWidget::CtlListWidget(QWidget* parent) : QListWidget(parent)//, ctlCommonEnhance(this)
{
	this->setSortingEnabled(true);
	//this->sortItems(Qt::AscendingOrder);	 
}

CtlListWidget::~CtlListWidget()
{
}

void CtlListWidget::setObjectName(const QString &name)
{
	QListWidget::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}

void	CtlListWidget::addItem(const QString & label)
{
	QListWidget::addItem(label);
}
void	CtlListWidget::addItems(const QStringList & labels)
{
	QListWidget::addItems(labels);
}

void	CtlListWidget::addItem(const std::string & label)
{
	QListWidget::addItem(StringUtility::fromStdString(label));
}
void	CtlListWidget::addItems(const STDStringList & labels)
{
	QStringList lst;
	STDStringList::const_iterator iter;
	for (iter = labels.begin(); iter != labels.end(); iter++)
	{
		lst.append(StringUtility::fromStdString(*iter));
	}
	QListWidget::addItems(lst);
}


QList<QListWidgetItem *>	CtlListWidget::findItems(const std::string & text, Qt::MatchFlags flags) const
{
	QString str = StringUtility::fromStdString(text);
	return QListWidget::findItems(str, flags);
}

QList<QListWidgetItem *>	CtlListWidget::findItems(const QString & text, Qt::MatchFlags flags) const
{	 
	return QListWidget::findItems(text, flags);
}


void	CtlListWidget::insertItem(int row, const QString & label)
{
	QListWidget::insertItem(row, label);
}
void	CtlListWidget::insertItems(int row, const QStringList & labels)
{
	QListWidget::insertItems(row, labels);
}

void	CtlListWidget::insertItem(int row, const std::string & label)
{
	QString str = StringUtility::fromStdString(label);
	QListWidget::insertItem(row, str);
}
void	CtlListWidget::insertItems(int row, const STDStringList & labels)
{
	QStringList lst;
	STDStringList::const_iterator iter;
	for (iter = labels.begin(); iter != labels.end(); iter++)
	{
		lst.append(StringUtility::fromStdString(*iter));
	}
	QListWidget::insertItems(row,lst);
}
 

QT_END_NAMESPACE
 