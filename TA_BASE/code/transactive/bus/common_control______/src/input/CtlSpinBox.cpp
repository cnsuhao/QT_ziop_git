 
#include "CtlSpinBox.h"
#include "bus/common_control/src/common/StringUtility.h"
QT_BEGIN_NAMESPACE
CtlSpinBox::CtlSpinBox(QWidget* parent):  QSpinBox(parent) 
{
	
}

CtlSpinBox::~CtlSpinBox()
{
}

void CtlSpinBox::setObjectName(const QString &name)
{
	QSpinBox::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}

void CtlSpinBox::setToolTip(std::string str)
{
	QSpinBox::setToolTip(StringUtility::fromStdString(str));
}
void CtlSpinBox::setWhatsThis(std::string str)
{
	QSpinBox::setWhatsThis(StringUtility::fromStdString(str));
}


std::string CtlSpinBox::textFromValueEx(int value) const
{
	return StringUtility::toStdString(this->textFromValue(value));
}

int	CtlSpinBox::valueFromText(const std::string &text) const
{
	const QString str = StringUtility::fromStdString(text);
	return QSpinBox::valueFromText(str);
}

void CtlSpinBox::setValue(int val)
{
	QSpinBox::setValue(val);
}
 
QT_END_NAMESPACE
 