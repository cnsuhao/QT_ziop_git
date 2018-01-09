/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/scada/EquipmentStatusViewer/src/dlgbars.h $
  * @author:  Ripple
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  */

#ifndef __DLGBARS_H__
#define __DLGBARS_H__

//#include "bcgdurationctrl.h"

class CDlgToolBar : public CToolBar
{
// Construction
public:
	CDlgToolBar();
	//CStatic m_HistoryTypeText;
	//CComboBox m_HistoryTypeCtrl;
	//CStatic m_RangeText;
	//CBCGDurationCtrl m_RangeCtrl;
	//CStatic m_StartText;
	//CDateTimeCtrl m_StartDateTimeCtrl;

// Implementation
public:
	virtual ~CDlgToolBar();
	//void DisableApply();
	// Setup the bitmaps for the now button
	//void AddNowBitmaps();
	// Set the bitmap for the now button
	//void SetNowBitmap();
    
    //void DisableAll();
    //void EnableAll();

protected:
	// Generated message map functions
	//{{AFX_MSG(CDlgToolBar)
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);
	//afx_msg void EnableApply();
	//afx_msg void OnStartDateTimeChange(NMHDR* pNMHDR, LRESULT* pResult);
	//afx_msg void OnRangeChanged(long lTotalSeconds);
	DECLARE_EVENTSINK_MAP()
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	//int m_LedOn;
	//int m_LedOff;
};

class CDlgStatusBar : public CStatusBar
{

public:
	CDlgStatusBar();


public:
	virtual ~CDlgStatusBar();	

    void setLastMsg()
    {
        GetWindowText(m_lastMsgDisplayed.str());
    }

    TAUnicodeString getLastMsg()
    {
        return m_lastMsgDisplayed;
    }

	void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
protected:

	//{{AFX_MSG(CDlgStatusBar)
	afx_msg LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM lParam);	
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

private:
	
    // Last message displayed in status bar before server went down
    TAUnicodeString m_lastMsgDisplayed;
};



#endif //__DLGBARS_H__




