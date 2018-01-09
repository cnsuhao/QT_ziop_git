#if !defined(TACMDICHILDWND_H_INCLUDED)
#define TACMDICHILDWND_H_INCLUDED

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000


namespace TA_Base_Bus
{

    class TACMDIChildWnd : public CMDIChildWnd
    {
        DECLARE_DYNAMIC(TACMDIChildWnd)

    public:

        afx_msg LRESULT OnSetLanguage(WPARAM wParam, LPARAM lParam);
        afx_msg void OnLangCommand(UINT nID);
        afx_msg void GetMessageString(UINT nID, CString& strMessage) const;
        afx_msg BOOL OnToolTipText(UINT nID , NMHDR* pNMHDR , LRESULT * pResult);

        DECLARE_MESSAGE_MAP()
    };

}


#endif
