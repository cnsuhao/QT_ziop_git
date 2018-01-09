 

#include "bus/common_control/src/itemview/CtlTableWidget.h"
#include "bus/common_control/src/common/StringUtility.h"
#include "bus/common_control/src/common/DateTimeUtility.h"
QT_BEGIN_NAMESPACE

CtlTableWidgetItem::CtlTableWidgetItem(UISortType dataType , int type)
   :QTableWidgetItem(type)
   ,m_dataType(dataType)
{
}

CtlTableWidgetItem::CtlTableWidgetItem(const QString &text, UISortType dataType , int type) 
	:QTableWidgetItem(text, type)
	,m_dataType(dataType)
{
	 
}

 
CtlTableWidgetItem::CtlTableWidgetItem(const QIcon &icon, const QString &text, UISortType dataType, int type)
	:QTableWidgetItem(icon, text, type)
	, m_dataType(dataType)
{
}

CtlTableWidgetItem::CtlTableWidgetItem(const std::string &text, UISortType dataType, int type)
	: QTableWidgetItem(StringUtility::fromStdString(text), type)
	, m_dataType(dataType)
{

}


CtlTableWidgetItem::CtlTableWidgetItem(const QIcon &icon, const std::string &text, UISortType dataType, int type)
	:QTableWidgetItem(icon,StringUtility::fromStdString(text), type)
	, m_dataType(dataType)
{
}

void CtlTableWidgetItem::setText(const std::string &text)
{
	QTableWidgetItem::setText(StringUtility::fromStdString(text));
}
void CtlTableWidgetItem::setToolTip(const std::string &toolTip)
{
	QTableWidgetItem::setToolTip(StringUtility::fromStdString(toolTip));
}

void CtlTableWidgetItem::setWhatsThis(const std::string &whatsThis)
{
	QTableWidgetItem::setWhatsThis(StringUtility::fromStdString(whatsThis));
}

 

bool CtlTableWidgetItem ::operator< (const QTableWidgetItem &other) const
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
		QDateTime dt=DateTimeUtility::toQDateTime(this->text());
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



/*CtlTableWidget******************************************************************************/


CtlTableWidget::CtlTableWidget(QWidget* parent):  QTableWidget(parent)//, ctlCommonEnhance(this)
{
	 setSelectionBehavior(QAbstractItemView::SelectRows);
	 setSelectionMode(QAbstractItemView::ContiguousSelection);	
	 setSortingEnabled(true);
	 connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sortTableView(int)));
}

CtlTableWidget::~CtlTableWidget()
{
}
void CtlTableWidget::setObjectName(const QString &name)
{
	QTableWidget::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}

 

void CtlTableWidget::sortTableView(int column)
{
    bool ascending = (horizontalHeader()->sortIndicatorSection() == column && horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder);
    Qt::SortOrder order = ascending ? Qt::AscendingOrder : Qt::DescendingOrder;
	sectionClickedEvent(column,order);
}
void CtlTableWidget::sectionClickedEvent(int column, Qt::SortOrder order)
{
	return ;
}



QList<QTableWidgetItem *>	CtlTableWidget::findItems(const QString &text, Qt::MatchFlags flags) const
{
	return QTableWidget::findItems(text, flags);
}
QList<QTableWidgetItem *>	CtlTableWidget::findItems(const std::string &text, Qt::MatchFlags flags) const
{
	QString str = StringUtility::fromStdString(text);
	return QTableWidget::findItems(str, flags);
}

void	CtlTableWidget::setVerticalHeaderLabels(const QStringList &labels)
{	 
	QTableWidget::setVerticalHeaderLabels(labels);
}
void	CtlTableWidget::setVerticalHeaderLabels(const STDStringList &labels)
{
	QStringList lst;
	STDStringList::const_iterator iter;
	for (iter = labels.begin(); iter != labels.end(); iter++)
	{
		lst.append(StringUtility::fromStdString(*iter));
	}
	QTableWidget::setVerticalHeaderLabels(lst);
}


void	CtlTableWidget::setHorizontalHeaderLabels(const QStringList &labels)
{
	QTableWidget::setHorizontalHeaderLabels(labels);
}
void	CtlTableWidget::setHorizontalHeaderLabels(const STDStringList &labels)
{
	QStringList lst;
	STDStringList::const_iterator iter;
	for (iter = labels.begin(); iter != labels.end(); iter++)
	{
		lst.append(StringUtility::fromStdString(*iter));
	}
	QTableWidget::setHorizontalHeaderLabels(lst);
}

void	CtlTableWidget::setCurrentItem(CtlTableWidgetItem *item)
{
	QTableWidget::setCurrentItem(item);
}

void	CtlTableWidget::setCurrentItem(CtlTableWidgetItem *item, QItemSelectionModel::SelectionFlags command)
{
	QTableWidget::setCurrentItem(item, command);
}

void	CtlTableWidget::setItem(int row, int column, CtlTableWidgetItem *item)
{
	QTableWidget::setItem(row, column, item);
}
void	CtlTableWidget::setItemPrototype(const CtlTableWidgetItem *item)
{
	QTableWidget::setItemPrototype(item);
}

QT_END_NAMESPACE
 