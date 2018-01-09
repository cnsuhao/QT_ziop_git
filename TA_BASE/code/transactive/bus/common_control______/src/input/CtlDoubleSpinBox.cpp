 
#include "CtlDoubleSpinBox.h"
#include "bus/common_control/src/common/StringUtility.h"
QT_BEGIN_NAMESPACE
CtlDoubleSpinBox::CtlDoubleSpinBox(QWidget* parent):  QDoubleSpinBox(parent) 
{
	
}

CtlDoubleSpinBox::~CtlDoubleSpinBox()
{
}

void CtlDoubleSpinBox::setObjectName(const QString &name)
{
	QDoubleSpinBox::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}

void CtlDoubleSpinBox::setToolTip(std::string str)
{
	QDoubleSpinBox::setToolTip(StringUtility::fromStdString(str));
}
void CtlDoubleSpinBox::setWhatsThis(std::string str)
{
	QDoubleSpinBox::setWhatsThis(StringUtility::fromStdString(str));
}
 
std::string CtlDoubleSpinBox::textFromValueEx(double value) const
{
	return StringUtility::toStdString(this->textFromValue(value));
}

double	CtlDoubleSpinBox::valueFromText(const std::string &text) const
{
	const QString str = StringUtility::fromStdString(text);
	return QDoubleSpinBox::valueFromText(str);
}

void CtlDoubleSpinBox::setValue(double val)
{
	QDoubleSpinBox::setValue(val);
}


QT_END_NAMESPACE
 