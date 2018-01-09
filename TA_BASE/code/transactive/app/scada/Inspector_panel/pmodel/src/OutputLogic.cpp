#include "OutputLogic.h"
#include "bus\scada\proxy_library\src\IDataPointProxy.h"
#include "core\utilities\src\RunParams.h"
#include "core\exceptions\src\ScadaProxyException.h"
#include "CommonExceptionsCorbaDef.h"
#include "core\exceptions\src\ObjectResolutionException.h"
#include "bus\scada\proxy_library\src\IDataNodeProxy.h"
#include "core\corba\src\CorbaUtil.h"
#include "DataPointControlHelper.h"
#include "OutputLogicException.h"




#define CATCH_EXCEPTION(Action) 		catch (TA_Base_Core::ScadaProxyException &e) \
		{\
			errorCode = Ex_Code_ScadaProxyException;\
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, #Action "Catche ScadaProxyException[%s]", e.what() );\
		}\
		catch (TA_Base_Core::NotAvailableException &e)\
		{\
			errorCode = Ex_Code_NotAvailableException;\
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, #Action "Catche NotAvailableException[%s]", e.reason );\
		}\
		catch (TA_Base_Core::BadParameterException& e)\
		{\
			errorCode = Ex_Code_BadParameterException;\
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, #Action "Catche BadParameterException[%s]", e.reason );\
		}\
		catch (TA_Base_Core::AccessDeniedException& e)\
		{\
			errorCode = Ex_Code_AccessDeniedException;\
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, #Action "Catche AccessDeniedException[%s]", e.reason );\
		}\
		catch ( const TA_Base_Core::ObjectResolutionException & e)\
		{\
			errorCode = Ex_Code_ObjectResolutionException;\
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, #Action "Catche ObjectResolutionException[%s]", e.what() );\
		}\
		catch ( const TA_Base_Core::OperationModeException & e)\
		{\
			errorCode = Ex_Code_OperationModeException;\
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, #Action "Catche OperationModeException[%s]", e.reason );\
		}\
		catch ( const CORBA::Exception &)\
		{\
			errorCode = Ex_Code_CORBA_Exception;\
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, #Action "Catche CORBA::Exception");\
		}\
		catch (...)\
		{\
			errorCode = Ex_Code_UnknownException;\
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, #Action "Catche UnknownException");\
		}




namespace TA_Base_App
{

	OutputLogic::OutputLogic(CEquipment*	pEquipment)
	:m_pEquipment(pEquipment)
	{

	}

	OutputLogic::~OutputLogic()
	{

	}


