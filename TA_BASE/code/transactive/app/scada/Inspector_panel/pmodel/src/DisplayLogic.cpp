#include "DisplayLogic.h"
#include "core\exceptions\src\ScadaProxyException.h"
#include "core\exceptions\src\TransactiveException.h"
#include "bus\scada\proxy_library\src\IDataNodeProxy.h"
#include "bus\scada\common_library\src\DpValue.h"
#include <atltime.h>
#include "core\exceptions\src\ObjectResolutionException.h"
#include "CommonExceptionsCorbaDef.h"
#include "core\corba\src\CorbaUtil.h"
#include "core\utilities\src\RunParams.h"
#include "DataPointControlHelper.h"
#include "OutputLogicException.h"


namespace TA_Base_App
{
	InhibitItem::InhibitItem()
		:isWritable(false),
		isCtrlInhibit(false),
		isScanInhibit(false),
		isAlarmInhibit(false),
		isOverrideInhibit(false),
		isMmsInhibit(false)
	{
	}


	OverrideItem::OverrideItem()
		:isOverride(false)
	{
	}



	DisplayLogic::DisplayLogic(CEquipment*	pEquipment)
		:m_pEquipment(pEquipment)
	{

	}

	DisplayLogic::~DisplayLogic()
	{

	}


	InformationItem		DisplayLogic::getInfomationDisplayItem(unsigned int iPoint)
	{
		InformationItem rtnItem;
		try
		{
			TA_Base_Bus::DataPointProxySmartPtr& childDataPoint = m_pEquipment->getChildDataPoint(iPoint);
			
			rtnItem.iPoint			= iPoint;
			rtnItem.description		= childDataPoint->getDescription();
			rtnItem.shortName		= childDataPoint->getShortNameOfType();

			rtnItem.valueString		= this->getCurrentValueAsString(childDataPoint);

			rtnItem.qualityString	= childDataPoint->getQualityAsString();
  
			time_t timeStamp = 0L;
			timeStamp = (time_t) childDataPoint->getTimestamp();
			//todo->//CTime now(timeStamp);
			//CString s = now.Format("%H:%M:%S");						

			//rtnItem.timestamp = s.GetBuffer(s.GetLength());
		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", e.what());
		}
		catch (TA_Base_Core::TransactiveException &e)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "Unexpected Transactive Exception - %s", e.what());
		}
		catch (...)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "Unexpected Exception");
		}
		return rtnItem;
	}

	std::vector<InformationItem>	DisplayLogic::getAllInfomationDisplayItem()
	{
		
		std::vector<unsigned long> tmpDataPoints = m_pEquipment->getNotWriteableIndex();
		std::vector<InformationItem> rtnInformationItems;
		rtnInformationItems.reserve(tmpDataPoints.size());

		for(int index=0; index < tmpDataPoints.size(); ++index)
		{
			InformationItem tmpItem = this->getInfomationDisplayItem(tmpDataPoints[index]);
			rtnInformationItems.push_back(tmpItem);
		}
		return rtnInformationItems;
	}


	ControlItem		DisplayLogic::getControlDisplayItem(unsigned int iPoint)
	{
		ControlItem rtnItem;
		try
		{
			TA_Base_Bus::DataPointProxySmartPtr& childDataPoint = m_pEquipment->getChildDataPoint(iPoint);
			rtnItem.iPoint			= iPoint;
			rtnItem.description		= childDataPoint->getDescription();
			rtnItem.type			= childDataPoint->getShortNameOfType();
			rtnItem.state			= this->getControlStatusString(childDataPoint->getControlState() );
			if(rtnItem.state.size() > 0)
			{
				rtnItem.newValue		= childDataPoint->getValueAsString();
			}
			rtnItem.units			= childDataPoint->getUnit();
		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", e.what());
		}
		catch (TA_Base_Core::TransactiveException &e)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "Unexpected Transactive Exception - %s", e.what());
		}
		catch (...)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "Unexpected Exception");
		}
		return rtnItem;
	}


	std::vector<ControlItem>	DisplayLogic::getAllControlDisplayItem()
	{
		std::vector<unsigned long>& tmpDataPoints = m_pEquipment->getWriteableIndex();
		std::vector<ControlItem> rtnControlItems;
		rtnControlItems.reserve(tmpDataPoints.size());

		for(int index=0; index < tmpDataPoints.size(); ++index)
		{
			ControlItem tmpItem = this->getControlDisplayItem(tmpDataPoints[index]);
			rtnControlItems.push_back(tmpItem);
		}

		return rtnControlItems;
	}


	InhibitItem		DisplayLogic::getInhibitDisplayItem(unsigned int iPoint)
	{
		InhibitItem rtnItem;
		try
		{
			DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);
			rtnItem.iPoint		= iPoint;
			rtnItem.description = dataPoint->getDescription();
			rtnItem.type		= dataPoint->getShortNameOfType();
			rtnItem.isWritable	= dataPoint->isWriteable();
			if(rtnItem.isWritable == false)
			{
				rtnItem.isScanInhibit	= dataPoint->isScanInhibited();
				rtnItem.isAlarmInhibit	= dataPoint->isAlarmInhibited();
				rtnItem.isMmsInhibit	= dataPoint->isMmsAlarmInhibited();

				TA_Base_Bus::BooleanTag dnCombineMMS = m_pEquipment->getDataNode()->getCombinedInhibitMms(); 
				if (dnCombineMMS.quality == TA_Base_Bus::QUALITY_GOOD_NO_SPECIFIC_REASON)
				{
					rtnItem.isMmsInhibit |= dnCombineMMS.value;
				}

				rtnItem.valueString		= this->getCurrentValueAsString(dataPoint);
			}
			else
			{
				rtnItem.isCtrlInhibit	= dataPoint->isControlInhibited();
			}
		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", e.what());
		}
		return rtnItem;
	}


	std::vector<InhibitItem>	DisplayLogic::getAllInhibitDisplayItem()
	{
		std::vector<unsigned long>& inputDataPoints = m_pEquipment->getNotWriteableIndex();
		std::vector<unsigned long>& outputDataPoints = m_pEquipment->getWriteableIndex();
		std::vector<InhibitItem>  rtnInhibitItems;
		rtnInhibitItems.reserve(inputDataPoints.size() + outputDataPoints.size());

		for(int index =0;index < inputDataPoints.size(); ++index)
		{
			InhibitItem tmpItem = this->getInhibitDisplayItem(inputDataPoints[index]);
			rtnInhibitItems.push_back(tmpItem);
		}

		for(int index =0;index < outputDataPoints.size(); ++index)
		{
			InhibitItem tmpItem = this->getInhibitDisplayItem(outputDataPoints[index]);
			rtnInhibitItems.push_back(tmpItem);
		}

		return rtnInhibitItems;
	}


	std::vector<LimitDetailItem>		DisplayLogic::getAllLimitDetailDisplayItem(unsigned int iPoint)
	{
		std::vector<LimitDetailItem> rtnItemVector;
		try
		{
			DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);
			TA_Base_Bus::AlarmPropertyVect alarmProperties = dataPoint->getAlarmProperties();
			rtnItemVector.reserve(alarmProperties.size());
			for (unsigned int index = 0; index < alarmProperties.size(); index++)
			{
				LimitDetailItem newItem;

				newItem.thresholdValue = 0.0;
				switch(dataPoint->getValue().getType())
				{
				case TA_Base_Bus::DPT_INT16_TYPE:
				case TA_Base_Bus::DPT_INT32_TYPE:
				case TA_Base_Bus::DPT_INT16S_TYPE:
				case TA_Base_Bus::DPT_INT32S_TYPE:
				case TA_Base_Bus::DPT_UINT16_TYPE:
				case TA_Base_Bus::DPT_UINT32_TYPE:
				case TA_Base_Bus::DPT_IEEE32_TYPE:
					newItem.thresholdValue = alarmProperties[index].threshold;
					break;

				case TA_Base_Bus::DPT_ENUM_TYPE:
					newItem.thresholdValue = alarmProperties[index].enumValue;
					break;

				default:
					break;
				}

				newItem.typeString	= alarmTypeToString(alarmProperties[index].type);
				newItem.isEnable	= alarmProperties[index].enabled;
				newItem.currentValueString = getCurrentValueAsString(dataPoint);
				fillThresholdString(newItem,dataPoint);	
				rtnItemVector.push_back(newItem);
			}
		}
		catch (TA_Base_Core::ScadaProxyException&ex)
		{

		}
		return rtnItemVector;
	}


	MeterItem	DisplayLogic::getMeterItemDisplayItem(unsigned int iPoint)
	{
		MeterItem rtnItem;
		try
		{
			DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);
			rtnItem.iPoint				= iPoint;
			rtnItem.description			= dataPoint->getDescription();
			rtnItem.type				= dataPoint->getShortNameOfType();
			rtnItem.currentValue		= getCurrentValueAsString(dataPoint);
		}
		catch (TA_Base_Core::ScadaProxyException&ex)
		{

		}
		return rtnItem;
	}


	std::vector<MeterItem>		DisplayLogic::getAllMeterDisplayItem()
	{
		std::vector<MeterItem>	rtnMeterItems;
		std::vector<unsigned long>& datapoints = m_pEquipment->getMeterIndex();
		
		for(int index = 0; index < datapoints.size(); ++index)
		{
			MeterItem tmpItem = this->getMeterItemDisplayItem(datapoints[index]);
			rtnMeterItems.push_back(tmpItem);
		}

		return rtnMeterItems;
	}


	OverrideItem	DisplayLogic::getOverrideDisplayItem(unsigned int iPoint)
	{
		OverrideItem rtnItem;
		try
		{
			DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);
			rtnItem.iPoint		= iPoint;
			rtnItem.description = dataPoint->getDescription();
			rtnItem.type		= dataPoint->getShortNameOfType();
			rtnItem.overrideValue = "";
			rtnItem.currentValue  = getCurrentValueAsString(dataPoint);
			rtnItem.isOverride	= dataPoint->isOverrideEnabled();

			if(rtnItem.isOverride)
			{
				rtnItem.overrideValue = getCurrentValueAsString(dataPoint);

				rtnItem.currentValue = dataPoint->getFieldValue();
				rtnItem.currentValue += " ";
				rtnItem.currentValue += dataPoint->getUnit();
			}
		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", e.what());
		}
		return rtnItem;
	}


	std::vector<OverrideItem>	DisplayLogic::getAllOverrideDisplayItem()
	{
		std::vector<unsigned long>& datapoints = m_pEquipment->getNotWriteableIndex();
		std::vector<OverrideItem>   rtnOverrideItems;
		rtnOverrideItems.reserve(datapoints.size());
		for(int index =0; index < datapoints.size(); ++index)
		{
			OverrideItem tmpItem = this->getOverrideDisplayItem(datapoints[index]);
			rtnOverrideItems.push_back(tmpItem);
		}

		return rtnOverrideItems;
	}

	bool	DisplayLogic::isActionPermitted(unsigned int iPoint, AccessControlActions action)
	{
		unsigned long entitykey = m_pEquipment->getChildDataPoint(iPoint)->getEntityKey();
		return AccessControlHelper::getInstance()->IsActionPermitted(entitykey,action);
	}


	int	DisplayLogic::constructControlStateString(std::vector<std::string>& parameters)
	{
		std::string rtnString;
		std::vector<TA_Base_Bus::DataPointProxySmartPtr*>& dataPoints = m_pEquipment->getChildDataPointVector();
		std::vector<unsigned long>& vectorIndex = m_pEquipment->getWriteableIndex(); 

		// Only proceed if the control point all config valid
		if ( dataPoints.size() > 0 && vectorIndex.size() > 0 )
		{
			// Create a triggering entity to entity map
			// Find the latest control status for the equipment as well as each control point
			int   maxIndex = -1;
			time_t timeStamp = 0L;
			CTime maxTimestamp(timeStamp);
			CTime curTimestamp(timeStamp);
			for(int i=0; i<vectorIndex.size(); i++)
			{
				int iPoint = vectorIndex.at(i);
				int latest = -1;

				try
				{
					timeStamp = (time_t) (*dataPoints[iPoint])->getLastStatusChangeTimestamp();
					curTimestamp = CTime(timeStamp);

					if( maxTimestamp < curTimestamp)
					{
						maxTimestamp = curTimestamp;
						maxIndex = iPoint;
					}
				}
				catch (TA_Base_Core::ScadaProxyException &e)
				{
					//timestamp is not ready
					LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", e.what());
				}
				catch ( const TA_Base_Core::ObjectResolutionException & ex )
				{
					LOG_EXCEPTION_CATCH(SourceInfo, "TA_Base_Core::ObjectResolutionException" ,ex.what() );
				}
				catch ( const TA_Base_Core::OperationModeException & )
				{
					LOG_EXCEPTION_CATCH(SourceInfo, "TA_Base_Core::OperationModeException", "Caught Operation Mode exception" );
				}
				catch ( const CORBA::Exception & ex )
				{
					LOG_EXCEPTION_CATCH ( SourceInfo, "CORBA::Exception", ( TA_Base_Core::CorbaUtil::getInstance().exceptionToString(ex) ).c_str() );
				}
				catch (...)
				{
					LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", "Unknown Exception" );
				}
			}

			try
			{
				// Show the latest control message in the status window
				if( maxIndex!=-1 && (*dataPoints[maxIndex])->isWriteable() )
				{
					//rtnString = addControlStateToStatusLine ( (*dataPoints[maxIndex]), (*dataPoints[maxIndex])->getControlState() );
					int statusCode = addControlStateToStatusLine ( (*dataPoints[maxIndex]), (*dataPoints[maxIndex])->getControlState(), parameters );
					return statusCode;
				}
			}
			catch (TA_Base_Core::ScadaProxyException &e)
			{
				// Not always available
				LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", e.what());
			}
			catch ( const TA_Base_Core::ObjectResolutionException & ex )
			{
				LOG_EXCEPTION_CATCH(SourceInfo, "TA_Base_Core::ObjectResolutionException" ,ex.what() );
			}
			catch ( const TA_Base_Core::OperationModeException & )
			{
				LOG_EXCEPTION_CATCH(SourceInfo, "TA_Base_Core::OperationModeException", "Caught Operation Mode exception" );
			}
			catch ( const CORBA::Exception & ex )
			{
				LOG_EXCEPTION_CATCH ( SourceInfo, "CORBA::Exception", ( TA_Base_Core::CorbaUtil::getInstance().exceptionToString(ex) ).c_str() );
			}
			catch (...)
			{
				LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", "Unknown Exception" );
			}
		}
		return 0;
	}

	int DisplayLogic::addControlStateToStatusLine ( TA_Base_Bus::DataPointProxySmartPtr & datapoint, TA_Base_Bus::EDataPointControlState state, std::vector<std::string>&parameters ) //wenching++ (TD14179)
	{
		FUNCTION_ENTRY ("addControlStateToStatusLine()");

		bool addStatusLine = false;
		std::string returnString = "";
		std::stringstream statusLine;
		std::string value = "";
		int rtnMsgCode = 0;
		parameters.clear();

		try
		{
			// only proceed if the data point is an output data point
			if ( true == datapoint->isWriteable() )
			{
				//TD15649.
				statusLine << datapoint->getDescription() << ": " << datapoint->getValueAsString() << " - ";

				switch ( state ) 
				{
				case written:
					{
						//TD15649
						statusLine << "Sent.";
						addStatusLine = true;
						returnString = DPT_CONTROL_WRITTEN;
						rtnMsgCode = CONTROL_STATUS_SENT_0;
						break;
					}

				case pending:
					{
						statusLine << "Pending.";
						addStatusLine = true;
						returnString = DPT_CONTROL_PENDING;
						rtnMsgCode = CONTROL_STATUS_PENDING_0;
						break;
					}

				case conditionalSuccess:
					{
						statusLine << "Successful.";
						addStatusLine = true;
						returnString = DPT_CONTROL_CONDITIONALSUCCESS;
						rtnMsgCode = CONTROL_STATUS_SUCCESSFUL_0;
						break;
					}

				case success:
					{
						statusLine << "Successful.";
						addStatusLine = true;
						returnString = DPT_CONTROL_SUCCESS;
						rtnMsgCode = CONTROL_STATUS_SUCCESSFUL_0;
						break;
					}

				case failed:
					{
						statusLine << "Failed.";
						addStatusLine = true;
						returnString = DPT_CONTROL_FAILED;	
						rtnMsgCode = CONTROL_STATUS_FAIL_0;
						break;
					}

				case LCC_failed:
					{
						statusLine.str("");	// clear;
						statusLine << m_pEquipment->getDataNode()->getAssetName() << " ";
						statusLine << datapoint->getDescription() << ": " << datapoint->getValueAsString() << " - ";
						statusLine << "Failed due to ";
						statusLine << datapoint->getLaunchingConditionResultString();
						statusLine << ".";
						addStatusLine = true;
						returnString = DPT_CONTROL_LCC_FAILED;

						parameters.push_back(m_pEquipment->getDataNode()->getAssetName());
						parameters.push_back(datapoint->getDescription());
						parameters.push_back(datapoint->getValueAsString());
						parameters.push_back(datapoint->getLaunchingConditionResultString());
						rtnMsgCode = CONTROL_STATUS_LCC_FAIL_4;
						break;
					}

				case RCC_failed:
					{
						statusLine.str("");	// clear;
						statusLine << m_pEquipment->getDataNode()->getAssetName() << " ";
						statusLine << datapoint->getDescription() << ": " << datapoint->getValueAsString() << " - ";
						//CL19432++ - The AssetName of the related equipment must be added in the status message
						statusLine << "Failed due to Bad Return Condition.";
						//statusLine << "Sending control to " << datapoint->getEntityName();
						//statusLine << " failed due to Bad Return Condition.";
						//++CL19432
						addStatusLine = true;
						returnString = DPT_CONTROL_RCC_FAILED;

						parameters.push_back(m_pEquipment->getDataNode()->getAssetName());
						parameters.push_back(datapoint->getDescription());
						parameters.push_back(datapoint->getValueAsString());
						rtnMsgCode = CONTROL_STATUS_RCC_FAIL_3;
						break;
					}

				case comms_failed:
					{
						statusLine << "Failed due to Comms Error.";
						addStatusLine = true;
						returnString = DPT_CONTROL_COMMS_FAILED;

						rtnMsgCode = CONTROL_STATUS_COMMS_FAIL_0;
						break;
					}

				case noControl:
				default:
					{
						returnString = DPT_CONTROL_NOCONTROL;
						break;
					}
				}

				if ( true == addStatusLine )
				{
					LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", statusLine.str().c_str() );
					//this->postStatusLine(statusLine.str());
				}		
			}
		}
		catch (TA_Base_Core::ScadaProxyException& e)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", e.what());
		}
		catch ( const TA_Base_Core::ObjectResolutionException & ex )
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "TA_Base_Core::ObjectResolutionException" ,ex.what() );
		}
		catch ( const TA_Base_Core::OperationModeException & )
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "TA_Base_Core::OperationModeException", "Caught Operation Mode exception" );
		}
		catch ( const CORBA::Exception & ex )
		{
			LOG_EXCEPTION_CATCH ( SourceInfo, "CORBA::Exception", ( TA_Base_Core::CorbaUtil::getInstance().exceptionToString(ex) ).c_str() );
		}
		catch ( ... )
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, 
				"Unknown exception caught while adding the control status to status box");
		}

		FUNCTION_EXIT;
		//return returnString;
		//return statusLine.str();
		return rtnMsgCode;
	}


	std::map<unsigned int, bool >  DisplayLogic::getAllowedToWriteMap()
	{
		std::map<unsigned int, bool > writableMap;
		std::vector<unsigned long>& vectorIndex = m_pEquipment->getWriteableIndex();

		for(unsigned int index = 0;index < vectorIndex.size(); ++index)
		{
			bool isDPWriteAllowed = getDpAllowedToWrite( vectorIndex[index]);

			writableMap.insert( std::map<unsigned int, bool>::value_type(vectorIndex[index], isDPWriteAllowed) );
		}

		return writableMap;
	}

	bool DisplayLogic::getDpAllowedToWrite(unsigned int iPoint)
	{
		std::vector<TA_Base_Bus::DataPointProxySmartPtr*>& dataPoints = m_pEquipment->getChildDataPointVector();
		bool isDPWriteAllowed = isDPWriteAllowed = isActionPermitted(iPoint,DATA_POINT_WRITE);

		bool isControlInhibited = false;

		try
		{
			isControlInhibited = (*dataPoints[iPoint ])->isControlInhibited();
		}
		catch (...)
		{
			isControlInhibited = false;
		}
		return  isControlInhibited?false:isDPWriteAllowed;
	}

	bool	DisplayLogic::isWritableDp(unsigned int iPoint)
	{
		std::vector<unsigned long>& vectorIndex = m_pEquipment->getWriteableIndex(); 
		bool isValid = false;

		for(unsigned int index =0; index < vectorIndex.size(); ++index)
		{
			if(vectorIndex.at(index) == iPoint)
			{
				isValid = true;
				break;
			}
		}
		return isValid;
	}


	bool	DisplayLogic::isNotWritableDp(unsigned int iPoint)
	{
		std::vector<unsigned long>& vectorIndex = m_pEquipment->getNotWriteableIndex(); 
		bool isValid = false;

		for(unsigned int index =0; index < vectorIndex.size(); ++index)
		{
			if(vectorIndex.at(index) == iPoint)
			{
				isValid = true;
				break;
			}
		}
		return isValid;
	}


	bool	DisplayLogic::isControlStateInPending(unsigned int iPoint)
	{
		bool rtn = false;
		try
		{
			rtn = pending == m_pEquipment->getChildDataPoint(iPoint)->getControlState();
		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", e.what());
		}
		catch(...)
		{

		}
		return false;
	}

	bool	DisplayLogic::isBoolean(unsigned iPoint)
	{
		return m_pEquipment->getChildDataPoint(iPoint)->isBoolean();
	}

	bool	DisplayLogic::isDerived(unsigned iPoint)
	{
		return m_pEquipment->getChildDataPoint(iPoint)->isDerived();
	}

	bool	DisplayLogic::isNumber(unsigned iPoint)
	{
		return m_pEquipment->getChildDataPoint(iPoint)->isNumber();
	}

	bool	DisplayLogic::isText(unsigned iPoint)
	{
		return m_pEquipment->getChildDataPoint(iPoint)->isText();
	}

	std::vector<std::string> DisplayLogic::getAllValues(unsigned int iPoint)
	{
		std::vector<std::string> rtnValues;
		DataPointProxySmartPtr& datapoint = m_pEquipment->getChildDataPoint(iPoint);
		if (datapoint->isBoolean())
		{
			std::string label = "";
			try
			{
				label = datapoint->getBooleanLabel(true);
				rtnValues.push_back(label);
		
				label = datapoint->getBooleanLabel(false);
				rtnValues.push_back(label);
			}
			catch (TA_Base_Core::ScadaProxyException &e)
			{
				LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", e.what());
			}
		}
		else if (datapoint->isDerived())
		{
			try
			{
				DpvEnumLabelsMap derivedStates = datapoint->getDerivedStatesAsMap();

				DpvEnumLabelsMap::iterator it;

				for (it = derivedStates.begin(); it != derivedStates.end(); it++)
				{
					rtnValues.push_back((*it).second);
				}
			}
			catch (TA_Base_Core::ScadaProxyException &e)
			{
				LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", e.what());
			}
		}

		return rtnValues;
	}


	bool	DisplayLogic::isOkToOverride(unsigned int iPoint)
	{
		bool rtn = false;
		try
		{
			DataPointProxySmartPtr&dataPoint = m_pEquipment->getChildDataPoint(iPoint);
			bool	isPermitted	= isActionPermitted(iPoint,DATA_POINT_SET_FORCED_STATE);
			bool	isOverride				= dataPoint->isOverrideEnabled();
			rtn = isPermitted && (isOverride == false);

			/*if(!isPermitted)
			{
				std::stringstream statusLine;
				statusLine << dataPoint->getDescription() << ": "<<"No Access";
				postStatusLine(statusLine.str());
			}*///todo->

		}
		catch(TA_Base_Core::ScadaProxyException&ex)
		{

		}
		catch(TA_Base_Core::AccessDeniedException&ex)
		{

		}
		return rtn;

	}

	bool	DisplayLogic::isOkToRemoveOverride(unsigned int iPoint)
	{
		bool rtn = false;
		try
		{
			DataPointProxySmartPtr&dataPoint =m_pEquipment->getChildDataPoint(iPoint);
			bool	isPermitted	= isActionPermitted(iPoint,DATA_POINT_RESET_FORCED_STATE);
			bool	isOverride				= dataPoint->isOverrideEnabled();
			rtn = isPermitted && (isOverride == true);

			/*if(!isPermitted)
			{
				std::stringstream statusLine;
				statusLine << dataPoint->getDescription() << ": "<<"No Access";
				postStatusLine(statusLine.str());
			}*/ ///todo->

		}
		catch(TA_Base_Core::ScadaProxyException&ex)
		{

		}
		catch(TA_Base_Core::AccessDeniedException&ex)
		{

		}
		return rtn;
	}

	std::string		DisplayLogic::getDataPointDescription(unsigned int iPoint)
	{
		DataPointProxySmartPtr&dataPoint =m_pEquipment->getChildDataPoint(iPoint);
		return dataPoint->getDescription();
	}

	int    DisplayLogic::getDataPointDisplayOrder(unsigned int iPoint)
	{
		DataPointProxySmartPtr&dataPoint =m_pEquipment->getChildDataPoint(iPoint);
		return dataPoint->getDisplayOrder();
	}

	bool	DisplayLogic::isOkToSetScanInhibit(unsigned int iPoint, bool bRequestScanInhibit)
	{
		bool rtn = false;
		try
		{
			DataPointProxySmartPtr&dataPoint =m_pEquipment->getChildDataPoint(iPoint);
			bool prevInhibitState = dataPoint->isScanInhibited();
			rtn = prevInhibitState != bRequestScanInhibit;
		}
		catch (TA_Base_Core::ScadaProxyException& ex)
		{
			rtn = false;
		}
		return rtn;
	}

	bool	DisplayLogic::isOkToSetAlarmInhibit(unsigned int iPoint, bool bRequestAlarmInhibit)
	{
		bool rtn = false;
		try
		{
			DataPointProxySmartPtr&dataPoint =m_pEquipment->getChildDataPoint(iPoint);
			bool prevInhibitState = dataPoint->isAlarmInhibited();
			rtn = prevInhibitState != bRequestAlarmInhibit;
		}
		catch (TA_Base_Core::ScadaProxyException& ex)
		{
			rtn = false;
		}
		return rtn;
	}

	bool	DisplayLogic::isOkToSetControlInhibit(unsigned int iPoint, bool bRequestControlInhibit)
	{
		bool rtn = false;
		try
		{
			DataPointProxySmartPtr&dataPoint =m_pEquipment->getChildDataPoint(iPoint);
			bool prevInhibitState = dataPoint->isControlInhibited();
			rtn = prevInhibitState != bRequestControlInhibit;
		}
		catch (TA_Base_Core::ScadaProxyException& ex)
		{
			rtn = false;
		}
		return rtn;
	}

	bool	DisplayLogic::isOkToSetMmsInhibit(unsigned int iPoint, bool bRequestMmsInhibit)
	{
		bool rtn = false;
		try
		{
			DataPointProxySmartPtr&dataPoint =m_pEquipment->getChildDataPoint(iPoint);
			bool prevInhibitState = dataPoint->isMmsAlarmInhibited();
			rtn = prevInhibitState != bRequestMmsInhibit;
		}
		catch (TA_Base_Core::ScadaProxyException& ex)
		{
			rtn = false;
		}
		return rtn;
	}

	bool	DisplayLogic::isScanInhibitable(unsigned int iPoint)
	{
		try
		{
			DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);
			if ( true == m_pEquipment->getDataNode()->isTagged() && TA_Base_Bus::InhibitScan == m_pEquipment->getDataNode()->getTagInhibitState().inputInhibit)
			{
				return false;
			}

			bool isScanInhibitPermitted = TA_Base_App::AccessControlHelper::getInstance()->IsActionPermitted(dataPoint->getEntityKey(), DATA_POINT_SCAN_INHIBIT);
			bool isScanUnInhibitPermitted = TA_Base_App::AccessControlHelper::getInstance()->IsActionPermitted(dataPoint->getEntityKey(), DATA_POINT_SCAN_UNINHIBIT);

			if ( ( true == isScanInhibitPermitted ) && ( true == isScanUnInhibitPermitted ) )
			{
				return true;
			}
			else if ( ( false == isScanInhibitPermitted ) && ( false == isScanUnInhibitPermitted ) )
			{
				return false;
			}
			else
			{
				if ( ( true == isScanInhibitPermitted ) && 	( false == isScanUnInhibitPermitted ) )
				{
					if ( false == dataPoint->isScanInhibited() )
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else if ( ( false == isScanInhibitPermitted ) && ( true == isScanUnInhibitPermitted ) )
				{
					if ( true == dataPoint->isScanInhibited() )
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", e.what());
			return false;
		}
	}


	bool	DisplayLogic::isAlarmInhibitable(unsigned int iPoint)
	{
		try
		{
			DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);
			if ( true == m_pEquipment-> getDataNode()->isTagged() )
			{
				if (TA_Base_Bus::InhibitScan ==  m_pEquipment->getDataNode()->getTagInhibitState().inputInhibit ||
					TA_Base_Bus::InhibitAlarm ==  m_pEquipment->getDataNode()->getTagInhibitState().inputInhibit )
				{
					return false;
				}
			}

			if (dataPoint->isScanInhibited())
			{	
				return false;
			}

			bool isAlarmInhibitPermitted = TA_Base_App::AccessControlHelper::getInstance()->IsActionPermitted(dataPoint->getEntityKey(), DATA_POINT_ALARM_INHIBIT);
			bool isAlarmUnInhibitPermitted = TA_Base_App::AccessControlHelper::getInstance()->IsActionPermitted(dataPoint->getEntityKey(), DATA_POINT_ALARM_UNINHIBIT);
			
			if ( ( true == isAlarmInhibitPermitted ) && ( true == isAlarmUnInhibitPermitted ) )
			{
				return true;
			}
			else if ( ( false == isAlarmInhibitPermitted ) && ( false == isAlarmUnInhibitPermitted ) )
			{
				return false;
			}

			else
			{
				if ( ( true == isAlarmInhibitPermitted ) && ( false == isAlarmUnInhibitPermitted ) )
				{
					if ( false == dataPoint->isAlarmInhibited() )
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else if ( ( false == isAlarmInhibitPermitted ) && ( true == isAlarmUnInhibitPermitted ) )
				{
					if ( true == dataPoint->isAlarmInhibited() )
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", e.what());
			return false;
		}
	}


	bool	DisplayLogic::isMmsInhibitable(unsigned int iPoint)
	{
		try
		{
			DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);
			// cell should be disabled if the TagInhibitState is scaninhibit or alarm inhibit or mms inhibit
			if ( true == m_pEquipment->getDataNode()->isTagged() )
			{
				if (TA_Base_Bus::InhibitScan == m_pEquipment->getDataNode()->getTagInhibitState().inputInhibit ||
					TA_Base_Bus::InhibitAlarm == m_pEquipment->getDataNode()->getTagInhibitState().inputInhibit ||
					TA_Base_Bus::InhibitMmsAlarm == m_pEquipment->getDataNode()->getTagInhibitState().inputInhibit )
				{
					return false;
				}
			}

			if ( dataPoint->isScanInhibited() || dataPoint->isAlarmInhibited() )
			{
				return false;
			}

			bool isMMSInhibitPermitted = TA_Base_App::AccessControlHelper::getInstance()->IsActionPermitted( dataPoint->getEntityKey(),DATA_POINT_MMS_INHIBIT);
			bool isMMSUnInhibitPermitted = TA_Base_App::AccessControlHelper::getInstance()->IsActionPermitted( dataPoint->getEntityKey(),DATA_POINT_MMS_UNINHIBIT);

			if ( ( true == isMMSInhibitPermitted ) && ( true == isMMSUnInhibitPermitted ) )
			{
				return true;
			}

			else if ( ( false == isMMSInhibitPermitted ) && ( false == isMMSUnInhibitPermitted ) )
			{
				return false;
			}
			else
			{
				if ( ( true == isMMSInhibitPermitted ) && ( false == isMMSUnInhibitPermitted ) )
				{
					if ( false == dataPoint->isMmsAlarmInhibited() )
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else if ( ( false == isMMSInhibitPermitted ) && ( true == isMMSUnInhibitPermitted ) )
				{
					if ( true == dataPoint->isMmsAlarmInhibited() )
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", e.what());
			return false;
		}
	}

	bool	DisplayLogic::isControlInhibitable(unsigned int iPoint)
	{
		try
		{
			DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);
			if ( true == m_pEquipment->getDataNode()->isTagged() &&
				TA_Base_Bus::InhibitControl == m_pEquipment->getDataNode()->getTagInhibitState().outputInhibit )
			{
				return false;
			}

			bool isControlInhibitPermitted = TA_Base_App::AccessControlHelper::getInstance()->IsActionPermitted(dataPoint->getEntityKey(),DATA_POINT_CONTROL_INHIBIT);
			bool isControlUnInhibitPermitted = TA_Base_App::AccessControlHelper::getInstance()->IsActionPermitted(dataPoint->getEntityKey(),DATA_POINT_CONTROL_UNINHIBIT);

			if ( ( true == isControlInhibitPermitted ) && ( true == isControlUnInhibitPermitted ) )
			{
				return true;
			}
			else if ( ( false == isControlInhibitPermitted ) &&	( false == isControlUnInhibitPermitted ) )
			{
				return false;
			}

			else
			{
				if ( ( true == isControlInhibitPermitted ) && ( false == isControlUnInhibitPermitted ) )
				{
					if ( false == dataPoint->isControlInhibited() )
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else if ( ( false == isControlInhibitPermitted ) && ( true == isControlUnInhibitPermitted ) )
				{

					if ( true == dataPoint->isControlInhibited() )
					{
						return true;
					}
					else
					{
						return false;
					}
				}
				else
				{
					return false;
				}
			}
		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", e.what());
			return false;
		}
	}


	bool	DisplayLogic::checkValueAdjustable(unsigned int iPoint, const std::string& newValue,double& correction,double& adjustedValue)
	{
		bool rtnValue = true;
		//
		// sanity checks on value entered by the operator
		//
		std::stringstream valueStream;
		valueStream.precision(12);
		valueStream << newValue;

		DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);
		// if the value entered by the operator is purely a numerical string
		if ( valueStream >> correction )
		{
			//
			// get the some configured parameters that are required to
			// calculate the adjusted value for the Metered DataPoint, based on
			// the value entered by the operator
			//

			double accumulatedInputValue = 0;

			//TD15121, need not to read from database, use datapoint current value instead
			accumulatedInputValue = dataPoint->getValueAsDouble();

			// calculate the new totalised value, based on the entered correction,
			// the current accumulated input and the meter factor
			// TD15121, accumulatedInputValue already include meterFactor.
			adjustedValue = accumulatedInputValue + correction;

			// if eng unit conversion is defined
			if ( false == dataPoint->getUnit().empty() )
			{
				// if the adjusted value is outside the range
				if ( ( adjustedValue < dataPoint->getLimit ( true ) ) ||	// low eng limit
					( adjustedValue > dataPoint->getLimit ( false ) ) )	// high eng limit
				{
					// show text in Status box

					rtnValue = false;
				}
			}
		}

		// if the value entered by the operator is an alpha-numerical string
		else
		{
			rtnValue = false;
		}
		return rtnValue;
	}

	double	DisplayLogic::getChildValueAsDouble(unsigned int iPoint)
	{
		return m_pEquipment->getChildDataPoint(iPoint)->getValueAsDouble();
	}

	std::string	DisplayLogic::getChildValueAsString(UINT iPoint)
	{
		return	m_pEquipment->getChildDataPoint(iPoint)->getValueAsString();
	}

	std::string		DisplayLogic::getDataNodeAssetName()
	{
		return m_pEquipment->getDataNode()->getAssetName();
	}

	std::string		DisplayLogic::getDataNodeDescription()
	{
		return m_pEquipment->getDataNode()->getDescription();
	}

	std::string		DisplayLogic::getDataNodeLabel()
	{
		return m_pEquipment->getDataNode()->getLabel();
	}

	std::string		DisplayLogic::getDataPointName(unsigned int iPoint)
	{
		return m_pEquipment->getChildDataPoint(iPoint)->getEntityName();
	}


	std::string	DisplayLogic::getLimitInfoString(unsigned int iPoint)
	{
		std::ostringstream text;
		DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);

		text << "Type of \"";
		text << dataPoint->getDescription();
		text << "\" is ";
		text << dataPoint->getShortNameOfType();
		text << " (";
		text << dataPoint->getTypeAsString();
		text << ").";
		return text.str();
	}

	bool DisplayLogic::isLimitDp(unsigned int iPoint)
	{
		std::vector<unsigned long>& vectorIndex = m_pEquipment->getLimitIndex(); 
		bool isValid = false;
		for(unsigned int index =0; index < vectorIndex.size(); ++index)
		{
			if(vectorIndex.at(index) == iPoint)
			{
				isValid = true;
				break;
			}
		}
		return isValid;
	}

	TA_Base_Bus::ETagState		DisplayLogic::getDataNodeTaggingState()
	{
		TA_Base_Bus::ETagState rtnState = NotTagged;
		try
		{
			rtnState = m_pEquipment->getDataNode()->getTaggingState();
		}
		catch (ScadaProxyException&ex)
		{
			
		}
		return rtnState;
	}

	TA_Base_Bus::InhibitState   DisplayLogic::getDataNodeTagInhibitState()
	{
		TA_Base_Bus::InhibitState rtnState;
		rtnState.inputInhibit  = NoInputInhibit;
		rtnState.outputInhibit = NoOutputInhibit;
		
		try
		{
			rtnState =  m_pEquipment->getDataNode()->getTagInhibitState();
		}
		catch (ScadaProxyException&ex)
		{

		}

		return rtnState;
	}

	std::string					DisplayLogic::getDataNodePtwId()
	{
		std::string rtnString;
		try
		{
			rtnString = m_pEquipment->getDataNode()->getPtwId();
		}
		catch (ScadaProxyException&ex)
		{

		}
		return rtnString;
	}

	std::string					DisplayLogic::getDataNodeTagComment()
	{
		std::string rtnString;
		try
		{
			rtnString = m_pEquipment->getDataNode()->getTagComment();
		}
		catch (ScadaProxyException&ex)
		{

		}
		return rtnString;
	}

	//unsigned long	DisplayLogic::getDataNodeKey()
	//{
	//	return m_pEquipment->getDataNode()->getEntityKey();
	//}

	bool	DisplayLogic::hasInformationPoint()
	{
		return m_pEquipment->hasInformationPoint();
	}


	bool	DisplayLogic::hasControlPoint()
	{
		return m_pEquipment->hasControlPoint();
	}

	bool DisplayLogic::isInhibitEnable(bool buttonChecked, InhibitType type)
	{
		bool inhibitEnable = false;
		bool isInhibitPermitted = false;
		bool isUnInhibitPermitted = false;

		unsigned long dataNodeKey = m_pEquipment->getDataNode()->getEntityKey();
		switch(type)
		{
		case scanInhibit:	
			isInhibitPermitted = TA_Base_App::AccessControlHelper::getInstance()->IsActionPermitted(
				dataNodeKey, DATA_POINT_SCAN_INHIBIT);
			isUnInhibitPermitted = TA_Base_App::AccessControlHelper::getInstance()->IsActionPermitted(
				dataNodeKey, DATA_POINT_SCAN_UNINHIBIT);
			break;
		case alarmInhibit:
			isInhibitPermitted = TA_Base_App::AccessControlHelper::getInstance()->IsActionPermitted(
				dataNodeKey, DATA_POINT_ALARM_INHIBIT);
			isUnInhibitPermitted = TA_Base_App::AccessControlHelper::getInstance()->IsActionPermitted(
				dataNodeKey, DATA_POINT_ALARM_UNINHIBIT);
			break;
		case mmsInhibit:
			isInhibitPermitted = TA_Base_App::AccessControlHelper::getInstance()->IsActionPermitted(
				dataNodeKey, DATA_POINT_MMS_INHIBIT);
			isUnInhibitPermitted = TA_Base_App::AccessControlHelper::getInstance()->IsActionPermitted(
				dataNodeKey, DATA_POINT_MMS_UNINHIBIT);
			break;
		case controlInhibit:
			isInhibitPermitted = TA_Base_App::AccessControlHelper::getInstance()->IsActionPermitted(
				dataNodeKey, DATA_POINT_CONTROL_INHIBIT);
			isUnInhibitPermitted = TA_Base_App::AccessControlHelper::getInstance()->IsActionPermitted(
				dataNodeKey, DATA_POINT_CONTROL_UNINHIBIT);
			break;
		}


		// if the operator profile has access to both actions
		if ( ( true == isInhibitPermitted ) && ( true == isUnInhibitPermitted ) )
		{
			inhibitEnable = true;
		}
		// if the operator profile has no access to both actions
		else if ( ( false == isInhibitPermitted ) &&( false == isUnInhibitPermitted ) )
		{
			inhibitEnable = false;
		}
		// if the operator profile has access to one of the actions
		else
		{
			// if the operator profile has access to inhibit but
			// not to uninhibit the scan
			if ( ( true == isInhibitPermitted ) && ( false == isUnInhibitPermitted ) )
			{
				if ( false == buttonChecked )
					inhibitEnable = true;
				else
					inhibitEnable = false;
			}

			// if the operator profile has access to uninhibit but
			// not to inhibit the scan
			else if ( ( false == isInhibitPermitted ) && ( true == isUnInhibitPermitted ) )
			{
				if ( false == buttonChecked )
					inhibitEnable = false;
				else
					inhibitEnable = true;
			}
			// if none of the above conditions
			else
			{
				inhibitEnable = false;
			}
		}
		return inhibitEnable;
	}

	//---------------------------------private function---------------------------------


	std::string		DisplayLogic::getCurrentValueAsString(DataPointProxySmartPtr& dataPoint)
	{
		std::string currentValue("");
		std::string unitString("");

		// DataPoint Value - may be override or field value
		std::string alarmState = "";
		try
		{
			alarmState = dataPoint->getAlarmStateShortString();
		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{
		}
		catch (...)
		{
		}

		if (alarmState != "")
		{
			currentValue = alarmState + " ";
		}
		else
		{
			currentValue = "";
		}

		try
		{
			// Change temperature unit format from "0C" to "¡ãC" using ALT+0176 (TD12415)
			unitString = dataPoint->getUnit();

			// 			CString degreeString = unitString.c_str();
			// 			if (degreeString.Find("?C", 0) >= 0)
			// 			{
			// 				degreeString.Replace(_T('?'), _T('?'));
			// 			}
			// 			unitString.assign(degreeString);

			currentValue +=  dataPoint->getValueAsString();
			currentValue += " ";
			currentValue += unitString;

		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{
		}
		catch (...)
		{
		}	
		return currentValue;
	}


	std::string DisplayLogic::getControlStatusString ( TA_Base_Bus::EDataPointControlState state ) 
	{
		std::string returnString ( "" );

		try
		{	
			switch ( state )
			{
			case written:
				returnString = DPT_CONTROL_WRITTEN;
				break;

			case pending:
				returnString = DPT_CONTROL_PENDING;
				break;

			case conditionalSuccess:
				returnString = DPT_CONTROL_CONDITIONALSUCCESS;
				break;

			case success:
				returnString = DPT_CONTROL_SUCCESS;
				break;

			case failed:
				returnString = DPT_CONTROL_FAILED;
				break;

			case LCC_failed:
				returnString = DPT_CONTROL_LCC_FAILED;
				break;

			case RCC_failed:
				returnString = DPT_CONTROL_RCC_FAILED;
				break;

			case comms_failed:
				returnString = DPT_CONTROL_COMMS_FAILED;
				break;

			case noControl:
			default:
				returnString = DPT_CONTROL_NOCONTROL;
				break;
			}
		}
		catch (TA_Base_Core::ScadaProxyException& e)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", e.what());
		}
		catch ( ... )
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, 
				"Unknown exception caught while convert the control state to string");
		}

		return returnString;
	}


	std::string DisplayLogic::alarmTypeToString(TA_Base_Bus::EDataPointAlarms alarmType)
	{
		std::string result("");

		switch(alarmType) 
		{
		case TA_Base_Bus::DptOverScaleHi:
			result = TA_Base_Bus::DPTALM_OH;
			break;
		case TA_Base_Bus::DptOverScaleLo:
			result = TA_Base_Bus::DPTALM_OL;
			break;
		case TA_Base_Bus::DptHiHi:
			result = TA_Base_Bus::DPTALM_HIHI;
			break;
		case TA_Base_Bus::DptHi:
			result = TA_Base_Bus::DPTALM_HI;
			break;
		case TA_Base_Bus::DptLo:
			result = TA_Base_Bus::DPTALM_LO;
			break;
		case TA_Base_Bus::DptLoLo:
			result = TA_Base_Bus::DPTALM_LOLO;
			break;
		default:
			//no other alarm types for analogue datapoint
			break;
		}

		return result;
	}


	void DisplayLogic::fillThresholdString(LimitDetailItem& item, TA_Base_Bus::DataPointProxySmartPtr& datapoint)
	{
		// Threshold Value
		std::ostringstream threshold;
		threshold.clear();
		threshold.precision(10);

		threshold << item.thresholdValue;

		// if eng unit is defined, attached eng unit to the value
		if ( datapoint->getType() == DPT_ANALOGUE && false == datapoint->getUnit().empty() )
		{
			threshold << " " << datapoint->getUnit();
		}


		std::string currentValue = threshold.str() ;
		std::string::size_type posSearch = currentValue.find("0C"); 
		if( posSearch != std::string::npos )
		{
			currentValue.erase( posSearch, 1);
			//						currentValue.insert(posSearch, "?);
		}

		// CL16786++
		if( item.typeString == "OH" )
		{
			currentValue.insert( 0, "> " );
		}
		else if(item.typeString == "OL" )
		{
			currentValue.insert( 0, "< " );
		}
		// ++CL16786
		item.thresholdString = currentValue;
	}

	EDataPointStatusColor DisplayLogic::getDataPointStatusColor(unsigned int iPoint)
	{
		EDataPointStatusColor rtnColor = Blue;
		try
		{
			unsigned long CurrentStatus = m_pEquipment->getChildDataPoint(iPoint)->getStatus();

			switch ( CurrentStatus & TA_Base_Bus::QUALITY_MASK )
			{
			case TA_Base_Bus::QUALITY_GOOD:
				{
					if (CurrentStatus == TA_Base_Bus::QUALITY_GOOD_LOCAL_OVERRIDE)
					{
						rtnColor = Orange;	// orange
					}
					else if (CurrentStatus == TA_Base_Bus::QUALITY_GOOD_NO_SPECIFIC_REASON)
					{
						rtnColor = Green;
					}
				}
				break;
			case TA_Base_Bus::QUALITY_UNCERTAIN:
			case TA_Base_Bus::QUALITY_BAD:
				{
					rtnColor = Blue;
				}
				break;
			default:
				{
					rtnColor = Blue;
				}
				break;
			}

			// wenching++ (TD13684)
			// if the data point is in alarm state and its quality is anything but not 
			// TA_Base_Bus::QUALITY_BAD_NOT_CONNECTED, make sure a red icon is displayed
			if ( ( TA_Base_Bus::QUALITY_BAD_NOT_CONNECTED != CurrentStatus ) && 
				(  m_pEquipment->getChildDataPoint(iPoint)->isInAlarmState() ) &&
				( rtnColor != Orange ) &&
				( rtnColor != Blue ) )
			{
				rtnColor = Red;
			}
			// ++wenching (TD13684)
		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, 
				"getDataPointStatusImageIndex() - %s", e.what());

			rtnColor = Blue;
		}
		catch ( ... )
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, 
				"getDataPointStatusImageIndex() - %s", "Unknown Exception while accessing the DataPointProxySmartPtr.");

			rtnColor = Blue;
		}
		return rtnColor;
	}

	std::string DisplayLogic::getDisplayValueWithUnit(unsigned int iPoint)
	{
		std::string currentValue("");
		std::string unitString("");

		// DataPoint Value - may be override or field value
		std::string alarmState = "";
		try
		{
			alarmState = m_pEquipment->getChildDataPoint(iPoint)->getAlarmStateShortString();
		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{
			// Not always available
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, 
				"displayItem() - %s", e.what());

		}
		catch (...)
		{
			// Not always available
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, 
				"displayItem() - %s", "Unknown Exception");

		}	
		if (alarmState != "")
		{
			currentValue = alarmState + " ";
		}
		else
		{
			currentValue = "";
		}

		try
		{
			// Change temperature unit format from "0C" to "°C" using ALT+0176 (TD12415)
			unitString =m_pEquipment->getChildDataPoint(iPoint)->getUnit();

			// 			CString degreeString = unitString.c_str();
			// 			if (degreeString.Find("?C", 0) >= 0)
			// 			{
			// 				//				degreeString.Replace(_T('?'), _T('?));
			// 			}
			// 			unitString.assign(degreeString);

			currentValue +=  m_pEquipment->getChildDataPoint(iPoint)->getValueAsString();
			currentValue += " ";
			currentValue += unitString;

		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{
			// Not always available
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, 
				"displayItem() - %s", e.what());

		}
		catch (...)
		{
			// Not always available
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, 
				"displayItem() - %s", "Unknown Exception");

		}	

		return currentValue;
	}


	bool		DisplayLogic::findAlarmProperty(unsigned int iPoint, const std::string& alarmTypeString,AlarmProperty& config)
	{
		bool		rtn = false;
		if(alarmTypeString.size() > 0 )
		{
			TA_Base_Bus::AlarmPropertyVect alarmProperties = m_pEquipment->getChildDataPoint(iPoint)->getAlarmProperties();

			for (unsigned int index = 0; index < alarmProperties.size(); index++)
			{
				if(alarmTypeString == alarmTypeToString(alarmProperties[index].type) )
				{
					config = alarmProperties[index];
					rtn = true;
					break;
				}
			}
		}
		return rtn;
	}


	std::string	DisplayLogic::getDataPointUnit(unsigned int iPoint)
	{
		return m_pEquipment->getChildDataPoint(iPoint)->getUnit();
	}

	double	DisplayLogic::getDataPointLimit(unsigned int iPoint, bool lowLimit)
	{
		return m_pEquipment->getChildDataPoint(iPoint)->getLimit(lowLimit);
	}

	std::string	DisplayLogic::getNotes()
	{
		std::string rtnString;
		try
		{
			rtnString = m_pEquipment->getDataNode()->getNotes();
		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{
		
		}
		catch (...)
		{
		}
		return rtnString;
	}


	unsigned int DisplayLogic::getChildDataPointIndexFromEntityKey(unsigned long key)
	{
		return m_pEquipment->getChildDataPointIndexFromEntityKey(key);
	}


	bool	DisplayLogic::isOverrideEnabled(unsigned int iPoint)
	{
		bool rtn = false;
		try
		{
			rtn = m_pEquipment->getChildDataPoint(iPoint)->isOverrideEnabled();
		}
		catch (TA_Base_Core::ScadaProxyException &e)
		{

		}
		catch (...)
		{
		}
		return rtn;
	}

	bool	DisplayLogic::haveAllDataPointsConfigured()
	{
		return m_pEquipment->haveAllDataPointsConfigured();
	}


	unsigned long	DisplayLogic::getDataNodeKey()
	{
		return m_pEquipment->getDataNode()->getEntityKey();
	}

	std::vector<unsigned long>&		DisplayLogic::getWriteableIndex()
	{
		return m_pEquipment->getWriteableIndex();
	}

	std::vector<unsigned long>&		DisplayLogic::getMeterIndex()
	{
		return m_pEquipment->getMeterIndex();
	}

	std::vector<unsigned long>&		DisplayLogic::getLimitIndex()
	{
		return m_pEquipment->getLimitIndex();
	}

	void	DisplayLogic::registerObserver(TA_Base_Bus::IStateChangeObserver* observer, const char * updateType)
	{
		return m_pEquipment->registerObserver(observer,updateType);
	}

	void	DisplayLogic::unregisterObserver(TA_Base_Bus::IStateChangeObserver* observer, const char * updateType)
	{
		return m_pEquipment->unregisterObserver(observer,updateType);
	}

	unsigned int	DisplayLogic::getDirectControlPoint()
	{
		unsigned int rtnPoint = 0xFFFFFFFF;
		unsigned long entityKey = m_pEquipment->getDirectControlKey();
		if(entityKey != 0)
		{
			rtnPoint = m_pEquipment->getChildDataPointIndexFromEntityKey(entityKey);
		}
		return rtnPoint;
	}

	std::string		DisplayLogic::getDirectControlValue()
	{
		std::string rtnValue;
		std::string value = TA_Base_Core::RunParams::getInstance().get(VALUE_RPARAM);
		if(value.size() > 0 )
		{
			unsigned long entityKey = m_pEquipment->getDirectControlKey();
			if(0 != entityKey)
			{
				DataPointProxySmartPtr& datapoint = m_pEquipment->getChildDataPointFromEntityKey(entityKey);
				if( DataPointControlHelper::isControlValueValid(datapoint, value) )
				{
					rtnValue = value;
				}
			}
		}
		return rtnValue;
	}
}