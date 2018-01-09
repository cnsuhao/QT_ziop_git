#ifndef ALARMLISTMODEL_H
#define ALARMLISTMODEL_H
#define WIN32_LEAN_AND_MEAN

#include <QObject>
#include <QTimer>
#include <QTableView>
#include "bus/common_control/src/itemview/CtlAbstractTableModel.h"
#include "bus/alarm/alarm_list_control/src/IAlarmColumn.h"
#include "bus/alarm/alarm_list_control/pmodel/src/AlarmListModel.h"
#include "core/alarm/IDL/src/AlarmStoreCorbaDef.h"

 

namespace  TA_Base_Bus
{  
	class AlarmListTableModel : public CtlAbstractTableModel
	{
		Q_OBJECT
	public:
		AlarmListTableModel(QObject *parent);
		virtual ~AlarmListTableModel();
		virtual QVariant data_Display(int row, int col) const;
		virtual QVariant data_Colour(int row,Qt::ItemDataRole colorRule) const;
		virtual QVariant data_Tooltip(int row) const;
        QVariant headerData(int section, Qt::Orientation orientation, int role) const;
		int rowCount(const QModelIndex &parent = QModelIndex()) const ;
		int columnCount(const QModelIndex &parent = QModelIndex()) const;
	public slots:
		void OnTimer();
		
	signals:
		void updateAlarmObservers(bool wereAlarmsModified, bool wereAlarmsAdded,
			TA_Base_Core::IAlarmStoreCorbaDef::IndexInfo& details,
			unsigned long selectedAlarmCount);

	public:
	    void setColumns(const std::vector<IAlarmColumn*> &columns);
		/**
		* setNewDataFlag
		*
		* alarm store received update/new data, and need update UI on next refresh cycle immediately
		*	
		*/		
		void setNewDataFlag();
		/**
		* enableColumn
		*
		* This will display or hide the specified column. It can be called before or after startAlarmList().
		*
		* @param long - The column to show or hide
		* @param bool - True to view the column, false to hide it
		* @param int - The position the column should be inserted in in the list control. This only needs to be
		*              set if the column is being enabled.
		*/
		void enableColumn(long column, bool isEnabled, unsigned int position);
		void sortColumn(long column, bool ascending);
        void setPModel(AlarmListModel* alarmListModel);
		void setTableView(QTableView* tableView);	

		unsigned long getEarliestAlarmTime();
		unsigned long getLatestAlarmTime();
		 	
		/*void initData(const Filter& preFilter,const Filter& filter,const std::vector<unsigned long>& locations,
			const std::string& sessionId,long sortColumn,bool isAscendingSort,bool isHighestSeverityNeeded);*/
		//void changeToDefaultSort();

		void resetDataTable();

		// Methods for Select/UnSelect alarms
		// This function is used to check if alarm has been selected
		bool isAlarmItemSelected(std::string strAlarmID);

		// This function is used add alarms to the selected items
		void selectAlarmItem(unsigned long ulIndex);

		void selectRangeAlarms(unsigned long ulSelBegin, unsigned long ulSelEnd);

		// This function is used to unselect a particular alarm item.
		void unselectAlarmItem(unsigned long ulIndex);

		// Function to unselect all alarm items currently selected
		void unSelectAllAlarmItem();

		// Retrieve the number of selected alarms
		unsigned long getSelectedAlarmsCount();

		std::string getAlarmIdAt(unsigned long ulIndex);

		// Use to locate the current position of an alarm item. This is used in the new selection
		// mechanism to easily update the current position of the selected item.
		unsigned long findItemPos(const std::string& strAlarmID);

		void setListDisplayInfo(unsigned long ulIdxFrom, unsigned long ulIdxTo);

		bool isDeterminateColumn(const long columnID, const TAUtfString& strColumnCaption);
	private:
		
		/**
		* determineDrawingColours
		*
		* Given an alarm this method determines the foreground and background drawing colours. This is based
		* on the state of the alarm, whether it is selected or not and also whether the list is flashing or not.
		*
		* @param long - The index into the alarm list for the alarm.
		* @param unsigned long& - This is an out parameter and will be set with the value for  foreground
		* @param unsigned long& - This is an out parameter and will be set with the value for  background
		*/
		unsigned long determineDrawingColours(unsigned long  ulIndex, unsigned long& foregroundColour, unsigned long& backgroundColour) const;
		
		/**
		* getItemText
		*
		* get display item data;
		*
		* @param int, row number
		* @param int, column number
		*/
		TAUtfString getItemText(int row, int col) const;
		
		void setListDisplayInfo();

	    static const unsigned long BACKGROUND;
		// This is the default colour that will be used if no appropriate colour can be determined
		static const unsigned long DEFAULT_COLOUR;

		static const std::string NO_VALUE;
	private:
		// This contains the information for all columns in the list control. The map is keyed on the
		// column's unique identifiers.
		std::map<long, IAlarmColumn*>* m_alarmColumns;
		int m_descColIndex;
		// This vector stores the order of the columns. Each entry in the vector is an index into the
		// alarm column map (m_alarmColumns).
		std::vector<long> m_alarmColumnOrder;
		AlarmVectorWrap * m_pDisplayVector;
		AlarmDataSync*    m_alarmDataSync;
		AlarmListModel* m_alarmListModel;
	    QTableView*  m_tableView;

		// The timer  for updates and flashing 
		QTimer m_timer;

		// The sleep times for the timers.
		static const int UPDATE_SLEEP_IN_MILLISECONDS;

		// This indicates whether the alarm should draw in the bright colour or light colour when it
		// is redrawn. This is what simulates the flash.
		bool m_isFlashTime;
        int m_nFlashCount;
		//Keep index information
		TA_Base_Core::IAlarmStoreCorbaDef::IndexInfo m_info;

		//whether 
		bool m_needUpdateData;

		ULONG m_ulIdxFrom;
		ULONG m_ulIdxTo;
		int m_dataSize;
	};
}

#endif // ALARMLISTMODEL_H
