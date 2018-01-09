

#ifndef __ALARMLIST_DELEGATE_H_INCLUDED__
#define __ALARMLIST_DELEGATE_H_INCLUDED__

#include <QStyledItemDelegate>
#include <QApplication>

#include "bus/alarm/alarm_list_control/pmodel/src/AlarmListModel.h"

class AlarmListDelegate : public QStyledItemDelegate
{

public:
	AlarmListDelegate(TA_Base_Bus::AlarmListModel* pModel, QObject* parent) 
		: QStyledItemDelegate(parent)
		, m_pModel(pModel)
	{}

	void paint(QPainter* painter, const QStyleOptionViewItem& option,
		const QModelIndex& index) const
	{
		QStyleOptionViewItem itemOption(option);
		initStyleOption(&itemOption, index);

		if (itemOption.state & QStyle::State_Selected) // Remove selection
			itemOption.state = itemOption.state & ~QStyle::State_Selected;

		QApplication::style()->drawControl(QStyle::CE_ItemViewItem, &itemOption, painter, nullptr);

		
		// Draw a rectangle for selected items
		std::string alarmID = m_pModel->getDisplayDataWrap()->getItemString(itemOption.index.row(), ALARMID);
		if (m_pModel->getAlarmDataSync()->isAlarmItemSelected(alarmID))
		{
			//itemOption.state = itemOption.state & QStyle::State_Selected;
			//itemOption.palette.setColor(QPalette::Highlight, Qt::cyan);  // set your color here'
			
			QRect itemRect = itemOption.rect;
			itemRect.setTop(itemOption.rect.top() + 2);
			itemRect.setBottom(itemOption.rect.bottom() - 1);
			QVector<QLine> lines;
			QLine lineTop(itemRect.topLeft(), itemRect.topRight());
			QLine lineButtom(itemRect.bottomLeft(), itemRect.bottomRight());

			lines.push_back(lineTop);
			lines.push_back(lineButtom);
			if (itemOption.index.column() == 0)
			{
				itemRect.setLeft(itemOption.rect.left() + 1);
				
				QLine lineLeft(itemRect.topLeft(), itemRect.bottomLeft());
				lines.push_back(lineLeft);
			}
			else if (itemOption.index.column() == 9)
			{
				//itemRect.setRight(itemOption.rect.right() - 1);

				QLine lineRight(itemRect.topRight(), itemRect.bottomRight());
				lines.push_back(lineRight);
			}
			
			QPen pen(itemOption.palette.highlight().color(), 2, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
			painter->setPen(pen);
			painter->drawLines(lines);
			//painter->drawRect(itemOption.rect);
		}
	}
private:
	TA_Base_Bus::AlarmListModel* m_pModel;
};


#endif //__ALARMLIST_DELEGATE_H_INCLUDED__