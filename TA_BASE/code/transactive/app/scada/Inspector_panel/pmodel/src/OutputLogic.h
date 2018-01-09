#pragma  once

#include <string>
#include "bus/scada/common_library/src/CommonDefs.h"
#include "Equipment.h"
#include "InspectorPanelAuditMessageSender.h"


using namespace TA_Base_Bus;

namespace TA_Base_App
{
	class OutputLogic
	{
	public:
		OutputLogic(CEquipment*	pEquipment);
		~OutputLogic();
		bool isOkToSendControl( unsigned int iPoint, const std::string& strValue);
		void setValue(unsigned int iPoint, const std::string& strValue, std::string& responseMsg);
		void setManuallyOverrideValue (unsigned int iPoint, const std::string& overrideValue, std::string& responseString);
		void removeOverride (unsigned int iPoint, std::string& responseString);
		void setAlarmInhibit(unsigned int iPoint, bool bRequestAlarmInhibit, std::string& responseString);
		void setMmsInhibit(unsigned int iPoint, bool bRequestMmsInhibit, std::string& responseString);
		void setControlInhibit(unsigned int iPoint, bool bRequestControlInhibit,std::string& responseString);
		void setScanInhibit(unsigned int iPoint, bool bRequestScanInhibit, std::string& responseString);
		void setMeteredCorrectionValue(unsigned int iPoint, double correction, std::string& responseString);
		void setNotes(const std::string& notes, std::string& responseString);
		void setTagState(const ETagState tagState, const EOutputInhibitState outputInhibitState,const EInputInhibitState inputInhibitState,const std::string& ptwId, const std::string& tagComment, std::string& responseString);
		void removeTagState(std::string& responseString);
		void setAlarmProperties (unsigned int iPoint, const AlarmProperty & newAlarmProperty,std::string& responseString );
	public:
	private:
		InspectorPanelAuditMessageSender  m_auditMessageSender;
		CEquipment*	m_pEquipment;
	};
}