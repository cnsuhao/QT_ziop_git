/**
* The source code in this file is the property of 
* Ripple Systems and is not for redistribution
* in any form.
*
* Source: $File: //depot/3001_TIP/TA_BASE/transactive/app/event/event_viewer/src/ConstantItems.h $
* @author huirong.luo
* @version $Revision: #1 $
*
* Last modification: $DateTime: 2010/09/15 8:54:40 $
* Last modified by: xiangmei.lu
* 
* EventViewerConstants is the namespace which includes const items used in this application.
*
*/
#if !defined(CONSTANTITEMS_0123456789_1234567__INCLUDED_)
#define CONSTANTITEMS_0123456789_1234567__INCLUDED_

#include <windows.h>

namespace TA_Base_App
{

	const std::string TMType_1 = "O";
	const std::string TMType_2 = "M";
	const std::string TMType_3 = "B";
	const std::string TMType_4 = "";

	const int TMType_O_INDEX = 0;
	const int TMType_M_INDEX = 1;
	const int TMType_B_INDEX = 2;


	//PMODEL related String -----------------------------------------------------------------------------------
	const std::string  EVENT_DISPLAY_PMODEL =  "EventDisplayModel";
	const std::string  EVENT_VIEWER_DLG_PMODEL =  "EventViewerDlgPModel";
	const std::string  COMBINED_EVENT_FILTER_PMODEL =  "CombinedEventFilterPModel";
	//Notification of EventListComponent
	const std::string INSERT_COLUMN_NOTIFY = "InsertEventListCtrlColumn";
	const std::string SET_BACK_COLOR_NOTIFY = "SetBackColorEventListCtrl";
	//Notification of the EventViewerDlg
	const std::string INIT_PAGER_CTRL = "InitPagerCtrl";
	const std::string SETUP_TOOLBAR = "SetupToolbar";
	const std::string SETUP_MENU = "SetupMenu";
	const std::string SETUP_STATUS_BAR = "SetupStatusBar";
	const std::string INIT_EVENT_LIST_COMPONENT = "InitEventList";
	const std::string UPDATE_PREVIOUS_TOOLBAR = "UpdatePrevTool";
	const std::string UPDATE_NEXT_TOOLBAR = "UpdateNextTool";
	const std::string UPDATE_FILTER_DLG = "UpdateFilterDlg";
	const std::string POPULATE_NAMED_FILTERS = "ReloadNamedFiltersCombo";
	const std::string SHOW_WINDOW_TEXT = "ShowWindowText";
	const std::string SETUP_ALLEVENTS_MENU = "SetupAllEventsMenu";
	const std::string POST_MESSAGE_TO_UI = "PostMessagetoUI";
	const std::string SHOW_USER_MESSAGE_NOTIFY = "ShowUserMessageNotify";
	const std::string INVOKE_READY_TO_CLOSE = "InvokeReadyToClose";
	//Notification of the CombinedEventfilterDlg
	const std::string CLEAR_SORT_MASK = "CallClearSortMask";
	const std::string SET_TIME_CONTROLS = "SetTimeControls";
	const std::string SET_WINDOW_TEXT = "SetControlText";
	const std::string SET_SEVERITY_COMBOX_BOX = "SetSeverityControl";
	const std::string GET_FILTER_DATA = "getFilterData";
	const std::string POPULATE_TMTYPE = "populateTMType";
	const std::string RESET_FILTER_TIME = "resetStartEndTimeDefined";
	//
	const std::string SYSTEM_EDIT = "systemEdit";
	const std::string SUB_SYSTEM_EDIT = "subSystemEdit";
	//Common Notification
	const std::string INVOKE_POSTMESSGAE_NOTIFICATION = "InvokePostMessage";
	//---------------------------------------------------------------------------------------------------
	

	// change from -1. As this value will be converted into "unsigned long", 
	// and when it saved and loaded from DB, it may cause some problem in some oracle client.
	// Using 0 can avoid these problems. 
	const int EVENT_ONLY_KEY = 0;  

	// A ListItem is used to map a PKEY to a description string.
	// They are used for data from the locations, subsystems, alarmTypes and operators tables.
	struct ListItem
	{
		ListItem(unsigned long k=0, std::string s="") : pkey(k), str(s) {}

		
		bool operator<(const ListItem& rhs)
		{
			return str.compare(rhs.str) < 0;
		}

