/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/inspector_panel/src/ConfirmOverrideDialog.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  */
#if !defined(AFX_CONFIRMOVERRIDEDIALOG_H__B71B96E3_9427_447A_9324_1156EA18E105__INCLUDED_)
#define AFX_CONFIRMOVERRIDEDIALOG_H__B71B96E3_9427_447A_9324_1156EA18E105__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// ConfirmOverrideDialog.h : header file
//

//#include "app/scada/Inspector_Panel/src/Equipment.h"
#include "bus/mfc_extensions/src/numeric_edit_control/NumericEdit.h"
#include "../pmodel/src/overridelistctrlmodel.h"

namespace TA_Base_App
{
	/////////////////////////////////////////////////////////////////////////////
	// CConfirmOverrideDialog dialog

	class CConfirmOverrideDialog : public CDialog
	{
	// Construction
	public:
		CConfirmOverrideDialog(CWnd* pParent, int iPoint, OverrideListCtrlModel*		m_realPModel);

		std::string getOverrideValue();

	// Dialog Data
		//{{AFX_DATA(CConfirmOverrideDialog)
		enum { IDD = IDD_CONFIRM_OVERRIDE_DIALOG };
		//}}AFX_DATA

	// Overrides
		// ClassWizard generated virtual function overrides
		//{{AFX_VIRTUAL(CConfirmOverrideDialog)
		protected:
		virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
		//}}AFX_VIRTUAL

		void OnChangeNewValue();

	// Implementation
	protected:

		// Generated message map functions
		//{{AFX_MSG(CConfirmOverrideDialog)
		virtual BOOL OnInitDialog();
		virtual void OnOK();
		afx_msg void OnHelpButton(); //TD15124 azenitha
		//}}AFX_MSG
		DECLARE_MESSAGE_MAP()

	private:

		int							m_iPoint;
		OverrideListCtrlModel*		m_realPModel;
		CComboBox		m_valueComboBox;
		CNumericEdit	m_valueNumericEdit;
		CEdit			m_valueEdit;
		std::string		m_overrideValue;
	};

	//{{AFX_INSERT_LOCATION}}
	// Microsoft Visual C++ will insert additional declarations immediately before the previous line.
}

#endif // !defined(AFX_CONFIRMOVERRIDEDIALOG_H__B71B96E3_9427_447A_9324_1156EA18E105__INCLUDED_)
