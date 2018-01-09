//
// The source code in this file is the property of
// Ripple Systems and is not for redistribution
// in any form.
//
// Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/src/PlanManager.cpp $
// @author:  Bart Golab
// @version: $Revision: #2 $
//
// Last modification: $DateTime: 2016/10/18 15:45:45 $
// Last modified by:  $Author: huirong.luo $
//
// <description>

#include "StdAfx.h"
#include "core/utilities/src/RunParams.h"
#include "PlanManager.h"
#include "PlanAgentUpdateProcessor.h"
#include "TreeNodeFactory.h"
#include "OperatorRights.h"
#include "PlanAgentAccess.h"
#include "FloatingWindowThreadRegister.h"
#include "PlanManagerGUI.h"
#include "MainFrame.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App;
using TA_Base_Bus::TransActiveWinApp;


/////////////////////////////////////////////////////////////////////////////
// The one and only CPlanManagerApp object

CPlanManagerApp planManagerApp;


/////////////////////////////////////////////////////////////////////////////
// CPlanManagerApp

BEGIN_MESSAGE_MAP(CPlanManagerApp, TransActiveWinApp)
    // {{AFX_MSG_MAP(CPlanManagerApp)
        // NOTE - the ClassWizard will add and remove mapping macros here.
        // DO NOT EDIT what you see in these blocks of generated code!
    // }}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CPlanManagerApp construction/destruction

CPlanManagerApp::CPlanManagerApp() : TransActiveWinApp(TA_Base_Bus::MVT_MDI_FRAME, true)
{
    FUNCTION_ENTRY( "CPlanManagerApp" );

    // TODO: add construction code here,
    // Place all significant initialization in InitInstance

    FUNCTION_EXIT;
}


CPlanManagerApp::~CPlanManagerApp()
{
    FUNCTION_ENTRY( "~CPlanManagerApp" );
    FUNCTION_EXIT;
}


/////////////////////////////////////////////////////////////////////////////
// CPlanManagerApp initialization

BOOL CPlanManagerApp::InitInstance()
{
    FUNCTION_ENTRY( "InitInstance" );

    // Standard initialization
    AfxInitRichEdit();

    FUNCTION_EXIT;
    return TransActiveWinApp::InitInstance();
}


int CPlanManagerApp::ExitInstance()
{
    FUNCTION_ENTRY( "ExitInstance" );

    FloatingWindowThreadRegister::destroyInstance();
    TreeNodeFactory::destroyInstance();
    PlanAgentAccess::destroyInstance();
    PlanAgentUpdateProcessor::destroyInstance();
    OperatorRights::destroyInstance();

    FUNCTION_EXIT;
    return TransActiveWinApp::ExitInstance();
}



TA_Base_Bus::IApplicationController * TA_Base_App::CPlanManagerApp::createApplicationController()
{
    TA_Base_Bus::IApplicationController * pController = new TA_Base_App::PlanManagerGUI(this, "Plan Manager");
    return pController;
}

CWnd* TA_Base_App::CPlanManagerApp::createMainView( UINT & dlgID )
{
    CMainFrame * pFrame = new CMainFrame();

    if (!pFrame)
        return NULL;
    m_pMainWnd = pFrame;
    // create and load the frame with its resources
    pFrame->LoadFrame(IDR_MAINFRAME,
        WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
        NULL);
    return pFrame;
}
