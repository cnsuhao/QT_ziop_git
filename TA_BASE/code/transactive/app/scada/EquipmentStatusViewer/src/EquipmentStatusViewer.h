/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/EquipmentStatusViewer/src/EquipmentStatusViewer.h $
 * @author:  Rod Rolirad
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/09/03 14:11:12 $
 * Last modified by:  $Author: CM $
 * 
 * Main Application class
 *
 */

#if !defined(AFX_EQUIPMENTSTATUSVIEWER_H__13EF9430_909E_4319_9238_94999F357E69__INCLUDED_)
#define AFX_EQUIPMENTSTATUSVIEWER_H__13EF9430_909E_4319_9238_94999F357E69__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#ifndef __AFXWIN_H__
	#error include 'stdafx.h' before including this file for PCH
#endif

#include "resource.h"		
#include "../pmodel/src/EquipmentStatusViewerAppController.h"
#include "bus/generic_gui_view/src/TransActiveWinApp.h"

namespace TA_Base_App
{

	class CEquipmentStatusViewerApp : public TA_Base_Bus::TransActiveWinApp
	{
	public:
		CEquipmentStatusViewerApp();
		~CEquipmentStatusViewerApp(){};
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CEquipmentStatusViewerApp)
	public:
		virtual BOOL InitInstance();
		//}}AFX_VIRTUAL


		//{{AFX_MSG(CEquipmentStatusViewerApp)
			// NOTE - the ClassWizard will add and remove member functions here.
			//    DO NOT EDIT what you see in these blocks of generated code !
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()
	
	protected:
		/** create Application Controller
         * 
         *
         *  @return IApplicationController *
         */
        IApplicationController * createApplicationController();

		/** create main View
		 * 
		 *
		 *  @param dlgID
		 *  @return CWnd*
		 */
		CWnd* createMainView(UINT & dlgID);
	};

} // TA_Base_App

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQUIPMENTSTATUSVIEWER_H__13EF9430_909E_4319_9238_94999F357E69__INCLUDED_)
