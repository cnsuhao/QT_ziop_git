/**
* The source code in this file is the property of 
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/event/event_viewer/src/EventViewer.h $
* @author:  Nick Jardine
* @version: $Revision: #2 $
*
* Last modification: $DateTime: 2016/10/18 15:45:45 $
* Last modified by:  $Author: huirong.luo $
* 
* EventViewer is the TransactiveWinApp derived main application class. This is the class that bootstraps the application.
*/

#if !defined(AFX_EVENTVIEWER_H__80C1DC2F_2204_4CCF_AEEA_44E407EBB90C__INCLUDED_)
#define AFX_EVENTVIEWER_H__80C1DC2F_2204_4CCF_AEEA_44E407EBB90C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
#error include 'stdafx.h' before including this file for PCH
#endif

#include <string>

#include "app/event/event_viewer/src/resource.h"		// main symbols

//#include "bus/generic_gui/src/TransActiveWinApp.h"
#include "bus/generic_gui_view/src/TransActiveWinApp.h"
#include "../pmodel/src/EventViewerAppController.h"

/////////////////////////////////////////////////////////////////////////////
// EventViewerApp:
// See EventViewer.cpp for the implementation of this class
//

class EventViewerApp : public TA_Base_Bus::TransActiveWinApp
{
public:
	EventViewerApp();

	//static const std::string RPARAM_ALARMID;

	// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EventViewerApp)
	//}}AFX_VIRTUAL

	// Implementation

	//{{AFX_MSG(EventViewerApp)
	// NOTE - the ClassWizard will add and remove member functions here.
	//    DO NOT EDIT what you see in these blocks of generated code !
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	//virtual BOOL InitInstance();
	TA_Base_Bus::IApplicationController * createApplicationController();

	CWnd* createMainView(UINT & dlgID);

	void doCommand(const char * commandName, const char * params);

	 virtual void prepareAppRun();

private:

};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTVIEWER_H__80C1DC2F_2204_4CCF_AEEA_44E407EBB90C__INCLUDED_)
