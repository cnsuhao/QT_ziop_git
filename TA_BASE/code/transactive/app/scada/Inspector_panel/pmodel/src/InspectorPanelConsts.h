
#ifndef INSPECTOR_PANEL_CONSTS_H__
#define INSPECTOR_PANEL_CONSTS_H__


#include "bus\generic_gui_pmod\src\CommandGateway.h"

namespace TA_Base_App
{
	#define EQUIPMENT_RPARAM		"ControlEntity"
	#define SCREEN_RPARAM			"ScreenName"
	#define VALUE_RPARAM			"ControlValue"

	#define APP_METHOD_SHOW_STATUS_MESSAGE		"ShowStatusMessage"

	#define	PMODEL_INSPECTOR_PANEL_DLG						"InspectorPanelDlgModel"
	#define	PMODEL_INFORMATION_LISTCTRL						"InformationListCtrlModel"
	#define	PMODEL_CONTROL_LISTCTRL							"ControlListCtrlModel"
	#define	PMODEL_NOTES_DLG								"NotesDlgModel"
	#define	PMODEL_TAG_DLG									"TagDlgModel"
	#define	PMODEL_INHIBIT_LISTCTRL							"InhibitListCtrlModel"
	#define	PMODEL_OVERRIDE_LISTCTRL						"OverrideListCtrlModel"
	#define	PMODEL_LIMITS_LISTCTRL							"LimitslListCtrlModel"
	#define	PMODEL_LIMIT_DETAIL_LISTCTRL					"LimitsDetaillListCtrlModel"
	#define	PMODEL_METER_LISTCTRL							"MeterlListCtrlModel"
	#define	PMODEL_STATUS_DIALOG							"StatusDialogModel"
	#define PMODEL_MAIN_TAB_CONTROL							"MainTabControl"
	#define PMODEL_DIRECT_CONTROL_DIALOG					"DirectControlDialogModel"

	#define DOMAIN_DATA_CHANGE_INIT_STATUS					"InitStatusChange"
	#define DOMAIN_DATA_CHANGE_ENTITY_DATA					"EntityDataChange"
	#define DOMAIN_DATA_CHANGE_PERTMITTION					"PertmittionChange"

	#define PMODEL_NOTIFY_REFRESH_ALL_DATA					"RefreshAllData"
	#define PMODEL_NOTIFY_UPDATE_DATA						"UpdateDataPointData"
	#define PMODEL_NOTIFY_PERTMISSION						"UpdatePermission"



	
	
	enum AccessControlActions	
	{	
		DATA_POINT_WRITE = 0,
		DATA_POINT_ALARM_INHIBIT = 1,
		DATA_POINT_ALARM_UNINHIBIT = 2,
		DATA_POINT_CONTROL_INHIBIT = 3,
		DATA_POINT_CONTROL_UNINHIBIT = 4,
		DATA_POINT_SCAN_UNINHIBIT = 5,
		DATA_POINT_SCAN_INHIBIT = 6,
		DATA_POINT_MMS_INHIBIT = 7,
		DATA_POINT_MMS_UNINHIBIT = 8,
		DATA_POINT_SET_FORCED_STATE = 9,
		DATA_POINT_RESET_FORCED_STATE = 10,
		DATA_POINT_METER_ADJUST = 11,
		DATA_NODE_TAG = 12,
		DATA_NODE_NOTES = 13,
		DATA_NODE_TAG_PTW = 14,
		IP_LIMITS_TAB_ACCESS = 15,
		ACCESS_CONTROL_ACTION_COUNT = 16 
	};

	enum ETabNumbers
	{
		InformationTab	= 0,
		ControlTab		= 1,
		NotesTab		= 2,
		TagTab			= 3,
		InhibitTab		= 4,
		OverrideTab		= 5,
		LimitsTab		= 6,
		MeterTab		= 7,
		StatusTab		= 8,
		NoTab			= 9
	};

	enum EDataPointStatusColor
	{
		Green,
		Blue,
		Red,
		Orange
	};

	
#define UserReplyControlConfirm  1

// 	enum EinitializeStatus
// 	{
// 		StartLoadDataNode = 0;
// 
// 	};
}

#endif