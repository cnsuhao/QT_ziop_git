#include "app/system_control/dll_host/dll_components/test_dll_component/src/stdafx.h"
#include "app/system_control/dll_host/dll_components/test_dll_component/src/TestDialog.h"
#include "app/system_control/dll_host/dll_components/dll_component_helper/src/MessageDialogHelper.h"
#include "core/threads/src/Thread.h"
#include <vector>


namespace
{
    struct AutoCloseDialogThread : public TA_Base_Core::Thread
    {
        AutoCloseDialogThread(MessageDialogHelper& dialog, unsigned long timeout)
            : m_dialog(dialog),
              m_timeout(timeout),
              m_running(false)
        {
            m_start = time(NULL);

            if ( 0 < timeout )
            {
                start();
            }
        }

        ~AutoCloseDialogThread()
        {
            terminateAndWait();
        }

        virtual void run()
        {
            m_running = true;

            while ( m_running )
            {
                unsigned long elapsedTime = time(NULL) - m_start;

                if ( m_timeout <= elapsedTime )
                {
                    break;
                }

                sleep( 1000 );
            }

            if ( m_running )
            {
                m_dialog.close();
            }
        }

        virtual void terminate()
        {
            m_running = false;
        }

    private:

        volatile bool m_running;
        time_t m_start;
        unsigned long m_timeout;
        MessageDialogHelper& m_dialog;
    };
}


// TestDialog dialog

IMPLEMENT_DYNAMIC(TestDialog, CDialog)

TestDialog::TestDialog(CWnd* pParent /*=NULL*/)
	: CDialog(TestDialog::IDD, pParent)
{
}


TestDialog::~TestDialog()
{
}


void TestDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialog::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_EDIT_CAPTION, m_caption);
    DDX_Control(pDX, IDC_EDIT_MESSAGE, m_message);
    DDX_Control(pDX, IDC_EDIT_BUTTON1, m_buttonName1);
    DDX_Control(pDX, IDC_EDIT_BUTTON2, m_buttonName2);
    DDX_Control(pDX, IDC_EDIT_BUTTON3, m_buttonName3);
    DDX_Control(pDX, IDC_EDIT_BUTTON4, m_buttonName4);
    DDX_Control(pDX, IDC_EDIT_TIMEOUT, m_timeoutInSeconds);
    DDX_Control(pDX, IDC_EDIT_AUTO_CLOSE, m_autoCloseTimeout);
}


BEGIN_MESSAGE_MAP(TestDialog, CDialog)
    ON_BN_CLICKED(IDC_BUTTON1, &TestDialog::OnBnClickedButton1)
END_MESSAGE_MAP()


// TestDialog message handlers


BOOL TestDialog::OnInitDialog()
{
    CDialog::OnInitDialog();

    m_caption.SetWindowText("Message Dialog");
    m_message.SetWindowText("Hello, World!");
    m_buttonName1.SetWindowText("Ignore");
    m_buttonName2.SetWindowText("Retry");
    m_buttonName3.SetWindowText("Abort");
    m_buttonName4.SetWindowText("");
    m_timeoutInSeconds.SetWindowText("10");
    m_autoCloseTimeout.SetWindowText("");

    return TRUE;
}


void TestDialog::OnBnClickedButton1()
{
    CString caption;
    CString message;
    CString buttonName1;
    CString buttonName2;
    CString buttonName3;
    CString buttonName4;
    std::vector<std::string> buttonNames;
    CString timeoutInSeconds;
    CString autoCloseTimeout;

    m_caption.GetWindowText(caption);
    m_message.GetWindowText(message);
    m_buttonName1.GetWindowText(buttonName1);
    m_buttonName2.GetWindowText(buttonName2);
    m_buttonName3.GetWindowText(buttonName3);
    m_buttonName4.GetWindowText(buttonName4);
    m_timeoutInSeconds.GetWindowText(timeoutInSeconds);
    m_autoCloseTimeout.GetWindowText(autoCloseTimeout);

    if ( FALSE == buttonName1.IsEmpty() )
    {
        buttonNames.push_back( (const char*)buttonName1 );
    }

    if ( FALSE == buttonName2.IsEmpty() )
    {
        buttonNames.push_back( (const char*)buttonName2 );
    }

    if ( FALSE == buttonName3.IsEmpty() )
    {
        buttonNames.push_back( (const char*)buttonName3 );
    }

    if ( FALSE == buttonName4.IsEmpty() )
    {
        buttonNames.push_back( (const char*)buttonName4 );
    }

    unsigned long timeout = ::atoi( timeoutInSeconds );
    unsigned long autoTimeout = ::atoi( autoCloseTimeout );

    MessageDialogHelper dlg(buttonNames, (const char*)caption, (const char*)message, timeout);
    AutoCloseDialogThread autoClose(dlg, autoTimeout);

    std::string result = dlg.show();
    autoClose.terminate();

    SetWindowText( result.c_str() );
}
