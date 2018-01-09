#include "StdAfx.h"
#include "TACView.h"


namespace TA_Base_Bus
{

    IMPLEMENT_DYNAMIC(TACView, CView)

    BEGIN_MESSAGE_MAP(TACView, CView)
        //{{AFX_MSG_MAP(TACView)
        ON_MESSAGE(WM_TASETLANG, &TACView::OnSetLanguage)
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()


    LRESULT TACView::OnSetLanguage(WPARAM wParam, LPARAM lParam)
    {
        for ( CWnd* pWnd = GetWindow(GW_CHILD); pWnd; pWnd = pWnd->GetWindow(GW_HWNDNEXT) )
        {
            pWnd->PostMessage(WM_TASETLANG, 0, 0);
        }

        return 0;
    }


    IMPLEMENT_DYNAMIC(TACTreeView, CTreeView)

    BEGIN_MESSAGE_MAP(TACTreeView, CTreeView)
        //{{AFX_MSG_MAP(TACView)
        ON_MESSAGE(WM_TASETLANG, &TACTreeView::OnSetLanguage)
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()


    LRESULT TACTreeView::OnSetLanguage(WPARAM wParam, LPARAM lParam)
    {
        for ( CWnd* pWnd = GetWindow(GW_CHILD); pWnd; pWnd = pWnd->GetWindow(GW_HWNDNEXT) )
        {
            pWnd->PostMessage(WM_TASETLANG, 0, 0);
        }

        return 0;
    }


    IMPLEMENT_DYNAMIC(TACScrollView, CScrollView)

    BEGIN_MESSAGE_MAP(TACScrollView, CScrollView)
        //{{AFX_MSG_MAP(TACView)
        ON_MESSAGE(WM_TASETLANG,  &TACScrollView::OnSetLanguage)
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()


    LRESULT TACScrollView::OnSetLanguage(WPARAM wParam, LPARAM lParam)
    {
        for ( CWnd* pWnd = GetWindow(GW_CHILD); pWnd; pWnd = pWnd->GetWindow(GW_HWNDNEXT) )
        {
            pWnd->PostMessage(WM_TASETLANG, 0, 0);
        }

        return 0;
    }


    IMPLEMENT_DYNAMIC(TACListView, CListView)

    BEGIN_MESSAGE_MAP(TACListView, CListView)
        //{{AFX_MSG_MAP(TACView)
        ON_MESSAGE(WM_TASETLANG,  &TACListView::OnSetLanguage)
        //}}AFX_MSG_MAP
    END_MESSAGE_MAP()


    LRESULT TACListView::OnSetLanguage(WPARAM wParam, LPARAM lParam)
    {
        for ( CWnd* pWnd = GetWindow(GW_CHILD); pWnd; pWnd = pWnd->GetWindow(GW_HWNDNEXT) )
        {
            pWnd->PostMessage(WM_TASETLANG, 0, 0);
        }

        // TODO: the CListCtrl doesn't have column IDs, cannot translate here!

        return 0;
    }

}
