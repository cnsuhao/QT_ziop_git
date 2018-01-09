#include "CtlTreeWidget.h"
#include "CtlTreeWidgetPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlTreeWidgetPlugin::CtlTreeWidgetPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}

void CtlTreeWidgetPlugin::initialize(QDesignerFormEditorInterface * /* core */)

{
	if (m_initialized)
		return;

	m_initialized = true;
}

bool CtlTreeWidgetPlugin::isInitialized() const
{
	return m_initialized;
}
 
QString CtlTreeWidgetPlugin::name() const
{
	return "CtlTreeWidget";
}

QString CtlTreeWidgetPlugin::group() const
{
	return tr("Common Control");
}

QString CtlTreeWidgetPlugin::toolTip() const
{
	return tr("CtlTreeWidget");
}

QString CtlTreeWidgetPlugin::whatsThis() const
{
	return tr("CtlTreeWidget");
}

QString CtlTreeWidgetPlugin::includeFile() const
{
	return "bus/common_control/src/itemview/CtlTreeWidget.h";
}

QIcon CtlTreeWidgetPlugin::icon() const
{
	return QIcon();
}

bool CtlTreeWidgetPlugin::isContainer() const
{
	return false;
}

QWidget * CtlTreeWidgetPlugin::createWidget(QWidget *parent)
{
	return new CtlTreeWidget(parent);
}

QString CtlTreeWidgetPlugin::domXml() const
{ 
	return QLatin1String("<widget class=\"CtlTreeWidget\" name=\"treWidget\">\n</widget>\n");
	return  "<ui language=\"c++\">\n"
		" <widget class=\"CtlTreeWidget\" name=\"treWidget\">\n"
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