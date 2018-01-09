#include "CtlScrollBar.h"
#include "CtlVerticalScrollBarPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlVerticalScrollBarPlugin::CtlVerticalScrollBarPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}


CtlVerticalScrollBarPlugin::~CtlVerticalScrollBarPlugin()
{
}


 
QString CtlVerticalScrollBarPlugin::name() const
{
	return "CtlVerticalScrollBar";
}

QString CtlVerticalScrollBarPlugin::group() const
{
	return tr("Common Control");
}

QString CtlVerticalScrollBarPlugin::toolTip() const
{
	return tr("CtlVerticalScrollBar");
}

QString CtlVerticalScrollBarPlugin::whatsThis() const
{
	return tr("CtlVerticalScrollBar");
}

QString CtlVerticalScrollBarPlugin::includeFile() const
{
	return "bus/common_control/src/input/CtlScrollBar.h";
}

QIcon CtlVerticalScrollBarPlugin::icon() const
{
	return QIcon();
}

bool CtlVerticalScrollBarPlugin::isContainer() const
{
	return false;
}

QWidget * CtlVerticalScrollBarPlugin::createWidget(QWidget *parent)
{

	CtlScrollBar* ctl = new  CtlScrollBar(parent);
	//ctl->setOrientation();
	return ctl;
}


 

QString CtlVerticalScrollBarPlugin::domXml() const
{ 
    
	return  "<ui language=\"c++\">\n"
		" <widget class=\"CtlScrollBar\" name=\"verticalScrollBar\">\n"
		"  <property name=\"geometry\">\n"
		"   <rect>\n"
		"    <x>0</x>\n"
		"    <y>0</y>\n"	
		"    <width>21</width>\n"
	    "    <height>301</height>\n"
		"   </rect>\n"
		"  </property>\n"			
		" <property name=\"orientation\" >\n"
		" <enum>Qt::Vertical</enum>\n"
		" </property>\n"
		" </widget>\n"
		"</ui>\n";
}

QT_END_NAMESPACE