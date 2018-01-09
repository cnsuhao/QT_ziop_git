/**
* The source code in this file is the property of
* ST Engineering Electronics (LSG) and is not for redistribution
* in any form.
*
* Source:   $File:   $
* @author:  Hoa Le
* @version: $Revision: #1 $
*
* Last modification: $DateTime: 2017/05/01 16:15:14 $
* Last modified by:  $Author: Hoa Le $
*
* The class implement customised rendering of items in the QTableWidget.
*/

#ifndef TABLEDELEGATE_H
#define TABLEDELEGATE_H

#include <QItemDelegate>

class TableDelegate : public QItemDelegate
{
public:
	TableDelegate(QObject * parent = NULL) :
		QItemDelegate(parent)
	{
	}

protected:
	virtual void drawFocus(QPainter *, const QStyleOptionViewItem
		&, const QRect &) const
	{
	};
private:
};

#endif