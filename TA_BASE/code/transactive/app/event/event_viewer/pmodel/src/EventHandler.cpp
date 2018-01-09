/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP/TA_BASE/transactive/app/event/event_viewer/src/EventHandler.cpp $
  * @author:  huirong.luo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2010/09/15 8:54:40 $
  * Last modified by: xiangmei.lu
  * 
  * EventHandler is ...
  *
  */


#include "EventHandler.h"
#include "app/event/event_viewer/pmodel/src/ATSHistoricalWorker.h"
#include "app/event/event_viewer/pmodel/src/ISCSHistoricalWorker.h"
#include "app/event/event_viewer/pmodel/src/ISCSRealTimeWorker.h"
#include "app/event/event_viewer/pmodel/src/ATSRealTimeWorker.h"
#include "app/event/event_viewer/pmodel/src/CombinedEventItem.h"
#include "ConstantItems.h"
#include "CombinedEventFilter.h"

#include "core/synchronisation/src/ThreadGuard.h"
#include "core/threads/src/Thread.h"


using namespace TA_Base_App;
using namespace EventViewerConstants;


namespace TA_Base_App{

	std::string EventHandler::s_sortColumnForRealTime= EVDisplayStrRes::GetInstance()->COLUMN_DATETIME;;
	bool EventHandler::s_sortAscendingForRealTime= false;;

	std::string EventHandler::s_sortColumnForHist= EVDisplayStrRes::GetInstance()->COLUMN_DATETIME;;
	bool EventHandler::s_sortAscendingForHist= false;;

	std::string EventHandler::s_sortColumnForGUI= EVDisplayStrRes::GetInstance()->COLUMN_DATETIME;;
	bool EventHandler::s_sortAscendingForGUI= false;;


    void EventHandler::resetSortingRuleForRealTime(std::string sColumn, bool sAsc)
	{
		s_sortColumnForRealTime = sColumn.c_str();
		s_sortAscendingForRealTime = sAsc;
	}

	void EventHandler::resetSortingRuleForHist(std::string sColumn, bool sAsc)
	{
		s_sortColumnForHist = sColumn.c_str();
		s_sortAscendingForHist = sAsc;
	}

	void EventHandler::resetSortingRuleForGUI(std::string sColumn, bool sAsc)
	{
		s_sortColumnForGUI = sColumn.c_str();
		s_sortAscendingForGUI = sAsc;
	}
	

	EventHandler::EventHandler(EventDisplayMode dispMode, GUIObserver * observer):IEventWorkObserver()
	{

		this->m_mode = dispMode;
		m_GUIObserver = observer;
		m_CurrentTaskStopped = true;
		m_taskSeqID = 0;
		m_ISCSArrived =false;
		m_ATSArrived = false;
		m_ATSSubscriptionErr = 0; 

	}

	EventHandler::~EventHandler(){
		releaseWorkers();
		clearList();
	}


	void EventHandler::setATSSubscriptionErr(long err)
	{
		m_ATSSubscriptionErr = err;
	}

	long EventHandler::getATSSubscriptionErr ()
	{
		return m_ATSSubscriptionErr;
	}

	SearchStatus EventHandler::getSearchStatus(TA_Base_Core::EventCategory eventCategory){

		SearchStatus ss =  TA_Base_App::FinishSearch;

		if(!m_workers.empty()){
			if ( m_workers.find(eventCategory)!= m_workers.end()){

				ss = m_workers.find(eventCategory)->second->getStatus();
			}
		}

		return ss;
	}


	void EventHandler::setMaxRecordNumber(long maxNumber){

	}
    
	const std::string  EventHandler::getColumnData (unsigned long index,string columnName)
	{
		TA_Base_Core::ThreadGuard guard2(m_displaylistLock);

		if( index<m_displaylist.size() ) 
		{
			DisplayItem * item = m_displaylist[index];
			if(item!= NULL)
			{
				return item->getColumnData(columnName).c_str();
			}
		}
		return "";

	}

	void EventHandler::setColumnData (unsigned long index,string columnName, string value)
	{
		TA_Base_Core::ThreadGuard guard2(m_displaylistLock);

		if( index<m_displaylist.size() ) 
		{
			DisplayItem * item = m_displaylist[index];
			if(item!= NULL)
			{
				item->setColumnData(columnName, value);
			}
		}

	}
    
	const std::string  EventHandler::getCompleteColumnData (unsigned long index,string columnName)
	{
		TA_Base_Core::ThreadGuard guard(m_displaylistLock);
		if( index<m_displaylist.size() ) 
		{
			return m_displaylist[index]->getCompleteColumnData(columnName).c_str();
		}
		return "";
		
	}



	void EventHandler::startParameter(FilterCriteria searchfilter, long seq_ID, 
		bool ISCSEnabled /*=true*/, bool ATSEnabled /*=true*/)
	{
		clearAndReserveFor2Lists( EventHandlerManager::getMaxEvents());

		m_GUIObserver->sendListViewUpdateByObserver(0,0);

		{
			TA_Base_Core::ThreadGuard guard( m_taskIDLock );
			m_taskSeqID = seq_ID;
			m_curFilterCriteria = searchfilter;
			m_CurrentTaskStopped = false;
		}


		if(m_mode ==RealTimeMode)
		{
			if(ISCSEnabled)
			{
				std::map<TA_Base_Core::EventCategory,IEventWorker*>::iterator iscsWorkerIt = m_workers.find(TA_Base_Core::ISCSCategory);
				if(iscsWorkerIt == m_workers.end())
				{
					IEventWorker* iscsRealTWorker = new ISCSRealTimeWorker(this);
					iscsRealTWorker->addWork( searchfilter, seq_ID);
					addWorker(TA_Base_Core::ISCSCategory, iscsRealTWorker);
				}
				else
				{
					iscsWorkerIt->second->addWork( searchfilter, seq_ID);
				}
			}

			
			if(ATSEnabled)
			{
				std::map<TA_Base_Core::EventCategory,IEventWorker*>::iterator atsWorkerIt = m_workers.find(TA_Base_Core::ATSCategory);
				if(atsWorkerIt == m_workers.end())
				{
					IEventWorker* atsRealTWorker = new ATSRealTimeWorker(this);
					atsRealTWorker->addWork( searchfilter, seq_ID);
					addWorker(TA_Base_Core::ATSCategory, atsRealTWorker);
				}
				else
				{
					atsWorkerIt->second->addWork( searchfilter, seq_ID);
				}
			}

		}else
		{
			if(ISCSEnabled)
			{
				std::map<TA_Base_Core::EventCategory,IEventWorker*>::iterator iscsWorkerIt = m_workers.find(TA_Base_Core::ISCSCategory);
				if(iscsWorkerIt == m_workers.end())
				{
					IEventWorker* iscsHistWorker = new ISCSHistoricalWorker(this);
					iscsHistWorker->addWork( searchfilter, seq_ID);
					addWorker(TA_Base_Core::ISCSCategory, iscsHistWorker);
				}
				else
				{
					iscsWorkerIt->second->addWork( searchfilter, seq_ID);
				}

			}
			if(ATSEnabled)
			{
				std::map<TA_Base_Core::EventCategory,IEventWorker*>::iterator atsWorkerIt = m_workers.find(TA_Base_Core::ATSCategory);
				if(atsWorkerIt == m_workers.end())
				{
					IEventWorker* atsHistWorker = new ATSHistoricalWorker(this);
					atsHistWorker->addWork( searchfilter, seq_ID);
					addWorker(TA_Base_Core::ATSCategory, atsHistWorker);
				}
				else
				{
					atsWorkerIt->second->addWork( searchfilter, seq_ID);
				}
			}
		
		}
		
		m_ATSArrived = false;
		m_ISCSArrived = false;

	}


