 

#include "bus/common_control/src/button/CtlRadioButton.h"
#include "bus/common_control/src/common/StringUtility.h"

QT_BEGIN_NAMESPACE
CtlRadioButton::CtlRadioButton(QWidget* parent):  QRadioButton(parent) 
{
	
}

CtlRadioButton::~CtlRadioButton()
{
}

void CtlRadioButton::setObjectName(const QString &name)
{
	QRadioButton::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}

 std::string CtlRadioButton::textEx()
 {
	 return  StringUtility::toStdString(this->text());
 }

 void CtlRadioButton::setText(const std::string &text)
 {
	 QRadioButton::setText(StringUtility::fromStdString(text));
 }

 void CtlRadioButton::setText(const QString &text)
 {
	 QRadioButton::setText(text);
 }

 void CtlRadioButton::setChecked(bool checked)
 {
	 QRadioButton::setChecked(checked);
 }

 void CtlRadioButton::setToolTip(std::string str)
 {
	 QRadioButton::setToolTip(StringUtility::fromStdString(str));
 }
 void CtlRadioButton::setWhatsThis(std::string str)
 {
	 QRadioButton::setWhatsThis(StringUtility::fromStdString(str));
 }

QT_END_NAMESPACE
 