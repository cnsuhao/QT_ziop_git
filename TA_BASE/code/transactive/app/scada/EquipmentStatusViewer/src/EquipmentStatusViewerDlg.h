/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/EquipmentStatusViewer/src/EquipmentStatusViewerDlg.h $
 * @author:  Rod Rolirad
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2013/09/03 14:11:12 $
 * Last modified by:  $Author: CM $
 * 
 * Dialog class of the main dialog responsible for displaying the equipment status list,
 * location, report names, toolbar, status bar etc
 *
 */

#if !defined(AFX_EQUIPMENTSTATUSVIEWERDLG_H__B1FC56F1_91F3_4680_9193_F5B9A34FE350__INCLUDED_)
#define AFX_EQUIPMENTSTATUSVIEWERDLG_H__B1FC56F1_91F3_4680_9193_F5B9A34FE350__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <vector>
#include "boost/shared_ptr.hpp"
#include "app/scada/EquipmentStatusViewer/src/StdAfx.h"
#include "app/scada/EquipmentStatusViewer/src/EquipmentStatusViewer.h"
#include "app/scada/EquipmentStatusViewer/src/dlgbars.h"
#include "app/scada/EquipmentStatusViewer/src/EquipmentStatusListCtrl.h"
#include "app/scada/EquipmentStatusViewer/pmodel/src/EquipmentStatusViewerDlgModel.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"

namespace TA_Base_App
{
	class CEquipmentStatusViewerDlg : public TA_Base_Bus::AbstractTransActiveDialog
	{

	public:
	
		CEquipmentStatusViewerDlg(TA_Base_Bus::ITransActiveWinApp * pWinApp);

		virtual ~CEquipmentStatusViewerDlg();

		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL);

		virtual void requestUserReply(TA_Base_Bus::UIReplyEventArgsPtr uiEvent);

		virtual std::string  getMyPModelName ();

		virtual void setupPModel();

		virtual void initUIControl();

		//{{AFX_DATA(CEquipmentStatusViewerDlg)
	    enum { IDD = IDD_ESV_DIALOG };
	    
	    //}}AFX_DATA
 
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CEquipmentStatusViewerDlg)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
		//}}AFX_VIRTUAL

	protected:
		// Generated message map functions
		//{{AFX_MSG(CEquipmentStatusViewerDlg)
		virtual BOOL OnInitDialog();
		afx_msg void OnViewStatusbar();
		afx_msg void OnViewToolbar();
		afx_msg void OnExport();
		afx_msg void OnPrint();
		afx_msg void OnColumnClickEquipmentList(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
		afx_msg void OnHelpEquipmentstatusviewerhelp();
		afx_msg void OnToolbarHelp();
		afx_msg void OnSelchangeLocationCombo();
		afx_msg void OnExit();
		afx_msg void OnHelpAboutequipmentstatusviewer();
		afx_msg void OnClickEquipmentList(NMHDR* pNMHDR, LRESULT* pResult);
    	afx_msg LRESULT OnListItemUpdate(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnSessionIdChange(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnSortListCtrl(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnRepopulateItem(WPARAM wParam, LPARAM lParam);
		afx_msg LRESULT OnSetLanguage(WPARAM wParam, LPARAM lParam);
	    afx_msg void OnClose();
	    afx_msg void OnSize(UINT nType, int cx, int cy);
		afx_msg BOOL OnToolTipText(UINT nID, NMHDR* pNMHDR, LRESULT* pResult);
	    //}}AFX_MSG
		DECLARE_MESSAGE_MAP();

	private:
		void CreateToolbar();

		/**
		 *	AddTextToButton()
		 *
		 *	Add text to the button of a toolbar
		 *
		 *	@param	buttonId	Id of the button to set the text for
		 *	@param	buttonLabel Text of the button
		 *
		 */
		void AddTextToButton(UINT buttonId, TAUnicodeString buttonLabel );

        /** 
          * UpdateStatusBar
          *
          * Redraw the text in the status bar to reflect the current filtering.
          *
          */
		void UpdateStatusBar();

        /** 
          * Resize
          *
          * Make the list control fill the available space.
          *
          */
        void Resize();

        /** 
          * AdjustGUIWhileLoading
          *
          * Disable or enable all the GUI components while loading the report
          *
          * @param aEnableAllGUIComponents  True = enable the components
          *                                 False = disable (grey out) the components
          */
		void AdjustGUIWhileLoading(bool aEnableAllGUIComponents);

		/**
		 *	AdjustGUIAccrodingPermission()
		 *
		 *	Changes the state of GUI controls based on the current state of the 
		 *	dialog, enables/disable buttons etc.
		 *
		 */
		void AdjustGUIAccrodingPermission();
		/**
  		 *  SetStatusBarText()
  		 *
		 *	Sets the message in the status bar control
		 *
		 *	@param	message		Message to display
		 *
  		 */
		void SetStatusBarText(int aIndex, const TAUnicodeString& message);

		/**
		 *	CalculateToolBarStatusBarHeights()
		 *
		 *	Calculates heights of status bar and toolbar controls for the window
		 *
		 */
		void CalculateToolBarStatusBarHeights();

		/**
		 *	CreateStatus()
		 *
		 *	Creates statusbar for the window
		 *
		 */
		void CreateStatusBar();

		/**
		 *	PopulateLocations()
		 *
		 *	Populates the combo box with all available locations
		 *
		 */
		void PopulateLocations();

		
		/**
		 *	PopulateEquipmentStatusList()
		 *
		 *	Populates the equipment status list with equipment/datapoint information corresponding
		 *	to the selected report.
		 *
		 *	@param	locationKey	location key of populating equipment status
		 *
		 */
		void PopulateEquipmentStatusList(unsigned long locationKey);
	private:
		bool							m_needSortListCtrl;
		CButton							m_StopButton;
		CComboBox						m_Location;
		CComboBox						m_ReportName;
		CEquipmentStatusListCtrl		m_EquipmentStatusList;
		CStatusBar						m_WndStatusBar;
		CDlgToolBar						m_WndToolBar;
		UINT							m_ToolBarHeight;
		UINT							m_StatusBarHeight;
		EquipmentStatusViewerDlgModel*	m_realPModel;
    };

} // TA_Base_App

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EQUIPMENTSTATUSVIEWERDLG_H__B1FC56F1_91F3_4680_9193_F5B9A34FE350__INCLUDED_)