	void EventHandler::clearList()
	{

		TA_Base_Core::ThreadGuard guard(m_storedlistLock);
		TA_Base_Core::ThreadGuard displayGuard(m_displaylistLock);

		//clear m_displayList and m_eventStoreList
		m_displaylist.clear();
		{
			// All event items not excluded by current filter are stored in client side.
			for(int i=0; i<(int)m_eventStoreList.size(); i++)
			{
				delete m_eventStoreList[i];
				m_eventStoreList[i] = NULL;
			}
			m_eventStoreList.clear();
		}
	}

	void EventHandler::clearToBeDelList(vector<DisplayItem*> toBeDelList)
	{

		//clear m_toBeDelList
		for(int i=0; i<(int)toBeDelList.size(); i++)
		{
			delete toBeDelList[i];
			toBeDelList[i] = NULL;
		}
		toBeDelList.clear();
	}

	void EventHandler::addWorker(TA_Base_Core::EventCategory eventCategory, TA_Base_App::IEventWorker* worker)
	{
		m_workers.insert(std::map<TA_Base_Core::EventCategory,IEventWorker*>::value_type(eventCategory, worker) );

 		worker->start();

	}

	// this will do terminate() first, then free vector m_worker
	void EventHandler::releaseWorkers()
	{

		std::map<TA_Base_Core::EventCategory,IEventWorker*>::iterator workerItr;
		for(workerItr=m_workers.begin(); workerItr!=m_workers.end(); workerItr++)
		{
			if( NULL != workerItr->second)
			{
				//to do : test here
				workerItr->second->terminateAndWait();
					//TerminateThread(); 
				delete workerItr->second;
				workerItr->second = NULL;
			}
		}	

		m_workers.clear();
	}

	void EventHandler::stopCurrentTask()
	{
		if (m_taskSeqID>0){
			TA_Base_Core::ThreadGuard guard(m_taskIDLock);
			m_CurrentTaskStopped = true;
		}		
	}

	bool EventHandler::IsThisTaskStopped( long taskID) 
	{
		TA_Base_Core::ThreadGuard guard(m_taskIDLock);
		if(taskID <m_taskSeqID || (taskID==m_taskSeqID && m_CurrentTaskStopped == true))
		{
			return true;
		}

		return false;

	}

	void EventHandler::setCategoryArrived(TA_Base_Core::EventCategory  eventCategory)
	{
		if(eventCategory==TA_Base_Core::ATSCategory)
		{
			m_ATSArrived = true;
		}
		else
		{
			m_ISCSArrived = true;
		}
	}

	void EventHandler::ATSSubscriptionErrNotification()
	{
		m_GUIObserver->reloadEvents();
	}


	// ICombinedEventData doesn't distinguish ISCS/ATS category, but DisplayItem does.
	void EventHandler::EventNotification(TA_Base_Core::EventCategory eventCategory, std::vector<TA_Base_Core::ICombinedEventData*> & eventlist, 
		ActionCommand command, long seq_ID /*=0*/, bool isLoadedData /*=false*/, bool loadFinished /*=false*/)
	{
			//if error received. update status bar to show the unhealthy state and return 
			if (command ==Reconnect)
			{
				if(eventCategory == TA_Base_Core::ATSCategory)
				{
					m_GUIObserver->statusBarUpdate(0, TA_Base_Core::ATSCategory, EVDisplayStrRes::GetInstance()->STATUS_ATS_UNHEALTHY);
				}
				else
				{
					m_GUIObserver->statusBarUpdate(0, TA_Base_Core::ISCSCategory, EVDisplayStrRes::GetInstance()->STATUS_ISCS_UNHEALTHY);
				}
				return;
			}

			// if the task id is an old one. then do nothing
			{	TA_Base_Core::ThreadGuard guard( m_taskIDLock);
				if( seq_ID < m_taskSeqID ){
					return;
				}
			}


			//if the list is loaded data( either realtime backlog or historical loading data) , and loading step has finished. 
			if(isLoadedData && loadFinished)
			{
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "EventNotification(), one of the 2 parts has finished loading. Add this Cache to storelist.");
				transformAndAddCache(eventlist, eventCategory);
				/*TA_Base_Core::EventCategory another = (eventCategory==TA_Base_Core::EventCategory::ATSCategory)? (TA_Base_Core::EventCategory::ISCSCategory) : (TA_Base_Core::EventCategory::ATSCategory);*/
				bool anotherHasArrived = (eventCategory==TA_Base_Core::ATSCategory)? m_ISCSArrived: m_ATSArrived;
				
				setCategoryArrived(eventCategory);
				
				//check whether the other real time thread has submitted the "full" backlog
				if ( m_workers.size()==2 && anotherHasArrived && bothHaveRecord() )
				{
					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "EventNotification(), both of the 2 parts has finished loading. Merge the cache in storelist and update displaylist.");
					TA_Base_Core::ThreadGuard guard1( m_storedlistLock);
					TA_Base_Core::ThreadGuard guard2( m_displaylistLock);
					truncateFor2Catogery() ; 
					sortAndSynchronizeList();
					//assignToDispListAndSortDispList();
				}

