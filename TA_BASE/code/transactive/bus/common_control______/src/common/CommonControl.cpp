#include "bus/common_control/src/button/CtlButtonPlugin.h"
#include "bus/common_control/src/button/CtlRadioButtonPlugin.h"
#include "bus/common_control/src/button/CtlCheckBoxPlugin.h"


#include "bus/common_control/src/label/CtlLabelPlugin.h"

#include "bus/common_control/src/textedit/CtlPlainTextEditPlugin.h"
#include "bus/common_control/src/textedit/CtlLineEditPlugin.h"
#include "bus/common_control/src/textedit/CtlTextEditPlugin.h"


#include "bus/common_control/src/container/CtlGroupBoxPlugin.h"
#include "bus/common_control/src/container/CtlTabWidgetPlugin.h"


#include "bus/common_control/src/itemview/CtlTableViewPlugin.h"
#include "bus/common_control/src/itemview/CtlListViewPlugin.h"
#include "bus/common_control/src/itemview/CtlTableWidgetPlugin.h"
#include "bus/common_control/src/itemview/CtlListWidgetPlugin.h"

#include "bus/common_control/src/itemview/CtlTreeWidget.h"
#include "bus/common_control/src/itemview/CtlTreeWidgetPlugin.h"


#include "bus/common_control/src/input/CtlComboBoxPlugin.h"
#include "bus/common_control/src/input/CtlSpinBoxPlugin.h"
#include "bus/common_control/src/input/CtlDoubleSpinBoxPlugin.h"
#include "bus/common_control/src/input/CtlTimeEditPlugin.h"
#include "bus/common_control/src/input/CtlDateEditPlugin.h"
#include "bus/common_control/src/input/CtlDateTimeEditPlugin.h"
#include "bus/common_control/src/input/CtlHorizontalScrollBarPlugin.h"
#include "bus/common_control/src/input/CtlScrollBarPlugin.h"
#include "bus/common_control/src/input/CtlVerticalScrollBarPlugin.h"


#include <QtDesigner/QDesignerCustomWidgetCollectionInterface>
#include <QtCore/qplugin.h>
#include <QtCore/qdebug.h>
#include <QtGui/QIcon>
#include "CommonControl.h"
 
QT_BEGIN_NAMESPACE

CommonControl::CommonControl(QObject *parent)
    : QObject(parent)
{


	
    //add customized widget to plugin collection

	//m_plugins.append(new CtlButtonPlugin(this));
	//m_plugins.append(new CtlListWidgetPlugin(this));
	 
    m_plugins.append(new CtlButtonPlugin(this));
	m_plugins.append(new CtlRadioButtonPlugin(this));
	m_plugins.append(new CtlCheckBoxPlugin(this));

    m_plugins.append(new CtlLabelPlugin(this));

	m_plugins.append(new CtlLineEditPlugin(this));
	m_plugins.append(new CtlTextEditPlugin(this));
	m_plugins.append(new CtlPlainTextEditPlugin(this));

	m_plugins.append(new CtlGroupBoxPlugin(this));
	m_plugins.append(new CtlTabWidgetPlugin(this));

	m_plugins.append(new CtlListWidgetPlugin(this));
	m_plugins.append(new CtlTableViewPlugin(this));
	m_plugins.append(new CtlTableWidgetPlugin(this));	
	m_plugins.append(new CtlListViewPlugin(this));	
	m_plugins.append(new CtlTreeWidgetPlugin(this));

	m_plugins.append(new CtlScrollBarPlugin(this));

	m_plugins.append(new CtlComboBoxPlugin(this));
	m_plugins.append(new CtlSpinBoxPlugin(this));
	m_plugins.append(new CtlDoubleSpinBoxPlugin(this));
	m_plugins.append(new CtlTimeEditPlugin(this));
	m_plugins.append(new CtlDateEditPlugin(this));
	m_plugins.append(new CtlDateTimeEditPlugin(this));
	m_plugins.append(new CtlHorizontalScrollBarPlugin(this));
	
	m_plugins.append(new CtlVerticalScrollBarPlugin(this));
}

QList<QDesignerCustomWidgetInterface*> CommonControl::customWidgets() const
{
    return m_plugins;
}


//Export custom widget plugin to Qt Designer by below macro.
#if QT_VERSION < QT_VERSION_CHECK(5,0,0)
Q_EXPORT_PLUGIN2(commoncontrol, CommonControl)
#endif

QT_END_NAMESPACE