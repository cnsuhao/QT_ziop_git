
#include "app/alarm/alarm_manager/pmodel/src/AlarmManagerService.h"
#include "app/alarm/alarm_manager/pmodel/src/AlarmManagerConsts.h"
#include "app\alarm\alarm_manager\pmodel\src\PrefilterChangeEventArgs.h"
#include "app\alarm\alarm_manager\pmodel\src\ServerStatusEventArgs.h"
#include "app\alarm\alarm_manager\pmodel\src\StringEventArgs.h"
#include "app\alarm\alarm_manager\pmodel\src\FilterEventArgs.h"
#include "bus\security\authentication_library\src\AuthenticationLibrary.h"
//#include "bus/generic_gui_view/src/TransActiveMessage.h"

#include "core/synchronisation/src/ThreadGuard.h"
#include "core/utilities/src/DebugUtil.h"

#include "core\exceptions\src\DatabaseException.h"
#include "core\exceptions\src\DataException.h"
#include "core\exceptions\src\AssertException.h"
#include "core\exceptions\src\AuthenticationAgentException.h"
#include "core\exceptions\src\ObjectResolutionException.h"
#include "core\data_access_interface\src\ProfileAccessFactory.h"
#include "core\data_access_interface\src\IProfile.h"

#include "bus/generic_gui_entity/src/SystemControllerGroupUpdateSubscriber.h"

namespace TA_Base_App
{
	AlarmManagerService* AlarmManagerService::m_pInstance = NULL;
	TA_Base_Core::ReEntrantThreadLockable AlarmManagerService::m_singletonLock;

	const std::string AlarmManagerService::RPARAM_FILTERONASSET = "FilterOnAsset";
	const std::string AlarmManagerService::RPARAM_FILTERONENTITY = "FilterOnEntity";


	// GetInstance
	AlarmManagerService& AlarmManagerService::getInstance()
	{
		FUNCTION_ENTRY("getInstance");
		TA_Base_Core::ThreadGuard guard(m_singletonLock);
		if (NULL == m_pInstance)
		{
			m_pInstance = new AlarmManagerService();
		}

		FUNCTION_EXIT;
		return *m_pInstance;
	}

	// ReleaseInstance
	void AlarmManagerService::releaseInstance()
	{
		FUNCTION_ENTRY("releaseInstance");
		if (NULL != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
		FUNCTION_EXIT;
	}

	AlarmManagerService::AlarmManagerService()
		: m_groupUpdateSubscriber(NULL)
		, m_exceededMaxDisplayAlarm(false)
		, m_isLoadingAlarms(false)
		, m_hasAlarmListInit(false)
		, m_profileId(-1)
	{
		FUNCTION_ENTRY("Constructor");

		FUNCTION_EXIT;
	}

	AlarmManagerService::~AlarmManagerService()
	{
		FUNCTION_ENTRY("Destructor");
		FUNCTION_EXIT;
	}

	void AlarmManagerService::init()
	{
		FUNCTION_ENTRY("init");
		m_groupUpdateSubscriber = new TA_Base_Bus::SystemControllerGroupUpdateSubscriber(*this);
		std::string strLevel = TA_Base_Core::RunParams::getInstance().get(RPARAM_DEBUGLEVEL);
		if (!strLevel.empty())
		{
			TA_Base_Core::DebugUtil::EDebugLevel level = TA_Base_Core::DebugUtil::getInstance().getDebugLevelFromString(strLevel.c_str());
			TA_Base_Core::DebugUtil::getInstance().setLevel(level);
		}

		LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugDebug, "m_sessionId = atol(RunParams::GetInstance().Get(RPARAM_SESSIONID).c_str());");
		m_sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID).c_str();

