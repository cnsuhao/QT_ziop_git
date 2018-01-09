/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP/TA_BASE/transactive/app/event/event_viewer/src/ISCSRealTimeWorker.cpp $
  * @author:  huirong.luo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2010/09/15 8:54:40 $
  * Last modified by: xiangmei.lu
  * 
  * ISCSRealTimeWorker is ...
  *
  */

//#include "stdafx.h"
#include "ISCSRealTimeWorker.h"
#include "app/event/event_viewer/pmodel/src/ConstantItems.h"
#include "EventHandlerManager.h"


#include "bus/TA_MessageResource/resource.h"
//#include "bus/generic_gui/src/TransActiveMessage.h"
// 
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/data_access_interface/entity_access/src/EntityHelper.h"
#include "core/data_access_interface/src/ICombinedEventData.h"
#include "ISCSWorkerHelper.h"
#include "core/data_access_interface/src/CombinedEventData.h"
#include "app/event/event_viewer/pmodel/src/NotifyGUIPostMessage.h"



namespace TA_Base_App{


	ISCSRealTimeWorker::ISCSRealTimeWorker(IEventWorkObserver* observer): IEventWorker(observer){

		// set refresh Rate:
		m_refreshRate = EventHandlerManager::getRefreshRate();
		m_receiveRefreshSize = 0;

	}

