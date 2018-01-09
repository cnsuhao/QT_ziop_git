/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/incidents/incident_manager/pmodel/src/AlarmsGuiDlgPModel.cpp $
  * @author:  Noel R. Tajanlangit
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: Noel
  *
  */

#include "AlarmsGuiDlgPModel.h"

#include "bus/generic_gui_entity/src/SystemControllerGroupUpdateSubscriber.h"
#include "bus/sound/sound_client/src/SoundManagerProxy.h"
#include "bus/user_settings/src/SettingsMgr.h"

namespace TA_Base_App
{
	CAlarmsGuiDlgPModel::CAlarmsGuiDlgPModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
		: TA_Base_Bus::AbstractPModel(appController, pmodelName)
	{
		m_pBusLogic = new CAlarmsGuiDlgBusLogic();
		m_pBusLogic->registerObserver(this); 
	}

	CAlarmsGuiDlgPModel::~CAlarmsGuiDlgPModel(void)
	{
		if (NULL != m_pBusLogic)
		{
			m_pBusLogic->unregisterObserver(this);
			delete m_pBusLogic;
			m_pBusLogic = NULL;
		}
	}

	void CAlarmsGuiDlgPModel::initialize()
	{
		m_pBusLogic->initialize();
	}

	void CAlarmsGuiDlgPModel::launchAlarmManager(int xPos)
	{
		m_pBusLogic->launchAlarmManager(xPos);
	}

	TA_Base_Bus::SettingsMgr* CAlarmsGuiDlgPModel::getSettingsMgr()
	{
		return m_pBusLogic->getSettingsMgr();
	}

	std::string CAlarmsGuiDlgPModel::getSession()
	{
		return m_pBusLogic->getSession();
	}

	void CAlarmsGuiDlgPModel::initPreFiltering(TA_Base_Bus::Filter& preFilter, std::vector<unsigned long>& locations)
	{
		m_pBusLogic->initPreFiltering(preFilter, locations);
	}

	void CAlarmsGuiDlgPModel::updateAlarmSound(unsigned long severity, bool wereAlarmsAdded)
	{
		m_pBusLogic->updateAlarmSound(severity, wereAlarmsAdded);
	}

	void CAlarmsGuiDlgPModel::updatePreFilterAccordingToSystemState(TA_Base_Bus::Filter& preFilter)
	{
		m_pBusLogic->updatePreFilterAccordingToSystemState(preFilter);
	}

	void CAlarmsGuiDlgPModel::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{
		if (_stricmp(updateType, PMODEL_SESSION_CHANGE) == 0 || _stricmp(updateType, PMODEL_SYSTEM_STATUS) == 0 ||
			_stricmp(updateType, PMODEL_NOTIFY_UNMUTE) == 0)
		{
			notifyViews(updateType, args);
		}
	}

	void CAlarmsGuiDlgPModel::setExceededMaxDisplay(bool bExceeded)
	{
		m_pBusLogic->setExceededMaxDisplay(bExceeded);
	}

	bool CAlarmsGuiDlgPModel::hasExceededMaxDisplay()
	{
		return m_pBusLogic->hasExceededMaxDisplay();
	}
}