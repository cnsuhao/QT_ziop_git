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
* The class implement customized QAbstractTableModel.
* This model provides simple virtual callback method for displaying colour and tooltip item, like alarm & event list
*
*
*/


#ifndef CTLABSTRACTTABLEMODEL_H
#define CTLABSTRACTTABLEMODEL_H


#include <QtDesigner/QtDesigner> 
#include <QAbstractTableModel>
QT_BEGIN_NAMESPACE
 

class QDESIGNER_WIDGET_EXPORT CtlAbstractTableModel : public QAbstractTableModel
{ 
	Q_OBJECT		
public:
	explicit CtlAbstractTableModel(QObject *parent);
	virtual ~CtlAbstractTableModel();


	///get dipslay text based on row and col
	///Its subclass should override it.
	virtual QVariant data_Display(int row, int col)  const ;

	///get background color or foregrouand colour  based on row
	///Its subclass should override it.
	virtual QVariant data_Colour(int row,Qt::ItemDataRole colorRule) const ;

	///get tooltip  based on row
	///Its subclass should override it.
	virtual QVariant data_Tooltip(int row) const ;
    
private:
    virtual QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
   
	
};
QT_END_NAMESPACE
#endif // CTLABSTRACTTABLEMODEL_H
