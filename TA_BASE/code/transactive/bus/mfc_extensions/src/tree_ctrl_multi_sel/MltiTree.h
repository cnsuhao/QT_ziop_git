/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/mfc_extensions/src/tree_ctrl_multi_sel/MltiTree.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  */
#if !defined(AFX_MLTITREE_H__9F4750E5_C512_11D2_880A_02608C7CB3EE__INCLUDED_)
#define AFX_MLTITREE_H__9F4750E5_C512_11D2_880A_02608C7CB3EE__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
// MltiTree.h : header file
// Copyright (c) 1999 Richard Hazlewood
// This code is provided as-is.  Use at your own peril.
    
// SetItemState / GetItemState:
// These two methods have been over-scoped in the CMultiTree class to 
// replace the underlying CTreeCtrl versions. Because they are NOT virtual, 
// they must be called in the scope of the CMultiTree class (or derived class), 
// otherwise they will not work, e.g.: 

    /* if m_tree is a CMultiTree */
    // CTreeCtrl *pTree = &m_tree;
    // UINT nState = pTree->GetItemState(hItem, TVIS_FOCUSED|TVIS_SELECTED);

#include <afxtempl.h>

namespace TA_Base_Bus
{
    /////////////////////////////////////////////////////////////////////////////

    //take the liberty of using 1 for focused item - it used
    // to be defined anyway, before MS removed it.
    #ifndef TVIS_FOCUSED
    #define TVIS_FOCUSED	1
    #else
    #if TVIS_FOCUSED != 1
    #error TVIS_FOCUSED was assumed to be 1
    #endif
    #endif

    //define for custom notification
    // sent on check-box change
    // (Care: may clash in future)
    #ifdef _MST_TVN_CHKCHANGE
    #ifndef _MST_MULTI_CHECK
    #define _MST_MULTI_CHECK	//implied
    #endif
    #ifndef TVN_CHKCHANGE
    #define TVN_CHKCHANGEA	(TVN_FIRST-49)
    #define TVN_CHKCHANGEW	(TVN_FIRST-(49+49))
    #ifdef UNICODE
    #define TVN_CHKCHANGE	TVN_CHKCHANGEW
    #else
    #define TVN_CHKCHANGE	TVN_CHKCHANGEA
    #endif
    #endif //!TVN_CHKCHANGE
    #endif //_MST_TVN_CHKCHANGE

    typedef CTypedPtrList<CPtrList, HTREEITEM> CTreeItemList;

    /////////////////////////////////////////////////////////////////////////////
    // CMultiTree window

    #ifndef MST_AFX_EXT_CLASS
    #define MST_AFX_EXT_CLASS
    //define this if building class for DLL export/import
    // typically: #define MST_AFX_EXT_CLASS AFX_EXT_CLASS
    #endif

    #ifndef CMultiTree_BASE
    #define CMultiTree_BASE	CTreeCtrl
    #endif

    #ifdef IMPORT_EXTENSIONS
    class __declspec(dllimport) CMultiTree : public CMultiTree_BASE
    #else
    class AFX_EXT_CLASS CMultiTree : public CMultiTree_BASE
    #endif
    {
	    DECLARE_DYNAMIC(CMultiTree)
    // Construction
    public:
	    CMultiTree();

    // Attributes
    public:

    // Operations
    public:
	    UINT GetSelectedCount() const;
	    HTREEITEM GetFirstSelectedItem() const;
	    HTREEITEM GetNextSelectedItem(HTREEITEM hItem) const;

	    void SelectAll(BOOL bSelect = TRUE);	//doesn't affect focus
	    void SelectRange(HTREEITEM hFirst, HTREEITEM hLast, BOOL bOnly = TRUE);
	    BOOL IsSelected(HTREEITEM hItem) const {return !!(TVIS_SELECTED & CMultiTree_BASE::GetItemState(hItem, TVIS_SELECTED));};

	    //SelectChildren returns TRUE if focus was on a child item
	    BOOL SelectChildren(HTREEITEM hParent, BOOL bSelect = TRUE, BOOL bRecurse = TRUE);

	    //Note: not virtual, so must call this class (or derivative)
	    BOOL SetItemState(HTREEITEM hItem, UINT nState, UINT nStateMask);
	    UINT GetItemState(HTREEITEM hItem, UINT nStateMask) const;
	    BOOL SelectItem(HTREEITEM hItem);

	    //Note: GetSelectedItem should be taken as GetFocusedItem
	    // when in multi-select mode
	    HTREEITEM GetFocusedItem() const {ASSERT(m_bMulti);return CMultiTree_BASE::GetSelectedItem();};
	    BOOL FocusItem(HTREEITEM hItem);

	    BOOL SetMultiSelect(BOOL bMulti);
	    BOOL IsMultiSelect() const {return m_bMulti;};
	    BOOL IsEmulatedNotify() const {return m_bEmulated;};
	    BOOL SetBandingHit(BOOL bLabel) {BOOL b=m_bBandLabel;m_bBandLabel=bLabel;return b;};

	    void GetSelectedList(CTreeItemList& list) const;

    protected:
	    BOOL		m_bMulti;
        BOOL        m_bAllowDragSelection;  // To allow user to drag rectangle around items to select them
	    HTREEITEM	m_hSelect;	//for shift selection
	    BOOL		m_bEmulated;
	    BOOL		m_bBandLabel;

    protected:
	    void OnButtonDown(BOOL bLeft, UINT nFlags, CPoint point);
	    void DoPreSelection(HTREEITEM hItem, BOOL bLeft, UINT nFlags);
	    void DoAction(HTREEITEM hItem, BOOL bLeft, UINT nFlags, CPoint point);
	    void DoBanding(BOOL bLeft, UINT nFlags, CPoint point);
	    void UpdateSelectionForRect(LPCRECT pRect, UINT nFlags, CTreeItemList& list);
	    void SelectAllIgnore(BOOL bSelect, HTREEITEM hIgnore);

	    BOOL _SendNotify(LPNMHDR pNMHDR);

    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(CMultiTree)
	    //}}AFX_VIRTUAL

    // Implementation
    public:
	    virtual ~CMultiTree();

	    // Generated message map functions
    protected:
	    //{{AFX_MSG(CMultiTree)
	    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	    afx_msg void OnSetFocus(CWnd* pOldWnd);
	    afx_msg void OnKillFocus(CWnd* pNewWnd);
	    afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
	    afx_msg BOOL OnItemExpanding(NMHDR* pNMHDR, LRESULT* pResult);
	    afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	    //}}AFX_MSG

	    DECLARE_MESSAGE_MAP()
    };
}

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MLTITREE_H__9F4750E5_C512_11D2_880A_02608C7CB3EE__INCLUDED_)
