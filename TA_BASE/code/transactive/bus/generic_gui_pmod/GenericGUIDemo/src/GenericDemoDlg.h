// GenericGUIDialogDemoDlg.h : header file
//

#pragma once

#include "resource.h" 
#include "bus/generic_gui_view/src/TransActiveDialog.h"

	// CGenericDemoDlg dialog
	class CGenericDemoDlg : public TA_Base_Bus::AbstractTransActiveDialog
	{
		// Construction
	public:
        CGenericDemoDlg(TA_Base_Bus::ITransActiveWinApp * app, CWnd* pParent = NULL);	// standard constructor

		// Dialog Data
		enum { IDD = IDD_GENERICGUIDIALOGDEMO_DIALOG };

		void init();

	protected:
		virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support


		// Implementation
	protected:
		HICON m_hIcon;

		// Generated message map functions
		virtual BOOL OnInitDialog();
		afx_msg void OnPaint();
		afx_msg HCURSOR OnQueryDragIcon();
		DECLARE_MESSAGE_MAP()
	public:
		afx_msg void OnBnClickedOk();
		afx_msg void OnBnClickedCancel();
		afx_msg void OnLbnSelchangeList1();
		afx_msg void OnClose();
    protected:
        void setupPModel() {};
        virtual std::string getMyPModelName();

    };
