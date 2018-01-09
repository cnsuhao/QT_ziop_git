/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/NotesDialog.h $
  * @author:  Andy Parker
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  * 
  * This class manages the dialog which displays the maintenance tab shown
  * by the inspector panel.
  */

#ifndef NOTESDIALOG_H
#define NOTESDIALOG_H

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/scada/inspector_panel/src/resource.h"
#include "app/scada/inspector_panel/src/ITabDialog.h"

#include "bus/scada/common_library/src/AuditMessageUtility.h"
#include "../pmodel/src/notesdialogmodel.h"
#include "bus\generic_gui_view\src\TransActiveDialog.h"

#define	MESSAGE_REFRESH_ALL_DATA	                (WM_USER + 30)
#define	MESSAGE_UPDATE_DATA							(WM_USER + 31)
#define	MESSAGE_UPDATE_PERMISSION					(WM_USER + 32)

namespace TA_Base_App
{
	class CEquipment;

	/////////////////////////////////////////////////////////////////////////////
	// CNotesDialog dialog

	class CNotesDialog : public AbstractTransActiveDialog, public ITabDialog
	{
	
	public:

	// Construction
		CNotesDialog(TA_Base_Bus::ITransActiveWinApp*app,CWnd* pParent=NULL /*=NULL*/);  // standard constructor

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

	// Dialog Data
		//{{AFX_DATA(CNotesDialog)
		enum { IDD = IDD_NOTES };
		CEdit	m_notesEdit;
		//}}AFX_DATA


	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CNotesDialog)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		//}}AFX_VIRTUAL
		virtual void setupPModel();
		virtual std::string getMyPModelName();
	// Implementation
	protected:

		// Generated message map functions
		//{{AFX_MSG(CNotesDialog)
		afx_msg void OnRemove();
		afx_msg void OnUpdate();
		afx_msg void OnChangeEditnotes();
		afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
		afx_msg LRESULT onRefreshAllData(WPARAM wparam, LPARAM lparam);
	//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

		void DisplayEquipmentNotes();
		bool updateNotes();
		void updateButtons();
		void submitAuditMessage(std::string updateRemove, 
								std::string dataNode, unsigned long entityKey);
		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args);


	protected:
		NotesDialogModel*				m_realPModel;
		//CEquipment*						m_equipment;
		bool							m_notesChanged;
		bool							m_notesAccessible;
		bool							m_accessAllowed;
		int								m_originalHeight;
	};

	//{{AFX_INSERT_LOCATION}}
	// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}
#endif // #ifndef NOTESDIALOG_H
