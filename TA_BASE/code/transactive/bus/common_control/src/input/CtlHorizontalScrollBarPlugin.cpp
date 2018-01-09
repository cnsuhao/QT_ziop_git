#include "CtlScrollBar.h"
#include "CtlHorizontalScrollBarPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlHorizontalScrollBarPlugin::CtlHorizontalScrollBarPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}


CtlHorizontalScrollBarPlugin::~CtlHorizontalScrollBarPlugin()
{
}


 
QString CtlHorizontalScrollBarPlugin::name() const
{
	return "CtlHorizontalScrollBar";
}

QString CtlHorizontalScrollBarPlugin::group() const
{
	return tr("Common Control");
}

QString CtlHorizontalScrollBarPlugin::toolTip() const
{
	return tr("CtlHorizontalScrollBar");
}

QString CtlHorizontalScrollBarPlugin::whatsThis() const
{
	return tr("CtlHorizontalScrollBar");
}

QString CtlHorizontalScrollBarPlugin::includeFile() const
{
	return "bus/common_control/src/input/CtlScrollBar.h";
}

QIcon CtlHorizontalScrollBarPlugin::icon() const
{
	return QIcon();
}

bool CtlHorizontalScrollBarPlugin::isContainer() const
{
	return false;
}

QWidget * CtlHorizontalScrollBarPlugin::createWidget(QWidget *parent)
{

	CtlScrollBar* ctl = new  CtlScrollBar(parent);
	//ctl->setOrientation();
	return ctl;
}


 

QString CtlHorizontalScrollBarPlugin::domXml() const
{ 
    /* return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlScrollBar\" name=\"horizontalScrollBar\">\
		<property name=\"geometry\">\
		<rect>\
		<x>0</x>\
		<y>0</y>\
		<width>42</width>\
		<height>22</height>\
		</rect>\
		</property>\
		<property name=\"orientation\">\
		  <enum>Qt::Horizontal</enum>\
		</property>\
		</widget>\
		</ui>");*/

	/*" <property name=\"orientation\" >\n"
		" <enum>Qt::Horizontal</enum>\n"
		" </property>\n"*/


	return  "<ui language=\"c++\">\n"
		" <widget class=\"CtlScrollBar\" name=\"horizontalScrollBar\">\n"
		"  <property name=\"geometry\">\n"
		"   <rect>\n"
		"    <x>0</x>\n"
		"    <y>0</y>\n"	
		"    <width>251</width>\n"
	    "    <height>21</height>\n"
		"   </rect>\n"
		"  </property>\n"			
		" <property name=\"orientation\" >\n"
		" <enum>Qt::Horizontal</enum>\n"
		" </property>\n"
		" </widget>\n"
		"</ui>\n";
}

QT_END_NAMESPACE