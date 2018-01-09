#pragma once

#include "bus/generic_gui_pmod/src/TransActiveViewModel.h"
#include "bus\alarm\shared_alarm\src\AlarmDataItem.h"
#include "core\message\src\ConfigUpdateMessageSender.h"
#include "boost/shared_ptr.hpp"

// Forward declaration
namespace TA_Base_Core
{
	struct AlarmDetailCorbaDef;
	class IAlarmRuleData;
}

namespace TA_Base_Bus
{
	class SuppressAlarmDlgPModel
		: public AbstractPModel
	{

	public:
		SuppressAlarmDlgPModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
		~SuppressAlarmDlgPModel();

		bool wasSuccessful();
		void setWasSuccessful(bool bSuccessful);
		void inhibitAlarmByType();

		void inhibitAlarmByDescription();

		void setAlarmDetails(boost::shared_ptr<TA_Base_Bus::AlarmDataItem> alarm, std::string createdBySessionKey, unsigned long createdByProfileKey, std::string createdByOperatorName);

		std::string getInhibitAlarmTypeConfirmText();
		std::string getInhibitAlarmByDescConfirmText();
		std::string setUpDialogTitle(const std::string& dialogTitle);
	private:	
		void sendConfigUpdate(TA_Base_Core::IAlarmRuleData* suppressionRule);
		void showErrorMsg(std::string reason, std::string type);

	private:
		boost::shared_ptr<TA_Base_Bus::AlarmDataItem> m_alarmDetails;
		bool m_successful;
		std::string m_createdBySessionKey;
		unsigned long m_createdByProfileKey;
		std::string m_createdByOperatorName;

	};

}