/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/scheduling/scheduling_manager/src/SchedulingManager.cpp $
  * @author:  
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * This class inherits from TransActiveWinApp.
  */


#include "app/scheduling/scheduling_manager/src/stdafx.h"
#include "app/scheduling/scheduling_manager/src/SchedulingManager.h"
#include "app/scheduling/scheduling_manager/src/MainFrm.h"
#include "app/scheduling/scheduling_manager/src/SchedulingManagerView.h"
#include "app/scheduling/scheduling_manager/plugin/model/src/SchedulingManagerGUI.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using TA_Base_Bus::TransActiveWinApp;

namespace TA_Base_App
{
/////////////////////////////////////////////////////////////////////////////
// CSchedulingManagerApp

BEGIN_MESSAGE_MAP(CSchedulingManagerApp, TransActiveWinApp)
	//{{AFX_MSG_MAP(CSchedulingManagerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
	//}}AFX_MSG_MAP
	// Standard file based document commands
	ON_COMMAND(ID_FILE_NEW, CWinApp::OnFileNew)
	ON_COMMAND(ID_FILE_OPEN, CWinApp::OnFileOpen)
END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CSchedulingManagerApp construction

    CSchedulingManagerApp::CSchedulingManagerApp()
    : TransActiveWinApp( TA_Base_Bus::MVT_SDI_FRAME, true)
    {
	    // TODO: add construction code here,
	    // Place all significant initialization in InitInstance
    }

    /////////////////////////////////////////////////////////////////////////////
    // The one and only CSchedulingManagerApp object

    CSchedulingManagerApp theApp;

    /////////////////////////////////////////////////////////////////////////////
    // CSchedulingManagerApp initialization

    BOOL CSchedulingManagerApp::InitInstance()
    {
	    LoadStdProfileSettings();  // Load standard INI file options (including MRU)

        return TransActiveWinApp::InitInstance();
    }

	TA_Base_Bus::IGUIApplicationController * CSchedulingManagerApp::createApplicationController()
	{
		TA_Base_Bus::IGUIApplicationController * pController = new TA_Base_App::SchedulingManagerGUI(this, "Scheduling Manager");
		return pController;
	}

	CWnd* CSchedulingManagerApp::createMainView( UINT & dlgID )
	{
		TALoadLang(APPLICATION_ID_SchedulingManager);
		CMainFrame * pFrame = new CMainFrame(this);

		if (!pFrame)
			return NULL;
		m_pMainWnd = pFrame;
		// create and load the frame with its resources
		pFrame->LoadFrame(IDR_MAINFRAME,
			WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
			NULL);
		return pFrame;
	}



    /////////////////////////////////////////////////////////////////////////////
    // CSchedulingManagerApp message handlers

};
