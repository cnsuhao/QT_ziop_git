 

#include "bus/common_control/src/container/CtlTabWidget.h"
#include "bus/common_control/src/common/StringUtility.h"
QT_BEGIN_NAMESPACE
CtlTabWidget::CtlTabWidget(QWidget* parent) : QTabWidget(parent)//, ctlCommonEnhance(this)
{
	//this->setSortingEnabled(true);
	//this->sortItems(Qt::AscendingOrder);	 
}

CtlTabWidget::~CtlTabWidget()
{
}
  
int CtlTabWidget::addTab(QWidget *page, const std::string &label)
{
	return QTabWidget::addTab(page, StringUtility::fromStdString(label));
}
int CtlTabWidget::addTab(QWidget *page, const QIcon &icon, const std::string &label)
{
	return QTabWidget::addTab(page, icon,StringUtility::fromStdString(label));
}
 
int CtlTabWidget::insertTab(int index, QWidget *page, const std::string  &label)
{
	return QTabWidget::insertTab(index, page, StringUtility::fromStdString(label));
}
int CtlTabWidget::insertTab(int index, QWidget *page, const QIcon &icon, const std::string  &label)
{
	return QTabWidget::insertTab(index, page, icon,StringUtility::fromStdString(label));
}
 
void CtlTabWidget::setTabText(int index, const std::string  &label)
{
	QTabWidget::setTabText(index,   StringUtility::fromStdString(label));
}
void CtlTabWidget::setToolTip(std::string str)
{
	QTabWidget::setToolTip(StringUtility::fromStdString(str));
}
void CtlTabWidget::setWhatsThis(std::string str)
{
	QTabWidget::setWhatsThis(StringUtility::fromStdString(str));
}

void CtlTabWidget::setObjectName(const QString &name)
{
	QTabWidget::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}


QT_END_NAMESPACE
 