
#include "app/security/DutyManager/pmodel/src/DutyManagerBridge.h"
//#include "app/security/DutyManager/pmodel/src/CommonDefine.h"

#include "bus/generic_gui_view/src/HelpLauncher.h"

#include "core/data_access_interface/src/RightsAccessFactory.h"

using namespace TA_Base_Bus;



TA_Base_App::DutyManagerBridge* TA_Base_App::DutyManagerBridge::s_instance					= NULL;
TA_Base_Core::NonReEntrantThreadLockable TA_Base_App::DutyManagerBridge::m_singletonLock;

namespace TA_Base_App
{
	
	DutyManagerBridge::DutyManagerBridge( ) :
		IConfigDataObserver		(DutyDataStore::getDataCache()				),
		IRegionDutyObserver		(DutyDataStore::getRegionDutyMatrix()		),
		ISubsystemDutyObserver	(DutyDataStore::getSubsystemDutyMatrix()	),
		ISessionObserver		(DutyDataStore::getSessionCache()			),				
		m_dataCache				(DutyDataStore::getDataCache()				),
		m_regionDutyMatrix		(DutyDataStore::getRegionDutyMatrix()		),
		m_subsystemDutyMatrix	(DutyDataStore::getSubsystemDutyMatrix()	),
		m_sessionCache			(DutyDataStore::getSessionCache()			),		
		m_isCentralProfile		(true)
	{
		m_originalTitleText = DutyManagerBridgeConsts::DEFAUNT_TITLE_TEXT;		

		//start(); start by appcontroller
	}
	
	DutyManagerBridge::~DutyManagerBridge()
	{
		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "~DutyManagerBridge()");

