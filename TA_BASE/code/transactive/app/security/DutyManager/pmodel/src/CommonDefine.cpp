#include "app/security/DutyManager/pmodel/src/CommonDefine.h"

namespace TA_Base_App
{


	const std::string DutyManagerBridgeConsts::DEFAUNT_TITLE_TEXT = "Rights Manager";


	//
	// DutyManagerEventArgs
	//

	DutyManagerEventArgs::DutyManagerEventArgs(int typeCode,std::string& message)
		:EventArgs(typeCode)
	{
		m_message = message;

		m_exception = "";
		m_exceptionMessage = "";
		m_exceptionDetail = "";
	}

	DutyManagerEventArgs::DutyManagerEventArgs(int typeCode,std::string& exception,std::string& exMsg,std::string& exMsgDetail)
		:EventArgs(typeCode)
	{
		m_message = "";

		m_exception			= exception;
		m_exceptionMessage	= exMsg;
		m_exceptionDetail	= exMsgDetail;
	}

	DutyManagerEventArgs::DutyManagerEventArgs(int typeCode,int locationId)
		:EventArgs(typeCode)
	{
		m_locationId = locationId;

		m_message			= "";
		m_exception			= "";
		m_exceptionMessage	= "";
		m_exceptionDetail	= "";
	}

	DutyManagerEventArgs::~DutyManagerEventArgs()
	{
		m_message = "";
	}

	std::string	DutyManagerEventArgs::getMessage()
	{
		return m_message;
	}
	int DutyManagerEventArgs::getLocationId()
	{
		return m_locationId;
	}

	bool DutyManagerEventArgs::getError(std::string& exception,std::string& exMsg,std::string& exMsgDetail)
	{
		bool rtn = false;
		if ( DutyManagerEventArgs::m_typeCode == DM_MESSAGE_BOX_ERROR )
		{
			exception	= m_exception;
			exMsg		= m_exceptionMessage;
			exMsgDetail = m_exceptionDetail;

			rtn = true;
		}
		return rtn;
	}

	SessionChangedEventArgs::SessionChangedEventArgs(int typeCode,SessionUpdate sessionUpdate)
		:EventArgs(typeCode)
	{
		m_sessionUpdate = sessionUpdate;
	}

	SessionChangedEventArgs::~SessionChangedEventArgs()
	{}

	RegionDutyChangedEventArgs::RegionDutyChangedEventArgs(int typeCode,RegionDutyUpdate dutyUpdate)
		:EventArgs(typeCode)
	{
		m_dutyUpdate = dutyUpdate;
	}

	RegionDutyChangedEventArgs::~RegionDutyChangedEventArgs()
	{}

	SubsystemDutyChangedEventArgs::SubsystemDutyChangedEventArgs(int typeCode,SubsystemDutyUpdate dutyUpdate)
		:EventArgs(typeCode)
	{
		m_dutyUpdate = dutyUpdate;
	}

	SubsystemDutyChangedEventArgs::~SubsystemDutyChangedEventArgs()
	{}

	//
	// PopulateDataEventArgs
	//
	PopulateDataEventArgs::PopulateDataEventArgs(int typeCode,bool pStatic,bool pDynamic)
		:EventArgs(typeCode)
	{
		m_pStatic = pStatic;
		m_pDynamic = pDynamic;
	}
	PopulateDataEventArgs::~PopulateDataEventArgs()
	{}

	bool PopulateDataEventArgs::isPopulateStaticData()
	{
		return m_pStatic;
	}
	bool PopulateDataEventArgs::isPopulateDynamicData()
	{
		return m_pDynamic;
	}
}