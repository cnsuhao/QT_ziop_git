/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3002_TIP/TA_BASE/transactive/app/scada/inspector_panel/src/AccessControlHelper.cpp $
  * @author:  Ripple
  * @version: $Revision: #7 $
  *
  * Last modification: $DateTime: 2013/07/10 15:49:51 $
  * Last modified by:  $Author: jinmin.huang $
  *
  */

#include "AccessControlHelper.h"
//#include "app/scada/Inspector_Panel/src/InspectorPanel.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/synchronisation/src/ThreadGuard.h"
#include "core/utilities/src/RunParams.h"


using namespace TA_Base_Core;
using namespace TA_Base_Bus;

static const long TRY_CONNECT_RIGHT_AGENT_TIME = 500;

namespace TA_Base_App
{
	AccessControlHelper* AccessControlHelper::m_singleton = NULL;

	AccessControlHelper::~AccessControlHelper()
	{
		TA_THREADGUARD(m_entitySubsystemActionLock);
		m_EntityInfoVector.clear();
		m_entitySubsystemMap.clear();
		m_subsystemActionsMap.clear();
		m_ActionsList.clear();

		if(m_pRightsLib) delete m_pRightsLib;
	}

	AccessControlHelper::AccessControlHelper()
	:m_isRunning(false),
	 m_pRightsLib(NULL),
	 m_SessionId("")
	{ 
		InitActionsMap();

		TA_Base_Bus::RightsLibraryFactory rightsFactory;
		if ( TA_Base_Core::RunParams::getInstance().isSet("DebugAlwaysTrueRightsLib"))
		{
			m_pRightsLib = rightsFactory.buildRightsLibrary(TA_Base_Bus::ALWAYS_TRUE_RL);
		}
		else
		{
			m_pRightsLib = rightsFactory.buildRightsLibrary(TA_Base_Bus::DEFAULT_RL, TA_Base_Bus::RightsLibraryFactory::MANAGER);
		}
		
		m_SessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
	}

	AccessControlHelper* AccessControlHelper::getInstance()
	{
		if (m_singleton == NULL)
		{
			m_singleton = new AccessControlHelper;
		}

		return m_singleton;
	}

	// Initialises the access control map 
	void AccessControlHelper::InitAccessControlMap(const std::vector<AccessEntityInfo>& entityInfo)
	{
		if( m_isRunning == true)
		{
			this->terminateAndWait();
		}

		{
			TA_THREADGUARD(m_entitySubsystemActionLock);
			m_SessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
			m_EntityInfoVector	= entityInfo;
			AssortSubsystem();
		}

		if( m_isRunning == false)
		{	
			start();
		}
		
	}

	void	AccessControlHelper::updateSession(const std::string& newSession)
	{
		if(m_SessionId.size() > 0 && newSession.size() > 0 && m_SessionId != newSession )
		{
			if( m_isRunning == true)
			{
				this->terminateAndWait();
			}

			{
				TA_THREADGUARD(m_entitySubsystemActionLock);
				m_SessionId = newSession;
				AssortSubsystem();
			}
			
			if( m_isRunning == false)
			{	
				start();
			}
		}
	}


	void	AccessControlHelper::registerObserver(IPermissionnNotification* observer)
	{
		TA_ASSERT(observer != NULL, "observer is NULL");
		TA_THREADGUARD(m_observerLock);
		m_observers.push_back(observer);
	}

	void	AccessControlHelper::unRegisterObserver(IPermissionnNotification* observer)
	{
		if(NULL == observer)
		{
			return;
		}

		TA_THREADGUARD(m_observerLock);
		//todo->
		m_observers.clear();
	}

	void	AccessControlHelper::notifyObservers()
	{
		TA_THREADGUARD(m_observerLock);
		for(unsigned int index = 0; index < m_observers.size(); index++)
		{
			m_observers[index]->processPermissionnChange();
		}
		
	}

