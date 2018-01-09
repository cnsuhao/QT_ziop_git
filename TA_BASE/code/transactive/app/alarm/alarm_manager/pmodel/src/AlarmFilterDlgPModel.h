#pragma once
#include "app/alarm/alarm_manager/pmodel/src/AlarmFilterService.h"
#include "app\alarm\alarm_manager\pmodel\src\AlarmManagerConsts.h"
#include "app/alarm/alarm_manager/pmodel/src/SubsytemDataCache.h"

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"
#include "bus/alarm/alarm_list_control/pmodel/src/Filter.h"
#include "core/data_access_interface/src/NamedFilter.h"

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
	class AlarmFilterDlgPModel :
		public TA_Base_Bus::AbstractPModel
	{
	public:
	
		AlarmFilterDlgPModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
		~AlarmFilterDlgPModel();

		/**
		* notifyUpdate
		*
		* Calls the notifyUpdate() function of the AdminManager to retrieve the
		* up-to-date process data.
		*/
		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args = NULL);

		/**
		* getCurrentFilterString
		*
		* This is to return string of the named filter parameters for display on caption, etc
		*
		* @return: A param string of the named filter for display
		*/
		std::string getCurrentFilterString();

		void retrieveListItems();

		unsigned long getSeverityColour(TA_Base_Core::IAlarmSeverityData* severityData);

		TA_Base_Bus::Filter* getFilterObject();

		TA_Base_Bus::Filter* getUserDefinedFilterObject();

		TA_Base_Core::NamedFilter* getNamedFilterObject();

		void selectUserFilter();

		// Button Function
		void applyFilter();
		void clearFilter();
		void saveFilter();
		void deleteFilter();

		void cancel();

		// Data setters and getters
		ListItems getSystemList() { return m_systemTypes; }
		ListItems getSubSystemList() { return m_subsystems; }
		ListItems getSubSystemTypeList() { return m_subsystemTypes; }
		ListItems getOperatorList() { return m_operators; }
		ListItems getLocationList() { return m_locations; }
		ListItems getAlarmTypeList() { return m_alarmTypes; }
		ListSeverityItems getSeverityList() { return m_severityList; }

		// Selected Items
		void addSelectedSystem(const std::string& system);
		void setSelSystemList(const ListItems& systemList);
		ListItems getSelSystemList();

		void addSelectedSubsystem(unsigned long subSystemKey);
		void setSelSubSystemList(const ListItems& subSystemList);
		ListItems getSelSubSystemList();

		void addSelectedSubsystemType(const std::string& subSystemType);
		void setSelSubSystemTypeList(const ListItems& subSystemTypeList);
		ListItems getSelSubSystemTypeList();

		void addSelectedOperator(const std::string& operatorName);
		void setSelOperatorList(const ListItems& operatorList);
		ListItems getSelOperatorList();

		void addSelectedLocation(unsigned long locationKey);
		void setSelLocationList(const ListItems& locationList);
		ListItems getSelLocationList();

		void addSelectedAlarmType(unsigned long alarmTypeKey);
		void setSelAlarmTypeList(const ListItems& alarmTypeList);
		ListItems getSelAlarmTypeList();

		void addSelectedSeverity(unsigned long severity);
		void removeSelectedSeverity(unsigned long severity);
		void setSelSeverityList(const ListSeverityItems& items) { m_severityList = items; }
		ListSeverityItems getSelSeverityList();

		void setEquipmentID(const std::string& equipmentID) { m_EquipmentID = equipmentID; }
		std::string getEquipmentID() { return m_EquipmentID; }

		void setDescription(const std::string& description) { m_Description = description; }
		std::string getDescription() { return m_Description; }

		void changeNamedFilter(const std::string& namedFilter); // { m_NamedFilter = namedFilter; }
		std::string getNamedFilter() { return m_NamedFilter; }

		void setChkFromDateTime(bool bChecked);// { m_chkFromDateTime = bChecked; }
		bool getChkFromDateTime() { return m_chkFromDateTime; }

		void setFromDate(time_t date); // { m_FromDate = date; }
		time_t getFromDate() { return m_FromDate; }

		void setFromTime(time_t ptime); // { m_FromTime = time; }
		time_t getFromTime(){ return m_FromTime; }

		time_t getFromDateTime();
		time_t getToDateTime();

		void setChkToDateTime(bool bChecked); // { m_chkToDateTime = bChecked; }
		bool getChkToDateTime() { return m_chkToDateTime; }

		void setToDate(time_t date); // { m_ToDate = date; }
		time_t getToDate() { return m_ToDate; }

		void setToTime(time_t time); // { m_ToTime = time; }
		time_t getToTime() { return m_ToTime; }

		void setOperationalMode(OMFilter omFlag) { m_OperationalMode = omFlag; setUpdateNeeded(true); }
		OMFilter getOperationalMode() { return m_OperationalMode; }

		void setAckState(AckFilter ackState) { m_AckState = ackState; setUpdateNeeded(true); }
		AckFilter getAckState() { return m_AckState; }

		void setAlarmState(StateFilter alarmState) { m_AlarmState = alarmState; setUpdateNeeded(true); }
		StateFilter getAlarmState() { return m_AlarmState; }

		std::list<std::string> getFilterNameList();

		/**
		* setUpdateNeeded
		*
		* Call this method when the controls are changed or a filter applied
		*
		* @param TRUE => enable 'Apply' button. FALSE => disable 'Apply' button.
		*/
		void setUpdateNeeded(bool updateNeeded);
		bool updateNeeded() { return m_updateNeeded; };

		/**
		* setNamedFilter
		*
		* This is to filter the alarm list control to the named filter passed in
		*
		* @param: A name string of the filter for filtering.
		*/
		void setNamedFilter(std::string filterName);

		void setNewFilter(std::string filterName);

		time_t getEarliestAlarmTime() { return 0; }; // Todo:
		time_t getLatestAlarmTime() { return 0; } // Todo:

		//void showFilter();

	private:
		/**
		* createFilter
		*
		* This method creates a Filter object based on the user's selections.
		*
		* @return A Filter object containing all information required for filtering.
		*/
		TA_Base_Bus::Filter createFilter();

		void addTimeFilters(TA_Base_Bus::Filter& filter);
		void addStateFilters(TA_Base_Bus::Filter& filter);
		void addOMTypeFilters(TA_Base_Bus::Filter& filter);

	private: // Variables
		// This member is used to access and store the filter names
		// It is the current Named Filter selected and applied
		TA_Base_Core::NamedFilter m_nmFilter;

		// This member is used to store the filter for alarm list control.
		// It is the current filter for the alarm list
		TA_Base_Bus::Filter m_filter;

		// This member is used to store the current filter used in the main dialog.
		std::string m_strCurDlgFilter;

		// This will hold the userDefine filter
		//TA_Base_Bus::Filter m_filterUserDef;

		// This member is to set whether user has saved the filter
		// True = saved, false = not saved
		bool m_savedFilter;

		// This member is used for checking if the current filter was deleted
		// 0 = no changes
		// 1 = updated
		// -1 = deleted
		// OnCancel we check if this variable and set the to default filter if 1 or -1
		int m_nCurFilterState;

		// This member is used to store state of the filters
		// true if a filter is added, updated, or removed
		bool m_bNMFilterUpdated;

		// This member is to set whether there is any change made to the filter dlg
		// True = changed, false = not changed
		bool m_updateNeeded;

		// These ListItems variables are for the FULL list of items, not the selected ones
		ListItems m_operators;
		ListItems m_alarmTypes;
		ListItems m_locations;
		ListItems m_subsystems;
		ListItems m_subsystemTypes;
		ListItems m_systemTypes;
		ListSeverityItems m_severityList;

		// The following variables are for the CURRENTLY SELECTED items in the control only
		ListItems m_selectedSubsystems;
		ListItems m_selectedLocations;
		ListItems m_selectedOperators;
		ListItems m_selectedAlarmTypes;
		ListItems m_selectedSubsystemTypes;
		ListItems m_selectedSystems;
		ListSeverityItems m_selectedServerity;

		std::string m_EquipmentID;
		std::string m_Description;
		std::string m_NamedFilter;
		bool m_chkFromDateTime;
		time_t m_FromDate;
		time_t m_FromTime;
		bool m_chkToDateTime;
		time_t m_ToDate;
		time_t m_ToTime;
		OMFilter m_OperationalMode;
		AckFilter m_AckState;
		StateFilter m_AlarmState;

		time_t  m_bakfromDateTime;
		time_t  m_bakToDateTime;

		AlarmFilter::FilterIndex filterIndex;
	};

}