	ISCSRealTimeWorker::~ISCSRealTimeWorker()
	{
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "~ISCSRealTimeWorker() finished.");
	}

	// before this function called. m_status was just set as InSearching
	void ISCSRealTimeWorker::doCurrentTask()
	{
		// start loading step :
		bool needtoQuery = loadBacklog(m_filter, m_curTaskSeqID);  // after this function, m_status will be set as FinishSearch or EndButNotFinishSearch

		sleep(500);

		if( getStatus() == FinishSearch && needtoQuery )
		{
			// start to refresh step :
			std::vector<TA_Base_Core::ICombinedEventData*> events;
			while( !m_needToTerminate && !IsThisTaskStopped(m_curTaskSeqID) ){
				loadRefreshList(events, m_curTaskSeqID);
				sleep(m_refreshRate);     //default :5s

				// TODO: Just for testing! should change back to front 2 line.
// 				loadRefreshList_Test(events, m_curTaskSeqID);
// 				sleep (5000);
			}
		}

	}


	void ISCSRealTimeWorker::terminate(){

		//stopCurrentTask();
		m_needToTerminate = true;

		LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugInfo,"ISCSRealTimeWorker terminated");

	}

	void ISCSRealTimeWorker::loadRefreshList_Test(std::vector<TA_Base_Core::ICombinedEventData*> & events, long taskID){
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugTrace, "Start loading new refresh events");

		try{
			bool enableVisibilityRule = TA_Base_Core::RunParams::getInstance().isSet(EventViewerConstants::RPARAM_NO_VISIBILITY_RULE.c_str()) ? false : true;

			std::vector<TA_Base_Core::ICombinedEventData*> eventsdd;
			events = eventsdd;
			for (int i=0; i<10; i++)
			{
				// Get the source table ID and convert it to an event type.
				TA_Base_Core::ECombinedEventType eventType = TA_Base_Core::E_AUDIT_EVENT ;

				// Get the pkey for the event.
				unsigned long pkey = i+1;

				// Get the time the event was created.
				struct timeb currentTime;
				ftime(&currentTime);
				timeb createTime = currentTime;

				// Get the severity name.
				char severityName [2];
				//itoa(i%3+1, severityName, 10);;
				_itoa_s(i%3+1, severityName, 10);

				// Get the asset name.
				std::string assetName = "Test_ISCS_Asset";

				// Get the description.
				std::string description = "Test_ISCS_Discription";
				std::string eventTypeName = "";

				std::string alarmStatusEventType = "AlarmOpened";    // add this for column "Alarm Status"


				// Get the value.
				std::string value = "Test_ISCS_Value";

				// Get the MMS state information.
				std::string mmsState = "";


				// Get the DSS state information.
				std::string dssState = "";

				std::string avlState = "";

				// Get the operator name.
				std::string operatorName = "Test_ISCS_Operator";

				// Get the alarm comment.
				std::string alarmComment = "";

				// Get the alarm ID.
				std::string alarmId = "";

				// Get the alarm ack.
				std::string alarmAck = "";

				// Get the alarm status.
				std::string alarmStatus = "";

				std::string operation_mode = "";

				TA_Base_Core::ICombinedEventData * newEvent =  new TA_Base_Core::CombinedEventData( eventType, pkey, createTime, severityName, assetName, description,
					value, mmsState, dssState, avlState, operatorName, alarmComment, alarmId, alarmAck, alarmStatus, alarmStatusEventType, operation_mode,TA_Base_Core::ISCSCategory);

				events.push_back(newEvent);

			}

			if(!m_needToTerminate && !IsThisTaskStopped(taskID)){
				m_wokerObserver->EventNotification(TA_Base_Core::ISCSCategory, events, TA_Base_App::NoCommand, taskID);
			}

			m_receiveRefreshSize += events.size();
			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Task(%d)ISCS Real-time REFRESH: Found %d new elements. Has received totally %d refreshing events",taskID, events.size(), m_receiveRefreshSize );

		}	
		catch(...)
		{
			// If there's a problem (any problem), notify the user, then return 0.
			NotifyGUIPostMessage::getInstance().showUserMsg(IDS_UE_040030);
			LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugWarn,"Database load was not successful");
			m_wokerObserver->EventNotification(TA_Base_Core::ISCSCategory, events, TA_Base_App::Reconnect, taskID);
		}

	}


	void ISCSRealTimeWorker::loadRefreshList(std::vector<TA_Base_Core::ICombinedEventData*> & events, long taskID){
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugTrace, "Start loading new refresh events");

		try{
			bool enableVisibilityRule = TA_Base_Core::RunParams::getInstance().isSet(EventViewerConstants::RPARAM_NO_VISIBILITY_RULE.c_str()) ? false : true;

			events = TA_Base_Core::CombinedEventAccessFactory::getInstance().getDesignatedEventsSinceLastLoad(enableVisibilityRule);

			if(!m_needToTerminate && !IsThisTaskStopped(taskID)){
				m_wokerObserver->EventNotification(TA_Base_Core::ISCSCategory, events, TA_Base_App::NoCommand, taskID);
			}
			
			m_receiveRefreshSize += events.size();
			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Task(%d)ISCS Real-time REFRESH: Found %d new elements. Has received totally %d refreshing events",taskID, events.size(), m_receiveRefreshSize );

		}	
		catch(...)
		{
			// If there's a problem (any problem), notify the user, then return 0.
			NotifyGUIPostMessage::getInstance().showUserMsg(IDS_UE_040030);
			LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugWarn,"Database load was not successful");
			m_wokerObserver->EventNotification(TA_Base_Core::ISCSCategory, events, TA_Base_App::Reconnect, taskID);
		}
		
	}

	// return -1 means: this task should be terminate
	int ISCSRealTimeWorker::submitLoadedEventAndGetNextEvents(long taskID, std::vector<TA_Base_Core::ICombinedEventData*> events, bool loadingFinished)
	{
		int nextEventsTotal =0;
		if(!m_needToTerminate && !IsThisTaskStopped(taskID)){
			m_wokerObserver->EventNotification(TA_Base_Core::ISCSCategory, events, TA_Base_App::NoCommand,taskID , true);
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Task(%d)ISCS Real-time backlog loading, get first %d events.",taskID,events.size());
			//m_wokerObserver->preloadListView();

			// then, collect all the rest events together and notify observer:
			std::vector<TA_Base_Core::ICombinedEventData*> restEvents;
			while (!events.empty())
			{
				if(!m_needToTerminate && !IsThisTaskStopped(taskID))
				{
					events = TA_Base_Core::CombinedEventAccessFactory::getInstance().getNextEvents();
					//LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Task(%d)ISCS Real-time backlog loading, getNextEvents() return %d events.",taskID,events.size());
					restEvents.reserve(restEvents.size() + events.size()); 
					restEvents.insert(restEvents.end(), events.begin(), events.end());
				}
				else
				{
					setStatus(EndButNotFinishSearch);
					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Task(%d)ISCS Real-time backlog loading stops.(Loading end but not finished)",taskID);
					return -1;
				}

			}

			nextEventsTotal = nextEventsTotal + restEvents.size();

			if( !m_needToTerminate && !IsThisTaskStopped(taskID) )
			{
				// here means loading step finished!
				if(!loadingFinished)
				{
					m_wokerObserver->EventNotification(TA_Base_Core::ISCSCategory, restEvents, TA_Base_App::NoCommand,taskID, true, false);
					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Task(%d)ISCS Real-time backlog loading, get rest %d events.",taskID,restEvents.size());
					return nextEventsTotal;
				}
				else
				{
					setStatus(FinishSearch);
					m_wokerObserver->EventNotification(TA_Base_Core::ISCSCategory, restEvents, TA_Base_App::NoCommand,taskID, true, true);
					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Task(%d)ISCS Real-time backlog loading, get rest %d events.",taskID,restEvents.size());
					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Task(%d)ISCS Real-time backlog loading stops.",taskID);
					return nextEventsTotal;
				}

			}
			else
			{
				setStatus(EndButNotFinishSearch);
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Task(%d)ISCS Real-time backlog loading stops(Loading end but not finished).",taskID);
				return -1;
			}

		}
		else
		{
			setStatus(EndButNotFinishSearch);
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Task(%d)ISCS Real-time backlog loading stops(Loading end but not finished).",taskID);
			return -1;
		}

		return nextEventsTotal;
	}

	// This function will set status to FinisheSearch(if all the (backlog)event has been loaded) 
	// or EndButNotFinishSearch( this task is stopped before all the (backlog) events loaded )
	bool ISCSRealTimeWorker::loadBacklog(FilterCriteria filterCriteria, long taskID)
	{
		bool res = true;

		std::vector<TA_Base_Core::ICombinedEventData*> events;

		// for some situation that will load nothing:
		// e.g. User selected ATS system but no ISCS system, or user selected ATS subsystem but no ISCS subystem
		if ( (m_filter.SystemNameEqualOr.size()!=0 || m_filter.SubsystemNameEqualOr.size() != 0 ) &&
			m_filter.appSubsystemIds.size() ==0 &&  m_filter.physSubsystemIds.size()==0 )
		{
			res = false;
			// here means loading step finished!
			setStatus(FinishSearch);
			m_wokerObserver->EventNotification(TA_Base_Core::ISCSCategory, events, TA_Base_App::NoCommand,taskID, true, true);
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Task(%d)ISCS Real-time backlog loading, get 0 events.",taskID);
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Task(%d)ISCS Real-time backlog loading stops.",taskID);
			return res;
		}

		TA_Base_Core::CombinedEventAccessFactory::EventFilter filter;
		ISCSWorkerHelper::populatePublicFilter(filterCriteria, filter);

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Task(%d)ISCS Real-time backlog loading starts.", taskID);
		try{
			bool enableVisibilityRule = TA_Base_Core::RunParams::getInstance().isSet(EventViewerConstants::RPARAM_NO_VISIBILITY_RULE.c_str()) ? false : true;

			unsigned long queryEndPkey = TA_Base_Core::CombinedEventAccessFactory::getInstance().getMaxEventPKey();
			unsigned long queryStartPkey=0;
			std::string firstPartSqlCache="";
			unsigned long hasLoadedSize = 0;
			unsigned long maxSize = EventHandlerManager::getMaxISCSEvents();

			for(int i=0; i<3 && !m_needToTerminate && !IsThisTaskStopped(taskID) ; i++)  // divide loading into 3(maximum) phrase , each phrase query in REALTIME_LOAD_BATCH events.
			{
				
				queryStartPkey = (queryEndPkey - REALTIME_LOAD_BATCH)>0 ? (queryEndPkey - REALTIME_LOAD_BATCH):0 ;

				events = TA_Base_Core::CombinedEventAccessFactory::getInstance().
					loadDesignatedEventsInASection(queryStartPkey,queryEndPkey, firstPartSqlCache, enableVisibilityRule, filter, maxSize -hasLoadedSize);
				hasLoadedSize+= events.size();

				if(hasLoadedSize>=maxSize) //has loaded max size of events just by "first" return events(even without "getNext")
				{
					submitLoadedEventAndGetNextEvents(taskID, events,true);
					return res;
				}else // hasn't reach max size of events by the "first" return events by sql
				{	
					int nextEventsTotal = submitLoadedEventAndGetNextEvents(taskID, events, false);
					if(nextEventsTotal == -1 ) return res; // task has been terminated
					hasLoadedSize += nextEventsTotal; 

					if(hasLoadedSize>=maxSize || queryEndPkey <= REALTIME_LOAD_BATCH) // if max size reached or there's no more events to be loaded
					{
						std::vector<TA_Base_Core::ICombinedEventData*> emptyEvents;
						submitLoadedEventAndGetNextEvents(taskID,emptyEvents, true );  //notify this loading has been finished.
						return res; // has a bug: when seq meets the max
					}
					else //still has events to be loaded
					{
						queryEndPkey = queryEndPkey - REALTIME_LOAD_BATCH;  
						continue;
					}				
				}
			}

			//if hasLoadedSize<maxSize, need to load once again:
			if( hasLoadedSize<maxSize )
			{
				events = TA_Base_Core::CombinedEventAccessFactory::getInstance().
					loadDesignatedEventsInASection( 0, queryEndPkey, firstPartSqlCache, enableVisibilityRule, filter, maxSize -hasLoadedSize);
				hasLoadedSize+= events.size();
				submitLoadedEventAndGetNextEvents(taskID, events,true);
				return res;
			}
			
		}
		catch(...)
		{
			// If there's a problem (any problem), notify the user, then return 0.
			NotifyGUIPostMessage::getInstance().showUserMsg(IDS_UE_040030);
			LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugWarn,"Database load was not successful");
			m_wokerObserver->EventNotification(TA_Base_Core::ISCSCategory, events, TA_Base_App::Reconnect, taskID);
		}	
		
		return res;
	}

};

