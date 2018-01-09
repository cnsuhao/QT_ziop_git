#include "InspectorPanelAuditMessageSender.h"


namespace TA_Base_App
{

	InspectorPanelAuditMessageSender::InspectorPanelAuditMessageSender()
		:m_auditMessageUtility (TA_Base_Core::InspectorPanelAudit::Context)
	{

	}

	
	InspectorPanelAuditMessageSender::~InspectorPanelAuditMessageSender()
	{

	}


	void InspectorPanelAuditMessageSender::submitInhibitAuditMessage(const std::string& inhibitType, const std::string& addRemove, 
		const std::string& dataPoint, unsigned long entityKey)
	{
		TA_Base_Core::DescriptionParameters descParams;

		TA_Base_Core::NameValuePair inhibitTypePair("InhibitType", inhibitType);
		TA_Base_Core::NameValuePair addRemovePair("AddedRemoved", addRemove);
		TA_Base_Core::NameValuePair dataPointPair("DataPoint", dataPoint);

		descParams.clear();
		descParams.push_back(&inhibitTypePair);
		descParams.push_back(&addRemovePair);
		descParams.push_back(&dataPointPair);

		m_auditMessageUtility.logAuditMessageWithOperatorId(TA_Base_Core::InspectorPanelAudit::DataPointInhibitAddRemove,
			descParams, entityKey);
	}


	void InspectorPanelAuditMessageSender::submitOverrideAuditMessage(const std::string& value, const std::string& addRemove, 
		const std::string& dataPoint, unsigned long entityKey)
	{
		TA_Base_Core::DescriptionParameters descParams;

		TA_Base_Core::NameValuePair valuePair("Value", value);
		TA_Base_Core::NameValuePair addRemovePair("AddedRemoved", addRemove);
		TA_Base_Core::NameValuePair dataPointPair("DataPoint", dataPoint);

		descParams.clear();
		descParams.push_back(&valuePair);
		descParams.push_back(&addRemovePair);
		descParams.push_back(&dataPointPair);

		m_auditMessageUtility.logAuditMessageWithOperatorId(TA_Base_Core::InspectorPanelAudit::DataPointOverrideAddRemove,
			descParams, entityKey);
	}

	void InspectorPanelAuditMessageSender::submitNotesAuditMessage(const std::string& updateRemove, 
		const std::string& dataNode, unsigned long entityKey)
	{
		TA_Base_Core::DescriptionParameters descParams;

		TA_Base_Core::NameValuePair updateRemovePair("UpdatedRemoved", updateRemove);
		TA_Base_Core::NameValuePair dataNodePair("DataNode", dataNode);

		descParams.clear();
		descParams.push_back(&updateRemovePair);
		descParams.push_back(&dataNodePair);

		m_auditMessageUtility.logAuditMessageWithOperatorId(TA_Base_Core::InspectorPanelAudit::DataNodeNotesUpdateRemove,
			descParams, entityKey);
	}


	void InspectorPanelAuditMessageSender::submitTagAuditMessage(const std::string& tagType, const std::string& addRemove, 
		const std::string& dataNode, unsigned long entityKey)
	{
		TA_Base_Core::DescriptionParameters descParams;

		TA_Base_Core::NameValuePair tagTypePair("TagType", tagType);
		TA_Base_Core::NameValuePair addRemovePair("AddedRemoved", addRemove);
		TA_Base_Core::NameValuePair dataNodePair("DataNode", dataNode);

		descParams.clear();
		descParams.push_back(&tagTypePair);
		descParams.push_back(&addRemovePair);
		descParams.push_back(&dataNodePair);

		m_auditMessageUtility.logAuditMessageWithOperatorId(TA_Base_Core::InspectorPanelAudit::DataNodeTagAddRemove,
			descParams, entityKey);
	}
}