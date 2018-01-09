#pragma once

#include "bus/generic_gui_view/src/TransActiveDialog.h"

// TestCommonDlg dialog

class TestCommonDlg : public TA_Base_Bus::AbstractDialog
{
	DECLARE_DYNAMIC(TestCommonDlg)

public:
	TestCommonDlg(TA_Base_Bus::ITransActiveWinApp * app, CWnd* pParent = NULL);   // standard constructor
	virtual ~TestCommonDlg();

// Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
protected:
    void setupPModel() {};
    virtual std::string getMyPModelName();
};
