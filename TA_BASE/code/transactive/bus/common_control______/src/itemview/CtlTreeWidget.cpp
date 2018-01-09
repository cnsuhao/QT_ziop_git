 

#include "bus/common_control/src/itemview/CtlTreeWidget.h"
#include "bus/common_control/src/common/StringUtility.h"
QT_BEGIN_NAMESPACE
CtlTreeWidget::CtlTreeWidget(QWidget* parent) : QTreeWidget(parent)//, ctlCommonEnhance(this)
{
	//this->setSortingEnabled(true);
	//this->sortItems(Qt::AscendingOrder);	 
}

CtlTreeWidget::~CtlTreeWidget()
{
}
 

void CtlTreeWidget::setObjectName(const QString &name)
{
	QTreeWidget::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}

QList<QTreeWidgetItem *>	CtlTreeWidget::findItems(const std::string & text, Qt::MatchFlags flags) const
{
	QString str = StringUtility::fromStdString(text);
	return QTreeWidget::findItems(str, flags);
}

QList<QTreeWidgetItem *>	CtlTreeWidget::findItems(const QString & text, Qt::MatchFlags flags) const
{	 
	return QTreeWidget::findItems(text, flags);
}

 

void	CtlTreeWidget::setHeaderLabel(  const QString & label)
{
	QTreeWidget::setHeaderLabel( label);
}

void	CtlTreeWidget::setHeaderLabel( const std::string & label)
{
	QString str = StringUtility::fromStdString(label);
	QTreeWidget::setHeaderLabel(str);
}


void	CtlTreeWidget::setHeaderLabels( const QStringList & labels)
{
	QTreeWidget::setHeaderLabels( labels);
}


void	CtlTreeWidget::setHeaderLabels(const STDStringList & labels)
{
	QStringList lst;
	STDStringList::const_iterator iter;
	for (iter = labels.begin(); iter != labels.end(); iter++)
	{
		lst.append(StringUtility::fromStdString(*iter));
	}
	QTreeWidget::setHeaderLabels(lst);
}
 

QT_END_NAMESPACE
 