#include "CtlLineEdit.h"
#include "CtlLineEditPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlLineEditPlugin::CtlLineEditPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}


CtlLineEditPlugin::~CtlLineEditPlugin()
{
}


 
QString CtlLineEditPlugin::name() const
{
	return "CtlLineEdit";
}

QString CtlLineEditPlugin::group() const
{
	return tr("Common Control");
}

QString CtlLineEditPlugin::toolTip() const
{
	return tr("CtlLineEdit");
}

QString CtlLineEditPlugin::whatsThis() const
{
	return tr("CtlLineEdit");
}

QString CtlLineEditPlugin::includeFile() const
{
	return "bus/common_control/src/textedit/CtlLineEdit.h";
}

QIcon CtlLineEditPlugin::icon() const
{
	return QIcon();
}

bool CtlLineEditPlugin::isContainer() const
{
	return false;
}

QWidget * CtlLineEditPlugin::createWidget(QWidget *parent)
{
	return new CtlLineEdit(parent);
}


 

QString CtlLineEditPlugin::domXml() const
{ 
     return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlLineEdit\" name=\"txtLd\">\
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