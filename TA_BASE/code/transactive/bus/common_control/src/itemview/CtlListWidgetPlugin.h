/**
* The source code in this file is the property of 
* ST Engineering Electronics (LSG) and is not for redistribution
* in any form.
*
* Source:   $File:   $
* @author:  Luo Huirong
* @version: $Revision: #1 $
*
* Last modification: $DateTime: 2015/11/20 16:15:14 $
* Last modified by:  $Author: Luo Huirong $
* 
* The class enables Qt Designer to access and construct custom widgets.
*  
*
*/

#ifndef _CtlListWidgetPlugin_H_
#define _CtlListWidgetPlugin_H_

#include <QDesignerCustomWidgetInterface>
QT_BEGIN_NAMESPACE
class CtlListWidgetPlugin : public QObject, public QDesignerCustomWidgetInterface
{
	Q_OBJECT
	Q_INTERFACES(QDesignerCustomWidgetInterface)


public:
	explicit CtlListWidgetPlugin(QObject* parent = 0);

	bool isInitialized() const;
	void initialize(QDesignerFormEditorInterface *core) Q_DECL_OVERRIDE;

	///Returns the class name of the custom widget supplied by the interface.
	///The name returned must be identical to the class name used for the custom widget.
	virtual QString name() const;

	///Returns the name of the group to which the custom widget belongs.
	virtual QString group() const;

	virtual QString toolTip() const;

	virtual  QString whatsThis() const;

	///Returns the path to the include file that uic uses when creating code for the custom widget.
	virtual QString includeFile() const;

	///Returns the icon used to represent the custom widget in Qt Designer's widget box.
	virtual QIcon icon() const;

	///Returns the XML that is used to describe the custom widget's properties to Qt Designer.
	QString domXml() const;


	///Returns true if the custom widget is intended to be used as a container; otherwise returns false.
	///Most custom widgets are not used to hold other widgets, so their implementations of this function
	///will return false, but custom containers will return true to ensure that they behave correctly in Qt Designer.
	bool isContainer() const;

	///Returns a new instance of the custom widget, with the given parent.
	QWidget *createWidget(QWidget *parent);

private:
    bool m_initialized;
};
QT_END_NAMESPACE

#if QT_VERSION < 0x050000
Q_EXPORT_PLUGIN2(CtlListWidgetPlugin, CtlListWidgetPlugin)
#endif // QT_VERSION < 0x050000


#endif
