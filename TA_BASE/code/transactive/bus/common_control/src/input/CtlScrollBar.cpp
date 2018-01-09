 
#include "CtlScrollBar.h"
#include "bus/common_control/src/common/StringUtility.h"
QT_BEGIN_NAMESPACE
CtlScrollBar::CtlScrollBar(QWidget* parent) : QScrollBar(parent) 
{
	
}

CtlScrollBar::CtlScrollBar(Qt::Orientation o, QWidget *parent ) : QScrollBar(o,parent) 
{

}

CtlScrollBar::~CtlScrollBar()
{
}

void CtlScrollBar::setObjectName(const QString &name)
{
	QScrollBar::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}

void CtlScrollBar::setToolTip(std::string str)
{
	QScrollBar::setToolTip(StringUtility::fromStdString(str));
}
void CtlScrollBar::setWhatsThis(std::string str)
{
	QScrollBar::setWhatsThis(StringUtility::fromStdString(str));
}
 
QT_END_NAMESPACE
 