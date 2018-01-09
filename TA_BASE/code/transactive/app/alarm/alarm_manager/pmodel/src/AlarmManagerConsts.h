#ifndef ALARM_MANAGER_CONSTS_H__
#define ALARM_MANAGER_CONSTS_H__

 
#define PMODEL_AlarmGUIDlg "AlarmGUIDlg"
#define PMODEL_AlarmFilterDlg "AlarmFilterDlg"
#define PMODEL_MultiSelDlg "MultiSelDlg"
#define PMODEL_SuppressAlarmRule "SuppressAlarmRuleDlg"

// Events
#define EV_FILTER_CHANGE			"Filter Change Event"
#define EV_FILTERDLG_BTN_UPDATE		"Button Update"
#define EV_POPULATE_NAMED_FILTER	"Populate Named Filter"
#define EV_POPULATE_FILTER_SEL		"Populate Filter Selection"
#define EV_SET_ALARM_FILTER			"Set Alarm Filter"

#define EV_SERVER_STATUS			"Server Status"
#define EV_SYSTEM_STATUS			"System Status"
#define EV_SESSION_CHANGED			"Session Changed"
#define EV_FILTER_ON_ASSET			"Filter On Asset"
#define EV_FILTER_ON_ENTITY			"Filter On Entity"

// Window Message ID
#define WM_SET_ALARM_FILTER	1000 // Start ID
#define WM_APPLY_FILTER		1001 

#include <map>

static const unsigned long PROFILE_LOCAL = 0;
static const unsigned long PROFILE_CENTRAL = 1;

namespace TA_Base_App
{
	const int APP_NOTIFY_FILTER_CONFIRM_SAVE = 1;  ///< confirm save
	const int APP_NOTIFY_FILTER_SAVE_ERROR = 2;  ///<  error happen
	const int APP_NOTIFY_FILTER_CONFIRM_DELETE = 3;  ///< confirm save
	const int APP_NOTIFY_FILTER_DELETED_ERROR = 4;  ///< confirm save

	const int WM_ON_FILTER_DLG_UPDATE = 1;

	const std::string RPARAM_FILTERONASSET = "FilterOnAsset";
	const std::string RPARAM_FILTERONENTITY = "FilterOnEntity";
	const std::string RPARAM_XPOS = "AppXpos";
	const std::string RPARAM_YPOS = "AppYpos";
	const std::string RPARAM_HEIGHT = "AppHeight";
	const std::string RPARAM_WIDTH = "AppWidth";

	const std::string NO_MATCHING_ALARMS_MESSAGE = "No alarms match the current filter";


	enum FilterDlgEvents
	{
		FILTER_CHANGE = 0,
		CREATE_SEVERITY,
		BUTTON_STATE,
		POPULATE_NAMED_FILTER,
		POPULATE_FILTER_SELECTION
	};

	enum OMFilter
	{
		OPERATION = 0,
		MAINTENANCE,
		BOTH,	
		ALL_OM
	};
	
	enum AckFilter
	{
		ACKED = 0,
		UNACKED,
		ALL_ACK
	};

	enum StateFilter
	{
		OPEN = 0,
		CLOSED,
		ALL_STATE
	};

	enum FilterDlgButton
	{
		BTN_CLEAR = 0,
		BTN_SAVE,
		BTN_DELETE,
		BTN_OK,
		BTN_CANCEL,
		BTN_APPLY,
	};

	typedef std::map<FilterDlgButton, bool> T_ButtonStateMap;

	namespace AlarmFilter
	{
		////////////////////////////////////////////////////////////////////////
		// Constant string for use in the Named Filter Database
		//
		// Change "Default Filter" to "No Filter" for 955 project.[lwk]
		//const std::string AlarmFilterService::FILTER_DEFAULT("Default Filter");
		const std::string FILTER_DEFAULT = "No Filter";

		const std::string FILTER_TOTIME = "ToTime";
		const std::string FILTER_FROMTIME = "FromTime";
		const std::string FILTER_SUBSYSTEM = "Subsystem";
		const std::string FILTER_SUBSYSTEMTYPE = "SubsystemType";
		const std::string FILTER_SYSTEM = "System";
		const std::string FILTER_LOCATION = "Location";
		const std::string FILTER_OPERATOR = "Operator";
		const std::string FILTER_EQUIPMENT_ID = "Equipment ID";
		const std::string FILTER_DESCRIPTION = "Description";
		const std::string FILTER_SEVERITY = "Severity";
		const std::string FILTER_ALARMTYPE = "Alarm Type";

		const std::string FILTER_ACKSTATE = "AckState";
		const std::string FILTER_STATE = "State";
		const std::string FILTER_OPEN = "Open";
		const std::string FILTER_CLOSE = "Close";
		const std::string FILTER_ACKED = "Acknowledged";
		const std::string FILTER_UNACKED = "Unacknowledged";
		const std::string FILTER_ALL = "All";

		const std::string FILTER_OMTYPE = "OMType";
		const std::string FILTER_OMTYPE_OPERATION = "OMType Operation";
		const std::string FILTER_OMTYPE_MAINTENANCE = "OMType Maintenance";
		const std::string FILTER_OMTYPE_BOTH = "OMType Both";
		const std::string FILTER_OMTYPE_ALL = "OMType All";

		enum FilterIndex
		{
			INDEX_DEFAULT,
			INDEX_USER_DEFINED,
			INDEX_LIST,
		};
	}
}

#endif