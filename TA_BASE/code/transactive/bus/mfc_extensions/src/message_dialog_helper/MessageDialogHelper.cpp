#include "..\stdafx.h"
#include "bus/mfc_extensions/src/message_dialog_helper/MessageDialogHelper.h"
#include "bus/mfc_extensions/src/message_box/XMessageBox.h"


#define ID_CLOSE   10
#define ID_TIMEOUT 11
#define ID_TIMER   123

// MessageDialogHelper dialog
extern HWND         ghWndMessageBox;
namespace TA_Base_Bus
{

    const TCHAR* MessageDialogHelper::CANCEL   = _T("CANCEL");
    const TCHAR* MessageDialogHelper::TIMEOUT  = _T("TIMEOUT");
    const TCHAR* MessageDialogHelper::CLOSE    = _T("CLOSE");


    IMPLEMENT_DYNAMIC(MessageDialogHelper, CDialog)


    MessageDialogHelper::MessageDialogHelper(CWnd* pParent /*=NULL*/)
        : CDialog(MessageDialogHelper::IDD, pParent),
          m_timeoutInSeconds(0)
    {
    }

	MessageDialogHelper::MessageDialogHelper( const std::vector<TAUnicodeString>& buttonNames, const TAUnicodeString& caption, const TAUnicodeString& message, UINT nStyle, const unsigned long timeoutInSeconds/* =0 */ )
		: CDialog(MessageDialogHelper::IDD, NULL),
		m_caption(caption),
		m_message(message),
		m_timeoutInSeconds(timeoutInSeconds)
	{
		for ( size_t i = 0; i < buttonNames.size(); ++i )
		{
			m_buttonID2NameMap[IDC_MESSAGE_BUTTON1 + i] = buttonNames[i];
		}
	}


    MessageDialogHelper::~MessageDialogHelper()
    {
    }


    void MessageDialogHelper::DoDataExchange(CDataExchange* pDX)
    {
        CDialog::DoDataExchange(pDX);
    }


    BEGIN_MESSAGE_MAP(MessageDialogHelper, CDialog)
        ON_BN_CLICKED(IDC_MESSAGE_BUTTON1, &MessageDialogHelper::OnBnClickedMessageButton1)
        ON_BN_CLICKED(IDC_MESSAGE_BUTTON2, &MessageDialogHelper::OnBnClickedMessageButton2)
        ON_BN_CLICKED(IDC_MESSAGE_BUTTON3, &MessageDialogHelper::OnBnClickedMessageButton3)
        ON_BN_CLICKED(IDC_MESSAGE_BUTTON4, &MessageDialogHelper::OnBnClickedMessageButton4)
        ON_WM_TIMER()
    END_MESSAGE_MAP()


    // MessageDialogHelper message handlers

    BOOL MessageDialogHelper::OnInitDialog()
    {
        CDialog::OnInitDialog();

        for ( size_t i = IDC_MESSAGE_BUTTON1; i <= IDC_MESSAGE_BUTTON4; ++i )
        {
            GetDlgItem(i)->ShowWindow(SW_HIDE);
        }

        for ( std::map<int, TAUnicodeString>::iterator it = m_buttonID2NameMap.begin(); it != m_buttonID2NameMap.end(); ++it )
        {
            GetDlgItem(it->first)->SetWindowText( it->second.c_str() );
            GetDlgItem(it->first)->ShowWindow(SW_SHOW);
        }

        GetDlgItem(IDC_STATIC_MESSAGE)->SetWindowText( m_message.c_str() );
        SetWindowText(m_caption.c_str());

        if ( m_timeoutInSeconds != 0 )
        {
            m_startTime = time(NULL);
            SetTimer(ID_TIMER, 1000, NULL);
        }

        return TRUE;
    }


    const TCHAR * MessageDialogHelper::show()
    {
		
        switch ( DoModal() )
        {
        case IDOK:
            return m_result.GetString();

        case IDCANCEL:
            return CANCEL;

        case ID_TIMEOUT:
            return TIMEOUT;

        case ID_CLOSE:
        default:
            return CLOSE;
        }
    }


    void MessageDialogHelper::close()
    {
        if ( IsWindow(m_hWnd) )
        {
            EndDialog(ID_CLOSE);
        }
    }


    void MessageDialogHelper::OnBnClickedMessageButton1()
    {
        buttonClicked(IDC_MESSAGE_BUTTON1);
    }


    void MessageDialogHelper::OnBnClickedMessageButton2()
    {
        buttonClicked(IDC_MESSAGE_BUTTON2);
    }


    void MessageDialogHelper::OnBnClickedMessageButton3()
    {
        buttonClicked(IDC_MESSAGE_BUTTON3);
    }


    void MessageDialogHelper::OnBnClickedMessageButton4()
    {
        buttonClicked(IDC_MESSAGE_BUTTON4);
    }


    void MessageDialogHelper::buttonClicked(int buttonID)
    {
        m_result = m_buttonID2NameMap[buttonID];

        KillTimer(ID_TIMER);
        EndDialog(IDOK);
    }


    void MessageDialogHelper::OnTimer(UINT_PTR nIDEvent)
    {
        unsigned long elapsedTime = time(NULL) - m_startTime;

        if ( m_timeoutInSeconds <= elapsedTime )
        {
            KillTimer(ID_TIMER);
            EndDialog(ID_TIMEOUT);
        }

        CDialog::OnTimer(nIDEvent);
    }

	int MessageDialogHelper::showMessageBox( HWND hwnd, LPCTSTR lpszMessage, LPCTSTR lpszCaption /*= NULL*/, UINT uStyle /*= MB_OK|MB_ICONEXCLAMATION*/, UINT uHelpId /*= 0*/ )
	{
		ghWndMessageBox = NULL;
		return XMessageBox(hwnd, lpszMessage,lpszCaption, uStyle, uHelpId);
	}

	void MessageDialogHelper::closeMessageBox()
	{
		if(ghWndMessageBox != NULL)
		{
			::EndDialog(ghWndMessageBox, IDCLOSE);
		}
	}
}
