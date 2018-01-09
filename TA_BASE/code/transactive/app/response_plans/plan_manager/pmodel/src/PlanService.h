#pragma once

#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "app/response_plans/plan_manager/pmodel/src/PlanManagerConsts.h"
#include "core/data_access_interface/entity_access/src/PlanManagerEntityData.h"

#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/threads/src/Thread.h"
#include "core/corba/src/CorbaUtil.h"

#include "app/response_plans/plan_manager/src/PlanManagerCorbaDefImpl.h"

namespace TA_Base_Core
{
	class SettingsMgr;
	class PlanManagerEntityData;
}

namespace TA_Base_App
{
	class PlanService 
		: public TA_Base_Core::RunParamUser,
		public TA_Base_Bus::AbstractStateChangeSubject<TA_Base_Bus::IStateChangeObserver>
	{
	public:
		// The reference to the one and only PlanManager Service object
		static PlanService& getInstance();

		void setMainViewPosition();
		void setupGUIEntity();
		void entityChanged(TA_Base_Core::IEntityData* guiEntity);

		std::string checkLocations();
		int activatePManagerServant();
		std::string registerStationAgent(int iIndex);
		bool isStepServantActive();

		void registerRunParam();
		std::string getRunParam(const char* name);
		void setRunRaram(const char* name, const char* value);

		void onRunParamChange(const std::string& name, const std::string& value);
		std::string getDefaultPlanAgentLocationName();
		void cleanUp();

	private: // Functions
		PlanService();
		~PlanService();
		// Made private as this is a singleton
		PlanService(const PlanService& rhs);
		PlanService& operator=(const PlanService &);		

	private: // Variables
        /** 
          * The one and only instance.
          */
        static PlanService* m_pInstance;

		class PlanRegisterThread : public TA_Base_Core::Thread
		{
		public:
			PlanRegisterThread(PlanManagerCorbaDefImpl* parent, unsigned long location, unsigned long interval);
			~PlanRegisterThread();
			void run();
			void terminate();
			void reRegister();

		private:
			unsigned long m_location;
			unsigned long m_interval;
			PlanManagerCorbaDefImpl* m_parent;
			bool m_terminated;
			bool m_bRegistered;
			TA_Base_Core::Condition  m_waitObject;
		};

        /**
          * Used to synchronise access to the singleton.
          */
        static TA_Base_Core::ReEntrantThreadLockable m_singletonLock;

        /**
          * Used to synchronise access to the process data.
          */
        TA_Base_Core::NonReEntrantThreadLockable m_processLock;

		PlanManagerCorbaDefImpl *m_planManagerServant;

		bool m_execPlanIdChanged;
		bool m_viewPlanIdChanged;
		bool m_assocAlarmIdChanged;
	};
}