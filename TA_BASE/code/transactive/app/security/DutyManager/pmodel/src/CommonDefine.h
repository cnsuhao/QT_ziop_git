#ifndef DUTY_MANAGER_BRIDGE_CONSTS_H__
#define DUTY_MANAGER_BRIDGE_CONSTS_H__

#include <sstream>

#include <string>
#include <set>
#include <vector>
#include <string>
#include <list>
#include <map>

#include <algorithm>

#include "core/utilities/src/DebugUtil.h"

#include "core/message/idl/src/GenericMessageCorbaDef.h"

#include "core/configuration_updates/src/IOnlineUpdatable.h"

#include "core/synchronisation/src/NonReEntrantThreadLockable.h"

#include "core/configuration_updates/src/IOnlineUpdatable.h"
#include "core/configuration_updates/src/ConfigUpdateDetails.h"
#include "core/data_access_interface/src/IOperator.h"
#include "core/data_access_interface/src/IProfile.h"
#include "core/data_access_interface/src/IActionGroup.h"
#include "core/data_access_interface/src/IRegion.h"
#include "core/data_access_interface/src/ISubsystem.h"

#include "core/data_access_interface/entity_access/src/IConsole.h"
#include "core/data_access_interface/src/OperatorAccessFactory.h"
#include "core/data_access_interface/src/ProfileAccessFactory.h"
#include "core/data_access_interface/src/ActionGroupAccessFactory.h"
#include "core/data_access_interface/src/LocationAccessFactory.h"
#include "core/data_access_interface/src/RegionAccessFactory.h"
#include "core/data_access_interface/src/SubsystemAccessFactory.h"
#include "core/data_access_interface/entity_access/src/ConsoleAccessFactory.h"

#include "bus/security/duty_agent/idl/src/DutyAgentTypeCorbaDef.h"
#include "bus/security/duty_monitor_framework/src/DMFTypeDefs.h"

#include "bus/generic_gui_pmod/src/PModelDataType.h"



#include "bus/security/security_utilities/src/SeObserver.h"
#include "bus/security/security_utilities/src/SeSubject.h"

#include "bus/security/security_utilities/src/DbCacheLookup.h"
#include "bus/security/duty_monitor_framework/src/DbCacheLookupEx.h"
#include "bus/security/duty_monitor_framework/src/DataCache.h"

typedef unsigned char   uint8_t;
typedef unsigned short  uint16_t;
typedef unsigned int    uint32_t;
typedef char			int8_t;
typedef short			int16_t;
typedef int 			int32_t;

// to view
const int VIEW_SIGNAL							= 0;
const int DM_MESSAGE_BOX_INFO					= 1;
const int DM_MESSAGE_BOX_WARNING				= 2;
const int DM_MESSAGE_BOX_ERROR					= 3;
const int DM_UPDATE_TITLE_TEXT					= 4;
const int DM_UPDATE_DLG							= 5;
const int DM_POPULATE_DATA						= 6;
const int DM_MESSAGE_BOX_WARNING_AND_CLOSE	    = 7;
const int DUTY_AGENT_RECOVER_UPDATE				= 8;
const int DM_RBN_DUTY_CHANGED					= 9;
const int DM_SUB_DUTY_CHANGED					= 11;
const int DM_SESSION_CHANGED					= 12;

// TO VIEW AND PRESENTATION
const int VIEW_AND_PRESENTATION_SIGNAL			= 1000;

// to presentation
const int PRESENTATION_SIGNAL					= 2000;


#define VIEW_DATA									"view_data"
#define VIEW_AND_PRESENTATION_DATA					"view_and_presentation_data"
#define PRESENTATION_DATA							"presentation_data"


// pmodel
#define PMODEL_DUTY_MANAGER_DLG						"DutyManagerDlgModel"
#define PMODEL_DUTY_MANAGER_PROPERTY_SHEET			"DutyManagerPropertySheetModel"
// pmodel		page
#define PMODEL_REGION_DUTY_PROPERTY_PAGE			"RegionDutyPropertyPageModel"
#define PMODEL_SUBSYSTEM_DUTY_PROPERTY_PAGE			"SubsystemDutyPropertyPageModel"
#define PMODEL_OTHER_SES_SUB_DUTY_PROPERTY_PAGE		"OtherSesSubDutyPropertyPageModel"

// pmodel		dlg
#define PMODEL_REGION_DUTY_TRANSFER_DLG				"RegionDutyTransferDlgModel"
#define PMODEL_SUBSYSTEM_DUTY_TRANSFER_DLG			"SubsystemDutyTransferDlgModel"

#define PMODEL_REGION_SELECTION_DLG					"RegionSelectionDlgModel"
#define PMODEL_SELECT_SE_SUB_DUTY_DLG				"SelectSesSubDutyDlgModel"

// praram
#define RPARAM_SHOWOTHERSES							"ShowOtherses"
#define RPARAM_SHOWSESSIONWORKSTATION				"ShowSessionworkstation"

// param
const char* const RPARAM_USELOCATIONLABEL			= "RegionIsLocation";
const char* const RPARAM_NORMALDUTYCOLOUR			= "NormalDutyColour";
const char* const RPARAM_DELEGATEDDUTYCOLOUR		= "DelegatedDutyColour";
const char* const RPARAM_DEGRADEDDUTYCOLOUR			= "DegradedDutyColour";
const char* const RPARAM_UNALLOCATEDSUBSYSTEMCOLOUR = "UnallocatedSubsystemColour";
const char* const RPARAM_PROJECTNAME				= "ProjectName";


