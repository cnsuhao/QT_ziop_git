// GenericGUIDialogDemo.h : main header file for the PROJECT_NAME application
//

#pragma once

#ifndef __AFXWIN_H__
#error "include 'stdafx.h' before including this file for PCH"
#endif

#include "resource.h"		// main symbols
#include "bus/generic_gui_view/src/TransActiveWinApp.h"

namespace TA_Base_App
{
	// CGenericDemoApp:
	// See GenericGUIDialogDemo.cpp for the implementation of this class
	//

	class CGenericDemoApp : public TA_Base_Bus::TransActiveWinApp
	{
	public:
        CGenericDemoApp();

		// Overrides
	public:
		virtual BOOL InitInstance();

		// Implementation

		DECLARE_MESSAGE_MAP()

        TA_Base_Bus::IGUIApplicationController * createApplicationController();

        CWnd* createMainView(UINT & dlgID);

	};

	extern CGenericDemoApp theApp;

}