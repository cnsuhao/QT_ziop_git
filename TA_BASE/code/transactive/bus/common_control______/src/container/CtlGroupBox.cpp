 

#include "bus/common_control/src/container/CtlGroupBox.h"
#include "bus/common_control/src/common/StringUtility.h"

QT_BEGIN_NAMESPACE
CtlGroupBox::CtlGroupBox(QWidget* parent):  QGroupBox(parent)
{
	
}

CtlGroupBox::~CtlGroupBox()
{
}

void CtlGroupBox::setObjectName(const QString &name)
{
	QGroupBox::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}
 

std::string CtlGroupBox::titleEx()
 {
	 return  StringUtility::toStdString(this->title()); 
 }

void CtlGroupBox::setTitle(const std::string &text)
 {
	 QGroupBox::setTitle(StringUtility::fromStdString(text));
 }

void CtlGroupBox::setTitle(const QString &text)
 {
	 QGroupBox::setTitle(text);
 }

void CtlGroupBox::setToolTip(std::string str)
{
	QGroupBox::setToolTip(StringUtility::fromStdString(str));
}
void CtlGroupBox::setWhatsThis(std::string str)
{
	QGroupBox::setWhatsThis(StringUtility::fromStdString(str));
}

 

QT_END_NAMESPACE
 