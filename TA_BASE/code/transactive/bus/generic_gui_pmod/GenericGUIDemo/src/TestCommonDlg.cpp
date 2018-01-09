// TestCommonDlg.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "TestCommonDlg.h"
#include "./pmodel/src/GenericDemoConsts.h"

// TestCommonDlg dialog

IMPLEMENT_DYNAMIC(TestCommonDlg, AbstractDialog)

TestCommonDlg::TestCommonDlg(TA_Base_Bus::ITransActiveWinApp * app, CWnd* pParent /*=NULL*/)
: TA_Base_Bus::AbstractDialog(app, TestCommonDlg::IDD, pParent)
{

}

TestCommonDlg::~TestCommonDlg()
{
}

void TestCommonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(TestCommonDlg, TA_Base_Bus::AbstractDialog)
	ON_BN_CLICKED(IDOK, &TestCommonDlg::OnBnClickedOk)
	ON_BN_CLICKED(IDCANCEL, &TestCommonDlg::OnBnClickedCancel)
END_MESSAGE_MAP()


// TestCommonDlg message handlers

void TestCommonDlg::OnBnClickedOk()
{
	// TODO: Add your control notification handler code here
	MessageBox(_T("are you ok?"));
}

void TestCommonDlg::OnBnClickedCancel()
{
	// TODO: Add your control notification handler code here
	OnCancel();
}


std::string TestCommonDlg::getMyPModelName()
{
    return PMODEL_TEST_COMMON_DLG; 
}