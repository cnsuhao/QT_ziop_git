#pragma once


#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"
#include "DatabaseCache.h"
#include "Filter.h"
#include "AlarmVectorWrap.h"
#include "SimpleDispMultiIndex.h"
#include "Sort.h"
#include "AlarmDataSync.h"
#include "RightsChecker.h"

namespace TA_Base_Bus
{
	class AlarmStore;
    class AlarmListModel :
        public TA_Base_Bus::AbstractPModel
    {
    public:
        AlarmListModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
        virtual ~AlarmListModel(void);


		/**
		* notifyUpdate
		* get notification of alarm store 		 
		*/
		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL);
 
		void connectToAlarmStore(const Filter& preFilter,const Filter& filter,const std::vector<unsigned long>& locations,
			const std::string& sessionId, Sort sort,bool isAscendingSort,bool isHighestSeverityNeeded);
   
		/**
		* getCurrentFilter
		*
		* This will retrieve the current filter the list is using. This must be called after a successful
		* call to startAlarmList().
		*
		* @return Filter - The current filter being used
		*/
		Filter getCurrentFilter() const;

		/**
		* setFilter
		*
		* This sets a new filter on the alarm store. This must be called after a successful
		* call to startAlarmList().
		*
		* @param Filter - The new filter to set
		*
		* @exception AlarmListCtrlException - Thrown if the filter is invalid
		*/
		void setFilter(const Filter& filter,bool isDefault);

		unsigned long getAlarmSeverityColour(unsigned long key, TA_Base_Core::IAlarmSeverityData::EColourType type);

		 
        void setDisplaySort(SORT_COLUMN sortType, bool bSortAccending);
 
 
		void defaultSort();

		void updatePreFilterLocationsAndSession( const Filter& preFilter,
			const std::vector<unsigned long>& locations,
			const std::string& sessionID );

		void reSetPreFilterLocationAndSession();
		/**
		* setListDisplayInfo
		* 
		* Retrieve displaying data item from alarm store via shared memory
		* 
		*/
		void setListDisplayInfo(ListDispInfo& listInfo);
        TA_Base_Core::IAlarmStoreCorbaDef::IndexInfo& getIndexInfo();
		/**
		* getDisplayDataWrap
		* 
		* get the display data wrap class
		* 
		*/
		AlarmVectorWrap* getDisplayDataWrap();
        AlarmSelectionWrap* getSelectionWrapper();
	    AlarmDataSync*    getAlarmDataSync();

	    bool isUpdating();

		/**
		* getRightsChecker
		*
		* This will return the object that allows rights to be checked
		*
		* @return RightsChecker& - The object to use to determine if the user can perform a certain action
		*/
		virtual RightsChecker& getRightsChecker()
		{
			return m_rightsChecker;
		}


		/**
		* getDatabaseCache
		*
		* This will return the object that caches database information for faster performance
		*
		* @return DatabaseCache& - The object to use to retrieve database information
		*/
		virtual DatabaseCache& getDatabaseCache()
		{
			return m_databaseCache;
		}

	private:

		 
		// This class interfaces to the alarm store for us
		AlarmStore* m_alarmStore;

		AlarmDataSync m_alarmDataSync;
		AlarmVectorWrap * m_pDisplayVector;
		// This object is used to retrieve database information. It caches the information for faster lookups.
		DatabaseCache m_databaseCache;

		// This object is used to determine whether the current operator can perform certain actions.
		RightsChecker m_rightsChecker;

		//sort column
		SORT_COLUMN m_curSortCol;
		// This indicates the order in which the list is currently sorted.
		bool m_sortAscending;
    };

}