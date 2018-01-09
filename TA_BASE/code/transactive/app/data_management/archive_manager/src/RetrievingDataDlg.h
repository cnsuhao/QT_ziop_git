#if !defined(AFX_RETRIEVINGDATADLG_H__5D02C1E1_7FC9_445D_B12F_70E2F6D1CF90__INCLUDED_)
#define AFX_RETRIEVINGDATADLG_H__5D02C1E1_7FC9_445D_B12F_70E2F6D1CF90__INCLUDED_

/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/data_management/archive_manager/src/RetrievingDataDlg.h $
  * @author:  Katherine Thomson
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Retrieves the data from the database for a manual archive.
  */

#include <string>
#include <afx.h>

#include "app/data_management/archive_manager/pmodel/src/resource.h"
#include "app/data_management/archive_manager/pmodel/src/WriteArchivePModel.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

namespace TA_Base_App
{
    class RetrievingDataDlg : public TA_Base_Bus::AbstractDialog
    {
    public:
        /**
          * RetrievingDataDlg
          *
          * Standard constructor.
          *
          * Preconditions:  fromDate <= toDate
          *                 toDate < todaysDate
          *
          * @param      const time_t& fromDate
          *             The first date in the range to archive.
          * @param      const time_t& toDate
          *             The last date in the range to archive.
          * @param      CWnd* pParent
          *             The parent window.
          */

        RetrievingDataDlg(CWnd* pParent = NULL);

        /**
          * ~RetrievingDataDlg
          *
          * Standard destructor.
          */

        virtual ~RetrievingDataDlg();
		
		
		virtual std::string getMyPModelName();
		virtual void setupPModel();
		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args);

        // Dialog Data
	    //{{AFX_DATA(RetrievingDataDlg)
	    enum { IDD = IDD_RETRIEVING_DATA_DIALOG };
	    CStatic	m_date;
	    CButton	m_stopButton;
	    CString	m_range;
	    //}}AFX_DATA


        // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(RetrievingDataDlg)
	    protected:
	    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	    //}}AFX_VIRTUAL

        // Implementation
    protected:

	    // Generated message map functions
	    //{{AFX_MSG(RetrievingDataDlg)
	    virtual BOOL OnInitDialog();
	    afx_msg void OnStop();
        //}}AFX_MSG
        afx_msg void OnClose(); 
	    afx_msg LRESULT OnCloseDlg(WPARAM, LPARAM);
        afx_msg LRESULT OnDisableDlg(WPARAM, LPARAM lparam);
	    afx_msg LRESULT OnShowCompleteDlg(WPARAM, LPARAM);
        DECLARE_MESSAGE_MAP()

    private:
        //
        // Disable the copy constructor and assignment operator.
        //

        RetrievingDataDlg( const RetrievingDataDlg& );
        RetrievingDataDlg& operator=( const RetrievingDataDlg& );
		
		WriteArchivePModel* m_realPModel;
    };

}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RETRIEVINGDATADLG_H__5D02C1E1_7FC9_445D_B12F_70E2F6D1CF90__INCLUDED_)
