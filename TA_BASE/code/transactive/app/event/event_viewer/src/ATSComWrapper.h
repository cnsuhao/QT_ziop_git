#pragma once
#include "MySink.h"
#include "app/event/event_viewer/pmodel/src/FilterCriteria.h"
#include <vector>
#include <map>
#include "core/data_access_interface/src/ICombinedEventData.h"
#include "app/event/event_viewer/pmodel/src/ActionCommand.h"
#include "atltime.h"

#include  "app/event/event_viewer/pmodel/src/IATSComWrapper.h"


//#import "../../../../../../bin/win32_nd/ATSBridgeUtil.dll" named_guids
#ifdef _DEBUG
#import "../../../../../../../sdk/win32_nd/ATSBridgeUtil.tlb" named_guids
#else
#import "../../../../../../sdk/win32_nr/ATSBridgeUtil.tlb" named_guids
#endif


using namespace ATSBridgeUtilLib;

using namespace TA_Base_Core;


namespace
{
	// Alarm State:
	const std::string ALARM_CLOSED("AlarmClosed");
	const std::string ALARM_OPENED("AlarmOpened");
	const std::string ALARM_NORMALISED("AlarmNormalised");
	const std::string ALARM_ACKNOWLEDGED("AlarmAcknowledged");
	const std::string ALARM_SUPPRESSED("AlarmSuppressed");
}

namespace TA_Base_App
{

	/*struct ATSSystemItem
	{
		ATSSystemItem(int k, std::string s) : pkey(k), systemName(s) {}


		bool operator<(const ATSSystemItem& rhs)
		{
			return systemName.compare(rhs.systemName) < 0;
		}

		int	pkey;
		std::string		systemName;
		std::vector<std::pair<int , std::string >>  subsystemVector;

	};*/  ////////////--- moved to Pmodel interface

	class ATSComWrapper:public IATSComWrapper
	{	
	public:
		ATSComWrapper(void);
		~ATSComWrapper(void);

		ActionCommand startATSSubscription(
			FilterCriteria filterCriteria,
			const unsigned int backloglength,				
			std::vector<TA_Base_Core::ICombinedEventData*>& events);

		ActionCommand  doStartTimeEndTimeQuery(std::vector<TA_Base_Core::ICombinedEventData*> & events, 
			FilterCriteria filterCriteria, unsigned long atsMaxLength);

		ActionCommand doPreviousPageQuery(std::vector<TA_Base_Core::ICombinedEventData*> & events, 
			FilterCriteria filterCriteria, unsigned long atsMaxLength);

		ActionCommand doNextPageQuery(std::vector<TA_Base_Core::ICombinedEventData*> & events, 
			FilterCriteria filterCriteria, unsigned long atsMaxLength);

		int initATSSubscriber();

		void transformToATSFilter(IATSFilterCriteria * pATSFilter, FilterCriteria & filterCriteria );
		void setSystemNameToATSFilter (IATSFilterCriteria *pATSFilter,FilterCriteria & filterCriteria);
		void setSubsystemNameToATSFilter (IATSFilterCriteria *pATSFilter,FilterCriteria & filterCriteria);
		void setOperatorToATSFilter (IATSFilterCriteria *pATSFilter,FilterCriteria & filterCriteria);
		void setLocationToATSFilter (IATSFilterCriteria *pATSFilter,FilterCriteria & filterCriteria);
		void setAssetToATSFilter (IATSFilterCriteria *pATSFilter,FilterCriteria & filterCriteria);
		void setDescriptionToATSFilter (IATSFilterCriteria *pATSFilter,FilterCriteria & filterCriteria);
		void setAlarmSeverityToATSFilter(IATSFilterCriteria *pATSFilter,FilterCriteria & filterCriteria);
		void setAlarmStateToATSFilter(IATSFilterCriteria *pATSFilter,FilterCriteria & filterCriteria);


		void  ATSComWrapper::setStartEndTimeToATSFilter ( ATSBridgeUtilLib::IATSFilterCriteria * atsfilter, 
			FilterCriteria & filterCriteria);

/*		void adjustATSFilterTime(ATSBridgeUtilLib::IATSFilterCriteria *  atsfilter);*/

		void setATSFilterStartTimeByCTime(ATSBridgeUtilLib::IATSFilterCriteria *  atsfilter, CTime time);
		void setATSFilterEndTimeByCTime(ATSBridgeUtilLib::IATSFilterCriteria *  atsfilter, CTime time);

		void setTMTypeToATSFilter(ATSBridgeUtilLib::IATSFilterCriteria * atsfilter, FilterCriteria & filterCriteria);
		void transformToISCSEvents (IATSEventList * pATSEvents, 
			std::vector<TA_Base_Core::ICombinedEventData*>& events , unsigned long lastKey = 0);

		ICombinedEventData * createCombinedEvent ( IATSEvent* pATSEvent, unsigned long key);

		ActionCommand stopATSSubscription(void);

		void truncateATSEvents (IATSEventList * pATSEvents, unsigned long atsMaxLength, bool cutSmallTime);

		virtual void cleanUp();
		virtual void doComInitialize();

		//void restartATSSubscriptionForRefresh();

		virtual int loadATSSystemsInfo( );
		virtual vector<ATSSystemItem> getATSSystemStructureVector();

		vector<std::string> getATSSystemsNames();
	/*	vector<std::string> getATSSubsystems();*/
	/*	vector<std::string> getATSSubsystemsBySystemNames( std::vector<std::string> sysNames);*/

		//ATSComWrapper* getInstanceRealTime();
		//void freeInstanceRealTime();
		//ATSComWrapper* getInstanceHistorical();
		//void freeInstanceHistorical();


	private:
		
		//ATSComWrapper * m_instance_realTime;
		//ATSComWrapper * m_instance_hist;

		bool m_ATS_func_debug_level;
		CComPtr<IATSEventSubscription> m_pSubscriber;
		IConnectionPoint* m_pCP;
		CMySink *m_pSink;
		IUnknown *m_pSinkUnk;

		vector<std::string> m_systemsNames;
		/*vector<std::pair<std::string,std::string>> m_subsystems;*/
		std::vector<ATSSystemItem> m_systemStructureVector;

		 DWORD m_dwAdvise;

		
	};

};
