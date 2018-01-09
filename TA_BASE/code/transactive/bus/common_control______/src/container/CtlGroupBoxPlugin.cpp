#include "CtlGroupBox.h"
#include "CtlGroupBoxPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlGroupBoxPlugin::CtlGroupBoxPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}

 
QString CtlGroupBoxPlugin::name() const
{
	return "CtlGroupBox";
}

QString CtlGroupBoxPlugin::group() const
{
	return tr("Common Control");
}

QString CtlGroupBoxPlugin::toolTip() const
{
	return tr("CtlGroupBox");
}

QString CtlGroupBoxPlugin::whatsThis() const
{
	return tr("CtlGroupBox");
}

QString CtlGroupBoxPlugin::includeFile() const
{
	return "bus/common_control/src/container/CtlGroupBox.h";
}

QIcon CtlGroupBoxPlugin::icon() const
{
	return QIcon();
}

bool CtlGroupBoxPlugin::isContainer() const
{
	return true;
}

QWidget * CtlGroupBoxPlugin::createWidget(QWidget *parent)
{
	return new CtlGroupBox(parent);
}


 

QString CtlGroupBoxPlugin::domXml() const
{ 
     return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlGroupBox\" name=\"grp\">\
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