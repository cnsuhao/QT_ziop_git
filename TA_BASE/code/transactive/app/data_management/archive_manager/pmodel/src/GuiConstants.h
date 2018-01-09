/**
* The source code in this file is the property of 
* Ripple Systems and is not for redistribution
* in any form.
*
* Source: $File:  $
* @author 
* @version $Revision: # $
*
* Last modification: $DateTime:  $
* Last modified by: 
* 
* 
*
*/
#if !defined(GUI_CONSTANTITEMS_0123456789_1234567__INCLUDED_)
#define GUI_CONSTANTITEMS_0123456789_1234567__INCLUDED_

#include <string>

#define WM_WRITE_COMPLETE           (WM_USER+1)
#define WM_CLOSE_DLG                (WM_USER+2)
#define WM_COMPLETE_DLG             (WM_USER+3)
#define WM_SERVER_STATUS_CHANGE     (WM_USER+4)
#define WM_SESSIONID_CHANGE         (WM_USER+5)
#define WM_DISABLE_DLG              (WM_USER+6)

namespace
{
	//
    // This will be displayed on the dialog with the number of CDs filled in.
    //

    //const std::string NUM_CDS_MSG( " CD(s) will be required to complete the transfer to CD.\nThe following files will be transferred:" );
	//const std::string NUM_DVDS_MSG( " DVD(s) will be required to complete the transfer.\nThe following files will be transferred:" );
    //const std::string INSERT_CD_MSG( "Please insert a blank CD and press OK to continue." );
   // const std::string INSERT_DVD_MSG( "Please insert a blank DVD and press OK to continue." );


	const std::string ARTIST = "Transactive";
	const std::string CaptionWriteComplete = "Write Complete";
	const std::string to_str = " to ";
	const std::string RetrievingData_DateRange_str = "Archiving data from ";
}
namespace TA_Base_App
{
	//PMODEL related String -----------------------------------------------------------------------------------
	const std::string  ARCHIVE_MANAGER_DLG_MODEL =  "ArchiveManagerDlgPModel";
	const std::string  RESTORE_ARCHIVE_DLG_PMODEL =  "RestoreArchiveDlgPModel";
	const std::string  WRITE_ARCHIVE_DLG_PMODEL =  "WriteArchiveDlgPModel";
	const std::string  BURN_MEDIA_DLG_PMODEL =  "BurnMediaDlgPModel";

	////////////
	//const std::string GET_RESOURCE_STRING_VALUE = "GetResourceStringValue_AR";
	const std::string THREAD_FUNCTION_INVOKE = "ThreadFunctionInvoke_AR";
	const std::string POST_MESSAGE_INVOKE = "PostMessageInvoke_AR";

	///Restore Archive Dlg 
	const std::string SHOW_USER_MESSAGE_NOTIFY = "ShowUserMessageNotify_AR";
	//const std::string SET_ITEM_STATUS_IN_LISTCTRL = "SetStatusInRestoreListCtrl_AR";
	//const std::string ENABLE_CLOSE_BUTTON_RESTORE = "EnableCloseButtonInRestore_AR";


	////RetrievingDate Dlg
	const std::string SET_RETRIEVING_DATA_DATE_CTRL = "setRetriveingDataDateCtrl_AR";
	const std::string POST_MESSAGE_TO_RETRIEVINGDATADLG = "PostMessageInvoke_RetrievingData_AR";
	const std::string SHOW_USER_MESSAGE_RETRIEVINGDATADLG = "ShowUserMessageNotify_RetrievingData_AR";


	///WriteArchive Dlg
	//const std::string SET_NUMCD_CD_STR = "setWindowTextForCD_DVD_AR";
	const std::string DELETE_WRITE_LIST_CTRL = "deleteWriteListCtrl_AR";


	//BurnMedia Dlg
	const std::string SET_STAGE_STR = "setWindowTextForStageString_AR";
	const std::string OPEN_FILEDIALOG_IMAGE_SEL = "openFileDialogforImageSelection_AR";
	const std::string APPEND_STRING_LOG_DISPLAY = "appendStringLogDisplay_AR";	
	const std::string SHOW_CLOSE_BUTTON_BURNMEDIA_DLG = "showCloseButton_AR";	
	const std::string DISABLECANCELBTN_BURNDLG = "DISABLECANCELBTN_BURNDLG";	
	const std::string BURN_FINISHED_STATUS = "BURN_FINISHED_STATUS";	
	const std::string SET_PROGRESS_PERCENT_BURN_DLG = "setProgressState_AR";	
};

#endif //GUI_CONSTANTITEMS_0123456789_1234567__INCLUDED_

