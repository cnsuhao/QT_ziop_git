/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/response_plans/plan_manager/prototype/src/ListCellComboBox.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  */
#if !defined(AFX_LISTCELLCOMBOBOX_H__D7A66435_C142_11D5_B649_0050BAB1D92B__INCLUDED_)
#define AFX_LISTCELLCOMBOBOX_H__D7A66435_C142_11D5_B649_0050BAB1D92B__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

// ListCellComboBox.h : header file
//

#include "ICellManager.h"


/////////////////////////////////////////////////////////////////////////////
// CListCellComboBox window

class CListCellComboBox : public CComboBox
{
// Construction
public:
    CListCellComboBox(CWnd *parent, CRect cellRect, int item, int subItem, CString text, ICellManager::CellData data);

// Attributes
public:

protected:
    bool    m_esc;
    int     m_item;
    int     m_subItem;

// Operations
public:
    void Update();

// Overrides
    // ClassWizard generated virtual function overrides
    // {{AFX_VIRTUAL(CListCellComboBox)
    public:
    virtual BOOL PreTranslateMessage(MSG* pMsg);
    // }}AFX_VIRTUAL

// Implementation
public:
    virtual ~CListCellComboBox();

    // Generated message map functions
protected:
    // {{AFX_MSG(CListCellComboBox)
    afx_msg void OnChar(UINT nChar, UINT nRepCnt, UINT nFlags);
    afx_msg void OnKillFocus(CWnd* pNewWnd);
    afx_msg void OnCloseup();
    // }}AFX_MSG

    DECLARE_MESSAGE_MAP()
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_LISTCELLCOMBOBOX_H__D7A66435_C142_11D5_B649_0050BAB1D92B__INCLUDED_)
