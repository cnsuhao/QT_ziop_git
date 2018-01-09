/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/incidents/incident_manager/pmodel/src/AlarmsGuiDlgBusLogic.h $
  * @author:  Noel R. Tajanlangit
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: Noel
  *
  */

#ifndef __ALARMS_GUI_DLG_BUS_LOGIC_H_INCLUDED__
#define __ALARMS_GUI_DLG_BUS_LOGIC_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/system_control/banner/pages/iscs_page/pmodel/src/iscsPagePModelConst.h"

#include "bus/generic_gui_entity/src/ISystemStateUpdateObserver.h"
#include "bus/generic_gui_pmod/src/PModelInterface.h"
#include "bus/alarm/alarm_list_control/pmodel/src/Filter.h"
#include "core/utilities/src/RunParams.h"
#include <string>

namespace TA_Base_Bus
{
	// Forward declaration
	class SettingsMgr;
	class SoundManagerProxy;
	class SystemControllerGroupUpdateSubscriber;
}

namespace TA_Base_App
{

	class CAlarmsGuiDlgBusLogic
		: public TA_Base_Bus::ISystemStateUpdateObserver
		, public TA_Base_Core::RunParamUser
		, public TA_Base_Bus::AbstractStateChangeSubject<TA_Base_Bus::IStateChangeObserver>
	{
	public: //Functions
		// The name of the Alarm sound category.
		static const std::string ALARM_CATEGORY;

		CAlarmsGuiDlgBusLogic();
		~CAlarmsGuiDlgBusLogic(void);

		void initialize();

		void launchAlarmManager(int xPos);

		void onRunParamChange(const std::string& name, const std::string& value);

		TA_Base_Bus::SettingsMgr* getSettingsMgr();

		std::string getSession();

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

		/** 
          * initPreFiltering
          *
          * Creates a Filter object and a vector of locations based on
	      * the active session, so that they can be passed to the alarm
	      * list control.
          *
          * @param preFilter  The pre-filter to be applied.
          * @param locations  The locations we wish to display alarms from.
          *
          */
	    void initPreFiltering(TA_Base_Bus::Filter& preFilter, std::vector<unsigned long>& locations);

		/**
         * updateAlarmSound
         * 
         * The virtual method that updates the container's alarm severity tones.
         *
         * @param The severity tone to update as an unsigned long.
         * @param True if new alarms were inserted, and false otherwise.
         */
	    void updateAlarmSound(unsigned long severity, bool wereAlarmsAdded);

		/**
	     * updatePreFilterAccordingToSystemState
	     *
	     * Clears the pre-filter if the system is in degraded mode.
	     * 
	     * @param preFilter - Reference to the pre-filter that is to be changed.
	     */
        void updatePreFilterAccordingToSystemState(TA_Base_Bus::Filter& preFilter);

		void setExceededMaxDisplay(bool bExceeded) { m_exceededMaxDisplayAlarm = bExceeded; };
		bool hasExceededMaxDisplay() { return m_exceededMaxDisplayAlarm; };

	private: //Functions

	private: //Variables
		TA_Base_Bus::SettingsMgr* m_ptrSettingsMgr;
		TA_Base_Bus::SoundManagerProxy* m_soundInterface;
		TA_Base_Bus::SystemControllerGroupUpdateSubscriber* m_groupUpdateSubscriber;

		bool m_unmuteOnNewAlarm;
		std::string m_sessionId;
		std::string m_SoundFilePath;
		
		bool m_exceededMaxDisplayAlarm;
	};

}

#endif //__ALARMS_GUI_DLG_BUS_LOGIC_H_INCLUDED__