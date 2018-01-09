#include "AlarmListModel.h"
 
//alarm_list_control/pmodel

#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "core/alarm/IDL/src/AlarmMessageCorbaDef.h"
#include "bus/alarm/alarm_list_control/pmodel/src/AlarmStore.h"
#include "bus/alarm/alarm_list_control/pmodel/src/Se_Exception.h"
#include "core/synchronisation/src/ThreadGuard.h"

using namespace TA_Base_Core;

namespace TA_Base_Bus
{

    AlarmListModel::AlarmListModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
        : TA_Base_Bus::AbstractPModel(appController, pmodelName)
    {
		m_curSortCol=NOSORT_BY_HASH;
		m_sortAscending=false;
    }

    AlarmListModel::~AlarmListModel(void)
    {
		//unregister all messages
		m_alarmDataSync.unregisterObserver(this);
    }

	void AlarmListModel::connectToAlarmStore(const Filter& preFilter,const Filter& filter,const std::vector<unsigned long>& locations,
		                                const std::string& sessionId, Sort sort,bool isAscendingSort,bool isHighestSeverityNeeded)
	{
		// Create the alarm store so we can get all our alarms to add
		LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "Creating the alarm store");
		try
		{			 	 
			 m_alarmStore = new AlarmStore(preFilter, locations, sort, filter, sessionId, isHighestSeverityNeeded);
			if (m_alarmStore == NULL)
			{
				TA_THROW( TA_Base_Core::AlarmListCtrlException("Alarm Store could not be constructed. New failed") );
			}

		}
		catch( const TA_Base_Core::DataException& )
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "DataException", "Could not initialise the alarm store.");
			//UE-040022
			TA_THROW( TA_Base_Core::AlarmListCtrlException("The alarm list could not be displayed because the database configuration is invalid.") );
		}
		catch( const TA_Base_Core::DatabaseException& )
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "DatabaseException", "Could not initialise the alarm store");
			//UE-040022
			TA_THROW( TA_Base_Core::AlarmListCtrlException("The alarm list could not be displayed because the database configuration is invalid.") );
		}

		if( m_alarmStore != NULL && !m_alarmStore->isAlarmStoreValid())
		{
			// This means we have an invalid alarm store
			// Set the number of items to 1 so we can display an error message in the list control
			LOG_GENERIC(SourceInfo, DebugUtil::DebugWarn, "Indicating the alarm store is not yet initialised");
		}		 
		 
		defaultSort();
		LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "Starting timers for flashing and updating");		
		m_alarmDataSync.setStoreService(m_alarmStore);
		//TODO: Replace this with control message
		//m_alarmDataSync.setTarget(m_hWnd);
		m_alarmDataSync.initialize();
		//register all messages
		m_alarmDataSync.registerObserver(this);
 
	}

	void  AlarmListModel::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{
		//Update to View;
		notifyViews(updateType,args);
	}

	void AlarmListModel::updatePreFilterLocationsAndSession( const Filter& preFilter,	 
		const std::vector<unsigned long>& locations,
		const std::string& sessionID )
	{ 
		m_alarmStore->updatePreFilterLocationsAndSession( preFilter,
				locations,
				sessionID );	 
	}

	void AlarmListModel::reSetPreFilterLocationAndSession()
	{
		m_alarmStore->reSetPreFilterLocationAndSession();
	}
	
	void AlarmListModel::setFilter(const Filter& filter,bool isDeault)
	{
		FUNCTION_ENTRY("setFilter");

		TA_ASSERT(m_alarmStore != NULL, "This method must be called after startAlarmList has successfully completed");


		m_alarmDataSync.applyDutyIndexFilter(filter, isDeault);
		//m_alarmStore->setFilter(filter, m_alarmDataSync.getClientPos(),isDeault);
		m_alarmDataSync.unSelectAllAlarmItem();
		//m_bIgnoreEvent = false;

		FUNCTION_EXIT;
	}
	void AlarmListModel::setDisplaySort(SORT_COLUMN sortType, bool bSortAccending)
	{
		m_curSortCol=sortType;
		m_sortAscending=bSortAccending;
		m_alarmDataSync.setDisplaySort(sortType,bSortAccending);
	}
 
	void AlarmListModel::defaultSort()
	{
		if ( RunParams::getInstance().isSet(RPARAM_KRTCALARMSORTING))
		{
			//GF-84, alarm sorting
			if (m_curSortCol==SORT_BY_DATETIME && m_sortAscending==false)
			{
				return;
			}			
			m_curSortCol = SORT_BY_DATETIME;
			m_sortAscending = false;
		}
		else
		{
			if (m_curSortCol==SORT_BY_DEFAULT && m_sortAscending==true)
			{
				return;
			}			
			m_curSortCol = SORT_BY_DEFAULT;
			m_sortAscending = true;
		}
		
		m_alarmDataSync.setDisplaySort(m_curSortCol, m_sortAscending);
	}

	Filter AlarmListModel::getCurrentFilter() const
	{
		FUNCTION_ENTRY("getCurrentFilter");

		TA_ASSERT(m_alarmStore != NULL, "This method must be called after startAlarmList has successfully completed");

		FUNCTION_EXIT;
		return m_alarmStore->getCurrentFilter();	
	}

 
     TA_Base_Core::IAlarmStoreCorbaDef::IndexInfo& AlarmListModel::getIndexInfo()
	 {
		 return m_alarmDataSync.getIndexInfo();
	 }
 

	void AlarmListModel::setListDisplayInfo(ListDispInfo& listInfo)
	{
		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "AlarmListModel::setListDisplayInfo");
		m_alarmDataSync.setListDisplayInfo(listInfo);
	}

	AlarmVectorWrap* AlarmListModel::getDisplayDataWrap()
	{
		return m_alarmDataSync.getDisplayData();
	}

	AlarmSelectionWrap * AlarmListModel::getSelectionWrapper()
	{
		return m_alarmDataSync.getSelectionWrapper();
	}

	AlarmDataSync*    AlarmListModel::getAlarmDataSync()
	{
		return &m_alarmDataSync;
	}

	unsigned long AlarmListModel::getAlarmSeverityColour(unsigned long key, TA_Base_Core::IAlarmSeverityData::EColourType type)
	{
		return m_databaseCache.getAlarmSeverityColour(key,type);
	}

	bool AlarmListModel::isUpdating()
	{
		return m_alarmDataSync.isUpdating();
	}
	 
}