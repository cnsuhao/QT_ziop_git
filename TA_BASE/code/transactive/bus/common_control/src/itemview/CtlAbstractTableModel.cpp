#include "CtlAbstractTableModel.h"
QT_BEGIN_NAMESPACE

CtlAbstractTableModel::CtlAbstractTableModel(QObject *parent)
	 :QAbstractTableModel(parent)
{

}

CtlAbstractTableModel::~CtlAbstractTableModel()
{

}


QVariant CtlAbstractTableModel::data(const QModelIndex &index, int role ) const
{
	if (!index.isValid() ||	index.row() < 0  || index.column() < 0 ) return QVariant();
	
	if (role == Qt::DisplayRole || role == Qt::EditRole) 
	{
		 return  data_Display(index.row(),index.column());
	}
	else if (role==Qt::BackgroundRole || role==Qt::ForegroundRole )
	{
		return  data_Colour(index.row(),(Qt::ItemDataRole)role);
	}
	else if (role==Qt::ToolTipRole )
	{
		return  data_Tooltip(index.row());
	}
	return QVariant();

}

QVariant CtlAbstractTableModel::data_Colour(int row,Qt::ItemDataRole colorRule) const
{
	return QVariant();
}

QVariant CtlAbstractTableModel::data_Tooltip(int row) const
{
	return QVariant();
}

QVariant CtlAbstractTableModel::data_Display(int row, int col) const
{
	return QVariant();
}


QT_END_NAMESPACE