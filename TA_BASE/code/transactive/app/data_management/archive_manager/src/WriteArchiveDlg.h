/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/data_management/archive_manager/src/WriteArchiveDlg.h $
  * @author:  Alan Brims
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Dialog box to list the files that will be written to archive media,
  * and prompt the operator to begin the write process.
  *
  */

#if !defined(AFX_WRITEARCHIVEDLG_H__7F0333C5_A4F1_4DE2_A786_D061681B411E__INCLUDED_)
#define AFX_WRITEARCHIVEDLG_H__7F0333C5_A4F1_4DE2_A786_D061681B411E__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <afx.h>

#include "app/data_management/archive_manager/pmodel/src/resource.h"
#include "app/data_management/archive_manager/src/WriteArchiveListComponent.h"
#include "app/data_management/archive_manager/pmodel/src/WriteArchivePModel.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"
    
namespace TA_Base_App
{   
    class WriteArchiveDlg : public TA_Base_Bus::AbstractDialog
    {
    public:

        /**
          * WriteArchiveDlg
          *
          * Constructor used for automatic/scheduled archives.
          *
          * @param      CWnd* pParent
          *             The parent window.
          */

        WriteArchiveDlg(bool isManualArchive, CWnd* pParent = NULL);
        
        /**
          * WriteArchiveDlg
          *
          * Constructor used for manual archives.
          *
          * @param      const time_t& fromDate
          *             The first date in the range to write.
          * @param      const time_t& toDate
          *             The last date in the range to write.
          * @param      CWnd* pParent
          *             The parent window.
          */

        WriteArchiveDlg(const time_t& fromDate,
            const time_t& toDate, CWnd* pParent = NULL);  

        /**
          * ~WriteArchiveDlg
          *
          * Standard destructor.
          */

        virtual ~WriteArchiveDlg() {};

		///AbstractDialog Methods
		virtual std::string getMyPModelName();
		virtual void setupPModel();
		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args);
        
        // Dialog Data
        //{{AFX_DATA(WriteArchiveDlg)
	    enum { IDD = IDD_WRITE_ARCHIVE_DIALOG };
		CStatic	m_insertCDDVD;
	    CStatic	m_numCds;
	    CButton	m_okButton;
		WriteArchiveListComponent	m_fileList;
    	//}}AFX_DATA
        
        
        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(WriteArchiveDlg)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL
        
        // Implementation
    protected:
        
        // Generated message map functions
        //{{AFX_MSG(WriteArchiveDlg)
        virtual BOOL OnInitDialog();
	    virtual void OnOK();
	    afx_msg void OnItemChangedFileList(NMHDR* pNMHDR, LRESULT* pResult);
	    virtual void OnCancel();
	    afx_msg void OnClose();
	    //}}AFX_MSG
        DECLARE_MESSAGE_MAP()

    private:
        //
        // Disable the copy constructor and assignment operator.
        //

        WriteArchiveDlg( const WriteArchiveDlg& );
        WriteArchiveDlg& operator=( const WriteArchiveDlg& );

        /**
          * populateList
          *
          * Populates the file list with the requested archive files.
          */

        void populateList();

		/**
          * showCdBreakUp
          *
          * THIS IS A TEST METHOD ONLY.
          * It displays a message box showing how the files have
          * been split up into CDs.
          */
        
        void showCdBreakUp();

		// 
        // The date range is only limited when manually archiving.
        //
        bool m_isManualArchive;

		//
		// The width of column name of m_fileList;
		//
		UINT m_fileListColNameWidth;

		WriteArchivePModel* m_realPModel;
        
    };
    
} // TA_Base_App

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_WRITEARCHIVEDLG_H__7F0333C5_A4F1_4DE2_A786_D061681B411E__INCLUDED_)
