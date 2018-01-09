#include "CtlTableWidget.h"
#include "CtlTableWidgetPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlTableWidgetPlugin::CtlTableWidgetPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}

 
QString CtlTableWidgetPlugin::name() const
{
	return "CtlTableWidget";
}

QString CtlTableWidgetPlugin::group() const
{
	return tr("Common Control");
}

QString CtlTableWidgetPlugin::toolTip() const
{
	return tr("CtlTableWidget");
}

QString CtlTableWidgetPlugin::whatsThis() const
{
	return tr("CtlTableWidget");
}

QString CtlTableWidgetPlugin::includeFile() const
{
	return "bus/common_control/src/itemview/CtlTableWidget.h";
}

QIcon CtlTableWidgetPlugin::icon() const
{
	return QIcon();
}

bool CtlTableWidgetPlugin::isContainer() const
{
	return false;
}

QWidget * CtlTableWidgetPlugin::createWidget(QWidget *parent)
{
	return new CtlTableWidget(parent);
}


 

QString CtlTableWidgetPlugin::domXml() const
{ 
     return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlTableWidget\" name=\"lst\">\
		<property name=\"geometry\">\
		<rect>\
		<x>0</x>\
		<y>0</y>\
		<width>200</width>\
		<height>60</height>\
		</rect>\
		</property>\
		</widget>\
		</ui>");;
}

QT_END_NAMESPACE