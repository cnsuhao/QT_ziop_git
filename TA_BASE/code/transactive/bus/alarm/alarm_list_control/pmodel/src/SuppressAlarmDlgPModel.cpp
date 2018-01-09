#include "SuppressAlarmDlgPModel.h"
#include "AlarmListModelConsts.h"
#include "bus/TA_MessageResource/resource.h"
#include "core/data_access_interface/src/IAlarmTypeData.h"
#include "core/data_access_interface/src/AlarmTypeAccessFactory.h"
#include "core/data_access_interface/alarm_rule/src/IAlarmRuleData.h"
#include "core/data_access_interface/alarm_rule/src/SuppressionRuleData.h"
#include "core/data_access_interface/alarm_rule/src/AlarmRuleAccessFactory.h"
#include "core/data_access_interface/src/AlarmTypeData.h"
#include "core/data_access_interface/src/AlarmTypeAccessFactory.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"

#include "core/message/src/ConfigUpdateMessageSender.h"
#include "core/message/src/MessagePublicationManager.h"
#include "core/message/types/ConfigUpdate_MessageTypes.h"

#include "core/alarm/IDL/src/AlarmMessageCorbaDef.h"

#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/DataConfigurationException.h"
#include "core/exceptions/src/AlarmActionException.h"

namespace TA_Base_Bus
{

	SuppressAlarmDlgPModel::SuppressAlarmDlgPModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
		: TA_Base_Bus::AbstractPModel(appController, pmodelName)
		, m_successful(true)
		, m_createdByProfileKey(0)
	{
	}

	SuppressAlarmDlgPModel::~SuppressAlarmDlgPModel()
	{
	}

	void SuppressAlarmDlgPModel::setAlarmDetails(boost::shared_ptr<TA_Base_Bus::AlarmDataItem> alarm, std::string createdBySessionKey, unsigned long createdByProfileKey, std::string createdByOperatorName)
	{
		m_alarmDetails = alarm;
		m_createdBySessionKey = createdBySessionKey;
		m_createdByProfileKey = createdByProfileKey;
		m_createdByOperatorName = createdByOperatorName;
	}

	bool SuppressAlarmDlgPModel::wasSuccessful()
	{
		return m_successful;
	}

	void SuppressAlarmDlgPModel::setWasSuccessful(bool bSuccessful)
	{
		m_successful = bSuccessful;
	}

	void SuppressAlarmDlgPModel::inhibitAlarmByType()
	{
		FUNCTION_ENTRY("inhibitAlarmByType");

		// Get out the required alarm data
		//TA_Base_Core::IAlarmTypeData* alarmTypeData = TA_Base_Core::AlarmTypeAccessFactory::getInstance().getAlarmType( messageTypeKey, false );

		unsigned long entityKey = m_alarmDetails->assocEntityKey;
		//unsigned long alarmTypeKey = 0;
		unsigned long entityTypeKey = 0;

		TA_Base_Core::IAlarmTypeData* typeData = 0;
		typeData = TA_Base_Core::AlarmTypeAccessFactory::getInstance().getAlarmTypeByKey(m_alarmDetails->messageTypeKey, false);
		//alarmTypeKey = typeData->getKey();
		// need to convert the alarmTypeKey into a string
		//std::ostringstream atkStream;
		//atkStream << alarmTypeKey;

		std::string alarmAssetName(m_alarmDetails->assetName);
		std::string alarmDescription(m_alarmDetails->alarmDescription);
		std::string alarmValue(m_alarmDetails->alarmValue);

		// create the rule based on the alarm's data...
		TA_Base_Core::IAlarmRuleData* suppressionRule = TA_Base_Core::AlarmRuleAccessFactory::getInstance().createSuppressionRule(
			entityTypeKey,
			entityKey,
			m_alarmDetails->messageTypeKey,
			"submit",
			m_createdBySessionKey,
			alarmAssetName,
			alarmDescription,
			alarmValue
			);

		TA_Base_Core::SuppressionRuleData* suppressionDescRule = dynamic_cast<TA_Base_Core::SuppressionRuleData*> (suppressionRule);
		suppressionDescRule->setOperatorName(m_createdByOperatorName);
		suppressionDescRule->setProfileKey(m_createdByProfileKey);
		// and save to the database...
		bool successful = false;
		std::string reason("");
		try
		{
			suppressionRule->applyChanges();
			sendConfigUpdate(suppressionRule);
			successful = true;
		}
		catch (TA_Base_Core::DatabaseException& dbe)
		{
			reason = dbe.what();
			showErrorMsg(reason, "type");
			LOG_EXCEPTION_CATCH(SourceInfo, "DatabaseException", reason.c_str());
			return;
		}
		catch (TA_Base_Core::DataException& de)
		{
			reason = de.what();
			showErrorMsg(reason, "type");
			LOG_EXCEPTION_CATCH(SourceInfo, "DataException", reason.c_str());
			return;
		}
		catch (TA_Base_Core::DataConfigurationException& dce)
		{
			reason = dce.what();
			showErrorMsg(reason, "type");
			LOG_EXCEPTION_CATCH(SourceInfo, "DataConfigurationException", reason.c_str());
			return;
		}
		catch (...)
		{
			showErrorMsg("", "type");
			LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "Unknown exception occurred while saving suppression rule.");
			return;
		}

