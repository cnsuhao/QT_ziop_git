#include "CtlTimeEdit.h"
#include "CtlTimeEditPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlTimeEditPlugin::CtlTimeEditPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}


CtlTimeEditPlugin::~CtlTimeEditPlugin()
{
}


 
QString CtlTimeEditPlugin::name() const
{
	return "CtlTimeEdit";
}

QString CtlTimeEditPlugin::group() const
{
	return tr("Common Control");
}

QString CtlTimeEditPlugin::toolTip() const
{
	return tr("CtlTimeEdit");
}

QString CtlTimeEditPlugin::whatsThis() const
{
	return tr("CtlTimeEdit");
}

QString CtlTimeEditPlugin::includeFile() const
{
	return "bus/common_control/src/input/CtlTimeEdit.h";
}

QIcon CtlTimeEditPlugin::icon() const
{
	return QIcon();
}

bool CtlTimeEditPlugin::isContainer() const
{
	return false;
}

QWidget * CtlTimeEditPlugin::createWidget(QWidget *parent)
{
	return new CtlTimeEdit(parent);
}


 

QString CtlTimeEditPlugin::domXml() const
{ 
     return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlTimeEdit\" name=\"TimeEdit\">\
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