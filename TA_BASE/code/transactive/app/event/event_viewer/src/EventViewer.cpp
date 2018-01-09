/**
* The source code in this file is the property of 
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/event/event_viewer/src/EventViewer.cpp $
* @author:  Nick Jardine
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2016/10/18 15:45:45 $
* Last modified by:  $Author: huirong.luo $
* 
* EventViewer is the TransactiveWinApp derived main application class. This is the class that bootstraps the application.
*/

#include "app/event/event_viewer/src/stdafx.h"
#include "app/event/event_viewer/src/EventViewer.h"
#include "app/event/event_viewer/src/EventViewerDlg.h"



#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

//not using
//const std::string EventViewerApp::RPARAM_ALARMID = "AlarmId";

/////////////////////////////////////////////////////////////////////////////
// EventViewerApp

BEGIN_MESSAGE_MAP(EventViewerApp, TA_Base_Bus::TransActiveWinApp)
	//{{AFX_MSG_MAP(EventViewerApp)
	// NOTE - the ClassWizard will add and remove mapping macros here.
	//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// EventViewerApp construction

EventViewerApp theApp;

EventViewerApp::EventViewerApp()
: TransActiveWinApp(TA_Base_Bus::MVT_DIALOG, false)
{
	// TODO: add construction code here,
	// Place all significant initialization in InitInstance
}

TA_Base_Bus::IApplicationController * EventViewerApp::createApplicationController()
{
	TA_Base_Bus::IApplicationController * pController =  new TA_Base_App::EventViewerAppController(this, "Event Viewer");
	return pController;
}

CWnd* EventViewerApp::createMainView( UINT & dlgID )
{
	TALoadLang(APPLICATION_ID_EventViewer);
	//Construct the dialog
	EventViewerDlg* dlg = new EventViewerDlg(this);
	dlgID = EventViewerDlg::IDD;
	return dlg;
}

void EventViewerApp::doCommand( const char * commandName, const char * params )
{


}

void EventViewerApp::prepareAppRun()
{
	CWaitCursor cur;
	//ITransActiveView* abstractview = getMainView();
	EventViewerDlg * pDlg = dynamic_cast<EventViewerDlg * > (getMainView());
	TA_ASSERT(pDlg != NULL, "the application window is NULL!");

	pDlg->LockWindowUpdate();	
	pDlg->init();
	pDlg->UnlockWindowUpdate();
	pDlg->UpdateWindow();
}