#include "CtlTextEdit.h"
#include "CtlTextEditPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlTextEditPlugin::CtlTextEditPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}

CtlTextEditPlugin::~CtlTextEditPlugin()
{
}

 
QString CtlTextEditPlugin::name() const
{
	return "CtlTextEdit";
}

QString CtlTextEditPlugin::group() const
{
	return tr("Common Control");
}

QString CtlTextEditPlugin::toolTip() const
{
	return tr("CtlTextEdit");
}

QString CtlTextEditPlugin::whatsThis() const
{
	return tr("CtlTextEdit");
}

QString CtlTextEditPlugin::includeFile() const
{
	return "bus/common_control/src/textedit/CtlTextEdit.h";
}

QIcon CtlTextEditPlugin::icon() const
{
	return QIcon();
}

bool CtlTextEditPlugin::isContainer() const
{
	return false;
}

QWidget * CtlTextEditPlugin::createWidget(QWidget *parent)
{
	return new CtlTextEdit(parent);
}


 

QString CtlTextEditPlugin::domXml() const
{ 
     return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlTextEdit\" name=\"txtMd\">\
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