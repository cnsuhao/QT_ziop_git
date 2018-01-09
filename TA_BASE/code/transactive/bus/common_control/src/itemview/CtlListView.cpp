 

#include "bus/common_control/src/itemview/CtlListView.h"
QT_BEGIN_NAMESPACE
CtlListView::CtlListView(QWidget* parent):  QListView(parent) 
{
	 //setSelectionBehavior(QAbstractItemView::SelectRows);
	 //setSelectionMode(QAbstractItemView::ContiguousSelection);	
	  
}

CtlListView::~CtlListView()
{
}


void CtlListView::setObjectName(const QString &name)
{
	QListView::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}
 

QT_END_NAMESPACE
 