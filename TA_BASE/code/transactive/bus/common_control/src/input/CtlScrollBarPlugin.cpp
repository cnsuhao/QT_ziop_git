#include "CtlScrollBar.h"
#include "CtlScrollBarPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlScrollBarPlugin::CtlScrollBarPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}


CtlScrollBarPlugin::~CtlScrollBarPlugin()
{
}


 
QString CtlScrollBarPlugin::name() const
{
	return "CtlScrollBar";
}

QString CtlScrollBarPlugin::group() const
{
	return tr("Common Control");
}

QString CtlScrollBarPlugin::toolTip() const
{
	return tr("CtlScrollBar");
}

QString CtlScrollBarPlugin::whatsThis() const
{
	return tr("CtlScrollBar");
}

QString CtlScrollBarPlugin::includeFile() const
{
	return "bus/common_control/src/input/CtlScrollBar.h";
}

QIcon CtlScrollBarPlugin::icon() const
{
	return QIcon();
}

bool CtlScrollBarPlugin::isContainer() const
{
	return false;
}

QWidget * CtlScrollBarPlugin::createWidget(QWidget *parent)
{
	return new CtlScrollBar(parent);
}


 

QString CtlScrollBarPlugin::domXml() const
{ 
     return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlScrollBar\" name=\"ScrollBar\">\
		<property name=\"geometry\">\
		<rect>\
		<x>0</x>\
		<y>0</y>\
		<width>252</width>\
		<height>22</height>\
		</rect>\
		</property>\
		<property name=\"orientation\">\
		  <enum>Qt::Horizontal</enum>\
		</property>\
		</widget>\
		</ui>");
}

QT_END_NAMESPACE