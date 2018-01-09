 

#include "bus/common_control/src/button/CtlCheckBox.h"
#include "bus/common_control/src/common/StringUtility.h"

QT_BEGIN_NAMESPACE
CtlCheckBox::CtlCheckBox(QWidget* parent):  QCheckBox(parent) 
{
	
}

CtlCheckBox::~CtlCheckBox()
{
}

void CtlCheckBox::setObjectName(const QString &name)
{
	QCheckBox::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}

//void CtlCheckBox::setControlID(QString controlID)
//{
//	ctlCommonEnhance::setControlID(controlID);
//	this->setText(controlID);
//}

std::string CtlCheckBox::textEx()
 {
	 return  StringUtility::toStdString(this->text());
 }

void CtlCheckBox::setText(const std::string &text)
 {
	 QCheckBox::setText(StringUtility::fromStdString(text));
 }

void CtlCheckBox::setText(const QString &text)
 {
	 QCheckBox::setText(text);
 }

void CtlCheckBox::setCheckState(Qt::CheckState state)
{
	QCheckBox::setCheckState(state);
}

void CtlCheckBox::setToolTip(std::string str)
{
	QCheckBox::setToolTip(StringUtility::fromStdString(str));
}
void CtlCheckBox::setWhatsThis(std::string str)
{
	QCheckBox::setWhatsThis(StringUtility::fromStdString(str));
}
 

QT_END_NAMESPACE
 