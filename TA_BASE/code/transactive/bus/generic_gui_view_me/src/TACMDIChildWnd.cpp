#include "StdAfx.h"
#include "TACMDIChildWnd.h"
#include "TopmostWindowMessageSender.h"


namespace TA_Base_Bus
{

    IMPLEMENT_DYNAMIC(TACMDIChildWnd, CMDIChildWnd)

    BEGIN_MESSAGE_MAP(TACMDIChildWnd, CMDIChildWnd)
        //{{AFX_MSG_MAP(TA_CDialog)
        ON_MESSAGE(WM_TASETLANG, &TACMDIChildWnd::OnSetLanguage)
        ON_COMMAND_RANGE(LANGUAGE_ID_FIRST, LANGUAGE_ID_LAST, &TACMDIChildWnd::OnLangCommand)
        ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, &TACMDIChildWnd::OnToolTipText)
        ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, &TACMDIChildWnd::OnToolTipText)
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()


    void TACMDIChildWnd::OnLangCommand(UINT nID)
    {
        TASetCurrentLang( nID );
        TASetDefaultLang( nID );
        TopmostWindowMessageSender::instance().postMessage( WM_TASETLANG );
    }


    LRESULT TACMDIChildWnd::OnSetLanguage(WPARAM wParam, LPARAM lParam)
    {
        // TODO: do something for this wnd

        CDocument* pDoc = GetActiveDocument();

        if ( pDoc )
        {
            POSITION pos = pDoc->GetFirstViewPosition();

            while ( pos )
            {
                CView* pView = pDoc->GetNextView(pos);
                pView->Invalidate();
                pView->PostMessage(WM_TASETLANG, 0, 0);
            }
        }

        return 0;
    }


    void TACMDIChildWnd::GetMessageString(UINT nID, CString& strMessage) const
    {
        const CString& s = TALoadString( nID ).c_str();

        if ( s.IsEmpty() != TRUE )
        {
            strMessage = s;
        }
    }


    BOOL TACMDIChildWnd::OnToolTipText(UINT nID , NMHDR* pNMHDR , LRESULT * pResult)
    {
        return TAOnToolTipText(nID, pNMHDR, pResult);
    }

}
