/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source: $File: //depot/3001_TIP/TA_BASE/transactive/app/event/event_viewer/src/ISCSRealTimeWorker.h $
  * @author huirong.luo
  * @version $Revision: #1 $
  *
  * Last modification: $DateTime: 2010/09/15 8:54:40 $
  * Last modified by: xiangmei.lu
  * 
  * ISCSRealTimeWorker is ...
  *
  */

#if !defined(EA_06F6169A_2F42_41b0_968E_CA45E80573FC__INCLUDED_)
#define EA_06F6169A_2F42_41b0_968E_CA45E80573FC__INCLUDED_

#include "IEventWorker.h"
#include "IEventWorkObserver.h"
#include "core/data_access_interface/src/CombinedEventAccessFactory.h"

namespace TA_Base_App{

	class ISCSRealTimeWorker : public IEventWorker
	{

	public:
		ISCSRealTimeWorker(IEventWorkObserver* observer);
		virtual ~ISCSRealTimeWorker();
		/*virtual void run();*/
		virtual void doCurrentTask();
		virtual void terminate();
		
		/*void populatePublicFilter(FilterCriteria filterCriteria, TA_Base_Core::CombinedEventAccessFactory::EventFilter& filter, bool refresh = false );*/


		//Load back log length of ISCS real time events. Also including notifying the observer:
		//if return false, means by the current filter, it will never get any result from ISCS, so no need query in DB in this case
		//(e.g. when all the system/subsystem user choosed is belong to ATS only)
		bool loadBacklog(FilterCriteria tempFilter,long taskID);

		int submitLoadedEventAndGetNextEvents(long taskID, std::vector<TA_Base_Core::ICombinedEventData*> events, bool loadingFinished);

		//Load refresh list of ISCS real time events. Also including notifying the observer:
		void loadRefreshList(std::vector<TA_Base_Core::ICombinedEventData*> & events, long taskID);


		void loadRefreshList_Test(std::vector<TA_Base_Core::ICombinedEventData*> & events, long taskID);

// 		// reset stop mark as false 
// 		void resetStopMark();

// 
// 		bool m_stopCurrentTask;

		// ISCS Auot-refresh Rate , read from DB: EntityParameter table, "EventRefrshTime", 
		// Default value: 5000 (ms)
		unsigned long m_refreshRate;  
		TA_Base_Core::NonReEntrantThreadLockable m_stopCurrentTaksLock;

		unsigned long m_receiveRefreshSize ;
		
		static const unsigned long REALTIME_LOAD_BATCH = 20000;

		//for testing purpose
		void setNeedToTerminated(bool val){m_needToTerminate = val;};

	};
};

#endif // !defined(EA_06F6169A_2F42_41b0_968E_CA45E80573FC__INCLUDED_)
