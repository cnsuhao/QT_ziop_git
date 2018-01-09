 
#include "CtlComboBox.h"
#include "bus/common_control/src/common/StringUtility.h"

QT_BEGIN_NAMESPACE
CtlComboBox::CtlComboBox(QWidget* parent):  QComboBox(parent) 
{
	QObject::connect(this, SIGNAL(activated(const QString &)), this, SLOT(activatedSLOT(const QString &)));
	QObject::connect(this, SIGNAL(currentIndexChanged(const QString &)), this, SLOT(currentIndexChangedSLOT(const QString &)));
	QObject::connect(this, SIGNAL(currentTextChanged(const QString &)), this, SLOT(currentTextChangedSLOT(const QString &)));
	QObject::connect(this, SIGNAL(editTextChanged(const QString &)), this, SLOT(editTextChangedSLOT(const QString &)));	
}

CtlComboBox::~CtlComboBox()
{
}

void CtlComboBox::setObjectName(const QString &name)
{
	QComboBox::setObjectName(name);
	this->setAccessibleName(name);
	this->setAccessibleDescription(name);
}
//Customized method for Transactive 
void CtlComboBox::setToolTip(std::string str)
{
	QComboBox::setToolTip(StringUtility::fromStdString(str));
}
void CtlComboBox::setWhatsThis(std::string str)
{
	QComboBox::setWhatsThis(StringUtility::fromStdString(str));
}

void    CtlComboBox::addItem(const std::string & text, const QVariant & userData )
{
	QComboBox::addItem(StringUtility::fromStdString(text), userData);
}

void	CtlComboBox::addItem(const QIcon & icon, const std::string & text, const QVariant & userData)
{
	QComboBox::addItem(icon, StringUtility::fromStdString(text), userData);
}

void	CtlComboBox::addItems(const STDStringList & texts)
{
	QStringList lst;
	STDStringList::const_iterator iter;
	for (iter=texts.begin(); iter!=texts.end(); iter++)
	{
		lst.append(StringUtility::fromStdString(*iter));
	}
	QComboBox::addItems(const_cast<const QStringList&> (lst));
}

std::string   CtlComboBox::currentTextEx()
{
	return  StringUtility::toStdString(this->currentText());
}

int	CtlComboBox::findText(const std::string & text, Qt::MatchFlags flags) const
{
	return QComboBox::findText(StringUtility::fromStdString(text), flags);
}


void	CtlComboBox::insertItem(int index, const std::string & text, const QVariant & userData )
{
	QComboBox::insertItem(index, StringUtility::fromStdString(text), userData);
}

void	CtlComboBox::insertItem(int index, const QIcon & icon, const std::string & text, const QVariant & userData )
{
	QComboBox::insertItem(index, icon, StringUtility::fromStdString(text), userData);
}

void	CtlComboBox::insertItems(int index, const STDStringList & list)
{
	//QComboBox::insertItem(index, lst);
	QStringList lst;
	STDStringList::const_iterator iter;
	for (iter = list.begin(); iter != list.end(); iter++)
	{
		lst.append(StringUtility::fromStdString(*iter));
	}
	QComboBox::insertItems(index,const_cast<const QStringList&> (lst));
}

std::string 	CtlComboBox::itemTextEx(int index) const
{
	return  StringUtility::toStdString(this->itemText(index));
}

void	CtlComboBox::setItemText(int index, const std::string & text)
{
	QComboBox::insertItem(index, StringUtility::fromStdString(text));
}
 
void CtlComboBox::setEditText(const std::string &text)
{
	QString qstr = StringUtility::fromStdString(text);
 	QComboBox::setEditText(qstr);	 
}

void CtlComboBox::activatedSLOT(const QString & text)
{
	std::string str = StringUtility::toStdString(text);
	emit activated(str);
}

void CtlComboBox::currentIndexChangedSLOT(const QString & text)
{
	std::string str = StringUtility::toStdString(text);
	emit currentIndexChanged(str);
}
void CtlComboBox::currentTextChangedSLOT(const QString &text)
{
	std::string str = StringUtility::toStdString(text);
	emit currentTextChanged(str);
}
void CtlComboBox::editTextChangedSLOT(const QString &text)
{
	std::string str = StringUtility::toStdString(text);
	emit editTextChanged(str);
}

 
QT_END_NAMESPACE
 