		TA_Base_Core::MessageSubscriptionManager::getInstance().unsubscribeToMessages(this);


		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "~DutyManagerBridge().terminate()");

		terminateAndWait();

		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "~DutyManagerBridge().m_actionGroupsDB");

		m_actionGroupsDB.clear();

		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "~DutyManagerBridge().DutyDataStore::cleanUp()");

		// do not cleanup in original version
		//DutyDataStore::cleanUp();

		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "~DutyManagerBridge() over");

	}
	
	DutyManagerBridge* DutyManagerBridge::getInstance()
	{
		TA_Base_Core::ThreadGuard guard(m_singletonLock);

		if (s_instance == NULL)
		{
			s_instance = new DutyManagerBridge();
			if (s_instance == NULL)
			{
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugFatal,"New failed to allocate AdminManager");
				exit(1);
			}
		}

		return s_instance;
	}

	void DutyManagerBridge::setTitleText( const std::string & originalTitleText )
	{
		m_originalTitleText = originalTitleText;
	}

	TA_Base_Bus::ActionGroupKey DutyManagerBridge::getActionGroupDB(
		const TA_Base_Bus::ProfileKey profileKey,
		const TA_Base_Bus::SubsystemKey subsystemKey,
		const TA_Base_Bus::ESubsystemState subsystemState)
	{
		LOG_GENERIC(SourceInfo,DebugUtil::DebugDebug,"begin OtherSesSubDutyPropertyPage::getActionGroupDB");

		TA_Base_Bus::ActionGroupKey actionGroupKey = NO_ACTION_GROUP;

		try
		{
			// find in cache
			for (int i=0; i<(int)m_actionGroupsDB.size(); i++)
			{
				if ((m_actionGroupsDB[i].profileKey == profileKey) &&
					(m_actionGroupsDB[i].subsystemKey == subsystemKey) &&
					(m_actionGroupsDB[i].subsystemState == subsystemState))
				{
					actionGroupKey = m_actionGroupsDB[i].actionGroupKey;
					break;
				}
			}

			// not yet in cache
			if (actionGroupKey == NO_ACTION_GROUP)
			{
				// find in DB	
				actionGroupKey = TA_Base_Core::RightsAccessFactory::getInstance().getActionGroup(
					profileKey,
					subsystemKey,
					subsystemState);

				// insert in list
				TA_Base_App::PSSA pssa;
				pssa.profileKey = profileKey;
				pssa.subsystemKey = subsystemKey;
				pssa.subsystemState = subsystemState;
				pssa.actionGroupKey = actionGroupKey;
				m_actionGroupsDB.push_back(pssa);
			}
		}
		catch (...)
		{
			LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "TD15533: Unknown exception in getActionGroupDB()");
		}
		LOG_GENERIC(SourceInfo,DebugUtil::DebugDebug,"end OtherSesSubDutyPropertyPage::getActionGroupDB");

		return actionGroupKey;
	}

	//void DutyManagerBridge::getTransferableRegionList(TransferableRegionList& trList)
	//{
	//	try
	//	{
	//		//trList = m_dutyAgent.getTransferableRegions(m_currentSessionInfo);
	//		//m_currentSessionInfo(m_sessionCache.getSessionInfo(m_sessionCache.getCurrentSessionId()));

	//		// sessionInfo = m_currentSessionInfo
	//		SessionInfo sessionInfo= m_sessionCache.getSessionInfo(m_sessionCache.getCurrentSessionId());
	//		trList = m_dutyAgent.getTransferableRegions(sessionInfo);

	//	}
	//	catch (...)
	//	{ 
	//		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugWarn, 
	//			"Ignoring invalid transferable region [r: %lu, o: %lu, p: %lu]",
	//			m_currentSessionInfo.regionKey, m_currentSessionInfo.operatorKey, m_currentSessionInfo.profileKey);
	//	}
	//}



	// get
	TA_Base_Bus::DataCache&	DutyManagerBridge::getDataCache()
	{	
		return m_dataCache;
	}
	TA_Base_Bus::SessionCache&	DutyManagerBridge::getSessionCashe()
	{
		return m_sessionCache;
	}
	TA_Base_Bus::RegionDutyMatrix& DutyManagerBridge::getRegionDutyMatrix()
	{
		return m_regionDutyMatrix;
	}

	TA_Base_Bus::SubsystemDutyMatrix& DutyManagerBridge::getSubsystemDutyMatrix()
	{
		return m_subsystemDutyMatrix;
	}

	TA_Base_Bus::SessionInfoEx DutyManagerBridge::getCurrentSessionInfo()
	{
		return m_sessionCache.getSessionInfo( m_sessionCache.getCurrentSessionId() );
	}

	TA_Base_Bus::ProfileKeyList	DutyManagerBridge::getPreviousProfileId()
	{
		return m_previousProfileId;
	}

	// HelpLauncher
	void DutyManagerBridge::OnTransActiveHelp()
	{
		TA_Base_Bus::HelpLauncher::getInstance().displayHelp(true);
	}

	void DutyManagerBridge::OnAppAbout()
	{
		TA_Base_Bus::HelpLauncher::getInstance().displayAboutBox();
	}

	// SessionCache
	SessionId DutyManagerBridge::getCurrentSessionId()
	{
		return m_sessionCache.getCurrentSessionId();
	}

	// DataCache
	std::string DutyManagerBridge::getRegionDisplayName(RegionKey key)
	{
		return m_dataCache.getRegionDisplayName(key);
	}

	std::string DutyManagerBridge::getRegionName(RegionKey key)
	{
		return m_dataCache.getRegionName(key);
	}
	std::string DutyManagerBridge::getOperatorDescription(OperatorKey key)
	{
		return m_dataCache.getOperatorDescription(key);
	}
	std::string DutyManagerBridge::getProfileName(ProfileKey key)
	{
		return m_dataCache.getProfileName(key);
	}

	// LocationAccessFactory
	std::string DutyManagerBridge::getLocationNameByLocationKey(const unsigned long locationKey)
	{
		TA_Base_Core::ILocation* locattion = TA_Base_Core::LocationAccessFactory::getInstance().getLocationByKey(locationKey);
		return locattion->getName();
	}

	// DutyRighst
	bool DutyManagerBridge::canTransferRegions()
	{
		return m_dutyRights.canTransferRegions();
	}
	bool DutyManagerBridge::canTransferSubsystems(long lActionId)
	{
		return m_dutyRights.canTransferSubsystems(lActionId);
	}
	bool DutyManagerBridge::canDegradeSubsystems()
	{
		return m_dutyRights.canDegradeSubsystems();
	}
	// DutyAgent
	void DutyManagerBridge::degradeAllSubsystems()
	{
		m_dutyAgent.degradeAllSubsystems(m_sessionCache.getCurrentSessionId());
	}

	void DutyManagerBridge::transferAllSubsystemDuty()
	{
		m_dutyAgent.transferAllSubsystemDuty(m_currentSessionInfo);
	}

	void DutyManagerBridge::transferRegionDuty(const TA_Base_Bus::SessionInfo& sessionInfo, const RegionKeyList& regionKeys)
	{
		m_dutyAgent.transferRegionDuty(sessionInfo, regionKeys);
	}
	void DutyManagerBridge::transferSubsystemDuty(const TA_Base_Bus::SessionInfo& sessionInfo, const SubsystemTreeMap& subsystems)
	{
		m_dutyAgent.transferSubsystemDuty(sessionInfo, subsystems);
	}

	TransferableRegionList DutyManagerBridge::getTransferableRegions(const SessionInfo& session)
	{
		return m_dutyAgent.getTransferableRegions(session);
	}


	TransferableSubsystemList DutyManagerBridge::getTransferableSubsystems(const SessionInfo& session, const RegionKeyList& regionKeys)
	{
		 return m_dutyAgent.getTransferableSubsystems(session, regionKeys);
	}
}