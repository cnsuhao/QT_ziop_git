#include "convertFunction.h"


namespace TA_Base_App
{
	std::string		ConvertFunction::convertDataPointControlStateToString ( TA_Base_Bus::EDataPointControlState state ) 
	{
			std::string returnString ( "" );

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
		return returnString;
	}

	std::string ConvertFunction::convertAlarmTypeToString(TA_Base_Bus::EDataPointAlarms alarmType)
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
}