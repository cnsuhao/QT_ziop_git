/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/StatusDialog.h $
  * @author:  Darren Sampson
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This class manages the dialog which displays the status tab shown
  * by the inspector panel.
  */

#ifndef STATUSDIALOG_H
#define STATUSDIALOG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/scada/inspector_panel/src/resource.h"
#include "app/scada/inspector_panel/src/ITabDialog.h"

#include "bus/scada/common_library/src/CommonDefs.h"
#include "bus\generic_gui_view\src\TransActiveDialog.h"
#include "../pmodel/src/statusdialogmodel.h"

namespace TA_Base_App
{
	class CEquipment;

	/////////////////////////////////////////////////////////////////////////////
	// CStatusDialog dialog

	class CStatusDialog : public AbstractTransActiveDialog, public ITabDialog
	{
	
	public:
		// Construction
		CStatusDialog(TA_Base_Bus::ITransActiveWinApp*app, CWnd* pParent = NULL);   // standard constructor

		//
		// ITabDialog Implementation
		//
		virtual void refreshAllData();
		virtual void updateDataPoint(unsigned long key, TA_Base_Bus::ScadaEntityUpdateType updateType);
		virtual void updateDataNode(TA_Base_Bus::ScadaEntityUpdateType updateType);
		virtual bool isUnsavedChangeOnTab();
		virtual bool isOkToChangeTab();
		virtual void getWindowRect(CRect* rect);
		virtual unsigned int getMinimumHeight();
		virtual void resize(unsigned int height);
		virtual void updatePermissions(/*TA_Base_Bus::RightsLibrary* rightsMgr*/);
		virtual bool isAnyDataPointsDisplayedInTab() const;
		virtual void resetList();
		
		virtual void setupPModel();
		virtual std::string getMyPModelName();
	// Dialog Data
		//{{AFX_DATA(CStatusDialog)
	enum { IDD = IDD_STATUS };
	CEdit	m_statusInfo;
	//}}AFX_DATA


	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CStatusDialog)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		//}}AFX_VIRTUAL

	// Implementation
	protected:
		// Generated message map functions
		//{{AFX_MSG(CStatusDialog)
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

		//CEquipment*			m_equipment;
		StatusDialogModel*		m_realPModel;
		bool					m_accessAllowed;
	};

	//{{AFX_INSERT_LOCATION}}
	// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // #ifndef STATUSDIALOG_H
