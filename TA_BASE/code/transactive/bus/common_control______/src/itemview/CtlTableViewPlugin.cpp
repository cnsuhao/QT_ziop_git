#include "CtlTableView.h"
#include "CtlTableViewPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlTableViewPlugin::CtlTableViewPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}

 
QString CtlTableViewPlugin::name() const
{
	return "CtlTableView";
}

QString CtlTableViewPlugin::group() const
{
	return tr("Common Control");
}

QString CtlTableViewPlugin::toolTip() const
{
	return tr("CtlTableView");
}

QString CtlTableViewPlugin::whatsThis() const
{
	return tr("CtlTableView");
}

QString CtlTableViewPlugin::includeFile() const
{
	return "bus/common_control/src/itemview/CtlTableView.h";
}

QIcon CtlTableViewPlugin::icon() const
{
	return QIcon();
}

bool CtlTableViewPlugin::isContainer() const
{
	return false;
}

QWidget * CtlTableViewPlugin::createWidget(QWidget *parent)
{
	return new CtlTableView(parent);
}


 

QString CtlTableViewPlugin::domXml() const
{ 
     return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlTableView\" name=\"lst\">\
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