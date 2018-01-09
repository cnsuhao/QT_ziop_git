#pragma once
#include "bus/mfc_extensions/src/Resource.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"

#include <string>
#include <vector>
#include <map>

// MessageDialogHelper dialog

namespace TA_Base_Bus
{

// Make this work in a DLL
#ifdef IMPORT_EXTENSIONS
	class __declspec(dllimport) MessageDialogHelper : public CDialog
#else
	class AFX_EXT_CLASS MessageDialogHelper : public CDialog
#endif
    {
        DECLARE_DYNAMIC(MessageDialogHelper)

    public:

        MessageDialogHelper(CWnd* pParent = NULL);   // standard constructor
		MessageDialogHelper( const std::vector<TAUnicodeString>& buttonNames, const TAUnicodeString& caption, const TAUnicodeString& message, UINT nStyle, const unsigned long timeoutInSeconds = 0 );
		virtual ~MessageDialogHelper();

		int showMessageBox(HWND hwnd, 
			LPCTSTR lpszMessage,
			LPCTSTR lpszCaption = NULL, 
			UINT uStyle = MB_OK|MB_ICONEXCLAMATION,
			UINT uHelpId = 0);

		void closeMessageBox();
        // Dialog Data
        enum { IDD = IDD_DIALOG_MESSAGE };

    protected:

        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        virtual BOOL OnInitDialog();

    public:

        const TCHAR * show();
        void close();

    private:

        afx_msg void OnBnClickedMessageButton1();
        afx_msg void OnBnClickedMessageButton2();
        afx_msg void OnBnClickedMessageButton3();
        afx_msg void OnBnClickedMessageButton4();
        afx_msg void OnTimer(UINT_PTR nIDEvent);
        void buttonClicked( int buttonID );

    private:

        std::map<int, TAUnicodeString> m_buttonID2NameMap;
        TAUnicodeString m_caption;
        TAUnicodeString m_message;
        unsigned long m_timeoutInSeconds;
        TAUnicodeString m_result;
        time_t m_startTime;

    public:

        static const TCHAR* CANCEL;
        static const TCHAR* TIMEOUT;
        static const TCHAR* CLOSE;

        DECLARE_MESSAGE_MAP()
    };

}
