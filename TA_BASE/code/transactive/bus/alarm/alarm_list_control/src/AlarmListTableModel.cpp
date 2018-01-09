

#include "AlarmListTableModel.h"
#include "bus\alarm\alarm_list_control\src\columns\ColumnIdentifiers.h"
#include <QColor>
#include <QTableView>
#include "core\exceptions\src\AlarmListCtrlException.h"
#include "core/utilities/src/DebugUtil.h"

using TA_Base_Core::DebugUtil;
using TA_Base_Core::RunParams;
 
using TA_Base_Core::ThreadGuard;

namespace  TA_Base_Bus
{ 
	const unsigned long AlarmListTableModel::BACKGROUND = 255*255*255;
	const unsigned long AlarmListTableModel::DEFAULT_COLOUR = 0;
	const std::string AlarmListTableModel::NO_VALUE="";
	const int AlarmListTableModel::UPDATE_SLEEP_IN_MILLISECONDS = 400; // This is also the flash time 1500
	AlarmListTableModel::AlarmListTableModel(QObject *parent)
		: CtlAbstractTableModel(parent), 
		m_alarmListModel(NULL),
		m_pDisplayVector(NULL), 
		m_isFlashTime(false),
		m_nFlashCount(0),
		m_ulIdxFrom(0),
		m_ulIdxTo(0),
		m_dataSize(0)
	{
	      m_alarmColumns=new std::map<long, IAlarmColumn*>();
	}

	AlarmListTableModel::~AlarmListTableModel()
	{
	     delete m_alarmColumns;
	}


	void AlarmListTableModel::setTableView(QTableView* tableView)
	{
	    m_tableView=tableView;
	}

	void AlarmListTableModel::setPModel(AlarmListModel* alarmListModel)
	{
		 m_alarmListModel=alarmListModel;
		 m_pDisplayVector=m_alarmListModel->getDisplayDataWrap();
		 m_alarmDataSync=m_alarmListModel->getAlarmDataSync();

		 // Set up a timer so we know when to update the list and make it flash 
		 connect(&m_timer, SIGNAL(timeout()), this, SLOT(OnTimer()));
		 m_timer.start(UPDATE_SLEEP_IN_MILLISECONDS);
	}

	void AlarmListTableModel::setListDisplayInfo()
	{
		ListDispInfo listInfo;
		//get the first dispalyed row;		
		listInfo.ulTopIndex = m_tableView->verticalScrollBar()->value(); //m_tableView->indexAt(m_tableView->rect().topLeft()).row();
		int nCountPerPage = 50; // m_tableView->indexAt(m_tableView->rect().bottomRight()).row() - listInfo.ulTopIndex + 1;

		listInfo.ulDisplayCount = nCountPerPage;
		listInfo.ulNextPageBuffer = nCountPerPage;
		listInfo.ulPrevPageBuffer = nCountPerPage;
		LOG2(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "AlarmListTableModel::setListDisplayInfo: ulTopIndex: %d count per page: %d", listInfo.ulTopIndex, nCountPerPage);
		this->m_alarmListModel->setListDisplayInfo(listInfo);

	}

	void AlarmListTableModel::setListDisplayInfo(unsigned long ulIdxFrom, unsigned long ulIdxTo)
	{
		if (ulIdxFrom != m_ulIdxFrom || ulIdxTo != m_ulIdxTo)
		{
			m_ulIdxFrom = ulIdxFrom;
			m_ulIdxTo = ulIdxTo;

			setListDisplayInfo();

			LOG2(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "setListDisplayInfo: From = %ld -> To = %ld.", m_ulIdxFrom, m_ulIdxTo);
		}
	}

