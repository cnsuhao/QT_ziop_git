

#include "app/alarm/alarm_manager/pmodel/src/SuppressionRulesDlgBus.h"
#include "core/data_access_interface/alarm_rule/src/AlarmRuleAccessFactory.h"
#include "core/data_access_interface/alarm_rule/src/SuppressionRuleData.h"
#include "core/data_access_interface/alarm_rule/src/IAlarmRuleData.h"
#include "core/data_access_interface/alarm_rule/src/AlarmRuleData.h"
#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/data_access_interface/entity_access/src/EntityData.h"
#include "core/data_access_interface/src/IRegion.h"
#include "core/data_access_interface/src/RegionAccessFactory.h"
#include "core\data_access_interface\src\LocationAccessFactory.h"
#include "core\data_access_interface\src\ILocation.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/AuthenticationAgentException.h"
#include "core/exceptions/src/ObjectResolutionException.h"

#include "core/message/src/ConfigUpdateMessageSender.h"
#include "core/message/src/MessagePublicationManager.h"
#include "core/message/types/ConfigUpdate_MessageTypes.h"

#include "bus/alarm/alarm_list_control/pmodel/src/RightsChecker.h"

#include "bus/security/access_control/actions/src/AccessControlledActions.h"
#include "bus/security/authentication_library/src/AuthenticationLibrary.h"

#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"

namespace TA_Base_App
{
	const int SuppressionRulesDlgBus::DEFAULT_REFRESH_TIME = 30;
	const std::string SuppressionRulesDlgBus::REFRESH_TIME_PARAM = "InhibitionDlgRefreshTime";

	SuppressionRulesDlgBus::SuppressionRulesDlgBus()
	{
		//Obtain current location key
		std::string locationKey = TA_Base_Core::RunParams::getInstance().get(RPARAM_LOCATIONKEY);
		//Convert to unsigned long and store it
		m_currentLocationKey = atol(locationKey.c_str());

		//Initialise refresh time
		m_refreshTime = DEFAULT_REFRESH_TIME;
		//Obtain the refresh time from configuration, if any
		std::auto_ptr<TA_Base_Core::IData> entityParam = std::auto_ptr<TA_Base_Core::IData>(TA_Base_Core::EntityAccessFactory::getInstance().getParameter
			(TA_Base_Core::RunParams::getInstance().get(RPARAM_ENTITYNAME), REFRESH_TIME_PARAM));
		if (entityParam.get() != 0 && entityParam->getNumRows() > 0 && entityParam->isNull(0, "VALUE") == false)
		{
			m_refreshTime = entityParam->getIntegerData(0, "VALUE");
		}
	}

	SuppressionRulesDlgBus::~SuppressionRulesDlgBus()
	{

	}

