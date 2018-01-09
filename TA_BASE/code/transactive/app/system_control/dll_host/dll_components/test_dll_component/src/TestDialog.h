#pragma once


#include "app/system_control/dll_host/dll_components/test_dll_component/src/Resource.h"


// TestDialog dialog

class TestDialog : public CDialog
{
	DECLARE_DYNAMIC(TestDialog)

public:

	TestDialog(CWnd* pParent = NULL);   // standard constructor
	virtual ~TestDialog();

    // Dialog Data
	enum { IDD = IDD_DIALOG1 };

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
    virtual BOOL OnInitDialog();
    afx_msg void OnBnClickedButton1();

private:

    CEdit m_caption;
    CEdit m_message;
    CEdit m_buttonName1;
    CEdit m_buttonName2;
    CEdit m_buttonName3;
    CEdit m_buttonName4;
    CEdit m_timeoutInSeconds;
    CEdit m_autoCloseTimeout;

    DECLARE_MESSAGE_MAP()
};
