#pragma  once
#include "core\message\types\InspectorPanelAudit_MessageTypes.h"
#include "bus\scada\common_library\src\AuditMessageUtility.h"



namespace TA_Base_App
{
	class InspectorPanelAuditMessageSender
	{
	public:
		InspectorPanelAuditMessageSender();
		~InspectorPanelAuditMessageSender();
		void	submitInhibitAuditMessage(const std::string& inhibitType, const std::string& addRemove, const std::string& dataPoint, unsigned long entityKey);
		void	submitOverrideAuditMessage(const std::string& value, const std::string& addRemove, const std::string& dataPoint, unsigned long entityKey);
		void	submitNotesAuditMessage(const std::string& updateRemove, const std::string& dataNode, unsigned long entityKey);
		void	submitTagAuditMessage(const std::string& tagType, const std::string& addRemove, const std::string& dataNode, unsigned long entityKey);
	public:
	private:
		TA_Base_Bus::AuditMessageUtility		m_auditMessageUtility;
	};


	
}