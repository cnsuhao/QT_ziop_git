/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/security/DutyManager/src/DutyManagerApp.cpp $
  * @author:  Bart Golab
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * This class provides the entry point into the application.
  * It instantiates DutyManagerGUI, but leaves it to the
  * GenericGUI to display.
  *
  */

#include "StdAfx.h"
#include "app/security/DutyManager/src/DutyManager.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;

//
CString g_string2CString(const char * szSource )
{
	TAUtfString cstr;
	cstr.fromMultiByteString(szSource);
	return cstr.str ();
}

/////////////////////////////////////////////////////////////////////////////
// The one and only DutyManagerApp object

bool g_populated = false;
DutyManagerApp dutyManagerApp;


/////////////////////////////////////////////////////////////////////////////
// DutyManagerApp

BEGIN_MESSAGE_MAP(DutyManagerApp, TA_Base_Bus::TransActiveWinApp)
	//{{AFX_MSG_MAP(DutyManagerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// DutyManagerApp construction/destruction

DutyManagerApp::DutyManagerApp() : 
TA_Base_Bus::TransActiveWinApp( TA_Base_Bus::MVT_DIALOG ,true)
    //TA_Base_Bus::TransActiveWinApp(new DutyManagerGUI, "Rights Manager") //---------------MVT_DIALOG or MVT_PROPERTY_SHEET MVT_DOCVIEW_FRAME
{
    FUNCTION_ENTRY("DutyManagerApp()");

    // Note: Place all significant initialization in InitInstance

    FUNCTION_EXIT;
}


DutyManagerApp::~DutyManagerApp()
{
    FUNCTION_ENTRY("~DutyManagerApp()");
    FUNCTION_EXIT;
}

TA_Base_Bus::IApplicationController * DutyManagerApp::createApplicationController()
{
	TA_Base_Bus::IApplicationController * pController = new TA_Base_App::DutyManagerAppController(this, DUTY_MANAGER.c_str());
	return pController;
}

CWnd* DutyManagerApp::createMainView( UINT & dlgID )
{
	TALoadLang( APPLICATION_ID_DutyManager );

	//Construct the dialog
	DutyManagerDlg* dlg = new DutyManagerDlg(this);
	dlgID = DutyManagerDlg::IDD;
	return dlg;
}

void DutyManagerApp::invoke(const char * methodName, const TA_Base_Bus::NameValueParams & params)
{
	//if (strcmp(methodName, APP_METHOD_LOGIN) == 0)
	//{
	//	doLogin();
	//}
	//if (strcmp(methodName, APP_METHOD_LOCATE_SYSTEM_CONTROLLER) == 0)
	//{
	//	locateSystemController();
	//}

}

void DutyManagerApp::requestUserReply( TA_Base_Bus::UIReplyEventArgsPtr uiEvent )
{
	//int typeCode = uiEvent->getTypeCode();
	//TA_Base_Bus::UINotificationDataContextPtr dataContext = uiEvent->getMyDataContext();
	//if (dataContext->getParamEncode() == TA_Base_Bus::PARAM_ENCODE_STR_1)
	//{
	//	int msgID = dataContext->getTransActiveMessageId();
	//	std::string processName = dataContext->getParamValue(0);

	//	TA_Base_Bus::TransActiveMessage userMsg;
	//	userMsg << processName;
	//	userMsg.showMsgBox(msgID);

	//}
}

DutyManagerAppController * DutyManagerApp::getDutyManagerController()
{
	DutyManagerAppController * pController = dynamic_cast<DutyManagerAppController*> (m_appController);
	TA_ASSERT(pController != NULL, "DutyManager application controller should be valid");
	return pController;
}
