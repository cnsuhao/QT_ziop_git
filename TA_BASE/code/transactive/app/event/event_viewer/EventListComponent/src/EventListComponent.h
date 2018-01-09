/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/event/event_viewer/EventListComponent/src/EventListComponent.h $
 * @author:  Nick Jardine
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2015/10/09 15:42:35 $
 * Last modified by:  $Author: CM $
 * 
 * EventListComponent is the main list class. It is a subclass of CListCtrl, and is responsible for controlling the display
 * of list data, and providing functions to the main application such as print, sort, and export.
 */

#if !defined(AFX_EVENTLISTCOMPONENT_H__CEA29538_337D_45EF_BA62_C01BB75DD54D__INCLUDED_)
#define AFX_EVENTLISTCOMPONENT_H__CEA29538_337D_45EF_BA62_C01BB75DD54D__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// EventListComponent.h : header file
//

#include <algorithm>
#include <string>

#include "app/event/event_viewer/EventListComponent/src/EventListComponent_rc.h"
#include  "app/event/event_viewer/pmodel/src/EventViewerDlgPModel.h"
#include "bus/generic_gui_view/src/AbstractTransActiveView.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"

// Define User Messages sent by the list
// #define WM_USER_LIST_UPDATE             WM_USER + 0x500
// #define WM_USER_LOAD_COUNT              WM_USER + 0x501
// #define WM_USER_LOAD_UPDATE             WM_USER + 0x502
// #define WM_USER_LIST_SELECTION_UPDATE   WM_USER + 0x503
// #define WM_USER_LISTVIEW_UPDATE         WM_USER + 0x504


/////////////////////////////////////////////////////////////////////////////
// EventListComponent window


class EventListComponent : public CListCtrl, public TA_Base_Bus::AbstractTransActiveView
{
public:
	EventListComponent();

	
	int OnToolHitTest(CPoint point, TOOLINFO * pTI) const;
    BOOL OnToolTipText(UINT id, NMHDR* pNMHDR, LRESULT* pResult);

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EventListComponent)
protected:
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL


	// Generated message map functions
protected:
	//{{AFX_MSG(EventListComponent)
	afx_msg void OnGetdispinfo(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnColumnclick(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemChanged(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void OnItemSelected(NMHDR* pNMHDR, LRESULT* pResult);
    afx_msg void onDblclk(NMHDR* pNMHDR, LRESULT* pResult);
	//}}AFX_MSG
	afx_msg void OnCustomdrawEventList ( NMHDR* pNMHDR, LRESULT* pResult );

	DECLARE_MESSAGE_MAP()


public:
	
	/**
     * destructor
     */
	virtual ~EventListComponent();

   /**
     * SetItemCountEx
     *
     * Override this method so that we can reapply selections every time the list
     * is modified.
     *
     * @param size The number of elements in the list.
     * @param flags Identifies the behaviour of the list when inserting.
     */
    BOOL SetItemCountEx( int size, DWORD flags = LVSICF_NOINVALIDATEALL );

	 /**
     * setFilter
     *
     * Set the filter object that will be used by the list to load and sort data when required.
     * 
     * @param filter The filter object to use for data operations.
     */
    void setFilter();

	void ChangeColumnsCaption();

	/**
     * print
     *
     * This method prints the current contents of the event list. It will print only those columns that are currently visible,
     * but it will print ALL events in the list (not just those that are visible).
     *
     * NOTE: while this method is active, the main GUI is not accissible, although automatic refresh updates will continue 
     * if enabled.
     */
    void print();

	void clearSortMark();

	/**
     * clearList
     *
     * Wraps the DeleteAllItems method of CListCtrl to remove the active sort
     */
    virtual BOOL clearList();

	/**
     * exportEvents
     *
     * This method writes the ENTIRE contents of the event list to a file - INCLUDING columns that are currently hidden.
     *
     * NOTE: while this method is active, the main GUI is not accissible, although automatic refresh updates will continue 
     * if enabled.
     *
     * @param exportLocation The default export location. To not set a default location, ensure the property 
     * exportLocation.IsEmpty() == TRUE
     */
    //void exportEvents(const CString& exportLocation);

    /**
     * setColourCoding
     *
     * Flag determines if colour coding information is requested from the filter object. If enabled, the filter is queried
     * for colour coding information based on row ID and column position for every entry in the list.
     *
     * @param isEnabled True if colour coding is to be enabled, false otherwise.
     */
    void setColourCoding(const bool isEnabled);

	
	virtual std::string getMyPModelName();
	virtual void setupPModel();
	virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args);
	virtual HWND getViewHandle() { 
		HWND wnd = GetSafeHwnd();
		TA_ASSERT(wnd != NULL, "View should be initialized successfully");
		return wnd;
	}


private:

	 /**
     * CellRectFromPoint
     *
     * Determines the row, column, and bounding rect of a cell.
     *
     * @param point The point to be tested.
     * @param collrect To hold the bounding rect.
     * @param col To hold the column index.
     *
     * @return int The row index, or -1 on failure.
     */
    int CellRectFromPoint( CPoint & point, RECT * cellrect, int * col ) const;

	void resetSortBitmap(const std::string& columnName, const int columnId);

	void clearColumnMask(int columnId);

	bool m_isColourCoded;
    bool m_reapplyingSelection;

    // Variables for multiline tooltips.
    std::auto_ptr<TCHAR> m_pchTip;
    std::auto_ptr<WCHAR> m_pwchTip;
    BOOL m_bToolTipCtrlCustomizeDone;

	EventViewerDlgPModel * m_realPModel;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTLISTCOMPONENT_H__CEA29538_337D_45EF_BA62_C01BB75DD54D__INCLUDED_)