	bool SuppressionRulesDlgBus::getAllRules(std::vector<TA_Base_Core::SuppressionRuleData*>& suppressionRules)
	{
		// Retrieve the rules
		bool bSuccess = true;
		try
		{
			suppressionRules = TA_Base_Core::AlarmRuleAccessFactory::getInstance().getAllSuppressionRules();
		}
		catch (const TA_Base_Core::DatabaseException& ex)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "DatabaseException", ex.what());
			bSuccess = false;
		}
		catch (const TA_Base_Core::DataException& ex)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "DataException", ex.what());
			bSuccess = false;
		}
		catch (...)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "An unexpected exception was caught while retrieving suppression rules.");
			bSuccess = false;
		}

		return bSuccess;
	}

	bool SuppressionRulesDlgBus::getRulesByLocation(unsigned long locationKey, std::vector<TA_Base_Core::SuppressionRuleData*>& suppressionRules)
	{
		bool bSuccess = true;
		try
		{
			suppressionRules = TA_Base_Core::AlarmRuleAccessFactory::getInstance().getSuppressionRulesByLocation(locationKey);
		}
		catch (const TA_Base_Core::DatabaseException& ex)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "DatabaseException", ex.what());
			bSuccess = false;
		}
		catch (const TA_Base_Core::DataException& ex)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "DataException", ex.what());
			bSuccess = false;
		}
		catch (...)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "An unexpected exception was caught while retrieving suppression rules.");
			bSuccess = false;
		}

		return bSuccess;
	}

	void SuppressionRulesDlgBus::sendConfigUpdate(TA_Base_Core::IAlarmRuleData* suppressionRule)
	{
		FUNCTION_ENTRY("sendConfigUpdate");

		// First we iterate through and build up a list of changes. We are only interested in
		// which attributes have changed, not the values they have changed from and to.
		std::stringstream entityKey;
		entityKey << suppressionRule->getKey();
		std::vector<std::string> changes;
		changes.push_back(entityKey.str());

		// Get the key
		unsigned long keyOfChangedData = suppressionRule->getKey();

		// Send the message
		TA_Base_Core::ConfigUpdateMessageSender* configMessageSender = 0;
		configMessageSender = TA_Base_Core::MessagePublicationManager::getInstance().getConfigUpdateMessageSender(TA_Base_Core::ConfigUpdate::Context);
		TA_ASSERT(configMessageSender != NULL, "The config update message sender received is NULL");
		configMessageSender->sendConfigUpdateMessage(TA_Base_Core::ConfigUpdate::ConfigAlarmRule,
			keyOfChangedData,
			TA_Base_Core::Delete,
			changes);
		delete configMessageSender;
		configMessageSender = 0;

		FUNCTION_EXIT;
	}

	bool SuppressionRulesDlgBus::hasPermissionToRemoveRules()
	{
		FUNCTION_ENTRY("hasPermissionToRemoveRules");

		// Need alarm manager entity key to check the rights of this action
		TA_Base_Core::IEntityData* myEntity = TA_Base_Core::EntityAccessFactory::getInstance().getEntity(
			TA_Base_Core::RunParams::getInstance().get(RPARAM_ENTITYNAME));

		unsigned long entityKey = myEntity->getKey();
		delete myEntity;
		TA_Base_Bus::RightsChecker rightsChecker;

		if (!rightsChecker.canPerformAction(entityKey, TA_Base_Bus::aca_SUPER_REMOVE_ALARM_RULE))
		{
			if (!rightsChecker.canPerformAction(entityKey, TA_Base_Bus::aca_REMOVE_ALARM_RULE))
			{
				FUNCTION_EXIT;
				return false;
			}
		}

		FUNCTION_EXIT;
		return true;
	}


	bool SuppressionRulesDlgBus::hasPermissionToViewAllRules()
	{
		FUNCTION_ENTRY("hasPermissionToViewAllRules");

		// Need alarm manager entity key to check the rights of this action
		TA_Base_Core::IEntityData* myEntity = TA_Base_Core::EntityAccessFactory::getInstance().getEntity(
			TA_Base_Core::RunParams::getInstance().get(RPARAM_ENTITYNAME));

		unsigned long entityKey = myEntity->getKey();
		delete myEntity;
		TA_Base_Bus::RightsChecker rightsChecker;

		if (!rightsChecker.canPerformAction(entityKey, TA_Base_Bus::aca_SHOW_ALL_SYSTEM_ALARM_INHIBITION))
		{
			if (!rightsChecker.canPerformAction(entityKey, TA_Base_Bus::aca_SHOW_ALL_SYSTEM_ALARM_INHIBITION))
			{
				FUNCTION_EXIT;
				return false;
			}
		}

		FUNCTION_EXIT;
		return true;
	}

	bool SuppressionRulesDlgBus::filterByRegionAssignment(std::vector<TA_Base_Core::SuppressionRuleData*>& suppressionRules)
	{
		FUNCTION_ENTRY("filterByRegionAssignment");

		std::vector<unsigned long> regionKeys;
		getCurrentOperatorsRegionAssignment(regionKeys);

		// Filter out the rules that don't match the regions
		std::vector<TA_Base_Core::SuppressionRuleData*>::iterator itRule = suppressionRules.begin();
		while (itRule != suppressionRules.end())
		{
			if (*itRule != NULL)
			{
				unsigned long entityKey = (*itRule)->getEntityKey();
				unsigned long regionKey = 0;

				if (getRegionKeyByEntity(entityKey, regionKey))
				{
					std::vector<unsigned long>::iterator itRegionKey = std::find(regionKeys.begin(), regionKeys.end(), regionKey);
					if (itRegionKey != regionKeys.end())
					{
						++itRule;

						// Don't delete the pointer, these will be deleted when the list is cleared.
					}
					else
					{
						delete *itRule;
						*itRule = NULL;

						itRule = suppressionRules.erase(itRule);
					}
				}
			}
		}

		FUNCTION_EXIT;
		return true;
	}

	bool SuppressionRulesDlgBus::getCurrentOperatorsRegionAssignment(std::vector<unsigned long>& regionKeys)
	{
		FUNCTION_ENTRY("getCurrentOperatorsRegionAssignment");

		// Get the current operator
		std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
		unsigned long operatorId = 0;

		try
		{
			TA_Base_Bus::AuthenticationLibrary authLib(true);
			TA_Base_Bus::SessionInfo sessionInfo = authLib.getSessionInfo(sessionId, sessionId);
			operatorId = sessionInfo.UserId;
		}
		catch (const TA_Base_Core::AuthenticationAgentException& ex)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "AuthenticationAgentException", ex.what());
			FUNCTION_EXIT;
			return false;
		}
		catch (const TA_Base_Core::ObjectResolutionException& ex)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "ObjectResolutionException", ex.what());
			FUNCTION_EXIT;
			return false;
		}
		catch (...)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "An unexpection exception was caught while retrieving the current operator.");
			FUNCTION_EXIT;
			return false;
		}

		// Get the operator's region assignment
		std::vector<TA_Base_Core::IRegion*> regions;

		try
		{
			regions = TA_Base_Core::RegionAccessFactory::getInstance().getRegionsByOperator(operatorId);
		}
		catch (const TA_Base_Core::DatabaseException& ex)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "DatabaseException", ex.what());
			FUNCTION_EXIT;
			return false;
		}
		catch (const TA_Base_Core::DataException& ex)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "DataException", ex.what());
			FUNCTION_EXIT;
			return false;
		}
		catch (...)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "An unexpection exception was caught while retrieving the current operator's regions.");
			FUNCTION_EXIT;
			return false;
		}

		// Retrieve the region keys
		regionKeys.clear();
		std::vector<TA_Base_Core::IRegion*>::iterator itRegion;
		for (itRegion = regions.begin(); itRegion != regions.end(); ++itRegion)
		{
			if (*itRegion != NULL)
			{
				regionKeys.push_back((*itRegion)->getKey());

				delete *itRegion;
				*itRegion = NULL;
			}
		}

		FUNCTION_EXIT;
		return true;
	}


	bool SuppressionRulesDlgBus::getRegionKeyByEntity(unsigned long entityKey, unsigned long& regionKey)
	{
		try
		{
			TA_Base_Core::IEntityData* iEntityData = TA_Base_Core::EntityAccessFactory::getInstance().getEntity(entityKey);
			TA_Base_Core::EntityData* entityData = dynamic_cast <TA_Base_Core::EntityData*> (iEntityData);
			regionKey = entityData->getRegion();
			return true;
		}
		catch (const TA_Base_Core::DatabaseException& ex)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "DatabaseException", ex.what());
		}
		catch (const TA_Base_Core::DataException& ex)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "DataException", ex.what());
		}
		catch (...)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "An unexpected exception was caught while retrieving region key.");
		}

		return false;
	}


	void SuppressionRulesDlgBus::removeRule(std::vector<RuleInfo*>& pRuleInfo)
	{
		FUNCTION_ENTRY("removeRule");
		for (int idx = 0; idx < pRuleInfo.size(); idx++)
		{
			try
			{
				RuleInfo* ruleInfo = pRuleInfo[idx];
				TA_Base_Core::SuppressionRuleData* ruleDetails = ruleInfo->suppressionRule;

				ruleDetails->deleteThisObject();
				sendConfigUpdate(ruleDetails);

			}
			catch (const TA_Base_Core::DatabaseException& e)
			{
				std::ostringstream error;
				error << "Remove Suppression Rule failed " << e.what();

				//Todo: notify
				// TD14164 azenitha++
				/*TA_Base_Bus::TransActiveMessage userMsg;
				CString reason = e.what();
				userMsg << reason;
				userMsg.showMsgBox(IDS_UE_040041);*/
				/*AfxMessageBox(error.str().c_str());*/
				// TD14164 ++azenitha
			}
		}

		FUNCTION_EXIT;
	}

	std::map<std::string, unsigned long> SuppressionRulesDlgBus::getAllLocationData()
	{
		std::vector<TA_Base_Core::ILocation*>allLocationData;
		try
		{
			allLocationData = TA_Base_Core::LocationAccessFactory::getInstance().getAllLocations();
		}
		catch (TA_Base_Core::DatabaseException&)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError,
				"DatabaseException caught whilst retrieving locations. There were problems communicating with the database");
			for (unsigned int i = 0; i < allLocationData.size(); ++i)
			{
				delete allLocationData[i];
			}
			allLocationData.clear();
		}
		catch (TA_Base_Core::DataException&)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError,
				"DataException caught whilst retrieving locations. The data could not be converted to the required format");
			for (unsigned int i = 0; i < allLocationData.size(); ++i)
			{
				delete allLocationData[i];
			}
			allLocationData.clear();
		}
		catch (...)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError,
				"Caught unknown exception whilst retrieving locations.");
			for (unsigned int i = 0; i < allLocationData.size(); ++i)
			{
				delete allLocationData[i];
			}
			allLocationData.clear();
		}

		// store the locations into the map and delete the temporary structure
		m_locationNameKeyMap.clear();
		int count = 0;
		for (unsigned int i = 0; i < allLocationData.size(); ++i)
		{
			if (allLocationData[i]->getKey() != 0)
			{
				m_locationNameKeyMap.insert(std::pair<std::string, unsigned long>(allLocationData[i]->getName(), allLocationData[i]->getKey()));
				delete allLocationData[i];
			}
		}
		allLocationData.clear();

		return m_locationNameKeyMap;
	}

	void SuppressionRulesDlgBus::setCurrentLocation(const std::string locationName)
	{
		std::map<std::string, unsigned long>::iterator itr = m_locationNameKeyMap.find(locationName);
		if (itr != m_locationNameKeyMap.end())
		{
			// Store the key
			m_currentLocationKey = itr->second;
		}
	}
}