		// the new rule has been successfully applied/saved
		// send the config update
		//sendConfigUpdate(suppressionRule);

		FUNCTION_EXIT;
	}

	void SuppressAlarmDlgPModel::inhibitAlarmByDescription()
	{
		FUNCTION_ENTRY("inhibitAlarmByDescription");

		// Get out the required alarm data
		//TA_Base_Core::IAlarmTypeData* alarmTypeData = TA_Base_Core::AlarmTypeAccessFactory::getInstance().getAlarmType( messageTypeKey, false );

		unsigned long entityKey = m_alarmDetails->assocEntityKey;
		//unsigned long alarmTypeKey = 0;
		unsigned long entityTypeKey = 0;

		std::string alarmAssetName(m_alarmDetails->assetName);
		std::string alarmValue(m_alarmDetails->alarmValue);
		std::string alarmDescription(m_alarmDetails->alarmDescription);

		// create the rule based on the alarm's data...
		TA_Base_Core::IAlarmRuleData* suppressionRule = TA_Base_Core::AlarmRuleAccessFactory::getInstance().createSuppressionByDescriptionRule(
			entityTypeKey,
			entityKey,
			m_alarmDetails->messageTypeKey,
			"submit",
			alarmDescription,
			m_createdBySessionKey,
			alarmAssetName,
			alarmValue
			);
		TA_Base_Core::SuppressionRuleData* suppressionDescRule = dynamic_cast<TA_Base_Core::SuppressionRuleData*>(suppressionRule);
		suppressionDescRule->setOperatorName(m_createdByOperatorName);
		suppressionDescRule->setProfileKey(m_createdByProfileKey);

		// and save to the database...
		bool successful = false;
		std::string reason("");
		try
		{
			suppressionRule->applyChanges();
			sendConfigUpdate(suppressionRule);
			successful = true;
		}
		catch (TA_Base_Core::DatabaseException& dbe)
		{
			reason = dbe.what();
			showErrorMsg(reason, "description");
			LOG_EXCEPTION_CATCH(SourceInfo, "DatabaseException", reason.c_str());
			return;
		}
		catch (TA_Base_Core::DataException& de)
		{
			reason = de.what();
			showErrorMsg(reason, "description");
			LOG_EXCEPTION_CATCH(SourceInfo, "DataException", reason.c_str());
			return;
		}
		catch (TA_Base_Core::DataConfigurationException& dce)
		{
			reason = dce.what();
			showErrorMsg(reason, "description");
			LOG_EXCEPTION_CATCH(SourceInfo, "DataConfigurationException", reason.c_str());
			return;
		}
		catch (...)
		{
			showErrorMsg("", "description");
			LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "Unknown exception occurred while saving suppression rule.");
			return;
		}

		// the new rule has been successfully applied/saved
		// send the config update
		//sendConfigUpdate(suppressionRule);

		FUNCTION_EXIT;
	}

	std::string SuppressAlarmDlgPModel::getInhibitAlarmTypeConfirmText()
	{
		TA_Base_Core::IAlarmTypeData* typeData = 0;
		typeData = TA_Base_Core::AlarmTypeAccessFactory::getInstance().getAlarmTypeByKey(m_alarmDetails->messageTypeKey, false);
		
		std::string alarmDescription(m_alarmDetails->alarmDescription);
		std::string alarmValue(m_alarmDetails->alarmValue);

		// confirm the selection and inhibition
		std::string confirmDesc("Inhibit by TYPE :\n");
		confirmDesc += "of Alarm Type: " + typeData->getName();
		delete typeData;

		return confirmDesc;
	}

	std::string SuppressAlarmDlgPModel::getInhibitAlarmByDescConfirmText()
	{
		std::string alarmDescription(m_alarmDetails->alarmDescription);

		// confirm the selection and inhibition
		// string up the confirmation description
		std::string confirmDesc("Inhibit by DESCRIPTION :\n");
		// confirmDesc += "Asset/Entity Name: " + alarmAssetName;
		// confirmDesc += "of Alarm Type: " + typeData->getName();
		confirmDesc += "of Description: " + alarmDescription;
		
		return confirmDesc;
	}
	
	void SuppressAlarmDlgPModel::sendConfigUpdate(TA_Base_Core::IAlarmRuleData* suppressionRule)
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
			TA_Base_Core::Create,
			changes);
		delete configMessageSender;
		configMessageSender = 0;

		FUNCTION_EXIT;
	}

	std::string SuppressAlarmDlgPModel::setUpDialogTitle(const std::string& dialogTitle)
	{
		std::string retVal = dialogTitle;
		// Set the title of the dialog
		std::string replaceString("[AssetName].[AlarmType]");

		int assetIndex = dialogTitle.find(replaceString, 0);
		std::string alarmDetail = std::string(m_alarmDetails->assetName.c_str()) + ".";

		// Set the alarm type in the dialog
		TA_Base_Core::IAlarmTypeData* iAlarmTypeData = NULL;
		std::string alarmType("");
		try
		{
			iAlarmTypeData = TA_Base_Core::AlarmTypeAccessFactory::getInstance().getAlarmTypeByKey(m_alarmDetails->messageTypeKey, false);
			alarmType = iAlarmTypeData->getName();
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
			LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "An unexpected exception was caught while retrieving alarm type.");
		}

		if (iAlarmTypeData != NULL)
		{
			delete iAlarmTypeData;
		}

		alarmDetail += alarmType.c_str();
		retVal.replace(assetIndex, assetIndex + replaceString.size(), alarmDetail);
		
		return retVal;
	}

	void SuppressAlarmDlgPModel::showErrorMsg(std::string reason, std::string type)
	{
		// if the rule couldn't be save
		// let the user know why :)
		std::string errorMsg("");
		if (reason == "")
		{
			// Log the error message
			errorMsg += "An unknown error occurred preventing the inhibit-alarm-by-";
			errorMsg += type.c_str();
			errorMsg += " rule from being saved";
		}
		else
		{
			// Log a message to user that there may be duplication
			errorMsg += "An error occurred preventing the inhibit-alarm-by-";
			errorMsg += type.c_str();
			errorMsg += " rule from being saved.\nCause: ";
			errorMsg += reason.c_str();
		}

		// No need to double log the error
		// LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Error: %s", errorMsg.c_str() );
		
		//Todo: Request User Reply
		//TA_Base_Bus::TransActiveMessage userMsg;
		//userMsg << errorMsg;
		//TAUnicodeString errMsg = userMsg.constructMessage(IDS_UE_050130);
		//AfxMessageBox(errMsg.c_str(), MB_OK);

		TA_Base_Bus::UINotificationDataContextPtr dataContextPtr(new TA_Base_Bus::UINotificationDataContext(IDS_UE_050130));
		dataContextPtr->pushParamValue(errorMsg);

		TA_Base_Bus::UIReplyEventArgsPtr eventArgs(new TA_Base_Bus::UIReplyEventArgs(dataContextPtr, APP_NOTIFY_INHIBIT_ERROR));
		m_appController->getWinApp()->requestUserReply(eventArgs);

	}
}
