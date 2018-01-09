 

#include "bus/common_control/src/itemview/CtlTableView.h"
QT_BEGIN_NAMESPACE
CtlTableView::CtlTableView(QWidget* parent):  QTableView(parent) 
{
	 setSelectionBehavior(QAbstractItemView::SelectRows);
	 setSelectionMode(QAbstractItemView::ContiguousSelection);	
	 connect(horizontalHeader(), SIGNAL(sectionClicked(int)), this, SLOT(sortTableView(int)));
}

CtlTableView::~CtlTableView()
{
}


void CtlTableView::setObjectName(const QString &name)
{
	QTableView::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}


void CtlTableView::sortTableView(int column)
{
    bool ascending = (horizontalHeader()->sortIndicatorSection() == column && horizontalHeader()->sortIndicatorOrder() == Qt::AscendingOrder);
    Qt::SortOrder order = ascending ? Qt::AscendingOrder : Qt::DescendingOrder;
	sectionClickedEvent(column,order);
}
void CtlTableView::sectionClickedEvent(int column, Qt::SortOrder order)
{
	return ;
}

QT_END_NAMESPACE
 