				else
				{
					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "EventNotification(), no need to merge the cache. Just update displaylist.");
					sortAndSynchronizeList();
					//assignToDispListAndSortDispList();
				}
				m_GUIObserver->refreshListView();
				m_GUIObserver->statusBarUpdate(0, eventCategory, TA_Base_Core::ISCSCategory == eventCategory ?  EVDisplayStrRes::GetInstance()->STATUS_ISCS_READY : EVDisplayStrRes::GetInstance()->STATUS_ATS_READY);
			}
			else if(isLoadedData)
			{
				if(eventlist.size()>0)
				{
					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "EventNotification(), Get loaded data but this loading hasn't finished. Just Add cache and update displaylist.");
					transformAndAddCache(eventlist, eventCategory);
					sortAndSynchronizeList();
					//assignToDispListAndSortDispList();
					m_GUIObserver->refreshListView();
				}

			}
			else if (!isLoadedData) //real-time refresh
			{
// 				// for refresh data:
// 				if( eventlist.size()>0 )
// 				{
// 					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "EventNotification(), Get refresh data. Add cache, and cut the list if needed, and update displaylist.");
// 					transformAndAddCache( eventlist, eventCategory );
// 					TA_Base_Core::ThreadGuard guard1( m_storedlistLock);
// 					//TA_Base_Core::ThreadGuard guard2( m_displaylistLock);
// 					vector<DisplayItem*> toBeDelList = sortAndPrepareCutListByMaxSizeIfNeeded ( false );
// 					sortAndSynchronizeListAndDelTempList(toBeDelList);
// 					//assignToDispListAndSortDispList();
// 					//clearToBeDelList();
// 					m_GUIObserver->refreshListView();
// 				}

				// for refresh data:  improve performance by mergesort
				if( eventlist.size()>0 )
				{
					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "EventNotification(), Get refresh data. Add cache, and cut the list if needed, and update displaylist.");
					processRefreshEvents(eventlist, eventCategory);

					m_GUIObserver->refreshListView();
				}
			}	
	}

	std::vector<DisplayItem*>  EventHandler::transformData(std::vector<TA_Base_Core::ICombinedEventData*> & eventDatas,TA_Base_Core::EventCategory eventCategory)
	{
        std::vector<DisplayItem*> eventItems;

		TA_Base_Core::ICombinedEventData* tempData = NULL;
		for ( int i=0; i<eventDatas.size(); i++)
		{	
			try
			{
				tempData = eventDatas[i];
				//save into m_eventStoreList
				CombinedEventItem * newItem = new CombinedEventItem(tempData);
				newItem->setEventCategory(eventCategory) ;
				eventItems.push_back(newItem);	

				//release ICombinedEventData
				delete (tempData);
				tempData = NULL;
				eventDatas[i] = NULL;
			}
			catch ( ... )
			{
				// If there was any problem constructing the item don't add it to the display list.
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Exception caught when new CombinedEventItem from ICombinedEventData.");
				continue;
			}

		}
		return eventItems;

	}
	void EventHandler::processRefreshEvents(std::vector<TA_Base_Core::ICombinedEventData*> & refreshDatas,TA_Base_Core::EventCategory eventCategory)
	{
		//transfer ICombinedEventData vector to DisplayItem vector
		std::vector<DisplayItem*> refreshItems = transformData(refreshDatas,eventCategory);
		
		TA_Base_Core::ThreadGuard guard1( m_storedlistLock);

		int oldSize = m_eventStoreList.size();
		int cutNum = oldSize + refreshItems.size() - EventHandlerManager::getMaxEvents() ;
		vector<DisplayItem*> toBeDelList ;

		if(cutNum>0) //event size beyond max, need to cut.
		{
			bool cutFromRear = true;

			//( in realtime handler, s_sortColumnForRealTime/s_sortAscendingForRealTime always mean the same sorting rule 
			// in current m_eventStoreList: Because each time we set these 2 variable, we will sort/reverse the list according to them;
			// and each time we sort/reverse the list, we will reset these rule.

			if( s_sortColumnForRealTime == EVDisplayStrRes::GetInstance()->COLUMN_DATETIME )  //if current m_eventStoreList is sorted on createtime
			{
				stable_sort(refreshItems.begin(), refreshItems.end(), compare_events_for_RealTime());  //sort the new events first.
				mergeSort(m_eventStoreList, refreshItems);      //merge it to m_eventStoreList
				if(s_sortAscendingForRealTime == true)
				{
					cutFromRear = false;
				}
			}
			else
			{
				addCache(refreshItems);     //add new events list to m_eventStoreList, just add to the end of the list
				resetSortingRuleForRealTime(EVDisplayStrRes::GetInstance()->COLUMN_DATETIME, false);
				std::stable_sort(m_eventStoreList.begin(), m_eventStoreList.end(), compare_events_for_RealTime() );
			}

			toBeDelList = prepareCutListFromHeadOrRearByNum(cutNum, cutFromRear);

			sortAndSynchronizeListAndDelTempList(toBeDelList);  // will sort(if need) m_eventStoreList and copy to display list
		}
		else  //events not beyond max size, no need to cut
		{
			//sort new events base on the same sorting rule as m_eventStoreList: 
			stable_sort(refreshItems.begin(), refreshItems.end(), compare_events_for_RealTime());

			//merge to m_eventStoreList base on current sorting rule
			mergeSort(m_eventStoreList,refreshItems);

			sortAndSynchronizeListAndDelTempList(toBeDelList);
		}

	}

	//Before this function is called, both vector a and vector b has been sorted on "compare_events_for_RealTime".
	//use merge sort algorithm to merge a and b into a temp vector c, then copy c into a.
	void EventHandler::mergeSort(std::vector<DisplayItem*>& a, std::vector<DisplayItem*>& b)
	{
		std::vector<DisplayItem*> c;
		int aLen=a.size() ;
		int bLen =b.size();
		c.reserve(aLen + bLen);   

		compare_events_for_RealTime compare;
		int i = 0;
		int j=0;

		//get the smaller one and insert it into c.
		while(i<a.size() && j<b.size())
		{
			if(compare(a[i],b[j]))
			{
				c.push_back(a[i]);
				i++;
			}
			else
			{
				c.push_back(b[j]);
				j++;
			}
		}

		//if a hasn't been fully accessed: insert the rest into c
		if(i<a.size())
		{
			for(; i<a.size(); i++)
			{
				c.push_back(a[i]);
			}
		}

		//if b hasn't been fully accessed: insert the rest into c
		if(j<b.size())
		{
			for(; j<b.size(); j++)
			{
				c.push_back(b[j]);
			}
		}

		//clear a and copy c into a
		a.clear();
		a.reserve(c.size());
		a.assign(c.begin(),c.end());
	}
	
	bool EventHandler::bothHaveRecord()
	{
		TA_Base_Core::ThreadGuard guard (m_storedlistLock);
		bool ATS_found=false;
		bool ISCS_found = false;
		CombinedEventItem* tempItem = NULL;
		for(int i=0; i<(int)m_eventStoreList.size(); i++)
		{
			tempItem = dynamic_cast<CombinedEventItem*>(m_eventStoreList[i]);
			if ( ISCS_found && ATS_found )
			{
				break;
			}
			if( !ISCS_found && tempItem->getEventCategory() == TA_Base_Core::ISCSCategory)
			{
				ISCS_found = true;
			}else if( !ATS_found && tempItem->getEventCategory() == TA_Base_Core::ATSCategory)
			{
				ATS_found = true;
			}
		}

		return ATS_found&&ISCS_found ;
	
	}


	vector<DisplayItem*>  EventHandler::sortAndPrepareCutListByMaxSizeIfNeeded ( bool timeAscending)
	{

		TA_Base_Core::ThreadGuard guard (m_storedlistLock);
		int cutNum = 0;
		cutNum = m_eventStoreList.size() - EventHandlerManager::getMaxEvents();

		vector<DisplayItem*> toBeDelList;

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "before sorting in func sortAndCutByMaxSizeIfNeeded()");

		if(cutNum>0)
		{	
			// sort first:
			if(m_mode == EventDisplayMode::HistoricalMode)
			{
				resetSortingRuleForHist(EVDisplayStrRes::GetInstance()->COLUMN_DATETIME, timeAscending);
				std::stable_sort(m_eventStoreList.begin(), m_eventStoreList.end(), compare_events_for_Hist() );
			}
			else
			{
				resetSortingRuleForRealTime(EVDisplayStrRes::GetInstance()->COLUMN_DATETIME, timeAscending);
				std::stable_sort(m_eventStoreList.begin(), m_eventStoreList.end(), compare_events_for_RealTime() ); 
				//std::sort(m_eventStoreList.begin(), m_eventStoreList.end(), compare_events_for_RealTime() );   //for test

			}


			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "after sorting in func sortAndCutByMaxSizeIfNeeded()");

			toBeDelList = prepareCutListFromHeadOrRearByNum(cutNum, true);

			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "after cutting in func sortAndCutByMaxSizeIfNeeded()");

		}
		return toBeDelList;

	}

	void  EventHandler::sortAndCutByMaxSizeIfNeeded ( bool timeAscending)
	{

		TA_Base_Core::ThreadGuard guard (m_storedlistLock);
		int cutNum = 0;
		cutNum = m_eventStoreList.size() - EventHandlerManager::getMaxEvents();
		

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "before sorting in func sortAndCutByMaxSizeIfNeeded()");

		if(cutNum>0)
		{	
			// sort first:
			if(m_mode == EventDisplayMode::HistoricalMode)
			{
				resetSortingRuleForHist(EVDisplayStrRes::GetInstance()->COLUMN_DATETIME, timeAscending);
				std::stable_sort(m_eventStoreList.begin(), m_eventStoreList.end(), compare_events_for_Hist() );
			}
			else
			{
				resetSortingRuleForRealTime(EVDisplayStrRes::GetInstance()->COLUMN_DATETIME, timeAscending);
				std::stable_sort(m_eventStoreList.begin(), m_eventStoreList.end(), compare_events_for_RealTime() ); 
				//std::sort(m_eventStoreList.begin(), m_eventStoreList.end(), compare_events_for_RealTime() );   //for test
								
			}


			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "after sorting in func CutByMaxSizeIfNeeded()");

			cutLastByNum(cutNum);

			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "after cutting in func CutByMaxSizeIfNeeded()");
			
		}

	}

	unsigned long EventHandler::getSizeByCategory(TA_Base_Core::EventCategory eventCategory)
	{
		TA_Base_Core::ThreadGuard guard (m_storedlistLock);
		unsigned long cnt=0;
		for ( unsigned long i=0; i< m_eventStoreList.size(); i++)
		{
			CombinedEventItem* tempItem = dynamic_cast<CombinedEventItem*>(m_eventStoreList[i]);
			if(tempItem->getEventCategory() == eventCategory )
			{
				cnt ++;
			}
		}
		return cnt;
	}
	
	

    void EventHandler::cutLastByNum(unsigned long cutNum)
	{
		TA_Base_Core::ThreadGuard guard1 (m_storedlistLock); 

		std::vector<DisplayItem*>::iterator itr;

		for( int i=m_eventStoreList.size()-1; cutNum>0 && i>=0; i--)
		{
			CombinedEventItem* tempItem = dynamic_cast<CombinedEventItem*>(m_eventStoreList[i]);
			if( tempItem!=NULL )
			{
				delete tempItem;
				tempItem = NULL;
				m_eventStoreList.erase(m_eventStoreList.begin()+i);
				cutNum --;
			}

		}
	}

	std::vector<DisplayItem*> EventHandler::prepareCutListFromHeadOrRearByNum(unsigned long cutNum, bool fromRear)
	{
		TA_Base_Core::ThreadGuard guard1 (m_storedlistLock);

		std::vector<DisplayItem*> toBeDelList; 

		if(!fromRear)  //cut from head
		{
			for(int i=0; i<cutNum; i++)
			{
				CombinedEventItem* tempItem = dynamic_cast<CombinedEventItem*>(m_eventStoreList[i]);
				if( tempItem!=NULL )
				{
					toBeDelList.push_back(tempItem);  //move the old events to the temp list and delete them later.(after display list get the new list)
					//m_eventStoreList.erase(m_eventStoreList.begin()+i);
				}
			}
			m_eventStoreList.erase(m_eventStoreList.begin(), m_eventStoreList.begin()+cutNum);
		}
		else  //cut from end
		{
			for( int i=m_eventStoreList.size()-1; cutNum>0 && i>=0; i--)
			{
				CombinedEventItem* tempItem = dynamic_cast<CombinedEventItem*>(m_eventStoreList[i]);
				if( tempItem!=NULL )
				{
					toBeDelList.push_back(tempItem);  //move the old events to the temp list and delete them later.(after display list get the new list)
					//m_eventStoreList.erase(m_eventStoreList.begin()+i);
					m_eventStoreList.pop_back();
					cutNum --;
				}

			}
		}
		

		return toBeDelList;
	}

	void EventHandler::cutLastByCategoryAndNum( TA_Base_Core::EventCategory eventCategory, unsigned long cutNum)
	{
		TA_Base_Core::ThreadGuard guard1 (m_storedlistLock);
		TA_Base_Core::ThreadGuard guard2 (m_displaylistLock);

		std::vector<DisplayItem*>::iterator itr;

		for( int i=m_eventStoreList.size()-1; cutNum>0 && i>=0; i--)
		{
			CombinedEventItem* tempItem = dynamic_cast<CombinedEventItem*>(m_eventStoreList[i]);
			if( tempItem!=NULL && tempItem->getEventCategory() == eventCategory)
			{
				delete tempItem;
				tempItem = NULL;
				m_eventStoreList.erase(m_eventStoreList.begin()+i);
				cutNum --;
			}

		}

	}

	// find the specific worker and check the status, if it is FinishSearch, return true
	// If the worker doesn't exist, return true
	bool EventHandler::IsWorkerFinishedLoading(TA_Base_Core::EventCategory eventCategory){

		std::map<TA_Base_Core::EventCategory,IEventWorker*>::iterator workerIt;
		workerIt = m_workers.find(eventCategory);
		if(workerIt!= m_workers.end())
		{
			return  workerIt->second->getStatus() == FinishSearch ? true:false;
		}
		else
		{
			return true;
		}
	}


	void EventHandler::collectLastItemTimeForBothCategory(timeb & lastISCSItemsTime, timeb & lastATSItemsTime)
	{
		if(m_eventStoreList.size()==0) return;
		CombinedEventItem* tempItem = dynamic_cast<CombinedEventItem*>(m_eventStoreList.back());
		std::vector<DisplayItem *>::iterator itr = m_eventStoreList.end() -1 ;
		if ( tempItem->getEventCategory() == TA_Base_Core::ISCSCategory) 
		{
			lastISCSItemsTime = tempItem->getCreateTime();
			while ( itr != m_eventStoreList.begin() )
			{
				tempItem = dynamic_cast<CombinedEventItem*>( *itr );
				if ( tempItem->getEventCategory() == TA_Base_Core::ATSCategory)
				{
					lastATSItemsTime = tempItem->getCreateTime();
					return;
				}
				else 
				{
					itr --;
				}
			}

			// still need to check the first item because it is not included in the above loop
			tempItem = dynamic_cast<CombinedEventItem*>( m_eventStoreList.front() );
			if ( tempItem->getEventCategory() == TA_Base_Core::ATSCategory)
			{
				lastATSItemsTime = tempItem->getCreateTime();
			}

			return;
		}


		else
		{  
			lastATSItemsTime = tempItem->getCreateTime();
			while ( itr != m_eventStoreList.begin() )
			{
				tempItem = dynamic_cast<CombinedEventItem*>( *itr );
				if ( tempItem->getEventCategory() == TA_Base_Core::ISCSCategory)
				{
					lastISCSItemsTime = tempItem->getCreateTime();
					return;
				}
				else 
				{
					itr --;
				}
			}

			// still need to check the first item because it is not included in the above loop
			tempItem = dynamic_cast<CombinedEventItem*>( m_eventStoreList.front() );
			if ( tempItem->getEventCategory() == TA_Base_Core::ISCSCategory)
			{
				lastISCSItemsTime = tempItem->getCreateTime();
			}
			return;
		}
	}

	void EventHandler::cutLastOnesWithTimeStampSmallerThan (timeb timePoint)
	{
		TA_Base_Core::ThreadGuard guard1(m_storedlistLock);
		TA_Base_Core::ThreadGuard guard2(m_displaylistLock);

		//Finished //TODO: bug: wrong to cut the data at the same time point
		CombinedEventItem* tempItem = NULL;

		int cutNum =0;
		while (  m_eventStoreList.size()>0 )
		{
			tempItem = dynamic_cast<CombinedEventItem*>( m_eventStoreList.back());
			if ( tempItem != NULL && 
				 ( tempItem->getCreateTime().time < timePoint.time ||
				   ( tempItem->getCreateTime().time == timePoint.time && tempItem->getCreateTime().millitm < timePoint.millitm ) ) )
			{

				delete tempItem;
				tempItem =NULL;
				m_eventStoreList.pop_back();
				cutNum++;
			}
			else 
			{
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
					"******* Total %d events cutted for ATS & ISCS merging.******** ", cutNum);
				return; 
			}
		}
	}

	void EventHandler::cutLastOnesWithTimeStampBiggerThan (timeb timePoint)
	{
		TA_Base_Core::ThreadGuard guard1(m_storedlistLock);
		TA_Base_Core::ThreadGuard guard2(m_displaylistLock);

		//Finished //TODO: bug: wrong to cut the data at the same time point
		CombinedEventItem* tempItem = NULL;

		int cutNum =0;
		while (  m_eventStoreList.size()>0 )
		{
			tempItem = dynamic_cast<CombinedEventItem*>( m_eventStoreList.back());
			if ( tempItem != NULL && 
				 ( tempItem->getCreateTime().time > timePoint.time ||
				   (tempItem->getCreateTime().time == timePoint.time && tempItem->getCreateTime().millitm > timePoint.millitm ) ) )
			{

				delete tempItem;
				tempItem =NULL;
				m_eventStoreList.pop_back();
				cutNum++;
			}
			else 
			{
				// meet the another category, then stop cutting
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
					"******* Total %d events cutted for ATS & ISCS merging.******** ", cutNum);
				return; 
			}
		}
	}

    //This is only called in EventNotification,by worker thread
	//will only cut events when one side reach maximum length
	void EventHandler::findBothEndTimeAndCut(timeb & ISCSEndTime , timeb & ATSEndTime)
	{
		TA_Base_Core::ThreadGuard guard (m_storedlistLock);
		if(m_eventStoreList.size() <= 1) //there should be at least 2 items in the list
		{
			return;
		}

		// sort
		if(m_mode == EventDisplayMode::HistoricalMode)
		{
			resetSortingRuleForHist(EVDisplayStrRes::GetInstance()->COLUMN_DATETIME, true);
			std::stable_sort(m_eventStoreList.begin(), m_eventStoreList.end(), compare_events_for_Hist() );
		}else
		{
			resetSortingRuleForRealTime(EVDisplayStrRes::GetInstance()->COLUMN_DATETIME, true);
			std::stable_sort(m_eventStoreList.begin(), m_eventStoreList.end(), compare_events_for_RealTime() );
			
		}


		// after sorting, the last Item must have the biggest timestamp
		collectLastItemTimeForBothCategory(ISCSEndTime , ATSEndTime);

		// now cut if needed;
		// Finished //TODO: compair millitm if time is equal
		if( ISCSEndTime.time> ATSEndTime.time|| 
			(ISCSEndTime.time==ATSEndTime.time && ISCSEndTime.millitm > ATSEndTime.millitm ) )
		{
			if( ATSEventsReachMaxLength())
			{
				// then cut the last items in the list until meet another catogery's item:
				//cutTheLastOnesWithSameCategory();
				cutLastOnesWithTimeStampBiggerThan (ATSEndTime);
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "******* Some ISCS events cutted for merging. ats end time=%d, iscs end Time = %d******** ", ATSEndTime.time, ISCSEndTime.time);
			}
			
		}
		if( ISCSEndTime.time< ATSEndTime.time || 
			(ISCSEndTime.time==ATSEndTime.time && ISCSEndTime.millitm < ATSEndTime.millitm ))
		{
			if( ISCSEventsReachMaxLength())
			{
				// then cut the last items in the list until meet another catogery's item:
				//cutTheLastOnesWithSameCategory();
				cutLastOnesWithTimeStampBiggerThan (ISCSEndTime);
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "******* Some ATS events cutted for merging. ats end time=%d, iscs end Time = %d******** ", ATSEndTime.time, ISCSEndTime.time);

			}

		}

	}


	bool EventHandler::ATSEventsReachMaxLength()
	{
		int ATSMaxLength = EventHandlerManager::getMaxATSEvents();
		int ATSRealCount = 0;
		TA_Base_Core::ThreadGuard guard (m_storedlistLock);
		int listSize = m_eventStoreList.size();
		CombinedEventItem* tempItem = NULL;
		for (int i=0; i< listSize; i++)
		{
			tempItem = dynamic_cast<CombinedEventItem*>(m_eventStoreList[i]);
			if( tempItem!=NULL && tempItem->getEventCategory() == TA_Base_Core::ATSCategory )
			{
				ATSRealCount ++;
			}
		}
		if( ATSRealCount >= ATSMaxLength)
		{
			return true;
		}
		else
		{
			return false;
		}
	}
	bool EventHandler::ISCSEventsReachMaxLength()
	{
		int ISCSMaxLength = EventHandlerManager::getMaxISCSEvents();
		int ISCSRealCount = 0;
		TA_Base_Core::ThreadGuard guard (m_storedlistLock);
		int listSize = m_eventStoreList.size();
		CombinedEventItem* tempItem = NULL;
		for (int i=0; i< listSize; i++)
		{
			tempItem = dynamic_cast<CombinedEventItem*>(m_eventStoreList[i]);
			if( tempItem!=NULL && tempItem->getEventCategory() == TA_Base_Core::ISCSCategory )
			{
				ISCSRealCount ++;
			}
		}
		if( ISCSRealCount >= ISCSMaxLength)
		{
			return true;
		}
		else
		{
			return false;
		}
	}

	//will only cut events when one side reach maximum length
	void EventHandler::findBothStartTimeAndCut(timeb & ISCSStartTime , timeb & ATSStartTime)
	{

		TA_Base_Core::ThreadGuard guard1 (m_storedlistLock);
		
		if(m_eventStoreList.size() <= 1) //there should be at least 2 items in the list
		{
			return;
		}


		if(m_mode == EventDisplayMode::HistoricalMode) 
		{
			resetSortingRuleForHist(EVDisplayStrRes::GetInstance()->COLUMN_DATETIME, false);
			std::stable_sort(m_eventStoreList.begin(), m_eventStoreList.end(), compare_events_for_Hist() );
		}else
		{
			resetSortingRuleForRealTime(EVDisplayStrRes::GetInstance()->COLUMN_DATETIME, false);
			std::stable_sort(m_eventStoreList.begin(), m_eventStoreList.end(), compare_events_for_RealTime() );
		}
		


		// after sorting, the last Item must have the biggest timestamp
		collectLastItemTimeForBothCategory(ISCSStartTime , ATSStartTime);


		// now cut if needed
		// Finished //TODO: should compair millitm after compairing time
		if( ISCSStartTime.time>ATSStartTime.time || 
			(ISCSStartTime.time==ATSStartTime.time && ISCSStartTime.millitm > ATSStartTime.millitm ) )  
		{
			if( ISCSEventsReachMaxLength())
			{
				// then cut the last items in the list until meet another catogery's item:
				//cutTheLastOnesWithSameCategory();
				cutLastOnesWithTimeStampSmallerThan (ISCSStartTime);
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "******* Some ATS events cutted for merging. ats start time=%d, iscs start Time = %d******** ", ATSStartTime.time, ISCSStartTime.time);
			}
		}
		// now cut if needed
		if( ISCSStartTime.time < ATSStartTime.time|| 
			(ISCSStartTime.time==ATSStartTime.time && ISCSStartTime.millitm < ATSStartTime.millitm ) )  
		{
			if(ATSEventsReachMaxLength())
			{
				// then cut the last items in the list until meet another catogery's item:
				//cutTheLastOnesWithSameCategory();
				cutLastOnesWithTimeStampSmallerThan (ATSStartTime);
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "******* Some ISCS events cutted for merging. ats start time=%d, iscs start Time = %d******** ", ATSStartTime.time, ISCSStartTime.time);
			}
		}

	}


    //This is only called in EventNotification,by worker thread
	void EventHandler::truncateFor2Catogery( )
	{
		FUNCTION_ENTRY("truncateFor2Catogery");
		timeb t1, t2;

		if(m_mode == HistoricalMode && m_curFilterCriteria.searchDirection == TA_Base_App::Forward)
		{
			findBothEndTimeAndCut(t1 , t2);
			if(EventHandlerManager::getMaxATSEvents() + EventHandlerManager::getMaxISCSEvents() > EventHandlerManager::getMaxEvents() )
			{
				sortAndCutByMaxSizeIfNeeded(true);
			}

		}else
		{
			findBothStartTimeAndCut(t1 , t2);
			if(EventHandlerManager::getMaxATSEvents() + EventHandlerManager::getMaxISCSEvents() > EventHandlerManager::getMaxEvents() )
			{
				sortAndCutByMaxSizeIfNeeded(false);
			}
		}

		FUNCTION_EXIT;

	}


	void EventHandler::addCache( std::vector<DisplayItem*>& eventItems)
	{
		TA_Base_Core::ThreadGuard guard (m_storedlistLock);
		for(int i=0; i<eventItems.size(); i++)
		{
			m_eventStoreList.push_back(eventItems[i]);
		}
	}

	void EventHandler::transformAndAddCache( std::vector<TA_Base_Core::ICombinedEventData*>& eventDatas, TA_Base_Core::EventCategory eventCategory)
	{
		FUNCTION_ENTRY("transformAndAddCache");

		if (eventDatas.size() == 0) 
		{
			FUNCTION_EXIT;
			return;
		}


		TA_Base_Core::ThreadGuard guard (m_storedlistLock);

		TA_Base_Core::ICombinedEventData* tempData = NULL;

		for ( int i= 0; i<eventDatas.size(); i++) 
		{	
			try
			{
				tempData = eventDatas[i];
				//save into m_eventStoreList
				CombinedEventItem * newItem = new CombinedEventItem(tempData);
				newItem->setEventCategory(eventCategory) ;
				m_eventStoreList.push_back(newItem);	

				//release ICombinedEventData
				delete (tempData);
				tempData = NULL;
				eventDatas[i] = NULL;
			}
			catch ( ... )
			{
				// If there was any problem constructing the item don't add it to the display list.
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Exception caught when new CombinedEventItem from ICombinedEventData.");
				continue;
			}

		}

		FUNCTION_EXIT;

	}



	void EventHandler::clearAndReserveFor2Lists(unsigned long maxEvents){

		clearList();

		TA_Base_Core::ThreadGuard guard(m_storedlistLock);
		m_eventStoreList.reserve(maxEvents);
		TA_Base_Core::ThreadGuard displayGuard(m_displaylistLock);	
		m_displaylist.reserve(maxEvents);

	}

	//this function is only called by GUI thread
	void EventHandler::reverseDispListByGUI()
	{
	    TA_Base_Core::ThreadGuard guard (m_displaylistLock);
	 	std::reverse(m_displaylist.begin(), m_displaylist.end());
		
	}

	//This function is only called by GUI sorting(main thread)
	void EventHandler::sortDispListByGUI()
	{
		TA_Base_Core::ThreadGuard guard1 (m_displaylistLock);
		resetSortingRuleForGUI(DisplayItem::s_sortColumn, DisplayItem::s_sortAscending);
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "Before stable sorting, Event Count = %u", m_eventStoreList.size());
		std::stable_sort(m_displaylist.begin(), m_displaylist.end(), compare_events_For_GUI());
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "After stable sorting");
	}



	void EventHandler::sortAndSynchronizeList()
	{
		//FUNCTION_ENTRY("sortAndSynchronizeList");

	 	TA_Base_Core::ThreadGuard guard1 (m_storedlistLock);

		if(m_eventStoreList.size() == 0)
		{
			TA_Base_Core::ThreadGuard displayGuard(m_displaylistLock);  
			m_displaylist.clear();
			return;
		}


		if(m_mode == EventDisplayMode::HistoricalMode)
		{	

			resetSortingRuleForHist(DisplayItem::s_sortColumn, DisplayItem::s_sortAscending);
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "Before stable sorting, Event Count = %u", m_eventStoreList.size());
			std::stable_sort(m_eventStoreList.begin(), m_eventStoreList.end(), compare_events_for_Hist());

		}else
		{
			resetSortingRuleForRealTime(DisplayItem::s_sortColumn, DisplayItem::s_sortAscending);
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "Before stable sorting, Event Count = %u", m_eventStoreList.size());
			std::stable_sort(m_eventStoreList.begin(), m_eventStoreList.end(), compare_events_for_RealTime());
		}
		


		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "After stable sorting");

	 	TA_Base_Core::ThreadGuard displayGuard(m_displaylistLock);  
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "Before assigning");

	 	m_displaylist.clear();
		m_displaylist.reserve(m_eventStoreList.size());
	 	m_displaylist.assign(m_eventStoreList.begin(), m_eventStoreList.end());

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "After assigning");
		


		//FUNCTION_EXIT;
	}

	//this func is only called in realtime refresh notification
	//sorting may not be performed in this func, because if toBedelList has record, means m_eventStoreList has already been sorted by DateTime.
	void EventHandler::sortAndSynchronizeListAndDelTempList(vector<DisplayItem*> toBeDelList)
	{
		//FUNCTION_ENTRY("sortAndSynchronizeList");

		TA_Base_Core::ThreadGuard guard1 (m_storedlistLock);

		if(m_eventStoreList.size() == 0)
		{
			TA_Base_Core::ThreadGuard displayGuard(m_displaylistLock);  

			m_displaylist.clear();
			return;
		}


		//if toBeDelList.size>0, means m_eventStoreList is already been sorted on DateTime column, but may be ascending or descending
		if( toBeDelList.size()>0  )
		{
			if(DisplayItem::s_sortColumn == EVDisplayStrRes::GetInstance()->COLUMN_DATETIME)  //UI sorting column is the same as m_eventStoreList sorting column
			{
				if ( s_sortAscendingForRealTime == DisplayItem::s_sortAscending)
				{
					//no need to do sorting again.
				}else
				{
					//just reverse m_eventStoreList
					s_sortAscendingForRealTime = DisplayItem::s_sortAscending;
					std::reverse(m_eventStoreList.begin(), m_eventStoreList.end());
				}
			}
			else //UI sorting column is not the same as m_eventStoreList sorting column
			{
				resetSortingRuleForRealTime(DisplayItem::s_sortColumn, DisplayItem::s_sortAscending);
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "Before stable sorting, Event Count = %u", m_eventStoreList.size());
				std::stable_sort(m_eventStoreList.begin(), m_eventStoreList.end(), compare_events_for_RealTime());  
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "After stable sorting");
			}

		}
		else // toBeDelList.size()==0) m_eventStoreList no need to cut, and it's already sorted on "compare_events_for_RealTime"
		{
			if( s_sortColumnForRealTime == DisplayItem::s_sortColumn ) //if m_eventStoreList is sorting on the same column as UI, 
			{
				if(s_sortAscendingForRealTime == DisplayItem::s_sortAscending)
				{
					//no need to do sorting again.
				}
				else
				{
					//just reverse m_eventStoreList
					s_sortAscendingForRealTime = DisplayItem::s_sortAscending;
					std::reverse(m_eventStoreList.begin(), m_eventStoreList.end());
				}
			}
			else  //if m_eventStoreList is sorting on the different column with UI
			{
				resetSortingRuleForRealTime(DisplayItem::s_sortColumn, DisplayItem::s_sortAscending);
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "Before stable sorting, Event Count = %u", m_eventStoreList.size());
				std::stable_sort(m_eventStoreList.begin(), m_eventStoreList.end(), compare_events_for_RealTime());
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "After stable sorting");
			}
		}


		TA_Base_Core::ThreadGuard displayGuard(m_displaylistLock); 
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "Before assigning");
		m_displaylist.clear();
		m_displaylist.reserve(m_eventStoreList.size());
		m_displaylist.assign(m_eventStoreList.begin(), m_eventStoreList.end());

		clearToBeDelList(toBeDelList);

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "After assigning");



		//FUNCTION_EXIT;
	}
	

	bool EventHandler::IsAllTheWorkerFinishedLoading()
	{
		std::map<TA_Base_Core::EventCategory,IEventWorker*>::iterator workerIt;
		for(workerIt=m_workers.begin(); workerIt!=m_workers.end(); workerIt++)
		{
			if( NULL != workerIt->second && workerIt->second->getStatus() == InSearching)
			{
				return false;
			}
		}	

		return true;
	}

	unsigned long EventHandler::getDislplaySize()
	{
		
		TA_Base_Core::ThreadGuard guard (m_displaylistLock);
		return m_displaylist.size();
	}



	std::string EventHandler::getCreateTimeStrAt(unsigned long pos)
	{
		TA_Base_Core::ThreadGuard guard(m_displaylistLock);

		if( pos<= m_displaylist.size())
		{
			return static_cast<CombinedEventItem*>( m_displaylist[pos])->getCreateTimeStr().c_str();
		}else
		{
			return "";
		}
	}


	bool EventHandler::currentListWasCutted()
	{
		unsigned long ISCSLength = 0;
		unsigned long ATSLength = 0;

		if( m_workers.find(TA_Base_Core::ISCSCategory) != m_workers.end())
		{
			ISCSLength = getSizeByCategory( TA_Base_Core::ISCSCategory);
			if( ISCSLength >= EventHandlerManager::getMaxISCSEvents())
			{
				return true;
			}
		}
		if ( m_workers.find(TA_Base_Core::ATSCategory) != m_workers.end())
		{
			ATSLength = getSizeByCategory( TA_Base_Core::ATSCategory);
			if( ATSLength >= EventHandlerManager::getMaxATSEvents())
			{
				return true;
			}
		}

		if( (ISCSLength + ATSLength) >= EventHandlerManager::getMaxEvents())
		{
			return true;
		}
		
		return false;
	}

	//this is always called by GUI thread
	time_t EventHandler::getEarliestTimeFromList()
	{
		TA_Base_Core::ThreadGuard guard1 (m_displaylistLock);

		std::vector<DisplayItem*>::iterator itr;

		resetSortingRuleForGUI(EVDisplayStrRes::GetInstance()->COLUMN_DATETIME, true);
		itr = std::min_element(m_displaylist.begin(), m_displaylist.end(),compare_events_For_GUI() );
	

		if(itr!=m_displaylist.end())
		{
			CombinedEventItem* tempItem = dynamic_cast<CombinedEventItem*>(*itr);
			return tempItem->getCreateTime().time;
		}
		else
		{
			return 0;
		}

	}

	//this is always called by GUI thread
	time_t EventHandler::getLatestTimeFromList()
	{
		TA_Base_Core::ThreadGuard guard1 (m_displaylistLock);

		std::vector<DisplayItem*>::iterator itr;

		resetSortingRuleForGUI(EVDisplayStrRes::GetInstance()->COLUMN_DATETIME, true);
		itr = std::max_element(m_displaylist.begin(), m_displaylist.end(),compare_events_For_GUI() );
		

		if(itr!=m_displaylist.end())
		{
			CombinedEventItem* tempItem = dynamic_cast<CombinedEventItem*>(*itr);
			return tempItem->getCreateTime().time;
		}
		else
		{
			return 0;
		}


	}

	void EventHandler::setBothPkeyRightBoundary(FilterCriteria & filterCriteria) 
	{
		// use eventStoreList to find the pkey right boundary
		TA_Base_Core::ThreadGuard guard (m_storedlistLock);

		CombinedEventItem* tempItem = NULL;
		unsigned long tempKey = 0; 
		std::string tempATSEventID = "";
		/*unsigned*/ long tempTime_t = 0;

		// For ISCS:
		if( m_workers.find(TA_Base_Core::ISCSCategory) != m_workers.end())
		{
			// set ISCS biggest pkey to filterCriteria:
			for(int i=0; i<(int)m_eventStoreList.size(); i++)
			{
				tempItem = dynamic_cast<CombinedEventItem*>(m_eventStoreList[i]);
				if(tempItem!=NULL && tempItem->getEventCategory() == TA_Base_Core::ISCSCategory 
					)
				{
					if( tempTime_t < tempItem->getCreateTime().time || (tempTime_t == tempItem->getCreateTime().time &&tempItem->getKey()> tempKey)) 
					{
						tempKey = tempItem->getKey();
						tempTime_t = tempItem->getCreateTime().time;
					}
					
				}
			}
			filterCriteria.pkeyBoundary_ISCS = tempKey;
		}
		else
		{
			filterCriteria.pkeyBoundary_ISCS = 0;
		}

		//For ATS
		if( m_workers.find(TA_Base_Core::ATSCategory) != m_workers.end())
		{
			tempKey = 0;
			// set ISCS biggest pkey to filterCriteria:
			for(int i=0; i<(int)m_eventStoreList.size(); i++)
			{
				tempItem = dynamic_cast<CombinedEventItem*>(m_eventStoreList[i]);
				if(tempItem!=NULL && tempItem->getEventCategory() == TA_Base_Core::ATSCategory 
					&& tempItem->getKey()> tempKey)
				{
					tempKey = tempItem->getKey();
					tempATSEventID = tempItem->getATSEventID();
				}

			}
			filterCriteria.pkeyBoundary_ATS = tempATSEventID;
		}
		else
		{
			filterCriteria.pkeyBoundary_ATS = "";
		}
		
	}

	void EventHandler::setBothPkeyLeftBoundary(FilterCriteria & filterCriteria) 
	{
		// use eventStoreList to find the pkey right boundary
		TA_Base_Core::ThreadGuard guard (m_storedlistLock);

		CombinedEventItem* tempItem = NULL;
		unsigned long tempKey = 0; 
		/*unsigned*/ long tempTime_t =0;
		std::string tempATSEventID = "";

		// For ISCS:
		if( m_workers.find(TA_Base_Core::ISCSCategory) != m_workers.end())
		{
			// set ISCS smallest pkey to filterCriteria:
			for(int i=0; i<(int)m_eventStoreList.size(); i++)
			{
				tempItem = dynamic_cast<CombinedEventItem*>(m_eventStoreList[i]);
				
				if ( tempKey ==0 && tempItem!=NULL 
					&& tempItem->getEventCategory() == TA_Base_Core::ISCSCategory )
				{
					tempKey = tempItem->getKey();
					tempTime_t = tempItem->getCreateTime().time;
				}
				else if( tempKey!=0 &&  tempItem!=NULL && tempItem->getEventCategory() == TA_Base_Core::ISCSCategory 
					  /*tempItem->getKey()< tempKey*/ )
				{
					if (tempTime_t> tempItem->getCreateTime().time || (tempTime_t == tempItem->getCreateTime().time &&tempItem->getKey()< tempKey  ))
					{
						tempKey = tempItem->getKey();
						tempTime_t = tempItem->getCreateTime().time;
					}
					
				}
			}
			filterCriteria.pkeyBoundary_ISCS = tempKey;
		}
		else
		{
			filterCriteria.pkeyBoundary_ISCS = 0;
		}

		//For ATS
		if( m_workers.find(TA_Base_Core::ATSCategory) != m_workers.end())
		{
			tempKey = 0;
			// set ATS "smallest" ID to filterCriteria:
			for(int i=0; i<(int)m_eventStoreList.size(); i++)
			{
				tempItem = dynamic_cast<CombinedEventItem*>(m_eventStoreList[i]);
				if ( tempKey ==0 && tempItem!=NULL 
					&& tempItem->getEventCategory() == TA_Base_Core::ATSCategory )
				{
					tempKey = tempItem->getKey();
					tempATSEventID = tempItem->getATSEventID();
				}
				if(tempKey !=0 && tempItem!=NULL && tempItem->getEventCategory() == TA_Base_Core::ATSCategory 
					 && tempItem->getKey()< tempKey)
				{
					tempKey = tempItem->getKey();
					tempATSEventID = tempItem->getATSEventID();
				}

			}
			filterCriteria.pkeyBoundary_ATS = tempATSEventID;
		}
		else
		{
			filterCriteria.pkeyBoundary_ATS = "";
		}


	}


};