		bool operator==(const ListItem& rhs)
		{
			return ( pkey == rhs.pkey );
		}

		unsigned long	pkey;
		std::string		str;
	};

	// Use a list so we can sort it
	typedef std::list<ListItem> ListItems;
	typedef std::vector<unsigned long> SelectedItems;
};

namespace EventViewerConstants {

	const std::string RPARAM_NO_VISIBILITY_RULE = "NoVisibilityRule";
	//const std::string REFRESH_TIME = "EventRefreshTime";  // the parameter name saved in table EntityParameter
	//const std::string MAX_EVENTS = "MaxEvents";

	const int ATS_EVENT_COUNT_DEFAULT = 20000; 
	const int ISCS_EVENT_COUNT_DEFAULT = 20000;
	/*const unsigned long ISCS_REFRESH_SEC = 5;    this can be retrieved from DB : table "entityparametervalue" */ 

	const int WORKER_REFRESH_TASK_PERIOD = 200; //"ms"; worker sleep time before check for new task 

	const bool ENABLE_ISCS_DEFAULT = true;
	const bool ENABLE_ATS_DEFAULT = true;

	// This is the key value for "event" in "severity" (means alarm severity)on the filter dialogue.
	//const int EVENT_ONLY_KEY = 0;

	const unsigned int TWENTY_FOUR_HOURS = 24*60*60;
	const std::string EMPTY_STRING("");

	const COLORREF COLOUR_BLACK( RGB(0, 0, 0) );
	const COLORREF COLOUR_BEIGE( RGB( 230, 227, 194 ) );
	const COLORREF COLOUR_DARKBEIGE( RGB( 219, 186, 119 ) );

	const unsigned long DEFAULT_EVENT_LEVEL( 5 );

	// message information which will be shown at the left bottom of the EventView dialogue
	const std::string STATUS_SEPERATE (" | ");

    // RunParam constants.
    const std::string RPARAM_FILTER_SUBSYSTEM( "FilterSubsystem" );
    const std::string RPARAM_FILTER_LOCATION( "FilterLocation" );
    const std::string RPARAM_FILTER_OPERATOR( "FilterOperator" );
    const std::string RPARAM_FILTER_EVENTTYPE( "FilterEventtype" );  
	const std::string RPARAM_FILTER_SYSTEM( "FilterSystem" ); 
    const std::string RPARAM_FILTER_ASSET( "FilterAsset" );
    const std::string RPARAM_FILTER_DESCRIPTION( "FilterDescription" );
    const std::string RPARAM_FILTER_FROM( "FilterFrom" );
    const std::string RPARAM_FILTER_TO( "FilterTo" );
    const std::string RPARAM_FILTER_SEVERITY( "FilterSeverity" );
    const std::string RPARAM_FILTER_ALARMTYPE( "FilterAlarmtype" );
    const std::string RPARAM_FILTER_ALARMID( "AlarmId" );
	const std::string RPARAM_FILTER_ALARMCHILD( "AlarmChild");
    const std::string RPARAM_FILTER_DEFAULT( "FilterDefault" );


}


namespace 
{
	// Define messages for communication with filter dialog
	#define WM_USER_FILTER_DLG_CLOSED					WM_USER + 0x100
	#define WM_USER_FILTER_DLG_APPLY					WM_USER + 0x101
	#define WM_USER_FILTER_DLG_CANCEL					WM_USER + 0x102
	#define WM_USER_DISPLAY_SHUTDOWN_BOX				WM_USER + 0x103
	#define WM_USER_CLOSE_MAIN_WINDOW					WM_USER + 0x104
	#define WM_USER_FILTER_DLG_NAMED_FILTER_CHANGE		WM_USER + 0x105
	#define WM_USER_UPDATE_EVENTFILTERS_RIGHTS			WM_USER + 0X106 //TD17143
	#define WM_USER_UPDATE_DATABASECONNECTION_STRING	WM_USER + 0X107	
	//zhiqiang++begin
	#define WM_USER_POPULATE_EVENT_LEVELS   WM_USER + 0x506 
	#define WM_USER_APPLY_RIGHTS            WM_USER + 0x507
	//zhiqiang++end


};

#endif  // CONSTANTITEMS_0123456789_1234567__INCLUDED_

