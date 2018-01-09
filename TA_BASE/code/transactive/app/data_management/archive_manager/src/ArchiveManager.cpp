/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/data_management/archive_manager/src/ArchiveManager.cpp $
* @author:  Alan Brims
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2016/10/18 15:45:45 $
* Last modified by:  $Author: huirong.luo $
*
* User interface for Archive Operations
*
*/

#include "app/data_management/archive_manager/src/stdafx.h"
#include "app/data_management/archive_manager/src/ArchiveManager.h"
#include "app/data_management/archive_manager/src/ArchiveManagerDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using TA_Base_Bus::TransActiveWinApp;

namespace TA_Base_App
{

	/////////////////////////////////////////////////////////////////////////////
	// CArchiveManagerApp

	BEGIN_MESSAGE_MAP(CArchiveManagerApp, TransActiveWinApp)
		//{{AFX_MSG_MAP(CArchiveManagerApp)
		// NOTE - the ClassWizard will add and remove mapping macros here.
		//    DO NOT EDIT what you see in these blocks of generated code!
		//}}AFX_MSG
	END_MESSAGE_MAP()

	/////////////////////////////////////////////////////////////////////////////
	// CArchiveManagerApp construction

	CArchiveManagerApp::CArchiveManagerApp() : TransActiveWinApp(TA_Base_Bus::MVT_DIALOG, false)
	{
		// Place all significant initialization in InitInstance
	}

	/////////////////////////////////////////////////////////////////////////////
	// The one and only CArchiveManagerApp object

	CArchiveManagerApp theApp;


	TA_Base_Bus::IApplicationController * CArchiveManagerApp::createApplicationController()
	{
		TA_Base_Bus::IApplicationController * pController =  new TA_Base_App::ArchiveManagerAppController(this, "Archive Manager");
		return pController;
	}

	CWnd* CArchiveManagerApp::createMainView( UINT & dlgID )
	{
		TALoadLang(APPLICATION_ID_ArchiveManager);
		//Construct the dialog
		CArchiveManagerDlg *pDlg = new CArchiveManagerDlg(this);
		dlgID = CArchiveManagerDlg::IDD;
		return pDlg;
	}

	void CArchiveManagerApp::doCommand( const char * commandName, const char * params )
	{


	}

	void CArchiveManagerApp::prepareAppRun()
	{
		CWaitCursor cur;
		//ITransActiveView* abstractview = getMainView();
		CArchiveManagerDlg * pDlg = dynamic_cast<CArchiveManagerDlg * > (getMainView());
		TA_ASSERT(pDlg != NULL, "the application window is NULL!");

		pDlg->LockWindowUpdate();	
		//pDlg->init(); TODO
		pDlg->UnlockWindowUpdate();
		pDlg->UpdateWindow();
	}


} // TA_Base_App
