#include "CtlDoubleSpinBox.h"
#include "CtlDoubleSpinBoxPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlDoubleSpinBoxPlugin::CtlDoubleSpinBoxPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}


CtlDoubleSpinBoxPlugin::~CtlDoubleSpinBoxPlugin()
{
}


 
QString CtlDoubleSpinBoxPlugin::name() const
{
	return "CtlDoubleSpinBox";
}

QString CtlDoubleSpinBoxPlugin::group() const
{
	return tr("Common Control");
}

QString CtlDoubleSpinBoxPlugin::toolTip() const
{
	return tr("CtlDoubleSpinBox");
}

QString CtlDoubleSpinBoxPlugin::whatsThis() const
{
	return tr("CtlDoubleSpinBox");
}

QString CtlDoubleSpinBoxPlugin::includeFile() const
{
	return "bus/common_control/src/input/CtlDoubleSpinBox.h";
}

QIcon CtlDoubleSpinBoxPlugin::icon() const
{
	return QIcon();
}

bool CtlDoubleSpinBoxPlugin::isContainer() const
{
	return false;
}

QWidget * CtlDoubleSpinBoxPlugin::createWidget(QWidget *parent)
{
	return new CtlDoubleSpinBox(parent);
}


 

QString CtlDoubleSpinBoxPlugin::domXml() const
{ 
     return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlDoubleSpinBox\" name=\"doubleSpinBox\">\
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