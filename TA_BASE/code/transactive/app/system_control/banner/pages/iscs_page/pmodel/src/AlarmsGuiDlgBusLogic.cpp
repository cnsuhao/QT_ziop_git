/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/incidents/incident_manager/pmodel/src/AlarmsGuiDlgBusLogic.cpp $
  * @author:  Noel R. Tajanlangit
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: Noel
  *
  */

#include "AlarmsGuiDlgBusLogic.h"
#include "StringEventArgs.h"
#include "PrefilterChangeEventArgs.h"

#include "bus/generic_gui_entity/src/SystemControllerGroupUpdateSubscriber.h"
#include "bus/sound/sound_client/src/SoundManagerProxy.h"
#include "bus/user_settings/src/SettingsMgr.h"

#include "bus/security/authentication_library/src/AuthenticationLibrary.h"
#include "bus/generic_gui_pmod/src/AppLauncher.h"
#include "bus/generic_gui_pmod/src/PModelDataType.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"

#include "bus/application_types/src/apptypes.h"

#include "core/data_access_interface/src/IProfile.h"
#include "core/data_access_interface/src/ProfileAccessFactory.h"
#include "core/data_access_interface/entity_access/src/EntityData.h"
#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/SoundPlayerException.h"
#include "core/exceptions/src/ApplicationException.h"
#include "core/exceptions/src/AssertException.h"
#include "core/exceptions/src/AuthenticationAgentException.h"


#include "core/utilities/src/DebugUtil.h"



using namespace TA_Base_Core;

namespace TA_Base_App
{
	const std::string CAlarmsGuiDlgBusLogic::ALARM_CATEGORY = "Alarm";

	CAlarmsGuiDlgBusLogic::CAlarmsGuiDlgBusLogic()
		: m_groupUpdateSubscriber( NULL )
		, m_soundInterface( NULL )
		, m_ptrSettingsMgr( NULL )
		, m_unmuteOnNewAlarm( true )
	{
		try
		{
			m_soundInterface = new TA_Base_Bus::SoundManagerProxy();
		}
		catch(TA_Base_Core::SoundPlayerException& ex)
		{
			std::string error = "Unable to initialise SoundManager: ";
			error += ex.what();
			LOG_EXCEPTION_CATCH(SourceInfo, "SoundPlayerException", error.c_str());
		}

		m_groupUpdateSubscriber = new TA_Base_Bus::SystemControllerGroupUpdateSubscriber(*this);
	}

	CAlarmsGuiDlgBusLogic::~CAlarmsGuiDlgBusLogic(void)
	{
		if (m_soundInterface != NULL)
		{
			// Stop the sound from playing
			m_soundInterface->setPriority(ALARM_CATEGORY, 0);

			delete m_soundInterface;
			m_soundInterface = NULL;
		}

		if(m_groupUpdateSubscriber != NULL)
		{
			delete m_groupUpdateSubscriber;
			m_groupUpdateSubscriber = NULL;
		}

		if (m_ptrSettingsMgr)
		{
			delete m_ptrSettingsMgr;
			m_ptrSettingsMgr = NULL;
		}

		RunParams::getInstance().deregisterRunParamUser(this);
	}

	void CAlarmsGuiDlgBusLogic::initialize()
	{
		std::string strLevel = TA_Base_Core::RunParams::getInstance().get(RPARAM_DEBUGLEVEL);
		if (!strLevel.empty())
		{
			DebugUtil::EDebugLevel level = DebugUtil::getInstance().getDebugLevelFromString(strLevel.c_str());
			DebugUtil::getInstance().setLevel(level);
		}

		// Set Session Id
		m_sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
		LOG_GENERIC( SourceInfo, DebugUtil::DebugDebug, "m_sessionId set to %s", m_sessionId.c_str());
		RunParams::getInstance().registerRunParamUser(this, RPARAM_SESSIONID);

		// TD 6950: Start receiving message informing GUI when system is degraded/recovered
		m_groupUpdateSubscriber->startNotifications();


		TA_Base_Core::IData * dbdata = NULL;
		try
		{
			dbdata = TA_Base_Core::EntityAccessFactory::getInstance().getParameter("ControlStation", "UnmuteOnAlarm");
			if ( NULL != dbdata )
			{
				if ( 0 < dbdata->getNumRows() )
				{
					m_unmuteOnNewAlarm = dbdata->getBooleanData( 0, "VALUE", true );
				}
			}
		}
		catch ( TA_Base_Core::DatabaseException & e)
		{
			m_unmuteOnNewAlarm = true;

			std::ostringstream desc;
			desc << "Caught DatabaseException - Setting m_unmuteOnNewAlarm = true; Error: " << e.what();
			LOG_EXCEPTION_CATCH(SourceInfo, "DatabaseException", desc.str().c_str());
		}
		catch ( TA_Base_Core::DataException & e)
		{
			m_unmuteOnNewAlarm = true;

			std::ostringstream desc;
			desc << "Caught DataException - Setting m_unmuteOnNewAlarm = true; Error: " << e.what();
			LOG_EXCEPTION_CATCH(SourceInfo, "DataException", desc.str().c_str());
		}
		catch(...)
		{
			m_unmuteOnNewAlarm = true;

			std::ostringstream desc;
			desc << "Caught Unknown Exception - Setting m_unmuteOnNewAlarm = true";
			LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", desc.str().c_str());
		}

		delete dbdata;
		dbdata = NULL;
	}

