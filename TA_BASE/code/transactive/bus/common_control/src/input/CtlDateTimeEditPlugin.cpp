#include "CtlDateTimeEdit.h"
#include "CtlDateTimeEditPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlDateTimeEditPlugin::CtlDateTimeEditPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}


CtlDateTimeEditPlugin::~CtlDateTimeEditPlugin()
{
}


 
QString CtlDateTimeEditPlugin::name() const
{
	return "CtlDateTimeEdit";
}

QString CtlDateTimeEditPlugin::group() const
{
	return tr("Common Control");
}

QString CtlDateTimeEditPlugin::toolTip() const
{
	return tr("CtlDateTimeEdit");
}

QString CtlDateTimeEditPlugin::whatsThis() const
{
	return tr("CtlDateTimeEdit");
}

QString CtlDateTimeEditPlugin::includeFile() const
{
	return "bus/common_control/src/input/CtlDateTimeEdit.h";
}

QIcon CtlDateTimeEditPlugin::icon() const
{
	return QIcon();
}

bool CtlDateTimeEditPlugin::isContainer() const
{
	return false;
}

QWidget * CtlDateTimeEditPlugin::createWidget(QWidget *parent)
{
	return new CtlDateTimeEdit(parent);
}


 

QString CtlDateTimeEditPlugin::domXml() const
{ 
     return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlDateTimeEdit\" name=\"TimeEdit\">\
		<property name=\"geometry\">\
		<rect>\
		<x>0</x>\
		<y>0</y>\
		<width>42</width>\
		<height>22</height>\
		</rect>\
		</property>\
	    <property name=\"buttonSymbols\">\
		  <enum>QAbstractSpinBox::UpDownArrows</enum>\
		</property>\
		</widget>\
		</ui>");;
 
}

QT_END_NAMESPACE