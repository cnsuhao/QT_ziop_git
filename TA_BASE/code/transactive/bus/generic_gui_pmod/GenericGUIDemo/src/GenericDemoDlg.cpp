// GenericGUIDialogDemoDlg.cpp : implementation file
//

#include "stdafx.h"
#include "GenericDemoDlg.h"
#include "./pmodel/src/GenericDemoConsts.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif

#include "TestCommonDlg.h"


	// CGenericDemoDlg dialog

	CGenericDemoDlg::CGenericDemoDlg(TA_Base_Bus::ITransActiveWinApp * app, CWnd* pParent)
	: TA_Base_Bus::AbstractTransActiveDialog(app, CGenericDemoDlg::IDD, pParent)
	{
		m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);

        TA_Base_Bus::ViewResizeProperty properties;
        properties.canMaximise = true;
        properties.maxHeight = -1;
        properties.maxWidth = -1;
        properties.minHeight = 300;
        properties.minWidth = 500;
        
        this->m_resizeViewHelper->setViewResizeProperty(properties);
	}

	void CGenericDemoDlg::DoDataExchange(CDataExchange* pDX)
	{
		CDialog::DoDataExchange(pDX);
	}

    BEGIN_MESSAGE_MAP(CGenericDemoDlg, AbstractTransActiveDialog)
		ON_WM_SYSCOMMAND()
		ON_WM_PAINT()
		ON_WM_QUERYDRAGICON()
		//}}AFX_MSG_MAP
		ON_BN_CLICKED(IDOK, &CGenericDemoDlg::OnBnClickedOk)
		ON_BN_CLICKED(IDCANCEL, &CGenericDemoDlg::OnBnClickedCancel)
		ON_LBN_SELCHANGE(IDC_LIST1, &CGenericDemoDlg::OnLbnSelchangeList1)
		ON_WM_CLOSE()
	END_MESSAGE_MAP()


	// CGenericDemoDlg message handlers

	BOOL CGenericDemoDlg::OnInitDialog()
	{
		AbstractTransActiveDialog::OnInitDialog();

		// Set the icon for this dialog.  The framework does this automatically
		//  when the application's main window is not a dialog
		SetIcon(m_hIcon, TRUE);			// Set big icon
		SetIcon(m_hIcon, FALSE);		// Set small icon

		// TODO: Add extra initialization here

		return TRUE;  // return TRUE  unless you set the focus to a control
	}


	// If you add a minimize button to your dialog, you will need the code below
	//  to draw the icon.  For MFC applications using the document/view model,
	//  this is automatically done for you by the framework.

	void CGenericDemoDlg::OnPaint()
	{
		if (IsIconic())
		{
			CPaintDC dc(this); // device context for painting

			SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

			// Center icon in client rectangle
			int cxIcon = GetSystemMetrics(SM_CXICON);
			int cyIcon = GetSystemMetrics(SM_CYICON);
			CRect rect;
			GetClientRect(&rect);
			int x = (rect.Width() - cxIcon + 1) / 2;
			int y = (rect.Height() - cyIcon + 1) / 2;

			// Draw the icon
			dc.DrawIcon(x, y, m_hIcon);
		}
		else
		{
			CDialog::OnPaint();
		}
	}

	// The system calls this function to obtain the cursor to display while the user drags
	//  the minimized window.
	HCURSOR CGenericDemoDlg::OnQueryDragIcon()
	{
		return static_cast<HCURSOR>(m_hIcon);
	}

	void CGenericDemoDlg::init()
	{
		return;
	}

	void CGenericDemoDlg::OnBnClickedOk()
	{
		// TODO: Add your control notification handler code here
		TestCommonDlg dlg(m_pWinApp);
		dlg.DoModal();
	}

	void CGenericDemoDlg::OnBnClickedCancel()
	{
		OnClose();
	}

	void CGenericDemoDlg::OnLbnSelchangeList1()
	{
		// TODO: Add your control notification handler code here
	}

	void CGenericDemoDlg::OnClose()
	{
		// TODO: Add your message handler code here and/or call default

		AbstractTransActiveDialog::OnClose();
	}



    std::string CGenericDemoDlg::getMyPModelName()
    {
        return  PMODEL_GENERIC_DEMO_DLG;
    }