
#ifndef __SUPPRESSION_RULES_DLG_BUS_H_INCLUDED__
#define __SUPPRESSION_RULES_DLG_BUS_H_INCLUDED__

#include "bus/generic_gui_entity/src/ISystemStateUpdateObserver.h"
#include "bus/generic_gui_pmod/src/PModelInterface.h"

#include <map>

namespace TA_Base_Core
{
	class IAlarmRuleData;
	class SuppressionRuleData;
}

namespace TA_Base_App
{

	typedef struct
	{
		bool isCreatedByCurrentProfile;
		TA_Base_Core::SuppressionRuleData* suppressionRule;
	} RuleInfo;

	class SuppressionRulesDlgBus
		//: public TA_Base_Bus::ISystemStateUpdateObserver
	{
	public:
		SuppressionRulesDlgBus();
		virtual ~SuppressionRulesDlgBus();

		bool getAllRules(std::vector<TA_Base_Core::SuppressionRuleData*>& suppressionRules);
		bool getRulesByLocation(unsigned long locationKey, std::vector<TA_Base_Core::SuppressionRuleData*>& suppressionRules);
		void sendConfigUpdate(TA_Base_Core::IAlarmRuleData* suppressionRule);

		bool hasPermissionToRemoveRules();
		bool hasPermissionToViewAllRules();

		void removeRule(std::vector<RuleInfo*>& pRuleInfo);
		bool getCurrentOperatorsRegionAssignment(std::vector<unsigned long>& regionKeys);
		bool getRegionKeyByEntity(unsigned long entityKey, unsigned long& regionKey);
		bool filterByRegionAssignment(std::vector<TA_Base_Core::SuppressionRuleData*>& suppressionRules);

		void setCurrentLocation(const std::string locationName);
		unsigned long getCurrentLocationKey() { return m_currentLocationKey; };
		int getRefreshTime() { return m_refreshTime; };

		std::map<std::string, unsigned long> getAllLocationData();

		void setProfileID(unsigned long profileId) { m_profileId = profileId; };
		unsigned long getProfileID() { return m_profileId; };
	private:
		// Default refresh time for list
		static const int DEFAULT_REFRESH_TIME;
		static const std::string REFRESH_TIME_PARAM;

		int m_refreshTime;
		unsigned long m_profileId;
		unsigned long m_currentLocationKey;
		std::map<std::string, unsigned long> m_locationNameKeyMap;

	};
}


#endif //__SUPPRESSION_RULES_DLG_BUS_H_INCLUDED__