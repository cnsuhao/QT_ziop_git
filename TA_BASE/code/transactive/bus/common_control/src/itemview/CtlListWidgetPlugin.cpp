#include "CtlListWidget.h"
#include "CtlListWidgetPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlListWidgetPlugin::CtlListWidgetPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}

void CtlListWidgetPlugin::initialize(QDesignerFormEditorInterface * /* core */)

{
	if (m_initialized)
		return;

	m_initialized = true;
}

bool CtlListWidgetPlugin::isInitialized() const
{
	return m_initialized;
}
 
QString CtlListWidgetPlugin::name() const
{
	return "CtlListWidget";
}

QString CtlListWidgetPlugin::group() const
{
	return tr("Common Control");
}

QString CtlListWidgetPlugin::toolTip() const
{
	return tr("CtlListWidget");
}

QString CtlListWidgetPlugin::whatsThis() const
{
	return tr("CtlListWidget");
}

QString CtlListWidgetPlugin::includeFile() const
{
	return "bus/common_control/src/itemview/CtlListWidget.h";
}

QIcon CtlListWidgetPlugin::icon() const
{
	return QIcon();
}

bool CtlListWidgetPlugin::isContainer() const
{
	return false;
}

QWidget * CtlListWidgetPlugin::createWidget(QWidget *parent)
{
	return new CtlListWidget(parent);
}

QString CtlListWidgetPlugin::domXml() const
{ 
	return QLatin1String("<widget class=\"CtlListWidget\" name=\"lstWidget\">\n</widget>\n");
	return  "<ui language=\"c++\">\n"
		" <widget class=\"CtlListWidget\" name=\"lstWidget\">\n"
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