		// Register for the SessionId run param
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, RPARAM_SESSIONID);

		// Register for the FilterOnAsset run param
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, RPARAM_FILTERONASSET.c_str());

		// Register for the FilterOnEntity run param
		TA_Base_Core::RunParams::getInstance().registerRunParamUser(this, RPARAM_FILTERONENTITY.c_str());
		// TD 6950: Start receiving message informing GUI when system is degraded/recovered
		m_groupUpdateSubscriber->startNotifications();

		FUNCTION_EXIT;
	}

	void AlarmManagerService::serverDown()
	{
		FUNCTION_ENTRY("serverDown");
		m_statusBar = "Alarm Manager: Server down.";

		ServerStatusEventArgs* pEvent = new ServerStatusEventArgs(SERVER_DOWN);
		notifyObservers(EV_SERVER_STATUS, pEvent);
		
		delete pEvent;
		FUNCTION_EXIT;
	}

	void AlarmManagerService::serverUp()
	{
		FUNCTION_ENTRY("serverUp");
		m_statusBar = "Alarm Manager: Server connection re-established.";

		ServerStatusEventArgs* pEvent = new ServerStatusEventArgs(SERVER_UP);
		notifyObservers(EV_SERVER_STATUS, pEvent);

		delete pEvent;
		FUNCTION_EXIT;
	}

	void AlarmManagerService::onRunParamChange(const std::string& name, const std::string& value)
	{
		FUNCTION_ENTRY("onRunParamChange");

		if (name.compare(RPARAM_SESSIONID) == 0)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugDebug, "RunParamChange received for session ID. New name is %s and sesion ID is %s", name.c_str(), value.c_str());

			// Store the new session id.
			m_sessionId = value;

			// Update the pre-filter and locations.
			TA_Base_Bus::Filter preFilter;
			std::vector<unsigned long> locations;
			initPreFiltering(preFilter, locations);

			PrefilterChangeEventArgs* pEvent = new PrefilterChangeEventArgs(NORMAL_MODE, m_sessionId, &preFilter, locations);
			notifyObservers(EV_SESSION_CHANGED, pEvent);

			//Todo: Notify filter Update
			//m_alarmList.updatePreFilterLocationsAndSession(preFilter, locations, m_sessionId);

			// Reset the profile id
			setProfileId();

			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugDebug, "RunParamChange function ended for name: %s and session ID: %s", name.c_str(), m_sessionId.c_str());
		}
		//TD16109----
		//2)The current filter field should not be Default Filter.It should be showing the filter name of the filter based on selected alarms.
		// else if(name.compare(RPARAM_FILTERONASSET) == 0)
		if (name.compare(RPARAM_FILTERONASSET) == 0)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugDebug, "RunParamChange received for filter asset. New filter asset is %s", value.c_str());
			filterOnAsset(value);
			//populateNamedFilters(TA_Base_Core::RunParams::getInstance().get(RPARAM_FILTERONASSET.c_str()));
			

		}
		// else if(name.compare(RPARAM_FILTERONENTITY) == 0)
		if (name.compare(RPARAM_FILTERONENTITY) == 0)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugDebug, "RunParamChange received for filter entity. New filter entity is %s", value.c_str());
			filterOnEntity(value);
			//populateNamedFilters(TA_Base_Core::RunParams::getInstance().get(RPARAM_FILTERONENTITY.c_str()));
		}

		FUNCTION_EXIT;
	}

	void AlarmManagerService::notifySystemStateDegraded()
	{
		FUNCTION_ENTRY("notifySystemStateDegraded");
		// Retrieve the locations related to the current profile. We still only want alarms
		// from these locations.
		TA_Base_Bus::Filter* preFilter;
		std::vector<unsigned long> locations;
		initPreFiltering(*preFilter, locations);

		// Clear pre-filter as we don't want rights to be checked before an alarm is displayed.
		preFilter->removeAll();

		//Notify Prefilter changed
		PrefilterChangeEventArgs* pEvent = new PrefilterChangeEventArgs(DEGRADED_MODE, m_sessionId, preFilter, locations);
		notifyObservers(EV_SYSTEM_STATUS, pEvent);

		delete pEvent;
		FUNCTION_EXIT;
	}

	void AlarmManagerService::notifySystemStateNormal()
	{
		FUNCTION_ENTRY("notifySystemStateNormal");
		// Reset Pre-Filter
		TA_Base_Bus::Filter* preFilter;
		std::vector<unsigned long> locations;
		initPreFiltering(*preFilter, locations);

		//Notify Prefilter changed
		PrefilterChangeEventArgs* pEvent = new PrefilterChangeEventArgs(NORMAL_MODE, m_sessionId, preFilter, locations);
		notifyObservers(EV_SYSTEM_STATUS, pEvent);
		delete pEvent;
		FUNCTION_EXIT;
	}

	void AlarmManagerService::initPreFiltering(TA_Base_Bus::Filter& preFilter, std::vector<unsigned long>& locations)
	{
		locations.clear();

		TA_Base_Bus::SessionInfo sessionInfo;

		try
		{
			// Get some info about the current session.
			TA_Base_Bus::AuthenticationLibrary authLibrary(true);
			sessionInfo = authLibrary.getSessionInfo(m_sessionId, m_sessionId);
		}
		catch (TA_Base_Core::AuthenticationAgentException&)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "AuthenticationAgentException", "Could not retrieve session information");
			return;
		}
		catch (TA_Base_Core::ObjectResolutionException&)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "ObjectResolutionException", "Could not resolve Authentication Agent");
			return;
		}

		// The prefilter requires the session id.
		preFilter.addFilter(TA_Base_Bus::Filter::FILTER_PROFILE,
			TA_Base_Bus::Filter::COMPARE_EQUAL, m_sessionId);

		bool isCentralProfile = false;
		TA_Base_Core::IProfile* profile = NULL;

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
			catch (TA_Base_Core::DataException&)
			{
				if (NULL != profile)
				{
					delete profile;
				}

				LOG_EXCEPTION_CATCH(SourceInfo, "DataException", "Could not retrieve profile information");
			}
			catch (TA_Base_Core::DatabaseException&)
			{
				if (NULL != profile)
				{
					delete profile;
				}
				LOG_EXCEPTION_CATCH(SourceInfo, "DatabaseException", "Could not retrieve profile information");
			}
		}

		if (isCentralProfile)
		{
			// Central profiles require all locations.
			locations.clear();
		}
	}

	void AlarmManagerService::setProfileId()
	{
		FUNCTION_ENTRY("setProfileId");
		// Get the current profile
		std::vector<unsigned long> profiles;
		std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugDebug,
			"setProfileId function started with session ID obtained from RUNPARAM: %s, and member session ID: %s", sessionId.c_str(), m_sessionId.c_str());

		try
		{
			TA_Base_Bus::AuthenticationLibrary authLib(true);
			profiles = authLib.getProfiles(sessionId);
		}
		catch (const TA_Base_Core::AuthenticationAgentException& ex)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "AuthenticationAgentException", ex.what());
		}
		catch (const TA_Base_Core::ObjectResolutionException& ex)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "ObjectResolutionException", ex.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "Unknown error occurred while retrieving current profile.");
		}

		unsigned long profileKey = -1;
		if (profiles.size() == 0)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "No profiles were retrieved.");
		}
		else
		{
			profileKey = profiles[profiles.size() - 1];
		}

		m_profileId = profileKey;

		LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugDebug,
			"setProfileId function ended with profile key: %ul obtained", m_profileId);

		FUNCTION_EXIT;
	}

	void AlarmManagerService::filterOnAsset(const std::string& asset)
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "filterOnAsset");

		TA_Base_Bus::Filter filter;
		filter.addFilter(TA_Base_Bus::Filter::FILTER_ASSET_NAME,
			TA_Base_Bus::Filter::COMPARE_CONTAINS, asset);

		FilterEventArgs pEvent(false);
		pEvent.setFilter(&filter);
		
		notifyObservers(EV_FILTER_ON_ASSET, &pEvent);

		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "filterOnAsset");
	}


	void AlarmManagerService::filterOnEntity(const std::string& entity)
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "filterOnEntity");

		unsigned long entityKey;

		EntityNameToKeyMap::iterator itr = m_entityNameKeyMap.find(entity);

		if (m_entityNameKeyMap.end() == itr)
		{
			TA_Base_Core::IEntityData* entityData = NULL;
			try
			{
				entityData = TA_Base_Core::EntityAccessFactory::getInstance().getEntity(entity);
				entityKey = entityData->getKey();
				delete entityData;
				entityData = NULL;
				m_entityNameKeyMap[entity] = entityKey;
			}
			catch (...)
			{
				if (entityData != NULL)
				{
					delete entityData;
					entityData = NULL;
				}
				entityKey = 0;
			}

		}
		else
		{
			entityKey = itr->second;
		}

		TA_Base_Bus::Filter filter;
		filter.addFilter(TA_Base_Bus::Filter::FILTER_ASSOCIATED_ENTITY_KEY,
			TA_Base_Bus::Filter::COMPARE_EQUAL, entityKey);
		
		FilterEventArgs pEvent(false);
		pEvent.setFilter(&filter);
		notifyObservers(EV_FILTER_ON_ENTITY, &pEvent);

		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "filterOnEntity");
	}

	std::string AlarmManagerService::getStatusBarData()
	{
		FUNCTION_ENTRY("getStatusBarData");
		FUNCTION_EXIT;
		return m_statusBar;
	}
	
	void AlarmManagerService::setStatusBarData(const std::string& value)
	{
		FUNCTION_ENTRY("setStatusBarData");
		m_statusBar = value;
		FUNCTION_EXIT;
	}
}