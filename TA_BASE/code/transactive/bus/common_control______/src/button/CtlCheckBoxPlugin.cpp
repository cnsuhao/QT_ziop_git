#include "CtlCheckBox.h"
#include "CtlCheckBoxPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlCheckBoxPlugin::CtlCheckBoxPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}

 
QString CtlCheckBoxPlugin::name() const
{
	return "CtlCheckBox";
}

QString CtlCheckBoxPlugin::group() const
{
	return tr("Common Control");
}

QString CtlCheckBoxPlugin::toolTip() const
{
	return tr("CtlCheckBox");
}

QString CtlCheckBoxPlugin::whatsThis() const
{
	return tr("CtlCheckBox");
}

QString CtlCheckBoxPlugin::includeFile() const
{
	return "bus/common_control/src/button/CtlCheckBox.h";
}

QIcon CtlCheckBoxPlugin::icon() const
{
	return QIcon();
}

bool CtlCheckBoxPlugin::isContainer() const
{
	return false;
}

QWidget * CtlCheckBoxPlugin::createWidget(QWidget *parent)
{
	return new CtlCheckBox(parent);
}


 

QString CtlCheckBoxPlugin::domXml() const
{ 
     return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlCheckBox\" name=\"chk\">\
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