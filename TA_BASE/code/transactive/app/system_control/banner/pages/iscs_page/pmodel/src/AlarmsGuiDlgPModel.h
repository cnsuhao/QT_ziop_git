/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/incidents/incident_manager/pmodel/src/AlarmsGuiDlgPModel.h $
  * @author:  Noel R. Tajanlangit
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: Noel
  *
  */

#ifndef __ALARMS_GUI_DLG_PMODEL_H_INCLUDED__
#define __ALARMS_GUI_DLG_PMODEL_H_INCLUDED__

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "iscsPagePModelConst.h"
#include "AlarmsGuiDlgBusLogic.h"

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"
#include "bus/alarm/alarm_list_control/pmodel/src/Filter.h"

namespace TA_Base_App
{
	class CAlarmsGuiDlgPModel
		: public TA_Base_Bus::AbstractPModel
	{
	public: //Functions
		CAlarmsGuiDlgPModel(TA_Base_Bus::IApplicationController* appController,
			const char * pmodelName);

		~CAlarmsGuiDlgPModel(void);

		void initialize();
		void launchAlarmManager(int xPos);
		TA_Base_Bus::SettingsMgr* getSettingsMgr();

		std::string getSession();
		void initPreFiltering(TA_Base_Bus::Filter& preFilter, std::vector<unsigned long>& locations);
		void updateAlarmSound(unsigned long severity, bool wereAlarmsAdded);
		void updatePreFilterAccordingToSystemState(TA_Base_Bus::Filter& preFilter);

		void setExceededMaxDisplay(bool bExceeded);
		bool hasExceededMaxDisplay();

	protected:
		void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args);

	private: //Functions
		
	private: //Variables
		CAlarmsGuiDlgBusLogic* m_pBusLogic;
	};

}

#endif //__ALARMS_GUI_DLG_PMODEL_H_INCLUDED__