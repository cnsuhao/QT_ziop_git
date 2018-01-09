 

#include "bus/common_control/src/button/CtlButton.h"
QT_BEGIN_NAMESPACE
CtlButton::CtlButton(QWidget* parent):  QPushButton(parent), ctlCommonEnhance(this)
{
	
}

CtlButton::~CtlButton()
{
}

 void CtlButton::setControlID(TA_String controlID)
{
	ctlCommonEnhance::setControlID(controlID);
	this->setText(controlID);
}

 

QT_END_NAMESPACE
 