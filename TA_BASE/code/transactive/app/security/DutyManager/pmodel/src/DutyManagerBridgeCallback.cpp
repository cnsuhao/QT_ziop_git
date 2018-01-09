

#include "app/security/DutyManager/pmodel/src/DutyManagerBridge.h"
#include "core/message/types/DutyAgentBroadcastComms_MessageTypes.h"

using namespace TA_Base_Bus;

namespace TA_Base_App
{

	void DutyManagerBridge::run()
	{		
		LOG_GENERIC(SourceInfo,DebugUtil::DebugDebug,"DutyManagerBridge::run()");
		loadData();

		LOG_GENERIC(SourceInfo,DebugUtil::DebugDebug,"DutyManagerBridge::run() end");
	}

	void DutyManagerBridge::terminate()
	{
		
	}

	void DutyManagerBridge::dataChanged(const TA_Base_Bus::DataUpdate& dataUpdate)
	{
		//TA_ASSERT(m_parentWnd != NULL, "Parent window is NULL!");
		typedef std::map<TA_Base_Core::EModificationType, std::string> ActionMap;
		typedef std::map<TA_Base_Core::EObjectType, std::string> ItemMap;
		
		static ActionMap actionMap;
		static ItemMap itemMap;
		
		if (actionMap.empty())
		{
			actionMap.insert(std::make_pair(TA_Base_Core::Create, std::string("added")));
			actionMap.insert(std::make_pair(TA_Base_Core::Update, std::string("modified")));
			actionMap.insert(std::make_pair(TA_Base_Core::Delete, std::string("deleted")));
		}
		
		if (itemMap.empty())
		{
			itemMap.insert(std::make_pair(TA_Base_Core::OPERATOR, std::string("Operator")));
			itemMap.insert(std::make_pair(TA_Base_Core::PROFILE, std::string("Profile")));
			itemMap.insert(std::make_pair(TA_Base_Core::ACTION_GROUP, std::string("Action group")));
			itemMap.insert(std::make_pair(TA_Base_Core::LOCATION, std::string("Location")));
			itemMap.insert(std::make_pair(TA_Base_Core::REGION, std::string("Region")));
			itemMap.insert(std::make_pair(TA_Base_Core::SUBSYSTEM, std::string("Subsystem")));
		}
		
		ActionMap::const_iterator actionIt(actionMap.find(dataUpdate.updateType));
		ItemMap::const_iterator itemIt(itemMap.find(dataUpdate.itemType));
		
		if (actionIt == actionMap.end() || itemIt == itemMap.end())
		{
			return;
		}
		
		std::ostringstream msg;
		msg << itemIt->second << " configuration data has been " << actionIt->second << "." << std::endl
			<< TA_Base_Core::RunParams::getInstance().get(RPARAM_APPNAME) << " should be restarted for any" << std::endl
			<< "changes to become visible.";

		DutyManagerEventArgs* pDmea = new DutyManagerEventArgs(DM_MESSAGE_BOX_WARNING,msg.str());
		notifyObservers(VIEW_DATA, pDmea);

	}

	void DutyManagerBridge::dutyChanged(const RegionDutyUpdate& dutyUpdate)
	{
		RegionDutyChangedEventArgs* pDmea = new RegionDutyChangedEventArgs(DM_RBN_DUTY_CHANGED,dutyUpdate);
		notifyObservers(VIEW_DATA, pDmea);
	}

	void DutyManagerBridge::dutyChanged(const TA_Base_Bus::SubsystemDutyUpdate& dutyUpdate)
	{
		SubsystemDutyChangedEventArgs* pDmea = new SubsystemDutyChangedEventArgs(DM_SUB_DUTY_CHANGED,dutyUpdate);
		notifyObservers(VIEW_DATA, pDmea);
	}
	
	void DutyManagerBridge::sessionChanged(const TA_Base_Bus::SessionUpdate& sessionUpdate)
	{
		OnReceiveSessionUpdate(sessionUpdate);

		SessionChangedEventArgs* pDmea = new SessionChangedEventArgs(DM_SESSION_CHANGED,sessionUpdate);
		notifyObservers(VIEW_DATA, pDmea);
	}





	void DutyManagerBridge::OnReceiveSessionUpdate(const TA_Base_Bus::SessionUpdate& sessionUpdate)
	{
		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugDebug,"begin DutyManagerBridge::OnReceiveSessionUpdate");

		TA_Base_Bus::SessionId currentSessionId(m_sessionCache.getCurrentSessionId());

		if ( (sessionUpdate.updateType == TA_Base_Bus::TA_Security::SessionEnd) ||	(sessionUpdate.sessionId != currentSessionId) )
		{
			//---------------session end --> close dlg

			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugDebug,"updateType==SessionEnd or not current session, so end DutyManagerBridge::OnReceiveSessionUpdate");

			return ;
		}

		m_previousProfileId = m_currentSessionInfo.ProfileId;
		m_currentSessionInfo = m_sessionCache.getSessionInfo(currentSessionId);    

		initialiseCurrentSession();

		LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugDebug,"end DutyManagerBridge::OnReceiveSessionUpdate");
	}	


	void DutyManagerBridge::receiveSpecialisedMessage(const TA_Base_Core::CommsMessageCorbaDef& message)
	{
		FUNCTION_ENTRY("receiveSpecialisedMessage(const TA_Base_Core::CommsMessageCorbaDef& message)");

		// Determine the type of duty update based on message type.
		if( 0 == TA_Base_Core::DutyAgentBroadcastComms::DutyAgentRecoverMessage.getTypeKey().compare(message.messageTypeKey.in()) )
		{
			TA_Base_Bus::DutyAgentTypeCorbaDef::Key locationKey = 0;
			if((message.messageState >>= locationKey) != 0)
			{
				LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, 
					"Received Duty Agent switch over message for location %d. Will clean the duty cache", 
					locationKey );	
				//PostMessage(WM_USER_RECEIVE_DUTY_AGENT_RECOVER_UPDATE, 0, locationKey);

				DutyManagerEventArgs* pDmea = new DutyManagerEventArgs(DUTY_AGENT_RECOVER_UPDATE,locationKey);
				notifyObservers(VIEW_DATA, pDmea);
			}
		}


		FUNCTION_EXIT;

	}


}