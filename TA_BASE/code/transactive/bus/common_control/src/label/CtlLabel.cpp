 
#include "bus/common_control/src/label/CtlLabel.h"
#include "bus/common_control/src/common/StringUtility.h"
QT_BEGIN_NAMESPACE
CtlLabel::CtlLabel(QWidget* parent) :
    QLabel(parent)	 
{
	
}

CtlLabel::~CtlLabel()
{
}

std::string CtlLabel::textEx()
{
	return  StringUtility::toStdString(this->text());
}

void CtlLabel::setText(const std::string &text)
{
	QLabel::setText(StringUtility::fromStdString(text));
}

void CtlLabel::setText(const QString &text)
{
	QLabel::setText(text);
}

std::string CtlLabel::selectedTextEx() const
{
	return  StringUtility::toStdString(this->selectedText());
}

void CtlLabel::setToolTip(std::string str)
{
	QLabel::setToolTip(StringUtility::fromStdString(str));
}
void CtlLabel::setWhatsThis(std::string str)
{
	QLabel::setWhatsThis(StringUtility::fromStdString(str));
}

void CtlLabel::setObjectName(const QString &name)
{
	QLabel::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}

QT_END_NAMESPACE

