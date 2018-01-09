#include "CtlPlainTextEdit.h"
#include "CtlPlainTextEditPlugin.h"

#include <QtPlugin>
#include <QtDesigner/QExtensionFactory>
#include <QtDesigner/QExtensionManager>
#include <QtCore/qplugin.h>

#include <QDesignerFormEditorInterface>
#include <extrainfo.h>
 
QT_BEGIN_NAMESPACE
CtlPlainTextEditPlugin::CtlPlainTextEditPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}
CtlPlainTextEditPlugin::~CtlPlainTextEditPlugin()
{
}


 
QString CtlPlainTextEditPlugin::name() const
{
	return "CtlPlainTextEdit";
}

QString CtlPlainTextEditPlugin::group() const
{
	return tr("Common Control");
}

QString CtlPlainTextEditPlugin::toolTip() const
{
	return tr("CtlPlainTextEdit");
}

QString CtlPlainTextEditPlugin::whatsThis() const
{
	return tr("CtlPlainTextEdit");
}

QString CtlPlainTextEditPlugin::includeFile() const
{
	return "bus/common_control/src/textedit/CtlPlainTextEdit.h";
}

QIcon CtlPlainTextEditPlugin::icon() const
{
	return QIcon();
}

bool CtlPlainTextEditPlugin::isContainer() const
{
	return false;
}

QWidget * CtlPlainTextEditPlugin::createWidget(QWidget *parent)
{
	return new CtlPlainTextEdit(parent);
}


 

QString CtlPlainTextEditPlugin::domXml() const
{ 
     return QLatin1String("\
		<ui language=\"c++\">\
		<widget class=\"CtlPlainTextEdit\" name=\"txtPd\">\
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