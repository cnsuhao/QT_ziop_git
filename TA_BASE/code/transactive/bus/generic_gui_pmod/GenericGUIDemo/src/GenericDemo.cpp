// GenericGUIDialogDemo.cpp : Defines the class behaviors for the application.
//

#include "stdafx.h"
#include "GenericDemo.h"
#include "GenericDemoDlg.h"
#include "./pmodel/src/GenericDemoAppController.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

namespace TA_Base_App
{

    const char * APPLICATION_NAME = "GenericGUI Demo";

	// CGenericDemoApp

	BEGIN_MESSAGE_MAP(CGenericDemoApp, TA_Base_Bus::TransActiveWinApp)
		ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
	END_MESSAGE_MAP()



	// CGenericDemoApp construction

    CGenericDemoApp::CGenericDemoApp()
    : TA_Base_Bus::TransActiveWinApp( TA_Base_Bus::MVT_DIALOG, false )
	{
		// TODO: add construction code here,
		// Place all significant initialization in InitInstance
	}


	// The one and only CGenericDemoApp object

	CGenericDemoApp theApp;


	// CGenericDemoApp initialization

	BOOL CGenericDemoApp::InitInstance()
	{
		return TransActiveWinApp::InitInstance();
	}

    TA_Base_Bus::IGUIApplicationController * CGenericDemoApp::createApplicationController()
    {
        TA_Base_Bus::IGUIApplicationController *  pController = new GenericDemoAppController(this, APPLICATION_NAME);
        return pController;
    }

    CWnd* CGenericDemoApp::createMainView(UINT & dlgID)
    {
        CWnd * wnd = new CGenericDemoDlg(this);
		dlgID = CGenericDemoDlg::IDD;
        return wnd;
    }

}