#include "ace/OS.h"
#include "InspectorpanelAppController.h"
#include "core\utilities\src\RunParams.h"
#include "tagdialogmodel.h"
#include "bus\scada\proxy_library\src\IDataNodeProxy.h"
#include "bus\scada\proxy_library\src\IDataPointProxy.h"
#include "statusdialogmodel.h"
#include "bus\scada\proxy_library\src\DataPointProxySmartPtr.h"
#include "core\exceptions\src\ScadaProxyException.h"



namespace TA_Base_App
{

	StatusDialogModel::StatusDialogModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
	:AbstractEquipmentPModel(appController,pmodelName)
	{
	}


	StatusDialogModel::~StatusDialogModel(void)
	{
		
	}


	std::string		StatusDialogModel::getStatusInfo()
	{
		return m_text.str();
	}


	void	StatusDialogModel::updateDataNode(TA_Base_Bus::ScadaEntityUpdateType updateType)
	{
		/*try
		{
			TA_Base_Bus::DataNodeProxySmartPtr &dataNode = m_pEquipment->getDataNode();

			// Clear the edit box

			m_text.clear();

			m_text << "Diagnostic test information for Equipment Status\r\n\r\n";

			m_text << "DataNode CorbaName:\t" << dataNode->getAgentName() << ":" << dataNode->getEntityName() << "\r\n";
			m_text << "Proxy Config Valid:\t\t" << (dataNode->isConfigValid() ? "Yes" : "No") << "\r\n";
			m_text << "Entity Key:\t\t" << dataNode->getEntityKey() << "\r\n";

			if ( true == dataNode->isConfigValid())
			{
				m_text << "Description:\t\t"  << dataNode->getDescription() << "\r\n";
				m_text << "Label:\t\t\t"      << dataNode->getLabel() << "\r\n";
				m_text << "Short Label:\t\t"  << dataNode->getShortLabel() << "\r\n";
				m_text << "Asset Name:\t\t"   << dataNode->getAssetName() << "\r\n";
			}

			TA_Base_Bus::BooleanTag boolTag;
			TA_Base_Bus::EnumTag enumTag;

			m_text << "Quality Summary:\t\t";
			enumTag = dataNode->getQualitySummary();
			switch( ((long)enumTag.value) & TA_Base_Bus::QUALITY_MASK)
			{
			case TA_Base_Bus::QUALITY_BAD:
				m_text << "bad (" << TA_Base_Bus::QUALITY_BAD << ")";
				break;

			case TA_Base_Bus::QUALITY_UNCERTAIN:
				m_text << "uncertain (" << TA_Base_Bus::QUALITY_UNCERTAIN << ")";
				break;

			case TA_Base_Bus::QUALITY_GOOD:
				m_text << "good (" << TA_Base_Bus::QUALITY_GOOD << ")";
				break;
			}
			m_text << " - " << getQuality(enumTag.quality) << "\r\n";                

			m_text << "Alarm Summary:\t\t";
			boolTag = dataNode->getAlarmSummary();
			m_text << (const char *)( boolTag.value ? "alarmed" : "no alarm" )
				<< " - " << getQuality(boolTag.quality) << "\r\n";

			m_text << "Alarm Ack Summary:\t";
			enumTag = dataNode->getAlarmAckStateSummary();
			switch (enumTag.value)
			{
			case TA_Base_Bus::ScadaAlarmNack:
				m_text << "not acknowledged";
				break;

			case TA_Base_Bus::ScadaAlarmAck:
				m_text << "acknowledged";
				break;

			case TA_Base_Bus::ScadaAlarmNotPresent:
				m_text << "not present";
				break;

			case TA_Base_Bus::ScadaAlarmUnknown:
				m_text << "unknown";
				break;

			default:
				m_text << "undefined";
			}
			m_text << " (" << enumTag.value << ") - " << getQuality(enumTag.quality) << "\r\n";

			m_text << "Scan Inhibit Summary:\t";
			boolTag = dataNode->getScanInhibitSummary();
			m_text << ( boolTag.value ? "inhibit" : "not inhibited" )
				<< " - " << getQuality(boolTag.quality) << "\r\n";

			m_text << "Alarm Inhibit Summary:\t";
			boolTag = dataNode->getAlarmInhibitSummary();
			m_text << ( boolTag.value ? "inhibit" : "not inhibited" )
				<< " - " << getQuality(boolTag.quality) << "\r\n";

			m_text << "MMS Alarm Inhibit Summary:\t";
			boolTag = dataNode->getMmsAlarmInhibitSummary();
			m_text << ( boolTag.value ? "inhibit" : "not inhibited" )
				<< " - " << getQuality(boolTag.quality) << "\r\n";

			m_text << "Control Inhibit Summary:\t";
			boolTag = dataNode->getControlInhibitSummary();
			m_text << ( boolTag.value ? "inhibit" : "not inhibited" )
				<< " - " << getQuality(boolTag.quality) << "\r\n";

			m_text << "Override Summary:\t\t";
			boolTag = dataNode->getOverrideSummary();
			m_text << ( boolTag.value ? "override" : "not overridden" )
				<< " - " << getQuality(boolTag.quality) << "\r\n";

			m_text << "Control Pending Summary:\t";
			boolTag = dataNode->getControlPendingSummary();
			m_text << ( boolTag.value ? "pending" : "not pending" )
				<< " - " << getQuality(boolTag.quality) << "\r\n";

			if ( true == dataNode->isConfigValid())
			{
				try
				{
					std::vector<boost::shared_ptr<TA_Base_Core::IEntityData> > childDataPoints = dataNode->getChildDataPointEntities();

					if (childDataPoints.size() > 0)
					{
						m_text << "\r\n" << "Child DataPoints:" << "\r\n";

						std::vector<boost::shared_ptr<TA_Base_Core::IEntityData> >::iterator itDp;

						for (itDp = childDataPoints.begin(); itDp != childDataPoints.end(); itDp++)
						{
							m_text << (*itDp)->getAgentName() << ":" << (*itDp)->getName() << "\r\n";
						}
					}
				}
				catch (...)
				{
					m_text << "Child DataPoints: *** not available ***" << "\r\n";
				}

				try
				{
					std::vector<boost::shared_ptr<TA_Base_Core::IEntityData> > childDataNodes = dataNode->getChildDataNodeEntities();

					if (childDataNodes.size() > 0)
					{
						m_text << "\r\n" << "Child DataNodes:" << "\r\n";

						std::vector<boost::shared_ptr<TA_Base_Core::IEntityData> >::iterator itDn;

						for (itDn = childDataNodes.begin(); itDn != childDataNodes.end(); itDn++)
						{
							m_text << (*itDn)->getAgentName() << ":" << (*itDn)->getName() << "\r\n";
						}
					}
				}
				catch (...)
				{
					m_text << "Child DataNodes: *** not available ***" << "\r\n";
				}

				m_text << "\r\n" << "Diagnostic test information for DataPoint Status" << "\r\n";

				std::vector<TA_Base_Bus::DataPointProxySmartPtr*>& dataPoints = m_pEquipment->getChildDataPointVector();
				std::vector<TA_Base_Bus::DataPointProxySmartPtr*>::iterator dpIt;

				for (dpIt = dataPoints.begin(); dpIt != dataPoints.end(); dpIt++)
				{
					TA_Base_Bus::DataPointProxySmartPtr* pDataPointPtr = *dpIt;
					m_text << "DataPoint CorbaName:\t" << "\r\n";
					m_text << (*pDataPointPtr)->getAgentName() << ":" << (*pDataPointPtr)->getEntityName() << "\r\n";

					m_text << "Proxy Config Valid:\t\t" << ((*pDataPointPtr)->isConfigValid()?"Yes":"No") << "\r\n";

					if ((*pDataPointPtr)->isConfigValid())
					{
						m_text << "Description:\t\t" << (*pDataPointPtr)->getDescription() << "\r\n";

						m_text << "DataPoint Type\t\t" << (*pDataPointPtr)->getShortNameOfType()
							<< " (" << (*pDataPointPtr)->getTypeAsString() << " - ";

						if ((*pDataPointPtr)->isWriteable())
						{
							m_text << "Output";
						}
						else
						{
							m_text << "Input";
						}
						m_text << ")" << "\r\n";

						m_text << "Display Order:\t\t" << (*pDataPointPtr)->getDisplayOrder() << "\r\n";

						if ((*pDataPointPtr)->isWriteable())
						{
							m_text << "Last Control:\t\t";
						}
						else
						{
							m_text << "Value:\t\t\t";
						}

						try
						{
							m_text << (*pDataPointPtr)->getValueAsString();
							if ((*pDataPointPtr)->isNumber())
							{
								m_text << " (";
								try
								{
									m_text << (*pDataPointPtr)->getValueAsDouble();
								}
								catch (TA_Base_Core::ScadaProxyException&)
								{
									m_text << "**";
								}
								m_text << ")";
							}
							else if ((*pDataPointPtr)->isBoolean())
							{
								m_text << " (";
								try
								{
									bool value = (*pDataPointPtr)->getValueAsBool();
									if (value)
									{
										m_text << "True";
									}
									else
									{
										m_text << "False";
									}
								}
								catch (TA_Base_Core::ScadaProxyException&)
								{
									m_text << "**";
								}
								m_text << ")";
							}
						}
						catch (TA_Base_Core::ScadaProxyException&)
						{
							m_text << "*** not available ***";
						}
						m_text << "\r\n";

						if (!(*pDataPointPtr)->isWriteable())
						{
							m_text << "Timestamp:\t\t";

							try
							{
								const unsigned int bufLen = 27;
								char buf[bufLen];

								time_t timestamp = (*pDataPointPtr)->getTimestamp();
								struct tm newtime;
								ACE_OS::localtime_r( &timestamp, &newtime ); 
								m_text << ACE_OS::asctime_r( &newtime, buf, bufLen );
							}
							catch (TA_Base_Core::ScadaProxyException&)
							{
								m_text << "*** not available ***";
							}
						}
						m_text << "\r\n";

						m_text << "Data Type:\t\t";
						if ((*pDataPointPtr)->isBoolean())
						{
							m_text << "Boolean";
						}
						else if ((*pDataPointPtr)->isDerived())
						{
							m_text << "Derived";
						}
						else if ((*pDataPointPtr)->isNumber())
						{
							m_text << "Numeric";
						}
						else if ((*pDataPointPtr)->isText())
						{
							m_text << "m_text";
						}
						else
						{
							m_text << "Unknown";
						}
						m_text << "\r\n";

						if (!(*pDataPointPtr)->isWriteable())
						{
							m_text << "Quality:\t\t\t" << (*pDataPointPtr)->getQualityAsString()
								<< " (" << (*pDataPointPtr)->getStatus() << ")";
						}
						else
						{
							try
							{
								m_text << "Control State:\t\t";
								TA_Base_Bus::EDataPointControlState controlState = (*pDataPointPtr)->getControlState();

								switch (controlState)
								{
								case TA_Base_Bus::noControl:
									m_text << "noControl";
									break;

								case TA_Base_Bus::written:
									m_text << "written";
									break;

								case TA_Base_Bus::pending:
									m_text << "pending";
									break;

								case TA_Base_Bus::conditionalSuccess:
									m_text << "conditionalSuccess";
									break;

								case TA_Base_Bus::success:
									m_text << "success";
									break;

								case TA_Base_Bus::failed:
									m_text << "failed";
									break;

								case TA_Base_Bus::LCC_failed:
									m_text << "LCC_failed";
									break;

								case TA_Base_Bus::RCC_failed:
									m_text << "RCC_failed";
									break;

								case TA_Base_Bus::comms_failed:
									m_text << "comms_failed";
									break;
								}
							}
							catch (TA_Base_Core::ScadaProxyException&)
							{
								m_text << "*** not available ***";
							}
						}
						m_text << "\r\n";
					}
					m_text << "\r\n";
				}
			}
		}
		catch (...)
		{
		}*/
	}


