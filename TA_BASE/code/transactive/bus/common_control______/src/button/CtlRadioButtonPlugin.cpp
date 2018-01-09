#include "CtlRadioButton.h"
#include "CtlRadioButtonPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlRadioButtonPlugin::CtlRadioButtonPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}

 
QString CtlRadioButtonPlugin::name() const
{
	return "CtlRadioButton";
}

QString CtlRadioButtonPlugin::group() const
{
	return tr("Common Control");
}

QString CtlRadioButtonPlugin::toolTip() const
{
	return tr("CtlRadioButton");
}

QString CtlRadioButtonPlugin::whatsThis() const
{
	return tr("CtlRadioButton");
}

QString CtlRadioButtonPlugin::includeFile() const
{
	return "bus/common_control/src/button/CtlRadioButton.h";
}

QIcon CtlRadioButtonPlugin::icon() const
{
	return QIcon();
}

bool CtlRadioButtonPlugin::isContainer() const
{
	return false;
}

QWidget * CtlRadioButtonPlugin::createWidget(QWidget *parent)
{
	return new CtlRadioButton(parent);
}


 

QString CtlRadioButtonPlugin::domXml() const
{ 
     return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlRadioButton\" name=\"rad\">\
		<property name=\"geometry\">\
		<rect>\
		<x>0</x>\
		<y>0</y>\
		<width>100</width>\
		<height>30</height>\
		</rect>\
		</property>\
		</widget>\
		</ui>");;
}

QT_END_NAMESPACE