	void AccessControlHelper::AssortSubsystem()
	{
		std::vector<AccessEntityInfo>::iterator entityInfoVectorIt ;
		{
			TA_THREADGUARD(m_entitySubsystemActionLock);
			m_entitySubsystemMap.clear();
			m_subsystemActionsMap.clear();

			for(entityInfoVectorIt = m_EntityInfoVector.begin();entityInfoVectorIt != m_EntityInfoVector.end();entityInfoVectorIt++)
			{
				m_entitySubsystemMap.insert(std::make_pair(entityInfoVectorIt->entityKey,entityInfoVectorIt->subSystemKey));

				if (m_subsystemActionsMap.find(entityInfoVectorIt->subSystemKey) == m_subsystemActionsMap.end())
				{
					std::set<unsigned long> tNew;
					tNew.clear();
					m_subsystemActionsMap.insert(std::make_pair(entityInfoVectorIt->subSystemKey,tNew));
				}			
			}
		}
		LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo,"m_subsystemActionsMap.size :%d", m_subsystemActionsMap.size());
	}

	void AccessControlHelper::InitEntitiesRights()
	{
		SubsystemActionsMap::iterator subsystemActionMapIt = m_subsystemActionsMap.end();
		for(subsystemActionMapIt = m_subsystemActionsMap.begin(); subsystemActionMapIt != m_subsystemActionsMap.end(); subsystemActionMapIt++)
		{
			std::list<unsigned long> t_actionList ,t_getActionList;
			t_actionList = m_ActionsList;

			std::vector<AccessEntityInfo>::iterator iter = m_EntityInfoVector.begin();
			for (iter = m_EntityInfoVector.begin();iter != m_EntityInfoVector.end();iter++)
			{
				if (iter->subSystemKey == subsystemActionMapIt->first ) break;
			}
			if ( iter == m_EntityInfoVector.end() ) continue;

			try
			{
				LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, 
					"try to get rights from RightsAgent. subSystemKey: %d.size of getActionList:%d", (*iter).subSystemKey,t_getActionList.size());
				/*m_pRightsMgr->areActionsPermittedOnSubsystemAndLocSub(
					m_SessionId.c_str(), 
					(*iter).entityKey, 
					(*iter).locationKey,
					(*iter).subSystemKey,
					t_actionList, 
					t_getActionList,
					reason, 
					decisionModule);*/

				this->areActionsPermittedOnSubsystemAndLocSub(
					m_SessionId.c_str(), 
					(*iter).entityKey, 
					(*iter).locationKey,
					(*iter).subSystemKey,
					t_actionList, 
					t_getActionList);
				
				LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, 
					"GotActionList from RightsAgent. subSystemKey: %d.size of getActionList:%d", (*iter).subSystemKey,t_getActionList.size());
			}
			catch (const TA_Base_Core::RightsException& ex)
			{							
				LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError, "Cannot get Rights From RihgtsAgent.TA_Base_Core::RightsException:%s", ex.what());
				t_getActionList.clear();
				throw ( TA_Base_Core::RightsException(ex.what()) );
			} 
			catch (...)
			{	
				LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError, "Cannot get Rights From RihgtsAgent.Unkown!");
				t_getActionList.clear();
			}	

			subsystemActionMapIt->second.clear();
			std::list<unsigned long>::iterator listIt = t_getActionList.begin();
			
			{
				TA_THREADGUARD(m_entitySubsystemActionLock);
				for (; listIt != t_getActionList.end(); listIt++)
				{
					subsystemActionMapIt->second.insert(*listIt);
				}
			}
		}
	}


	bool AccessControlHelper::IsActionPermitted(unsigned long aEntityKey, AccessControlActions aAccessControlType)
	{
		TA_THREADGUARD(m_entitySubsystemActionLock);

		std::map<unsigned long, unsigned long >::iterator entitySubsystemIt = m_entitySubsystemMap.find(aEntityKey);
		if (entitySubsystemIt == m_entitySubsystemMap.end()) 
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo,"permit false"); 		
			return false;
		}
		SubsystemActionsMap::iterator subsystemActionsIt = m_subsystemActionsMap.find(entitySubsystemIt->second);
		if (subsystemActionsIt == m_subsystemActionsMap.end()) 
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo,"permit false"); 
			return false;
		}
		std::set<unsigned long>::iterator actionsSetIt = subsystemActionsIt->second.find(aAccessControlType);
		if ( actionsSetIt == subsystemActionsIt->second.end()) 
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo,"permit false"); 
			return false;
		}
		LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo,"permit true"); 
		return true;		
	}


	// Function maps an internal action to the actual action id stored in the database
	//
	void AccessControlHelper::InitActionsMap()
	{
		TA_THREADGUARD(m_entitySubsystemActionLock);
		m_EntityInfoVector.clear();
		m_ActionsList.clear();
		m_ActionsList.push_back(DATA_POINT_WRITE);
		m_ActionsList.push_back(DATA_POINT_ALARM_INHIBIT);
		m_ActionsList.push_back(DATA_POINT_ALARM_UNINHIBIT);
		m_ActionsList.push_back(DATA_POINT_CONTROL_INHIBIT);
		m_ActionsList.push_back(DATA_POINT_CONTROL_UNINHIBIT);
		m_ActionsList.push_back(DATA_POINT_SCAN_UNINHIBIT);
		m_ActionsList.push_back(DATA_POINT_SCAN_INHIBIT);
		m_ActionsList.push_back(DATA_POINT_MMS_INHIBIT);
		m_ActionsList.push_back(DATA_POINT_MMS_UNINHIBIT);
		m_ActionsList.push_back(DATA_POINT_SET_FORCED_STATE);
		m_ActionsList.push_back(DATA_POINT_RESET_FORCED_STATE);
		m_ActionsList.push_back(DATA_POINT_METER_ADJUST);
		m_ActionsList.push_back(DATA_NODE_TAG);
		m_ActionsList.push_back(DATA_NODE_NOTES);
		m_ActionsList.push_back(DATA_NODE_TAG_PTW);
		m_ActionsList.push_back(IP_LIMITS_TAB_ACCESS);
	}

	void AccessControlHelper::run()
	{
		m_isRunning 	= true;
		while(	m_isRunning)
		{
			try
			{
				LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "try to get entities rights");
				InitEntitiesRights();
				this->notifyObservers();
				m_isRunning = false;	
			}
			catch (const TA_Base_Core::RightsException& ex)
			{				
				//sleep(TRY_CONNECT_RIGHT_AGENT_TIME);
	
				for(int index =0;index <  10; index++)
				{
					if(!m_isRunning)
					{
						break;
					}
					else
					{
						m_condition.timedWait(TRY_CONNECT_RIGHT_AGENT_TIME);
					}
				}
				
			}
		}

		LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, 
					"Thread exit");
	}
	void AccessControlHelper::terminate()
	{
		m_isRunning = false;
		m_condition.signal();
	}



	void AccessControlHelper::areActionsPermittedOnSubsystemAndLocSub(const std::string& sessionId, unsigned long resourceKey, unsigned long locationKey, unsigned long subsystemKey,
			const std::list<unsigned long> & actionKeys, std::list<unsigned long> & returnActionList )
	{
		TA_Base_Bus::EDecisionModule decisionModule = TA_Base_Bus::UNDEFINED_DM;
		std::string reason("");
		
		returnActionList.clear();
		std::list<unsigned long>::const_iterator actionItr = actionKeys.begin();

		for(;actionItr!=actionKeys.end(); ++actionItr)
		{
			try
			{
				bool actionAllowed = m_pRightsLib->isActionPermittedOnResourceAndLocSub(
										sessionId, 
										resourceKey, 
                                        locationKey,
                                        subsystemKey,
										*actionItr, 
										reason, 
										decisionModule);
				if(actionAllowed)
				{
					returnActionList.push_back(*actionItr);
				}
			}
			catch(...)
			{
				LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError,"Catch unknown exception when calling rightslib");
			}
		}
	}
} // namespace