	void	StatusDialogModel::updateDataPoint(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType)
	{
	//	this->updateDataNode(updateType);
	}

	const char * const StatusDialogModel::getQuality( TA_Base_Bus::EQualityStatus quality ) const
	{
		switch (quality)
		{
		case TA_Base_Bus::QUALITY_BAD_CONFIGURATION_ERROR:
			return TA_Base_Bus::QUALITY_BAD_CONFIGURATION_ERROR_STRING;

		case TA_Base_Bus::QUALITY_BAD_NOT_CONNECTED:
			return TA_Base_Bus::QUALITY_BAD_NOT_CONNECTED_STRING;

		case TA_Base_Bus::QUALITY_BAD_DEVICE_FAILURE:
			return TA_Base_Bus::QUALITY_BAD_DEVICE_FAILURE_STRING;

		case TA_Base_Bus::QUALITY_BAD_SENSOR_FAILURE:
			return TA_Base_Bus::QUALITY_BAD_SENSOR_FAILURE_STRING;

		case TA_Base_Bus::QUALITY_BAD_LAST_KNOWN_VALUE:
			return TA_Base_Bus::QUALITY_BAD_LAST_KNOWN_VALUE_STRING;

		case TA_Base_Bus::QUALITY_BAD_COMM_FAILURE:
			return TA_Base_Bus::QUALITY_BAD_COMM_FAILURE_STRING;

		case TA_Base_Bus::QUALITY_BAD_OUT_OF_SERVICE:
			return TA_Base_Bus::QUALITY_BAD_OUT_OF_SERVICE_STRING;

		case TA_Base_Bus::QUALITY_UNCERTAINTY_NO_SPECIFIC_REASON:
			return TA_Base_Bus::QUALITY_UNCERTAINTY_NO_SPECIFIC_REASON_STRING;

		case TA_Base_Bus::QUALITY_UNCERTAINTY_LAST_USABLE_VALUE:
			return TA_Base_Bus::QUALITY_UNCERTAINTY_LAST_USABLE_VALUE_STRING;

		case TA_Base_Bus::QUALITY_UNCERTAINTY_SENSOR_NOT_ACCURATE:
			return TA_Base_Bus::QUALITY_UNCERTAINTY_SENSOR_NOT_ACCURATE_STRING;
			break;

		case TA_Base_Bus::QUALITY_UNCERTAINTY_ENG_UNIT_EXCEEDED:
			return TA_Base_Bus::QUALITY_UNCERTAINTY_ENG_UNIT_EXCEEDED_STRING;

		case TA_Base_Bus::QUALITY_UNCERTAINTY_SUB_NORMAL:
			return TA_Base_Bus::QUALITY_UNCERTAINTY_SUB_NORMAL_STRING;

		case TA_Base_Bus::QUALITY_GOOD_NO_SPECIFIC_REASON:
			return TA_Base_Bus::QUALITY_GOOD_NO_SPECIFIC_REASON_STRING;

		case TA_Base_Bus::QUALITY_GOOD_LOCAL_OVERRIDE:
			return TA_Base_Bus::QUALITY_GOOD_LOCAL_OVERRIDE_STRING;

		case TA_Base_Bus::QUALITY_BAD_NO_SPECIFIC_REASON:
		default:
			return TA_Base_Bus::QUALITY_BAD_NO_SPECIFIC_REASON_STRING;
		}
	}
};