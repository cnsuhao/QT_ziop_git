#pragma once

#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "app/alarm/alarm_manager/pmodel/src/SubsytemDataCache.h"
#include "bus/alarm/alarm_list_control/pmodel/src/Filter.h"
#include "core/data_access_interface/src/NamedFilter.h"
#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include "app/alarm/alarm_manager/pmodel/src/AlarmManagerConsts.h"

#include <list>
#include <string>
#include <vector>
#include <map>

namespace TA_Base_Core
{
	class IAlarmSeverityData;
}

namespace TA_Base_App
{
	class AlarmFilterService :
		public TA_Base_Bus::AbstractStateChangeSubject<TA_Base_Bus::IStateChangeObserver>
	{
	public:

		// GetInstance
		static AlarmFilterService& getInstance();

		// ReleaseInstance
		static void releaseInstance();

		virtual ~AlarmFilterService(void);
		
		unsigned long getSeverityColour(TA_Base_Core::IAlarmSeverityData* severityData);
		
		std::list<std::string> getFilterNameList();

		void retrieveLocations(ListItems& locationList);
		void retrieveSubsystems(ListItems& subsystemList);
		void retrieveSubsystemTypes(ListItems& subsystemtypeList, const ListItems& systemList);
		void retrieveSystems(ListItems& systemList);

		/**
		* retrieveOperators
		*
		* This method retrieves all operators from the database and stores it in
		* a map..
		*/
		void retrieveOperators(ListItems& operatorList);

		/**
		* retrieveSeverities
		*
		* This method retrieves all alarm severities from the database and stores it in
		* a map.
		*/
		void retrieveSeverities(ListSeverityItems& severityList);

		/**
		* retrieveAlarmTypes
		*
		* This method retrieves all alarm types from the database and stores it in
		* a vector.
		*/
		void retrieveAlarmTypes(ListItems& alarmTypeList);

		/**
		* deleteNamedFilter
		*
		* Delete the named filter from the database.
		*
		* @param NamedFilter - The NamedFilter obj or data to be deleted
		* @return bool - Return the success of delete false = not sucessful, otherwise true
		*
		*/
		bool deleteNamedFilter(TA_Base_Core::NamedFilter& nmFilter);

		/**
		* convertNamedFilterToFilter
		*
		* Convert the NamedFilter object to Filter object for use in alarm list control
		*
		* @param NamedFilter - The NamedFilter reference from which the Filter obj will be constructed
		* @return Filter - The Filter copy contructed with the input NamedFilter
		*
		*/
		TA_Base_Bus::Filter convertNamedFilterToFilter(TA_Base_Core::NamedFilter& nmFilter);

		/**
		* convertFilterToNamedFilter
		*
		* Convert the Filter object to NamedFilter object for use in this dialog
		*
		* @param Filter - The Filter reference from which the NamedFilter obj will be constructed
		* @return NamedFilter - The NamedFilter copy contructed with the input Filter
		*
		*/
		TA_Base_Core::NamedFilter convertFilterToNamedFilter(TA_Base_Bus::Filter& filter);

		/**
		* checkInNamedFilterDatabase
		*
		* Convert the Filter object to NamedFilter object for use in this dialog
		*
		* @param Name string - Name of the named filter to be checked
		* @return bool - Determine the presence of the filter. True if present, otherwise false
		*
		*/
		bool checkInNamedFilterDatabase(std::string curName);

		TA_Base_Bus::Filter getUserDefFilter() { return m_filterUserDef; };
		void setUserDefFilter(TA_Base_Bus::Filter pUserFilter) { m_filterUserDef = pUserFilter; };
	private: // Methods
		
		// Constructor
		AlarmFilterService();

	private:
		static AlarmFilterService* m_pInstance;
		static TA_Base_Core::NonReEntrantThreadLockable m_singletonLock;

		// This will hold the userDefine filter
		TA_Base_Bus::Filter m_filterUserDef;
	};


}
