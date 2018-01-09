#if !defined(AFX_DUTYMANAGER_H__5F04215F_509A_4311_82C5_842F9C2A0157__INCLUDED_)
#define AFX_DUTYMANAGER_H__5F04215F_509A_4311_82C5_842F9C2A0157__INCLUDED_

/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/DutyManager.h $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * Consolidates all the required includes in a single header.
  *
  */

#ifdef __WIN32__
	#pragma warning(disable: 4250 4786)
#endif

#include "app/security/DutyManager/pmodel/src/CommonDefine.h"

// System includes
#include <sstream>
//#include <string>
//#include <vector>
//#include <algorithm>

// Core includes
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/RunParams.h"
#include "core/exceptions/src/TransactiveException.h"
#include "core/exceptions/src/GenericGUIException.h"
#include "core/exceptions/src/UserSettingsException.h"
#include "core/data_access_interface/entity_access/src/DutyManagerEntityData.h"

// Bus includes
#include "bus/application_types/src/apptypes.h"
//#include "bus/generic_gui/src/AbstractDialogGUI.h"
//#include "bus/generic_gui/src/TransActiveDialog.h"
//#include "bus/generic_gui/src/TransActiveWinApp.h"
//#include "bus/generic_gui/src/TransActiveMessage.h"
//#include "bus/generic_gui/src/HelpLauncher.h"
//#include "bus/user_settings/src/SettingsMgr.h"
#include "bus/mfc_extensions/src/mfc_extensions.h"
#include "bus/security/duty_monitor_framework/src/DutyMonitorFramework.h"
#include "bus/security/duty_agent/idl/src/DutyAgentTypeCorbaDef.h"



// Rights Manager includes
#include "app/security/DutyManager/src/Resource.h"
#include "app/security/DutyManager/src/DutyManagerConstants.h"
#include "app/security/DutyManager/src/MessageBox.h"
#include "app/security/DutyManager/src/GridAssistant.h"
#include "app/security/DutyManager/src/DutyManagerPropertySheet.h"
#include "app/security/DutyManager/src/RegionDutyPropertyPage.h"
#include "app/security/DutyManager/src/SubsystemDutyPropertyPage.h"
#include "app/security/DutyManager/src/RegionDutyTransferDlg.h"
#include "app/security/DutyManager/src/SubsystemDutyTransferDlg.h"
#include "app/security/DutyManager/src/RegionSelectionDlg.h"
#include "app/security/DutyManager/src/DutyManagerDlg.h"
//#include "app/security/DutyManager/src/DutyManagerGUI.h"
#include "app/security/DutyManager/src/DutyManagerApp.h"
//#include "app/security/DutyManager/src/DutyManagerBridge.h"
#include "app/security/DutyManager/src/OtherSesSubDutyPropertyPage.h"
#include "app/security/DutyManager/src/SelectSesSubDutyDlg.h"


#include "bus/generic_gui_view/src/TransActiveMessage.h"

#include "bus/unicode_common_lib/src/TAUnicodeString.h"
//
//	The custom Windows messages passed within the Rights Manager
//


namespace
{
	const int OCC_LOCATION = 1;


	const char* TCS_TRAIN_COMMS_STN_SUBSYSTEM	= "TCS - Train Comms (Stn)";
	const char* PROFILE_GROUP_TCO				= "TCO";
	const char* TCS_SPECIAL_PROJECT_NAME		= "4669"; 


	const int SES_OPERATOR_COLUMN(0);
	const int SES_PROFILE_COLUMN(1);
	const int SES_WORKSTATION_COLUMN(2);

	const int SES_OPERATOR_COLUMN_WIDTH(160);
	const int SES_PROFILE_COLUMN_WIDTH(110);
	const int SES_WORKSTATION_COLUMN_WIDTH(80);

	const CString SES_OPERATOR_COLUMN_HEADING= TALoadString( STRING_ID_DUTY_MANAGER_0016 ).c_str();
	const CString SES_PROFILE_COLUMN_HEADING= TALoadString( STRING_ID_DUTY_MANAGER_0017 ).c_str();
	const CString SES_WORKSTATION_COLUMN_HEADING=TALoadString( STRING_ID_DUTY_MANAGER_0052 ).c_str();
	const CString STRING_NULL("");

	const CString MsgBoxErrCStr			= TALoadString( STRING_ID_DUTY_MANAGER_0000 ).c_str();//"During initialise Subsystem DutyGrid, no session persisted in DB";
	const CString MsgBoxErrCStr2		= TALoadString( STRING_ID_DUTY_MANAGER_0001 ).c_str();//"Still failed after retry"

	const CString REGION_COLUMN_HEADING		= TALoadString( STRING_ID_DUTY_MANAGER_0014 ).c_str();
	const CString REGION_COLUMN_HEADING_NLL("");

	const CString SUBSYSTEM_COLUMN_HEADING	= TALoadString( STRING_ID_DUTY_MANAGER_0018 ).c_str();
	const CString OPERATOR_COLUMN_HEADING		= TALoadString( STRING_ID_DUTY_MANAGER_0016 ).c_str();
	const CString PROFILE_COLUMN_HEADING		= TALoadString( STRING_ID_DUTY_MANAGER_0017 ).c_str();


	const CString PAGE_CAPTION("Locations");
	const CString All_LocationsStr("All Locations");//"During initialise Subsystem DutyGrid, no session persisted in DB";

	const int ALLOCATED_SUBSYSTEM(1);
	const int UNALLOCATED_SUBSYSTEM(~ALLOCATED_SUBSYSTEM);

	const int DEFAULT_ROW_HEIGHT(20);
	const int DEFAULT_COLUMN_WIDTH(70);

	const COLORREF NORMAL_DUTY_COLOUR(CLR_DEFAULT);							// white
	const COLORREF DELEGATED_DUTY_COLOUR(CLR_DEFAULT);						// white
	const COLORREF DEGRADED_DUTY_COLOUR(CLR_DEFAULT);						// white
	const COLORREF UNALLOCATED_SUBSYSTEM_COLOUR( RGB( 225, 225, 225 ) );	// light grey

	// rename
	//const int SUBSYSTEM_COLUMN(1);

}

namespace TA_Base_App
{
    static const enum
    {
		WM_SINGAL_NULL = (WM_USER + 1001),
        //WM_USER_RECEIVE_SESSION_UPDATE= ,  // wParam = TA_Base_Bus::SessionUpdate*, lParam = unused
        //WM_USER_RECEIVE_RGN_DUTY_UPDATE,                   // wParam = TA_Base_Bus::RegionDutyUpdate*, lParam = unused
        //WM_USER_RECEIVE_SUB_DUTY_UPDATE,                   // wParam = TA_Base_Bus::SubsystemDutyUpdate*, lParam = unused
		WM_OTHER_SES_SUB_DUTY_UPDATE,
		WM_RGN_DUTY_UPDATE,
		WM_SUB_DUTY_UPDATE,
		WM_SESSEION_UPDATE,
		WM_USER_RECEIVE_DUTY_AGENT_RECOVER_UPDATE,      // wParam = unused, lParam = locatiionKey
		WM_POPULATE_DATA
		
    };
}


CString g_string2CString(const char * szSource );


#endif // !defined(AFX_DUTYMANAGER_H__5F04215F_509A_4311_82C5_842F9C2A0157__INCLUDED_)