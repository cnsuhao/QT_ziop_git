#include "CtlDateEdit.h"
#include "CtlDateEditPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlDateEditPlugin::CtlDateEditPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}


CtlDateEditPlugin::~CtlDateEditPlugin()
{
}


 
QString CtlDateEditPlugin::name() const
{
	return "CtlDateEdit";
}

QString CtlDateEditPlugin::group() const
{
	return tr("Common Control");
}

QString CtlDateEditPlugin::toolTip() const
{
	return tr("CtlDateEdit");
}

QString CtlDateEditPlugin::whatsThis() const
{
	return tr("CtlDateEdit");
}

QString CtlDateEditPlugin::includeFile() const
{
	return "bus/common_control/src/input/CtlDateEdit.h";
}

QIcon CtlDateEditPlugin::icon() const
{
	return QIcon();
}

bool CtlDateEditPlugin::isContainer() const
{
	return false;
}

QWidget * CtlDateEditPlugin::createWidget(QWidget *parent)
{
	return new CtlDateEdit(parent);
}


 

QString CtlDateEditPlugin::domXml() const
{ 
     return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlDateEdit\" name=\"TimeEdit\">\
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