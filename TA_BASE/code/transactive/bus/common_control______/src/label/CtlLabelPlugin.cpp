#include "CtlLabel.h"
#include "CtlLabelPlugin.h"

#include <QtPlugin>
QT_BEGIN_NAMESPACE
CtlLabelPlugin::CtlLabelPlugin(QObject* parent) :
	QObject(parent),	m_initialized(false)
{
}
 

QString CtlLabelPlugin::name() const
{
	return "CtlLabel";
}

QString CtlLabelPlugin::group() const
{
	return tr("Common Control");
}

QString CtlLabelPlugin::toolTip() const
{
	return tr("CtlLabel");
}

QString CtlLabelPlugin::whatsThis() const
{
	return tr("CtlLabel");
}

QString CtlLabelPlugin::includeFile() const
{	 
	return "bus/common_control/src/label/CtlLabel.h";
}

QIcon CtlLabelPlugin::icon() const
{
	return QIcon();
}

bool CtlLabelPlugin::isContainer() const
{
	return false;
}

QWidget * CtlLabelPlugin::createWidget(QWidget *parent)
{
	return new CtlLabel(parent);
}

QString CtlLabelPlugin::domXml() const
{ 
	return QLatin1String("\
			<ui language=\"c++\">\
			<widget class=\"CtlLabel\" name=\"lab\">\
			<property name=\"geometry\">\
			<rect>\
			<x>0</x>\
			<y>0</y>\
			<width>100</width>\
			<height>30</height>\
			</rect>\
			</property>\
			<property name=\"text\">\
			<string>\lab</string>\
			</property>\
			</widget>\
			</ui>");;
}
//"  <property name=\"text\" >\n"
//"   <string>label</string>\n"
//"  </property>\n"
//#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
//Q_EXPORT_PLUGIN2(CtlLabelPlugin, CtlLabelPlugin)
//#endif
//<text>lab< / text>\
QT_END_NAMESPACE
