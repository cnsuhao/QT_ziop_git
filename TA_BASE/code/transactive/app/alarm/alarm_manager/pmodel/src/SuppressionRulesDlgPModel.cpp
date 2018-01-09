
#include "app\alarm\alarm_manager\pmodel\src\SuppressionRulesDlgPModel.h"
#include "app\alarm\alarm_manager\pmodel\src\SuppressionRulesDlgBus.h"

namespace TA_Base_App
{
	// Column 0
	const int SuppressionRulesDlgPModel::REMOVABLE_COLUMN = 0;
	const char* SuppressionRulesDlgPModel::REMOVABLE_COLUMN_NAME = "Removable";
	const int SuppressionRulesDlgPModel::REMOVABLE_COLUMN_WIDTH = 80;

	// Column 1
	const int SuppressionRulesDlgPModel::DATE_TIME_COLUMN = 1;
	const char* SuppressionRulesDlgPModel::DATE_TIME_COLUMN_NAME = "Date / Time";
	const int SuppressionRulesDlgPModel::DATE_TIME_COLUMN_WIDTH = 130;

	// Column 2
	const int SuppressionRulesDlgPModel::ASSET_NAME_COLUMN = 2;
	const char* SuppressionRulesDlgPModel::ASSET_NAME_COLUMN_NAME = "Asset Name";
	const int SuppressionRulesDlgPModel::ASSET_NAME_COLUMN_WIDTH = 190;

	// Column 3
	const int SuppressionRulesDlgPModel::ALARM_TYPE_COLUMN = 3;
	const char* SuppressionRulesDlgPModel::ALARM_TYPE_COLUMN_NAME = "Alarm Type";
	const int SuppressionRulesDlgPModel::ALARM_TYPE_COLUMN_WIDTH = 140;

	// Column 4
	const int SuppressionRulesDlgPModel::DESCRIPTION_COLUMN = 4;
	const char* SuppressionRulesDlgPModel::DESCRIPTION_COLUMN_NAME = "Description";
	const int SuppressionRulesDlgPModel::DESCRIPTION_COLUMN_WIDTH = 540;

	// Column 5
	const int SuppressionRulesDlgPModel::VALUE_COLUMN = 5;
	const char* SuppressionRulesDlgPModel::VALUE_COLUMN_NAME = "Value";
	const int SuppressionRulesDlgPModel::VALUE_COLUMN_WIDTH = 70;

	// Column 6
	const int SuppressionRulesDlgPModel::OPERATOR_COLUMN = 6;
	const char* SuppressionRulesDlgPModel::OPERATOR_COLUMN_NAME = "Operator";
	const int SuppressionRulesDlgPModel::OPERATOR_COLUMN_WIDTH = 77;

	const int SuppressionRulesDlgPModel::REMOVABLE_IMAGE = 0;
	const int SuppressionRulesDlgPModel::BLANK_IMAGE = 1;

	SuppressionRulesDlgPModel::SuppressionRulesDlgPModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName)
		: TA_Base_Bus::AbstractPModel(appController, pmodelName)
		, m_pSuppressRuleBusLogic(0)
	{
		m_pSuppressRuleBusLogic = new SuppressionRulesDlgBus();
	}

	SuppressionRulesDlgPModel::~SuppressionRulesDlgPModel()
	{

	}

	bool SuppressionRulesDlgPModel::getAllRules(std::vector<TA_Base_Core::SuppressionRuleData*>& suppressionRules)
	{
		return m_pSuppressRuleBusLogic->getAllRules(suppressionRules);
	}

	bool SuppressionRulesDlgPModel::getRulesByLocation(unsigned long locationKey, std::vector<TA_Base_Core::SuppressionRuleData*>& suppressionRules)
	{
		return m_pSuppressRuleBusLogic->getRulesByLocation(locationKey, suppressionRules);
	}
	void SuppressionRulesDlgPModel::sendConfigUpdate(TA_Base_Core::IAlarmRuleData* suppressionRule)
	{
		m_pSuppressRuleBusLogic->sendConfigUpdate(suppressionRule);
	}

	bool SuppressionRulesDlgPModel::hasPermissionToRemoveRules()
	{
		return m_pSuppressRuleBusLogic->hasPermissionToRemoveRules();
	}

	bool SuppressionRulesDlgPModel::hasPermissionToViewAllRules()
	{
		return m_pSuppressRuleBusLogic->hasPermissionToViewAllRules();
	}

	void SuppressionRulesDlgPModel::removeRule(std::vector<RuleInfo*>& pRuleInfo)
	{
		m_pSuppressRuleBusLogic->removeRule(pRuleInfo);
	}

	bool SuppressionRulesDlgPModel::getCurrentOperatorsRegionAssignment(std::vector<unsigned long>& regionKeys)
	{
		return m_pSuppressRuleBusLogic->getCurrentOperatorsRegionAssignment(regionKeys);
	}

	bool SuppressionRulesDlgPModel::getRegionKeyByEntity(unsigned long entityKey, unsigned long& regionKey)
	{
		return m_pSuppressRuleBusLogic->getRegionKeyByEntity(entityKey, regionKey);
	}

	void SuppressionRulesDlgPModel::setCurrentLocation(const std::string locationName)
	{
		m_pSuppressRuleBusLogic->setCurrentLocation(locationName);
	}

	unsigned long SuppressionRulesDlgPModel::getCurrentLocationKey()
	{
		return m_pSuppressRuleBusLogic->getCurrentLocationKey();
	}

	int SuppressionRulesDlgPModel::getRefreshTime()
	{
		return m_pSuppressRuleBusLogic->getRefreshTime();
	}

	std::map<std::string, unsigned long> SuppressionRulesDlgPModel::getAllLocationData()
	{
		return m_pSuppressRuleBusLogic->getAllLocationData();
	}

	void SuppressionRulesDlgPModel::setProfileID(unsigned long profileId)
	{
		m_pSuppressRuleBusLogic->setProfileID(profileId);
	}

	unsigned long SuppressionRulesDlgPModel::getProfileID()
	{
		return m_pSuppressRuleBusLogic->getProfileID();
	}
}