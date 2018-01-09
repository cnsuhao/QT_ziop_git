#include "CtlButton.h"
#include "CtlButtonPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlButtonPlugin::CtlButtonPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}

 
QString CtlButtonPlugin::name() const
{
	return "CtlButton";
}

QString CtlButtonPlugin::group() const
{
	return tr("Common Control");
}

QString CtlButtonPlugin::toolTip() const
{
	return tr("CtlButton");
}

QString CtlButtonPlugin::whatsThis() const
{
	return tr("CtlButton");
}

QString CtlButtonPlugin::includeFile() const
{
	return "bus/common_control/src/button/CtlButton.h";
}

QIcon CtlButtonPlugin::icon() const
{
	return QIcon();
}

bool CtlButtonPlugin::isContainer() const
{
	return false;
}

QWidget * CtlButtonPlugin::createWidget(QWidget *parent)
{
	return new CtlButton(parent);
}


 

QString CtlButtonPlugin::domXml() const
{ 
     return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlButton\" name=\"btn\">\
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