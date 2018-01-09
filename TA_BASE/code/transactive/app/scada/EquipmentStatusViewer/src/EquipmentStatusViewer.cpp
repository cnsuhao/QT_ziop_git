/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/EquipmentStatusViewer/src/EquipmentStatusViewer.cpp $
 * @author:  Rod Rolirad
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/09/03 14:11:12 $
 * Last modified by:  $Author: CM $
 * 
 * Main Application class
 *
 */

#ifdef __WIN32__
#pragma warning(disable:4786 4284)
#pragma warning(disable:4503)  // warning C4503: '  ' : decorated name length exceeded, name was truncated
#endif

#include "stdafx.h"
#include "app/scada/EquipmentStatusViewer/src/EquipmentStatusViewer.h"
#include "app/scada/EquipmentStatusViewer/src/EquipmentStatusViewerDlg.h"
#include "bus/language/src/TALanguage.h"


using TA_Base_Bus::TransActiveWinApp;

namespace TA_Base_App
{
	BEGIN_MESSAGE_MAP(CEquipmentStatusViewerApp, TransActiveWinApp)
		//{{AFX_MSG_MAP(CEquipmentStatusViewerApp)
			// NOTE - the ClassWizard will add and remove mapping macros here.
			//    DO NOT EDIT what you see in these blocks of generated code!
		//}}AFX_MSG
	END_MESSAGE_MAP()


	CEquipmentStatusViewerApp::CEquipmentStatusViewerApp() : TA_Base_Bus::TransActiveWinApp(TA_Base_Bus::MVT_DIALOG, false)
	{
	}

	BOOL CEquipmentStatusViewerApp::InitInstance()
	{
		HWND hWnd = ::FindWindow(NULL, _T("Equipment Datapoint Status Viewer")); 
		if (hWnd) 		
			return FALSE; 
		else
			return TransActiveWinApp::InitInstance();			
	}

	TA_Base_Bus::IApplicationController * CEquipmentStatusViewerApp::createApplicationController()
	{
		TA_Base_Bus::IApplicationController* appControler = new EquipmentStatusAppController(this, "Equipment Datapoint Status Viewer");
		return appControler;
	}

	CWnd* CEquipmentStatusViewerApp::createMainView( UINT & dlgID )
	{
		TALoadLang( APPLICATION_ID_EquipmentStatusViewer);
		CEquipmentStatusViewerDlg* pDlg = new CEquipmentStatusViewerDlg(this);
		dlgID = CEquipmentStatusViewerDlg::IDD;
		return pDlg;
	}

	CEquipmentStatusViewerApp theApp;	
}
