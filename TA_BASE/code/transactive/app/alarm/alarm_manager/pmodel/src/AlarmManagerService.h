
#ifndef __ALARM_MANAGER_SERVICE_H_INCLUDED_H__
#define __ALARM_MANAGER_SERVICE_H_INCLUDED_H__

#include "bus/generic_gui_entity/src/ISystemStateUpdateObserver.h"
#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "bus\alarm\alarm_list_control\pmodel\src\Filter.h"


#include "core\utilities\src\RunParams.h"
#include "core/synchronisation/src/ReEntrantThreadLockable.h"



namespace TA_Base_Bus
{
	// Forward declaration
	class SettingsMgr;
	class SystemControllerGroupUpdateSubscriber;
}

namespace TA_Base_App
{

	class AlarmManagerService
		: public TA_Base_Core::RunParamUser,
		public TA_Base_Bus::ISystemStateUpdateObserver,
		public TA_Base_Bus::AbstractStateChangeSubject<TA_Base_Bus::IStateChangeObserver>
	{
	public:
		// GetInstance
		static AlarmManagerService& getInstance();

		// ReleaseInstance
		static void releaseInstance();
		
		void init();

		/**
		* serverDown
		*
		* This implements the virtual function from AlarmViewerGUI. It
		* handles the situation in an event when the main server goes down.
		*/
		void serverDown();

		/**
		* serverUp
		*
		* This implements the virtual function from AlarmViewerGUI. It
		* handles the situation in an event when the main server
		* reestablishes comms.
		*/
		void serverUp();

		/**
		* onRunParamChange
		*
		* This implements the virtual function from AlarmViewerGUI. It
		* handles the situation in an event when the main server
		* reestablishes comms.
		*
		* @param The RunParam name.
		*
		* @param The new RunParam value.
		*/
		void onRunParamChange(const std::string& name, const std::string& value);

		/**
		* notifySystemStateDegraded
		*
		* This is called when the system is in degraded mode.
		*
		*/
		virtual void notifySystemStateDegraded();


		/**
		* notifySystemStateNormal
		*
		* This is called when the system has recovered from degraded mode
		* (ie. system is running normally).
		*
		*/
		virtual void notifySystemStateNormal();

		static const std::string RPARAM_FILTERONASSET;
		static const std::string RPARAM_FILTERONENTITY;

		std::string getStatusBarData();
		void setStatusBarData(const std::string& value);
		std::list<std::string > getFilterList();
		void setFilterList(const std::set<std::string >& value);

	private: // Functions
		AlarmManagerService();
		~AlarmManagerService();

		// Disable
		AlarmManagerService(const AlarmManagerService& rhs);
		AlarmManagerService& operator=(const AlarmManagerService &);

		void initPreFiltering(TA_Base_Bus::Filter& preFilter, std::vector<unsigned long>& locations);
		void setProfileId();

		void filterOnAsset(const std::string& asset);
		void filterOnEntity(const std::string& entity);

	private: // Variables
		static AlarmManagerService* m_pInstance;
		static TA_Base_Core::ReEntrantThreadLockable m_singletonLock;

		TA_Base_Core::ReEntrantThreadLockable m_threadLock;

		TA_Base_Bus::SettingsMgr* m_ptrSettingsMgr;
		unsigned long m_profileId;
		std::string	m_sessionId;
		std::string	m_statusBar;
		std::set<std::string > m_filterList;

		TA_Base_Bus::SystemControllerGroupUpdateSubscriber* m_groupUpdateSubscriber;

		typedef std::map<std::string, unsigned long> EntityNameToKeyMap;
		EntityNameToKeyMap	m_entityNameKeyMap;

		bool m_isToolBarVisible;
		bool m_hasAlarmListInit;
		bool m_exceededMaxDisplayAlarm;
		bool m_isLoadingAlarms; // indicator for loading all alarms



	};



}

#endif //__ALARM_MANAGER_SERVICE_H_INCLUDED_H__
