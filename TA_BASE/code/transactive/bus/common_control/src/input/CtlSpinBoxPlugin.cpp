#include "CtlSpinBox.h"
#include "CtlSpinBoxPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlSpinBoxPlugin::CtlSpinBoxPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}


CtlSpinBoxPlugin::~CtlSpinBoxPlugin()
{
}


 
QString CtlSpinBoxPlugin::name() const
{
	return "CtlSpinBox";
}

QString CtlSpinBoxPlugin::group() const
{
	return tr("Common Control");
}

QString CtlSpinBoxPlugin::toolTip() const
{
	return tr("CtlSpinBox");
}

QString CtlSpinBoxPlugin::whatsThis() const
{
	return tr("CtlSpinBox");
}

QString CtlSpinBoxPlugin::includeFile() const
{
	return "bus/common_control/src/input/CtlSpinBox.h";
}

QIcon CtlSpinBoxPlugin::icon() const
{
	return QIcon();
}

bool CtlSpinBoxPlugin::isContainer() const
{
	return false;
}

QWidget * CtlSpinBoxPlugin::createWidget(QWidget *parent)
{
	return new CtlSpinBox(parent);
}


 

QString CtlSpinBoxPlugin::domXml() const
{ 
     return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlSpinBox\" name=\"spinBox\">\
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