	unsigned long AlarmListTableModel::getEarliestAlarmTime()
	{         
		try
		{
			return m_info.earliestSourceTime.time;
		}
		catch( const TA_Base_Core::AlarmListCtrlException& )
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "AlarmListCtrlException", "Could not retrieve alarm details");
		}
		return 0;
	}

	unsigned long AlarmListTableModel::getLatestAlarmTime()
	{
		try
		{
			return m_info.latestSourceTime.time;
		}
		catch( const TA_Base_Core::AlarmListCtrlException& )
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "AlarmListCtrlException", "Could not retrieve alarm details");
		}
		return 0;
	}

	bool AlarmListTableModel::isDeterminateColumn(const long columnID, const TAUtfString& strColumnCaption)
	{
		FUNCTION_ENTRY("isDeterminateColumn");
		bool bFindColumn = false;
		for (std::map<long, IAlarmColumn*>::iterator iter = m_alarmColumns->begin(); iter != m_alarmColumns->end(); ++iter)
		{
			LOG2(SourceInfo, DebugUtil::DebugInfo, "========%d , %s", iter->second->getColumnIdentifier(), iter->second->getColumnHeaderText().std_str().c_str());
			if (iter->second->getColumnIdentifier() == columnID)
			{
				if (iter->second->getColumnHeaderText() == strColumnCaption)
				{
					bFindColumn = true;
					break;
				}
			}
		}

		FUNCTION_EXIT;
		return bFindColumn;
	}

	void AlarmListTableModel::setNewDataFlag()
	{
		m_needUpdateData=true;
	}

	void AlarmListTableModel::OnTimer() 
	{ 
		bool needRefresh=false;	
		if(m_needUpdateData && !m_alarmListModel->isUpdating())
		{
			needRefresh=true;
			LOG_GENERIC(SourceInfo, DebugUtil::DebugTrace,"alarm Refresh");
			try
			{
				int newSize=0;
				
				m_ulIdxFrom = m_tableView->verticalScrollBar()->value(); //m_tableView->indexAt(m_tableView->rect().topLeft()).row();
				int nCountPerPage = 50;//m_tableView->indexAt(m_tableView->rect().bottomRight()).row() - m_ulIdxFrom + 1;
				m_ulIdxTo = m_ulIdxFrom + nCountPerPage;
				
				setListDisplayInfo();		
				newSize= (int)m_pDisplayVector->getSize();

				this->layoutAboutToBeChanged();	
				if (m_dataSize<newSize)
				{
					beginRemoveRows(QModelIndex(), m_dataSize, newSize);
				}
				else
				{
					beginInsertRows(QModelIndex(), newSize, m_dataSize);
				}

				if (m_dataSize<newSize)
				{
					this->endRemoveRows();
				}
				else
				{
					this->endInsertRows();
				}

				QModelIndex topLeft = index(0, 0);
				QModelIndex bottomRight = index(rowCount() - 1, columnCount() - 1);

				
				TA_Base_Core::IAlarmStoreCorbaDef::IndexInfo tempInfo = m_alarmListModel->getIndexInfo();
				bool bWereAlarmAdded = (m_info.dutyAlarmVersion != tempInfo.dutyAlarmVersion);
				m_info.earliestSourceTime.millitm = tempInfo.earliestSourceTime.millitm;
				m_info.earliestSourceTime.time = tempInfo.earliestSourceTime.time;
				m_info.latestSourceTime.millitm = tempInfo.latestSourceTime.millitm;
				m_info.latestSourceTime.time = tempInfo.latestSourceTime.time;
				m_info.exceededMaxDisplay = tempInfo.exceededMaxDisplay;
				m_info.highestSeverity = tempInfo.highestSeverity;
				m_info.isLoadingAlarms = tempInfo.isLoadingAlarms;
				m_info.totalAlarms = tempInfo.totalAlarms;
				m_info.unackAlarms = tempInfo.unackAlarms;
				m_info.dutyAlarmVersion = tempInfo.dutyAlarmVersion;

				m_dataSize = newSize;
				emit updateAlarmObservers(true, bWereAlarmAdded, m_info, 0);
				emit dataChanged(topLeft, bottomRight);
				emit layoutChanged();
			}
			catch (...)
			{
				LOG_GENERIC(SourceInfo, DebugUtil::DebugError,"Alarm Sound update function is error");
			}

			//SetItemCountEx(m_pDisplayVector->getSize(), LVSICF_NOSCROLL | LVSICF_NOINVALIDATEALL);
			m_needUpdateData = false;	
		}
		else
		{
			if (m_alarmListModel->isUpdating()/*m_alarmSyn.isUpdating()*/) 
			{
				LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug,"alarm store is updating");
			}
		}
		m_nFlashCount += UPDATE_SLEEP_IN_MILLISECONDS;
		if (m_nFlashCount >= 1500)
		{
			m_isFlashTime = !m_isFlashTime;	
			m_nFlashCount = 0;
			//refresh UI
			emit layoutChanged();
		}	
		//TODO:Refresh view		 
		//if (needRefresh==true)			 ;
	}


	int AlarmListTableModel::rowCount(const QModelIndex & /*parent*/) const
	{	
		return m_dataSize;
	}

	int AlarmListTableModel::columnCount(const QModelIndex & /*parent*/) const
	{
		return m_alarmColumnOrder.size();
	}

	QVariant AlarmListTableModel::headerData(int section, Qt::Orientation orientation, int role) const
	{
		if (role == Qt::DisplayRole)
		{
			if (orientation == Qt::Horizontal) {

				long id=m_alarmColumnOrder[section];				 
				std::map<long, IAlarmColumn*>::const_iterator   matching = m_alarmColumns->find(id);
				if (matching != m_alarmColumns->end())
				{
					TAUtfString headertext = matching->second->getColumnHeaderText();
					return headertext;
				}					 
			}
		}
		return QVariant();
	}

	QVariant AlarmListTableModel::data_Display(int row, int col) const
	{		
		std::string  str=getItemText(row,col);
		QString   qstr=str.c_str();
		return qstr;
	}
	 
	QVariant AlarmListTableModel::data_Colour(int row,Qt::ItemDataRole colorRule) const
	{
		unsigned long foregroundColour=0;
	    unsigned long backgroundColour=0;

	    determineDrawingColours (row, foregroundColour, backgroundColour);
		if (colorRule==Qt::BackgroundColorRole)
		{
			int r = backgroundColour & 0xff;
			int g = (backgroundColour >> 8) & 0xff;
			int b = (backgroundColour >> 16) & 0xff;
			QColor backgcolor(r, g, b);

           //QColor backgcolor(backgroundColour);
		   return backgcolor;
		}
		else if (colorRule == Qt::ForegroundRole)
		{
			int r = foregroundColour & 0xff;
			int g = (foregroundColour >> 8) & 0xff;
			int b = (foregroundColour >> 16) & 0xff;
			QColor forecolor(r, g, b);

			//QColor forecolor(foregroundColour);		 
			return forecolor;
		}		
		return QVariant();
	}



	TAUtfString AlarmListTableModel::getItemText(int row, int col) const
	{
		int columnId = m_alarmColumnOrder[col];
		std::map<long, IAlarmColumn*>::iterator matching = m_alarmColumns->find(columnId);
		if (matching == m_alarmColumns->end())
		{
			return  NO_VALUE;
		}
		return matching->second->getItemText(row);		 
	}

	QVariant AlarmListTableModel::data_Tooltip(int row)  const
	{
		TAUtfString  str = getItemText(row, m_descColIndex);
		return str.q_str();
	}

	unsigned long AlarmListTableModel::determineDrawingColours(unsigned long ulIndex, unsigned long& foregroundColour, unsigned long& backgroundColour) const
	{
		unsigned long result = 0;	
		try
		{

			foregroundColour = DEFAULT_COLOUR;
			backgroundColour = BACKGROUND;
			TA_Base_Core::IAlarmSeverityData::EColourType foregroundAlarmType = TA_Base_Core::IAlarmSeverityData::OPEN_ACKED_FG1;
			TA_Base_Core::IAlarmSeverityData::EColourType backgroundAlarmType = TA_Base_Core::IAlarmSeverityData::OPEN_ACKED_BG1;

			std::string strAlarmID = m_pDisplayVector->getItemString(ulIndex, ALARMID);
			if (strAlarmID.size() == 0)
			{
				return result;
			}

			if( strcmp(m_pDisplayVector->getItemString(ulIndex, ALARM_ACKNOWLEDGE_BY).c_str(), "") == 0 )
			{
				// Not Acknowledged
				if (m_pDisplayVector->getItemNumber(ulIndex, STATE) == TA_Base_Core::AlarmOpen)
				{
					// Open
					if (m_isFlashTime)
					{
						// Use flash colour
						if (m_alarmDataSync->isAlarmItemSelected(strAlarmID))
						{
							// Use selection colours
							foregroundAlarmType = TA_Base_Core::IAlarmSeverityData::SELECTED_OPEN_UNACKED_FG2;
							backgroundAlarmType = TA_Base_Core::IAlarmSeverityData::SELECTED_OPEN_UNACKED_BG2;
							//result = CDRF_SKIPDEFAULT;
						}
						else
						{
							// Use normal colours
							foregroundAlarmType = TA_Base_Core::IAlarmSeverityData::OPEN_UNACKED_FG2;
							backgroundAlarmType = TA_Base_Core::IAlarmSeverityData::OPEN_UNACKED_BG2;
						}
					}
					else
					{
						// Use non-flash colour
						if (m_alarmDataSync->isAlarmItemSelected(strAlarmID))
						{
							// Use selection colours
							foregroundAlarmType = TA_Base_Core::IAlarmSeverityData::SELECTED_OPEN_UNACKED_FG1;
							backgroundAlarmType = TA_Base_Core::IAlarmSeverityData::SELECTED_OPEN_UNACKED_BG1;
							//result = CDRF_SKIPDEFAULT;
						}
						else
						{
							// Use normal colours
							foregroundAlarmType = TA_Base_Core::IAlarmSeverityData::OPEN_UNACKED_FG1;
							backgroundAlarmType = TA_Base_Core::IAlarmSeverityData::OPEN_UNACKED_BG1;
						}
					}

				}
				else
				{
					// Closed
					if (m_isFlashTime)
					{
						// Use flash colour
						if (m_alarmDataSync->isAlarmItemSelected(strAlarmID))
						{
							// Use selection colours
							foregroundAlarmType = TA_Base_Core::IAlarmSeverityData::SELECTED_CLOSED_UNACKED_FG2;
							backgroundAlarmType = TA_Base_Core::IAlarmSeverityData::SELECTED_CLOSED_UNACKED_BG2;
							//result = CDRF_SKIPDEFAULT;
						}
						else
						{
							// Use normal colours
							foregroundAlarmType = TA_Base_Core::IAlarmSeverityData::CLOSED_UNACKED_FG2;
							backgroundAlarmType = TA_Base_Core::IAlarmSeverityData::CLOSED_UNACKED_BG2;
						}
					}
					else
					{
						// Use non-flash colour
						if (m_alarmDataSync->isAlarmItemSelected(strAlarmID))
						{
							// Use selection colours
							foregroundAlarmType = TA_Base_Core::IAlarmSeverityData::SELECTED_CLOSED_UNACKED_FG1;
							backgroundAlarmType = TA_Base_Core::IAlarmSeverityData::SELECTED_CLOSED_UNACKED_BG1;
							//result = CDRF_SKIPDEFAULT;
						}
						else
						{
							// Use normal colours
							foregroundAlarmType = TA_Base_Core::IAlarmSeverityData::CLOSED_UNACKED_FG1;
							backgroundAlarmType = TA_Base_Core::IAlarmSeverityData::CLOSED_UNACKED_BG1;
						}
					}
				}
			}
			else
			{
				// Acknowledged
				if (m_pDisplayVector->getItemNumber(ulIndex, STATE) == TA_Base_Core::AlarmOpen)
				{
					// Open
					if (m_isFlashTime)
					{
						// Use flash colour
						if (m_alarmDataSync->isAlarmItemSelected(strAlarmID) )
						{
							// Use selection colours
							foregroundAlarmType = TA_Base_Core::IAlarmSeverityData::SELECTED_OPEN_ACKED_FG2;
							backgroundAlarmType = TA_Base_Core::IAlarmSeverityData::SELECTED_OPEN_ACKED_BG2;
							//result = CDRF_SKIPDEFAULT;
						}
						else
						{
							// Use normal colours
							foregroundAlarmType = TA_Base_Core::IAlarmSeverityData::OPEN_ACKED_FG2;
							backgroundAlarmType = TA_Base_Core::IAlarmSeverityData::OPEN_ACKED_BG2;
						}
					}
					else
					{
						// Use non-flash colour
						if ( m_alarmDataSync->isAlarmItemSelected(strAlarmID) )
						{
							// Use selection colours
							foregroundAlarmType = TA_Base_Core::IAlarmSeverityData::SELECTED_OPEN_ACKED_FG1;
							backgroundAlarmType = TA_Base_Core::IAlarmSeverityData::SELECTED_OPEN_ACKED_BG1;
							//result = CDRF_SKIPDEFAULT;
						}
						else
						{
							// Use normal colours
							foregroundAlarmType = TA_Base_Core::IAlarmSeverityData::OPEN_ACKED_FG1;
							backgroundAlarmType = TA_Base_Core::IAlarmSeverityData::OPEN_ACKED_BG1;
						}
					}
				}
			}

			foregroundColour = m_alarmListModel->getAlarmSeverityColour(m_pDisplayVector->getItemNumber(ulIndex, ALARM_SEVERITY), foregroundAlarmType);
			backgroundColour = m_alarmListModel->getAlarmSeverityColour(m_pDisplayVector->getItemNumber(ulIndex, ALARM_SEVERITY), backgroundAlarmType);
			// delete alarm;
		}
		catch( const TA_Base_Core::AlarmListCtrlException& )
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "AlarmListCtrlException", "Could not determine alarm severity of alarm to draw it in the correct colour");

		}
		return result;
	}

	void AlarmListTableModel::setColumns(const std::vector<IAlarmColumn*> &columns)
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "Adding the columns to the list");    

		for(unsigned int column = 0; column < columns.size(); ++column)
		{
			TA_ASSERT(columns[column] != NULL, "One of the columns passed in was NULL");

			if (columns[column]->isColumnEnabled())
			{				 
				if (columns[column]->getColumnIdentifier() == COL_DESCRIPTION)
					m_descColIndex = m_alarmColumnOrder.size();

				m_alarmColumnOrder.push_back(columns[column]->getColumnIdentifier());
			}
			columns[column]->setPModel(m_alarmListModel);
			// Store the column in the map. 
			m_alarmColumns->insert( std::map<long, IAlarmColumn*>::value_type(columns[column]->getColumnIdentifier(),
				columns[column]) );		
		}
	}

	void AlarmListTableModel::sortColumn(long column, bool ascending)
	{
		int columnId = m_alarmColumnOrder[column];
		std::map<long, IAlarmColumn*>::iterator matching = m_alarmColumns->find(columnId);
		if (matching == m_alarmColumns->end())
		{
			// not a relevant column
			LOG_GENERIC(SourceInfo, DebugUtil::DebugWarn, "Alarm sort column %d selected is not valid", column);			 
			return;
		}
		SORT_COLUMN curSortCol;
		curSortCol = matching->second->getSortColumn(); 
		m_alarmListModel->setDisplaySort(curSortCol, ascending);
	}

	void AlarmListTableModel::enableColumn(long column, bool isEnabled, unsigned int position)
	{
		FUNCTION_ENTRY("enableColumn");

		std::map<long, IAlarmColumn*>::iterator matching = m_alarmColumns->find(column);
		TA_ASSERT(matching != m_alarmColumns->end(), "The alarm column passed is invalid.");

		matching->second->setIsColumnEnabled(isEnabled);

		if (isEnabled)
		{
			// Add to order vector if not already in there
			if (m_alarmColumnOrder.end() != std::find(m_alarmColumnOrder.begin(), m_alarmColumnOrder.end(), column))
			{
				// Must add it into the list
				std::vector<long>::iterator insertPos = m_alarmColumnOrder.begin();
				std::advance(insertPos, position);
				m_alarmColumnOrder.insert( insertPos, column );
			}
		}
		else
		{
			// Remove from order vector
			std::vector<long>::iterator iter = std::find(m_alarmColumnOrder.begin(), m_alarmColumnOrder.end(), column);
			if (iter != m_alarmColumnOrder.end())
			{
				int removePos = std::distance(m_alarmColumnOrder.begin(), iter);
				m_alarmColumnOrder.erase(std::remove(m_alarmColumnOrder.begin(), m_alarmColumnOrder.end(), column));
			}
		}
		FUNCTION_EXIT;
	}

	void AlarmListTableModel::resetDataTable()
	{
		int curSize = 0;
		int oldSize = 0;
		oldSize = (int)m_pDisplayVector->getSize();
		
		this->layoutAboutToBeChanged();
		if (oldSize<curSize)
		{
			beginRemoveRows(QModelIndex(), oldSize, curSize);
		}
		else
		{
			beginInsertRows(QModelIndex(), curSize, oldSize);
		}
		bool bWereAlarmAdded = false;
		m_info.earliestSourceTime.millitm = 0;
		m_info.earliestSourceTime.time = 0;
		m_info.latestSourceTime.millitm = 0;
		m_info.latestSourceTime.time = 0;
		m_info.exceededMaxDisplay = false;
		m_info.highestSeverity = 0;
		m_info.isLoadingAlarms = false;
		m_info.totalAlarms = 0;
		m_info.unackAlarms = 0;
		m_info.dutyAlarmVersion = 0;
		if (oldSize<curSize)
		{
			this->endRemoveRows();
		}
		else
		{
			this->endInsertRows();
		}
		this->layoutChanged();
	}

	bool AlarmListTableModel::isAlarmItemSelected(std::string strAlarmID)
	{

		return m_alarmDataSync->isAlarmItemSelected(strAlarmID);
	}

	void AlarmListTableModel::selectAlarmItem(unsigned long ulIndex)
	{
		m_alarmDataSync->selectAlarmItem(ulIndex);
	}

	void AlarmListTableModel::selectRangeAlarms(unsigned long ulSelBegin, unsigned long ulSelEnd)
	{
		m_alarmDataSync->selectRangeAlarms(ulSelBegin, ulSelEnd);
	}

	void AlarmListTableModel::unselectAlarmItem(unsigned long ulIndex)
	{
		m_alarmDataSync->unselectAlarmItem(ulIndex);
	}

	void AlarmListTableModel::unSelectAllAlarmItem()
	{
		m_alarmDataSync->unSelectAllAlarmItem();
	}

	unsigned long AlarmListTableModel::getSelectedAlarmsCount()
	{
		return m_alarmDataSync->getSelectedAlarmsCount();
	}

	unsigned long AlarmListTableModel::findItemPos(const std::string& strAlarmID)
	{
		return m_pDisplayVector->findItemPos(strAlarmID);
	}

	std::string AlarmListTableModel::getAlarmIdAt(unsigned long ulIndex)
	{
		return m_pDisplayVector->getItemString(ulIndex, STRING_ATTR::ALARMID);
	}

}