	void	OutputLogic::setValue(unsigned int iPoint, const std::string& strValue, std::string& responseMsg)
	{
		int		errorCode = 0;
		std::stringstream message;
		std::string	resultString = "Failed";
		try
		{
			DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);

			message << dataPoint->getDescription() << ":Update value to \"" << strValue << "\".";

			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", message.str().c_str() );

			dataPoint->setValue(strValue, TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID));
			resultString = "Successfully";
		
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "%s", message.str().c_str() );
			
		}
		CATCH_EXCEPTION("SetValue");
		
		if(0!= errorCode)
		{
			throw OutputLogicException(errorCode);
		}
		message << resultString;
		responseMsg = message.str();
	}

	bool OutputLogic::isOkToSendControl( unsigned int iPoint, const std::string& strValue)
	{
		bool rtn = true;
		if (strValue.empty())
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Control value is empty, not processing!");
			rtn = false;
		}
		else
		{
			// Get the DataPoint array position
			TA_Base_Bus::DataPointProxySmartPtr &dataPoint = m_pEquipment->getChildDataPoint(iPoint);

			if (!DataPointControlHelper::isControlValueValid(dataPoint, strValue))
			{
				LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Control value is invalid!", strValue);
				return false;
			}
		}

		if(rtn == false)
		{
			OutputLogicException ex(Ex_Code_InvalidValue1_1);
			ex.appendParameter(strValue);
			throw ex;
		}
		return rtn;
	}


	void	OutputLogic::setManuallyOverrideValue (unsigned int iPoint, const std::string& overrideValue, std::string& responseString)
	{
		bool rtn = false;
		int errorCode = 0;
		std::stringstream statusLine;
		try
		{
			DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);
			statusLine << dataPoint->getDescription() << ": ";

			if ( overrideValue == "" )
			{
				OutputLogicException ex(Ex_Code_InvalidValue1_1);
				ex.appendParameter(overrideValue);
				throw ex;
			}
			else
			{

				// if this data point is an analogue, i.e not a boolean, not a text nor a derived,
				// do range checking
				if ( ( false == dataPoint->isBoolean() ) && ( false == dataPoint->isText() ) && ( false == dataPoint->isDerived() ) )
				{
					double doubleValue = 0;
					std::istringstream valueStream ( overrideValue );

					// if the value entered by the operator is purely a numerical string
					if ( valueStream >> doubleValue )
					{
						// if the eng unit is defined
						if ( "" != dataPoint->getUnit() )
						{
							// if the value entered by the operator is outside
							// the range
							if ( ( doubleValue < dataPoint->getLimit ( true ) ) ||	// low eng limit
								( doubleValue > dataPoint->getLimit ( false ) ) )	// high eng limit
							{
								// show text in Status box
								/*statusLine << "Override Failed: entered value is not within range.";
								responseString = statusLine.str();

								std::ostringstream message2;
								message2 << dataPoint->getDescription() << ": ";
								message2 << "Valid range is from ";
								message2 << dataPoint->getLimit(true) << " to ";
								//message2 << dataPoint->getLimit(false) << " ";

								DWORD Limit = dataPoint->getLimit(false);//jiangshengguang++ 4.29497e+009 to 4294967295
								message2 << Limit << " ";

								message2 << dataPoint->getUnit() << ".";													
								responseString += message2.str();
								rtn = false;*/
								
								std::ostringstream highStream,lowStream;
								lowStream<<dataPoint->getLimit(true);
								highStream<<dataPoint->getLimit(false);

								OutputLogicException ex(Ex_Code_InvalidValue2_3);
								ex.appendParameter(lowStream.str());
								ex.appendParameter(highStream.str());
								ex.appendParameter(dataPoint->getUnit());
								throw ex;
							}
						}
					}
					// if the value entered by the operator is an alpha-numerical string
					else
					{
						/*statusLine << "Override Failed: Value " << "\"" << overrideValue << "\" is not a valid number.";
						responseString = statusLine.str();
						rtn = false;*/

						OutputLogicException ex(Ex_Code_InvalidValue3_1);
						ex.appendParameter(overrideValue);
						throw ex;
					}
				}

				std::string sessionId ( TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID) );
				dataPoint->setManuallyOverrideValue ( overrideValue, sessionId );

				// show text in Status box
				statusLine << "Override \"" << overrideValue << "\" Successful.";
				responseString = statusLine.str();
				rtn = true;
			}
		}
		CATCH_EXCEPTION("SetManuallyOverrideValue");
		if(0!= errorCode)
		{
			throw OutputLogicException(errorCode);
		}
		return;
	}


	void	OutputLogic::removeOverride (unsigned int iPoint, std::string& responseString)
	{
		bool				result = false;
		int					errorCode = 0;
		std::stringstream	statusLine;
		std::string			resultString = "Failed";
		std::string			resonString;

		try
		{
			DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);
			statusLine << "Override OFF "<<dataPoint->getDescription() << ": ";

			std::string sessionId ( TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID) );

			dataPoint->setOverrideEnableStatus(false,sessionId);

			result = "Successful.";

			result = true;
		}
		CATCH_EXCEPTION("removeOverride");
		if(0!= errorCode)
		{
			throw OutputLogicException(errorCode);
		}

		statusLine << resultString;
		responseString = statusLine.str();
		return ;
	}


	void	OutputLogic::setScanInhibit(unsigned int iPoint, bool bRequestScanInhibit, std::string& responseString)
	{
		bool				result = false;
		int					errorCode = 0;
		std::stringstream	statusLine;
		std::string			resultString = "Failed";
		std::string			resonString;
		try
		{
			DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);

			statusLine << dataPoint->getDescription();
			statusLine << ": Scan Inhibit " << ( bRequestScanInhibit?"ON":"OFF" );

			// TD12137: Log an inhibit requested event BEFORE attempting to set the datapoint's scan inhibition state.
			//todo//submitAuditMessage("Scan Inhibit", bRequestScanInhibit?"ADD":"REMOVE",dataPoint->getEntityName(),dataPoint->getEntityKey());

			m_auditMessageSender.submitInhibitAuditMessage("Scan Inhibit", bRequestScanInhibit?"ADD":"REMOVE",dataPoint->getEntityName(),dataPoint->getEntityKey());
			bool prevInhibitState = dataPoint->isScanInhibited();
			if ( prevInhibitState == true && bRequestScanInhibit == false)
			{
				dataPoint->setInputInhibitStatus(TA_Base_Bus::InhibitAlarm, TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID));	
			}
			else 
			{
				dataPoint->setInputInhibitStatus(bRequestScanInhibit?TA_Base_Bus::InhibitScan:TA_Base_Bus::NoInputInhibit, TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID));										
			}

			resultString = " Successful.";
			result = true;
		}
		CATCH_EXCEPTION("setScanInhibit");
		if(0!= errorCode)
		{
			throw OutputLogicException(errorCode);
		}
		statusLine << resultString;
		responseString = statusLine.str();
		return;
	}


	void	OutputLogic::setAlarmInhibit(unsigned int iPoint, bool bRequestAlarmInhibit, std::string& responseString)
	{
		bool				result = false;
		int					errorCode = 0;
		std::stringstream	statusLine;
		std::string			resultString = "Failed";
		std::string			resonString;
		try
		{
			DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);

			statusLine << dataPoint->getDescription();
			statusLine << ": Alarm Inhibit " << ( bRequestAlarmInhibit?"ON":"OFF" );

			// TD12137: Log an inhibit requested event BEFORE attempting to set the datapoint's scan inhibition state.
			//todo->//submitAuditMessage(" Alarm Inhibit ", bRequestAlarmInhibit?"ADD":"REMOVE",dataPoint->getEntityName(),dataPoint->getEntityKey());

			m_auditMessageSender.submitInhibitAuditMessage(" Alarm Inhibit ", bRequestAlarmInhibit?"ADD":"REMOVE",dataPoint->getEntityName(),dataPoint->getEntityKey());
			bool prevInhibitState = dataPoint->isAlarmInhibited();
			if ( prevInhibitState == true && bRequestAlarmInhibit == false)
			{
				dataPoint->setInputInhibitStatus(TA_Base_Bus::InhibitMmsAlarm, TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID));	
			}
			else 
			{
				dataPoint->setInputInhibitStatus(bRequestAlarmInhibit?TA_Base_Bus::InhibitAlarm:TA_Base_Bus::NoInputInhibit, TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID));										
			}

			resultString = " Successful.";
			// set the flag to update the screen
			result = true;
		}
		CATCH_EXCEPTION("setAlarmInhibit");
		if(0!= errorCode)
		{
			throw OutputLogicException(errorCode);
		}
		statusLine << resultString;
		responseString = statusLine.str();
		return;
	}

	void	OutputLogic::setControlInhibit(unsigned int iPoint, bool bRequestControlInhibit,std::string& responseString)
	{
		bool				result = false;
		int					errorCode = 0;
		std::stringstream	statusLine;
		std::string			resultString = "Failed";
		std::string			resonString;

		try
		{
			DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);

			statusLine << dataPoint->getDescription();
			statusLine << ": Control Inhibit " << ( bRequestControlInhibit?"ON":"OFF" );

			// TD12137: Log an inhibit requested event BEFORE attempting to set the datapoint's scan inhibition state.
			//todo->//submitAuditMessage(" Control Inhibit ", bRequestControlInhibit?"ADD":"REMOVE",dataPoint->getEntityName(),dataPoint->getEntityKey());

			m_auditMessageSender.submitInhibitAuditMessage(" Control Inhibit ", bRequestControlInhibit?"ADD":"REMOVE",dataPoint->getEntityName(),dataPoint->getEntityKey());
			
			dataPoint->setOutputInhibitStatus(bRequestControlInhibit?TA_Base_Bus::InhibitControl:TA_Base_Bus::NoOutputInhibit, TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID));	

			resultString = " Successful.";
			// set the flag to update the screen
			result = true;
		}
		CATCH_EXCEPTION("setControlInhibit");
		if(0!= errorCode)
		{
			throw OutputLogicException(errorCode);
		}

		statusLine << resultString;
		responseString = statusLine.str();
		return;
	}

	void	OutputLogic::setMmsInhibit(unsigned int iPoint, bool bRequestMmsInhibit, std::string& responseString)
	{
		bool				result = false;
		int					errorCode = 0;
		std::stringstream	statusLine;
		std::string			resultString = "Failed";
		std::string			resonString;
		try
		{
			DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);

			statusLine << dataPoint->getDescription();
			statusLine << ": MMS Inhibit " << ( bRequestMmsInhibit?"ON":"OFF" );

			// TD12137: Log an inhibit requested event BEFORE attempting to set the datapoint's scan inhibition state.
			//todo->//submitAuditMessage(" MMS Inhibit ", bRequestMmsInhibit?"ADD":"REMOVE",dataPoint->getEntityName(),dataPoint->getEntityKey());
			m_auditMessageSender.submitInhibitAuditMessage(" MMS Inhibit ", bRequestMmsInhibit?"ADD":"REMOVE",dataPoint->getEntityName(),dataPoint->getEntityKey());
			dataPoint->setInputInhibitStatus(bRequestMmsInhibit? TA_Base_Bus::InhibitMmsAlarm:TA_Base_Bus::NoInputInhibit, TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID));
			resultString = " Successful.";
			result = true;
		}
		CATCH_EXCEPTION("setMmsInhibit");
		if(0!= errorCode)
		{
			throw OutputLogicException(errorCode);
		}
		statusLine << resultString;
		responseString = statusLine.str();
		return ;
	}


	void	OutputLogic::setMeteredCorrectionValue(unsigned int iPoint, double correction, std::string& responseString)
	{
		bool				result = false;
		int					errorCode = 0;
		std::stringstream	statusLine;
		std::string			resultString = "Failed";
		std::string			resonString;
		try
		{
			DataPointProxySmartPtr& dataPoint = m_pEquipment->getChildDataPoint(iPoint);

			statusLine<<"Adjusting Metered DataPoint \"" ;
			statusLine<<dataPoint->getDescription();
			statusLine<<"\" by "; //+ strValue;
			statusLine<<correction;

			dataPoint->setMeteredCorrectionValue(correction, TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID));
			result = true;

			resultString = "Successful";
		}
		CATCH_EXCEPTION("setMeteredCorrectionValue");
		if(0!= errorCode)
		{
			throw OutputLogicException(errorCode);
		}
		statusLine << resultString;
		return ;
	}

	
	void	OutputLogic::setAlarmProperties (unsigned int iPoint, const AlarmProperty & newAlarmProperty,std::string& responseString )
	{
		bool				result = false;
		int					errorCode = 0;
		std::stringstream	statusLine;
		std::string			resultString = "Failed";
		std::string			resonString;
		try
		{
			statusLine <<"Update AlarmProperties ";
			statusLine << m_pEquipment->getChildDataPoint(iPoint)->getDescription();
			statusLine << " : ";

			std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
			m_pEquipment->getChildDataPoint(iPoint)->setAlarmProperties(newAlarmProperty,sessionId);
			
			resultString = "Successful.";
			result = true;
		}
		CATCH_EXCEPTION("setAlarmProperties");
		if(0!= errorCode)
		{
			throw OutputLogicException(errorCode);
		}
		statusLine << resultString; 
		responseString = statusLine.str();
		return;
	}


	void	OutputLogic::setNotes(const std::string& notes, std::string& responseString)
	{
		bool				result = false;
		int					errorCode  = 0;
		std::stringstream	statusLine;
		std::string			resultString = "Failed";
		std::string			resonString;

		try
		{	
			statusLine <<"Update Notes ";
			statusLine << m_pEquipment->getDataNode()->getDescription();
			statusLine << " : ";

			std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
			m_pEquipment->getDataNode()->setNotes(notes, sessionId);
			
			resultString = "Successful.";
			result = true;
			//todo->//submitAuditMessage("Removed", m_realPModel->getDataNodeAssetName(), m_realPModel->getDataNodeEntityKey());
			m_auditMessageSender.submitNotesAuditMessage("Removed", m_pEquipment->getDataNode()->getAssetName(), m_pEquipment->getDataNode()->getEntityKey());
		}
		CATCH_EXCEPTION("setNotes");
		if(0!= errorCode)
		{
			throw OutputLogicException(errorCode);
		}

		statusLine << resultString;
		responseString = statusLine.str();
		return;
	}

	void OutputLogic::removeTagState(std::string& responseString)
	{
		bool				result = false;
		int					errorCode =0;
		std::stringstream	statusLine;
		std::string			resultString = "Failed";
		std::string			reasonString;
		try
		{
			statusLine<<"Remove Tag ";

			std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
			m_pEquipment->getDataNode()->setTagState(NotTagged, NoOutputInhibit, NoInputInhibit, "", "", sessionId);
			result = true;
			resultString = "Successful";
			//submitAuditMessage("Tag", "Added", m_realPModel->getDataNodeAssetName(), m_realPModel->getDataNodeEntityKey());

		}
		CATCH_EXCEPTION("removeTagState");
		if(0!= errorCode)
		{
			throw OutputLogicException(errorCode);
		}
		statusLine << resultString;
		responseString = statusLine.str();
		return;
	}

	void	OutputLogic::setTagState(const ETagState tagState, const EOutputInhibitState outputInhibitState,const EInputInhibitState inputInhibitState,const std::string& ptwId,	const std::string& tagComment, std::string& responseString)
	{
		bool				result = false;
		int					errorCode = 0;
		std::stringstream	statusLine;
		std::string			resultString = "Failed";
		std::string			reasonString;
		try
		{			
			if(tagState == NotTagged)
			{
				return ;
			}
			
			statusLine << "Tag ";
			if(tagState == TaggedPtw && ptwId.size() > 0)
			{
				statusLine << " With PTW";
			}

			if ( ( TA_Base_Bus::NoInputInhibit != inputInhibitState ) ||
				( TA_Base_Bus::NoOutputInhibit != outputInhibitState ) )
			{
				statusLine << " With";
				if ( TA_Base_Bus::InhibitScan == inputInhibitState )
				{
					statusLine << " Scan Inhibit ";
				}
				else if ( TA_Base_Bus::InhibitAlarm == inputInhibitState )
				{
					statusLine << " Alarm Inhibit ";
				}
				else if ( TA_Base_Bus::InhibitMmsAlarm == inputInhibitState )
				{
					statusLine << " MMS Inhibit ";
				}

				if ( TA_Base_Bus::InhibitControl == outputInhibitState )
				{
					statusLine << " Control Inhibit ";
				}					
			}

			// tag the equipment now
			std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
			m_pEquipment->getDataNode()->setTagState(Tagged, outputInhibitState, inputInhibitState, 
				ptwId, tagComment,sessionId);
			
			result = true;
			resultString = "Successful";
			//submitAuditMessage("Tag", "Added", m_realPModel->getDataNodeAssetName(), m_realPModel->getDataNodeEntityKey());
			m_auditMessageSender.submitTagAuditMessage("Tag", "Added", m_pEquipment->getDataNode()->getAssetName(),m_pEquipment->getDataNode()->getEntityKey());
				
		}
		CATCH_EXCEPTION("removeTagState");
		if(0!= errorCode)
		{
			throw OutputLogicException(errorCode);
		}
		statusLine << resultString;
		responseString = statusLine.str();
		return ;
	}
}