// define string
const std::string DUTY_MANAGER						("Duty Manager");
const std::string DUTY_MANAGER_PROPERTY_SHEET_NAME	("Rights Manager PS");
const std::string ALL_LOCATOINS						("All Locations");

// define value
const unsigned long NO_ACTION_GROUP					(0xFFFFFFFF);
const unsigned long CENTRAL_PROFILE_TYPE			(1); //1 is central profile //TD15533
const TA_Base_Bus::LocationKey ALL_LOCATION			(0); //0 is all location if central profile //TD15533


using namespace TA_Base_Core;
using namespace TA_Base_Bus;
//using namespace TA_Base_App;




namespace TA_Base_App
{
	class DutyManagerBridgeConsts
	{
	public:
		static const std::string DEFAUNT_TITLE_TEXT;

	};

	//
	// DutyManagerEventArgs,DM_UPDATE_TITLE_TEXT
	//

	class DutyManagerEventArgs : public TA_Base_Bus::EventArgs
	{
	public:
		DutyManagerEventArgs(int typeCode,std::string& message);
		DutyManagerEventArgs(int typeCode,int locationId);
		DutyManagerEventArgs(int typeCode,std::string& exception,std::string& exMsg,std::string& exMsgDetail);
		~DutyManagerEventArgs();

		std::string	getMessage();
		int getLocationId();
		bool getError(std::string& exception,std::string& exMsg,std::string& exMsgDetail);

	private:
		// DM_MESSAGE_BOX_WARNING
		std::string m_message;

		// DM_MESSAGE_BOX_ERROR
		// DutyMonitorFrameworkException and TransactiveException
		std::string m_exception;
		std::string m_exceptionMessage;
		std::string m_exceptionDetail;

		// APP_SERVICE_USER_RECEIVE_DUTY_AGENT_RECOVER_UPDATE
		int m_locationId;
	};




	// RegionDutyChangedEventArgs
	class RegionDutyChangedEventArgs : public TA_Base_Bus::EventArgs
	{
	public:
		RegionDutyChangedEventArgs(int typeCode,RegionDutyUpdate dutyUpdate);
		~RegionDutyChangedEventArgs();
		RegionDutyUpdate m_dutyUpdate;
	};

	// SubsystemDutyChangedEventArgs
	class SubsystemDutyChangedEventArgs : public TA_Base_Bus::EventArgs
	{
	public:
		SubsystemDutyChangedEventArgs(int typeCode,SubsystemDutyUpdate dutyUpdate);
		~SubsystemDutyChangedEventArgs();
		SubsystemDutyUpdate m_dutyUpdate;
	};

	// SessionChangedEventArgs
	class SessionChangedEventArgs : public TA_Base_Bus::EventArgs
	{
	public:
		SessionChangedEventArgs(int typeCode,SessionUpdate sessionUpdate);
		~SessionChangedEventArgs();
		SessionUpdate m_sessionUpdate;
	};

	//
	// PopulateDataEventArgs
	// 
	class PopulateDataEventArgs : public TA_Base_Bus::EventArgs
	{
	public:
		PopulateDataEventArgs(int typeCode,bool pStatic,bool pDynamic);
		~PopulateDataEventArgs();

		bool isPopulateStaticData();
		bool isPopulateDynamicData();

	private:
		bool m_pStatic;
		bool m_pDynamic;

	};

	typedef struct 
	{
		int baseImage;
		std::string baseText;
		int overrideImage;
		std::string overrideText;
		bool isOverrided;
	} CellData;

	typedef struct 
	{
		TA_Base_Bus::ProfileKey profileKey;
		TA_Base_Bus::SubsystemKey subsystemKey;
		TA_Base_Bus::ESubsystemState subsystemState;
		TA_Base_Bus::ActionGroupKey actionGroupKey;
	} PSSA;

	typedef std::vector<PSSA> ProfilesWithDuties;



	struct TransferableRegionComparator
	{
		bool operator()(const TA_Base_Bus::DutyAgentTypeCorbaDef::TransferableRegion& tr1, 
			const TA_Base_Bus::DutyAgentTypeCorbaDef::TransferableRegion& tr2) const
		{
			return tr1.regionKey < tr2.regionKey;
		}
	};

	struct TransferableSubsystemComparator
	{
		TransferableSubsystemComparator(TA_Base_Bus::DataCache& dataCache) : m_dataCache(dataCache) {}

		bool operator()(const TA_Base_Bus::DutyAgentTypeCorbaDef::TransferableSubsystem& ts1, 
			const TA_Base_Bus::DutyAgentTypeCorbaDef::TransferableSubsystem& ts2) const
		{
			if (ts1.regionKey == ts2.regionKey)
			{
				try
				{
					std::string subName1(m_dataCache.getSubsystemName(ts1.subsystemKey));
					std::string subName2(m_dataCache.getSubsystemName(ts2.subsystemKey));

					//return (::stricmp(subName1.c_str(), subName2.c_str()) < 0);
					return (_stricmp(subName1.c_str(), subName2.c_str()) < 0);
				}
				catch (...)
				{
					return false;
				}
			}

			return ts1.regionKey < ts2.regionKey;
		}

		TA_Base_Bus::DataCache& m_dataCache;
	};

}

#endif // DUTY_MANAGER_BRIDGE_CONSTS_H__