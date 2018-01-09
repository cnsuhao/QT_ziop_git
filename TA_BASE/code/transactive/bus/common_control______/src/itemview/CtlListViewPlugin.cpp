#include "CtlListView.h"
#include "CtlListViewPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlListViewPlugin::CtlListViewPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}

 
QString CtlListViewPlugin::name() const
{
	return "CtlListView";
}

QString CtlListViewPlugin::group() const
{
	return tr("Common Control");
}

QString CtlListViewPlugin::toolTip() const
{
	return tr("CtlListView");
}

QString CtlListViewPlugin::whatsThis() const
{
	return tr("CtlListView");
}

QString CtlListViewPlugin::includeFile() const
{
	return "bus/common_control/src/itemview/CtlListView.h";
}

QIcon CtlListViewPlugin::icon() const
{
	return QIcon();
}

bool CtlListViewPlugin::isContainer() const
{
	return false;
}

QWidget * CtlListViewPlugin::createWidget(QWidget *parent)
{
	return new CtlListView(parent);
}


 

QString CtlListViewPlugin::domXml() const
{ 
     return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlListView\" name=\"lst\">\
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