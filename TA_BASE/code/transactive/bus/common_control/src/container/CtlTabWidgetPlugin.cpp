#include "CtlTabWidget.h"
#include "CtlTabWidgetPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlTabWidgetPlugin::CtlTabWidgetPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}

void CtlTabWidgetPlugin::initialize(QDesignerFormEditorInterface * /* core */)

{
	if (m_initialized)
		return;

	m_initialized = true;
}

bool CtlTabWidgetPlugin::isInitialized() const
{
	return m_initialized;
}
 
QString CtlTabWidgetPlugin::name() const
{
	return "CtlTabWidget";
}

QString CtlTabWidgetPlugin::group() const
{
	return tr("Common Control");
}

QString CtlTabWidgetPlugin::toolTip() const
{
	return tr("CtlTabWidget");
}

QString CtlTabWidgetPlugin::whatsThis() const
{
	return tr("CtlTabWidget");
}

QString CtlTabWidgetPlugin::includeFile() const
{
	return "bus/common_control/src/container/CtlTabWidget.h";
}

QIcon CtlTabWidgetPlugin::icon() const
{
	return QIcon();
}

bool CtlTabWidgetPlugin::isContainer() const
{
	return false;
}

QWidget * CtlTabWidgetPlugin::createWidget(QWidget *parent)
{
	return new CtlTabWidget(parent);
}

QString CtlTabWidgetPlugin::domXml() const
{ 
	return QLatin1String("<widget class=\"CtlTabWidget\" name=\"tabWidget\">\n</widget>\n");
	return  "<ui language=\"c++\">\n"
		" <widget class=\"CtlTabWidget\" name=\"treWidget\">\n"
		"  <property name=\"geometry\">\n"
		"   <rect>\n"
		"    <x>0</x>\n"
		"    <y>0</y>\n"
		"    <width>200</width>\n"
		"    <height>200</height>\n"
		"   </rect>\n"
		"  </property>\n" 
		" </widget>\n"
		"</ui>\n";
}

QT_END_NAMESPACE