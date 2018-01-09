#pragma once
#include <afxcmn.h>
#include <afxcview.h>


namespace TA_Base_Bus
{

    class TACView : public CView
    {
        DECLARE_DYNAMIC(TACView)

    public:

        afx_msg LRESULT OnSetLanguage(WPARAM wParam, LPARAM lParam);

        DECLARE_MESSAGE_MAP()
    };


    class TACTreeView : public CTreeView
    {
        DECLARE_DYNAMIC(TACTreeView)

    public:

        afx_msg LRESULT OnSetLanguage(WPARAM wParam, LPARAM lParam);

        DECLARE_MESSAGE_MAP()
    };


    class TACScrollView : public CScrollView
    {
        DECLARE_DYNAMIC(TACScrollView)

    public:

        afx_msg LRESULT OnSetLanguage(WPARAM wParam, LPARAM lParam);

        DECLARE_MESSAGE_MAP()
    };


    class TACListView : public CListView
    {
        DECLARE_DYNAMIC(TACListView)

    public:

        afx_msg LRESULT OnSetLanguage(WPARAM wParam, LPARAM lParam);

        DECLARE_MESSAGE_MAP()
    };

}
