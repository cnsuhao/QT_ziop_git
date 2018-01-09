
#ifndef __SUPPRESSION_RULES_DLG_PMODEL_H_INCLUDED__
#define __SUPPRESSION_RULES_DLG_PMODEL_H_INCLUDED__

#include "app/alarm/alarm_manager/pmodel/src/SubsytemDataCache.h"
#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"

#include "app\alarm\alarm_manager\pmodel\src\SuppressionRulesDlgBus.h"

namespace TA_Base_App
{
	class SuppressionRulesDlgPModel
		: public TA_Base_Bus::AbstractPModel
	{
	public:
		SuppressionRulesDlgPModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
		~SuppressionRulesDlgPModel();

		bool getAllRules(std::vector<TA_Base_Core::SuppressionRuleData*>& suppressionRules);
		bool getRulesByLocation(unsigned long locationKey, std::vector<TA_Base_Core::SuppressionRuleData*>& suppressionRules);
		void sendConfigUpdate(TA_Base_Core::IAlarmRuleData* suppressionRule);

		bool hasPermissionToRemoveRules();
		bool hasPermissionToViewAllRules();

		void removeRule(std::vector<RuleInfo*>& pRuleInfo);
		bool getCurrentOperatorsRegionAssignment(std::vector<unsigned long>& regionKeys);
		bool getRegionKeyByEntity(unsigned long entityKey, unsigned long& regionKey);

		void setCurrentLocation(const std::string locationName);
		unsigned long getCurrentLocationKey();
		int getRefreshTime();

		std::map<std::string, unsigned long> getAllLocationData();

		void setProfileID(unsigned long profileId);
		unsigned long getProfileID();

		// Column 0
		static const int REMOVABLE_COLUMN;
		static const char* REMOVABLE_COLUMN_NAME;
		static const int REMOVABLE_COLUMN_WIDTH;

		// Column 1
		static const int DATE_TIME_COLUMN;
		static const char* DATE_TIME_COLUMN_NAME;
		static const int DATE_TIME_COLUMN_WIDTH;

		// Column 2
		static const int ASSET_NAME_COLUMN;
		static const char* ASSET_NAME_COLUMN_NAME;
		static const int ASSET_NAME_COLUMN_WIDTH;

		// Column 3
		static const int ALARM_TYPE_COLUMN;
		static const char* ALARM_TYPE_COLUMN_NAME;
		static const int ALARM_TYPE_COLUMN_WIDTH;

		// Column 4
		static const int DESCRIPTION_COLUMN;
		static const char* DESCRIPTION_COLUMN_NAME;
		static const int DESCRIPTION_COLUMN_WIDTH;

		// Column 5
		static const int VALUE_COLUMN;
		static const char* VALUE_COLUMN_NAME;
		static const int VALUE_COLUMN_WIDTH;

		// Column 6
		static const int OPERATOR_COLUMN;
		static const char* OPERATOR_COLUMN_NAME;
		static const int OPERATOR_COLUMN_WIDTH;

		static const int REMOVABLE_IMAGE;
		static const int BLANK_IMAGE;

		typedef enum
		{
			REMOVABLE = 0,
			DATE_TIME,
			ASSET_NAME,
			ALARM_TYPE,
			DESCRIPTION,
			VALUE,
			OPERATOR
		} EColumnType;

	private:
		SuppressionRulesDlgBus* m_pSuppressRuleBusLogic;
	};

}

#endif //__SUPPRESSION_RULES_DLG_PMODEL_H_INCLUDED__