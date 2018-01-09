/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/event/event_viewer/src/MultipleLoadDlg.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  */
#if !defined(AFX_MULTIPLELOADDLG_H__DC407973_7A3A_4552_B004_22F58A9D731E__INCLUDED_)
#define AFX_MULTIPLELOADDLG_H__DC407973_7A3A_4552_B004_22F58A9D731E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// MultipleLoadDlg.h : header file
//

#include <list>
//#include <string>
#include <vector>

#include "app/event/event_viewer/src/resource.h"
#include  "app/event/event_viewer/pmodel/src/ConstantItems.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"

using namespace TA_Base_App;
/////////////////////////////////////////////////////////////////////////////
// MultipleLoadDlg dialog
/////////////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
class MultipleLoadDlg : public CDialog
{
// Construction
public:
	MultipleLoadDlg(const TAUnicodeString& windowTitle, CWnd* pParent = NULL);

	SelectedItems getSelection() const;
	void setSelection(const SelectedItems& selection);
	void setItems(const ListItems& items, bool includeAllItem);

private:
	// Helper function. Returns true IFF m_includeAllItem && the ALL item
	// is one of the selections
	bool IsAllItemSelected() const;

	// The title of this dialog window
	TAUnicodeString m_windowTitle;

	// A list containing all items in the list
	ListItems m_listItems;

	// A vector containing only the keys of the selected items
	SelectedItems m_selectedItems;

	// Does this control have an "<All>" item that returns an empty selection
	// when it is included in the selected items?
	// Note that the behaviour of the "<All>" item is slightly non-intuitive:
	// if the ALL item is selected then this dialog returns no selection.
	// This is because this dialog is used to filter out alarms and if ALL
	// is selected for a particular filter then no filter is required.
	bool m_includeAllItem;

// Dialog Data
	//{{AFX_DATA(MultipleLoadDlg)
enum { IDD = IDD_MULTIPLE_LOAD };
CListBox	m_multiList;
//}}AFX_DATA


// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(MultipleLoadDlg)
	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	//}}AFX_VIRTUAL

// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(MultipleLoadDlg)
afx_msg void OnMultiok();
virtual BOOL OnInitDialog();
afx_msg void OnDestroy();
//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
	
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_MULTIPLELOADDLG_H__DC407973_7A3A_4552_B004_22F58A9D731E__INCLUDED_)