	void CAlarmsGuiDlgBusLogic::launchAlarmManager(int xPos)
	{
		// Launch AlarmManager
		try
		{
			LOG_GENERIC( SourceInfo, DebugUtil::DebugDebug, "launching Alarm Manager");
			TA_Base_Bus::TA_Rect rect = TA_Base_Bus::AppLauncher::getInstance().getRect(
				TA_Base_Bus::TA_GenericGui::SCREEN_CURRENT,
				TA_Base_Bus::TA_GenericGui::AREA_SCHEMATIC,
				xPos);

			TA_Base_Bus::AppLauncher::getInstance().launchApplication(
				ALARM_GUI_APPTYPE, 
				"", 
				TA_Base_Bus::TA_GenericGui::POS_BOUNDED|TA_Base_Bus::TA_GenericGui::POS_RESIZABLE,
				TA_Base_Bus::TA_GenericGui::ALIGN_FIT|TA_Base_Bus::TA_GenericGui::ALIGN_LEFT|TA_Base_Bus::TA_GenericGui::ALIGN_TOP,
				NULL,
				&rect);
		}
		catch(TA_Base_Core::ApplicationException& exApp)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "CAlarmsGuiDlgBusLogic::ApplicationException", "Could not launch Alarm Manager viewer");
			throw exApp;
		}
		catch(...)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "CAlarmsGuiDlgBusLogic::GeneralException", "General Exception: Could not launch Alarm Manager viewer");
			throw;
		}
	}

	void CAlarmsGuiDlgBusLogic::onRunParamChange(const std::string& name, const std::string& value)
	{
		FUNCTION_ENTRY( "onRunParamChange" );

		if(name.compare(RPARAM_SESSIONID) == 0)
		{
			// Store the new session id.
			m_sessionId = value;

			//// Update the pre-filter and locations.
			//StringEventArgs* objEventArgs = 
			//	new StringEventArgs(ISCS_EventArgs::SESSION_ID, m_sessionId);

			//notifyObservers(PMODEL_SESSION_CHANGE, objEventArgs);
			// Update the pre-filter and locations.
			TA_Base_Bus::Filter preFilter;
			std::vector<unsigned long> locations;
			initPreFiltering(preFilter, locations);

			PrefilterChangeEventArgs* pEvent = new PrefilterChangeEventArgs(NORMAL_MODE, m_sessionId, &preFilter, locations);
			notifyObservers(PMODEL_SESSION_CHANGE, pEvent);

			LOG_GENERIC( SourceInfo, DebugUtil::DebugDebug, "RunParamChange received for session ID. New sesion ID is %s", m_sessionId.c_str());
		}

		FUNCTION_EXIT;
	}

	TA_Base_Bus::SettingsMgr* CAlarmsGuiDlgBusLogic::getSettingsMgr()
	{
		FUNCTION_ENTRY("getSettingsMgr");
		FUNCTION_EXIT;
		return m_ptrSettingsMgr;
	}

	std::string CAlarmsGuiDlgBusLogic::getSession()
	{
		FUNCTION_ENTRY("getSettingsMgr");
		FUNCTION_EXIT;
		return m_sessionId;
	}

	void CAlarmsGuiDlgBusLogic::notifySystemStateDegraded()
	{
		FUNCTION_ENTRY( "notifySystemStateDegraded" );
		// Retrieve the locations related to the current profile. We still only want alarms
		// from these locations.
		TA_Base_Bus::Filter* preFilter;
		std::vector<unsigned long> locations;
		initPreFiltering(*preFilter, locations);

		// Clear pre-filter as we don't want rights to be checked before an alarm is displayed.
		preFilter->removeAll();

		//Notify Prefilter changed
		PrefilterChangeEventArgs* pEvent = new PrefilterChangeEventArgs(DEGRADED_MODE, m_sessionId, preFilter, locations);
		notifyObservers(PMODEL_SYSTEM_STATUS, pEvent);

		delete pEvent;

		//notifyObservers(PMODEL_SYSTEM_DEGRAGED, NULL);
		FUNCTION_EXIT;
	}


	void CAlarmsGuiDlgBusLogic::notifySystemStateNormal()
	{
		FUNCTION_ENTRY( "notifySystemStateNormal" );
		// Reset Pre-Filter
		TA_Base_Bus::Filter* preFilter;
		std::vector<unsigned long> locations;
		initPreFiltering(*preFilter, locations);

		//Notify Prefilter changed
		PrefilterChangeEventArgs* pEvent = new PrefilterChangeEventArgs(NORMAL_MODE, m_sessionId, preFilter, locations);
		notifyObservers(PMODEL_SYSTEM_STATUS, pEvent);

		//notifyObservers(PMODEL_SYSTEM_NORMAL, NULL);
		delete pEvent;
		FUNCTION_EXIT;
	}

	void CAlarmsGuiDlgBusLogic::initPreFiltering(TA_Base_Bus::Filter& preFilter, std::vector<unsigned long>& locations)
	{
		locations.clear();

		TA_Base_Bus::SessionInfo sessionInfo;

		try
		{
			// Get some info about the current session.
			TA_Base_Bus::AuthenticationLibrary authLibrary(true);
			sessionInfo = authLibrary.getSessionInfo(m_sessionId, m_sessionId);
		}
		catch(TA_Base_Core::AuthenticationAgentException&)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "AuthenticationAgentException", "Could not retrieve session information");
			return;
		}
		catch(TA_Base_Core::ObjectResolutionException&)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "ObjectResolutionException", "Could not resolve Authentication Agent");
			return;
		}

		// The prefilter requires the session id.
		preFilter.addFilter(TA_Base_Bus::Filter::FILTER_PROFILE, 
			TA_Base_Bus::Filter::COMPARE_EQUAL, m_sessionId);

		bool isCentralProfile = false;
		TA_Base_Core::IProfile* profile;

		for (std::vector<unsigned long>::iterator iter = sessionInfo.ProfileId.begin();
			iter != sessionInfo.ProfileId.end(); iter++)
		{
			try
			{
				profile = TA_Base_Core::ProfileAccessFactory::getInstance().getProfile(*iter);

				if (profile->getType() == PROFILE_LOCAL)
				{
					locations.push_back(sessionInfo.LocationId);
				}
				else  // PROFILE_CENTRAL
				{
					isCentralProfile = true;
				}

				delete profile;
			}
			catch(TA_Base_Core::DataException&)
			{
				delete profile;
				LOG_EXCEPTION_CATCH(SourceInfo, "DataException", "Could not retrieve profile information");
			}
			catch(TA_Base_Core::DatabaseException&)
			{
				delete profile;
				LOG_EXCEPTION_CATCH(SourceInfo, "DatabaseException", "Could not retrieve profile information");
			}
		}

		if (isCentralProfile)
		{
			// Central profiles require all locations.
			locations.clear();
		}
	}

	void CAlarmsGuiDlgBusLogic::updateAlarmSound(unsigned long severity, bool wereAlarmsAdded)
	{
		try
		{
			if (m_soundInterface != NULL)
			{
				if (severity == 0)
				{
					// Play no sound.
					m_soundInterface->setPriority(ALARM_CATEGORY, 0);
				}
				else
				{
					const int NUM_SEVERITIES = 5;					
					// Need to convert severities (1 highest, descending order) into priorities 
					// (1 lowest, ascending order).
					long alarmPriority = 0;				 
					alarmPriority = NUM_SEVERITIES + 1 - severity; 
					LOG_GENERIC( SourceInfo, DebugUtil::DebugDebug, "Sound severity= %d before=%d", alarmPriority,severity);
					m_soundInterface->setPriority(ALARM_CATEGORY, alarmPriority);       //TD16415
				}
			}
		}
		catch(TA_Base_Core::SoundPlayerException& ex)
		{
			std::string error = "Error setting priority on SoundManager: ";
			error += ex.what();
			LOG_EXCEPTION_CATCH(SourceInfo, "SoundPlayerException", error.c_str());
		}

		if (m_unmuteOnNewAlarm && wereAlarmsAdded)
		{
			// New alarms - clear the mute button if necessary.
			notifyObservers(PMODEL_NOTIFY_UNMUTE, NULL);
		}

	}

	void CAlarmsGuiDlgBusLogic::updatePreFilterAccordingToSystemState(TA_Base_Bus::Filter& preFilter)
	{
		// TD 6950: Check if the system is currently in degraded mode. If so, clear the pre-filter.
		std::string groupsOffline = RunParams::getInstance().get(RPARAM_GROUPSOFFLINE);
		if( true == groupsOffline.empty())
		{
			return;
		}

		std::string::size_type pos = 0;
		while(pos != std::string::npos)
		{
			pos = groupsOffline.find(",", 0);
			std::string group = groupsOffline.substr(0, pos);

			if(group == m_groupUpdateSubscriber->getCentralServerGroupName())
			{
				// System is degraded
				preFilter.removeAll();
				return;
			}

			if(pos != std::string::npos)
			{
				groupsOffline = groupsOffline.substr(pos+1, groupsOffline.size() - (pos + 1));
			}
		}
	}
}