#include "CtlComboBox.h"
#include "CtlComboBoxPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlComboBoxPlugin::CtlComboBoxPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}


CtlComboBoxPlugin::~CtlComboBoxPlugin()
{
}


 
QString CtlComboBoxPlugin::name() const
{
	return "CtlComboBox";
}

QString CtlComboBoxPlugin::group() const
{
	return tr("Common Control");
}

QString CtlComboBoxPlugin::toolTip() const
{
	return tr("CtlComboBox");
}

QString CtlComboBoxPlugin::whatsThis() const
{
	return tr("CtlComboBox");
}

QString CtlComboBoxPlugin::includeFile() const
{
	return "bus/common_control/src/input/CtlComboBox.h";
}

QIcon CtlComboBoxPlugin::icon() const
{
	return QIcon();
}

bool CtlComboBoxPlugin::isContainer() const
{
	return false;
}

QWidget * CtlComboBoxPlugin::createWidget(QWidget *parent)
{
	return new CtlComboBox(parent);
}


 

QString CtlComboBoxPlugin::domXml() const
{ 
     return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlComboBox\" name=\"cbold\">\
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