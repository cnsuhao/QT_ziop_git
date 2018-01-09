 

#include "bus/common_control/src/button/CtlButton.h"
#include "bus/common_control/src/common/StringUtility.h"

QT_BEGIN_NAMESPACE
CtlButton::CtlButton(QWidget* parent):  QPushButton(parent) 
{
	
}

CtlButton::~CtlButton()
{
	 
}

void CtlButton::setObjectName(const QString &name)
{
	QPushButton::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}

 

 std::string CtlButton::textEx()
 {
	 return  StringUtility::toStdString(this->text());
 }
 
 void CtlButton::setText(const std::string &text)
 {
	 QPushButton::setText(StringUtility::fromStdString(text));	 
 }

 void CtlButton::setText(const QString &text)
 {
	 QPushButton::setText(text);
 }

 void CtlButton::setToolTip(std::string str)
 {
	 QPushButton::setToolTip(StringUtility::fromStdString(str));
 }
 void CtlButton::setWhatsThis(std::string str)
 {
	 QPushButton::setWhatsThis(StringUtility::fromStdString(str));
 }

 

QT_END_NAMESPACE
 