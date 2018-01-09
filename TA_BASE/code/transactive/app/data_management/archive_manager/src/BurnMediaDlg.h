#if !defined(AFX_BurnMediaDlg_H__1468A462_430F_4418_BC92_334001E5F340__INCLUDED_)
#define AFX_BurnMediaDlg_H__1468A462_430F_4418_BC92_334001E5F340__INCLUDED_

/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI/TA_BASE/transactive/app/data_management/archive_manager/src/BurnMediaDlg.h $
  * @author:  Alan Brims
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2013/09/03 14:11:12 $
  * Last modified by:  $Author: CM $
  *
  * Dialog box to handle the CD Burn process.
  * This dialog is specific to CD-R and should be replaced with
  * a similar dialog if an alternative media is required.
  *
  */

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/data_management/archive_manager/pmodel/src/BurnMediaPModel.h"
#include "app/data_management/archive_manager/pmodel/src/resource.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"


namespace TA_Base_App
{
    class BurnMediaDlg : public TA_Base_Bus::AbstractDialog
    {
    public:
        /**
          * BurnMediaDlg
          *
          * Standard constructor.
          *
          * @param      const std::string& volumeLabel
          *             The label to apply to the CD.  The user will also
          *             be told to write this label on the CD once the write is complete.
          * @param      const std::string& cdLabel
          *             The label to apply to the CD electronically.
          *             KT 10 Mar 04: PW #3136 Added reformatted volume label for CD (cdLabel).
          * @param      const WriteArchiveHelper::FileDetailsVector& files
          *             The names (incl. full paths) of the files to write to CD.
          *             These are in the order in which they will be written to the CD.
          * @param      CWnd* pParent
          *             The parent of this dialog.
          */

        BurnMediaDlg( const std::string& volumeLabel, const std::string& cdLabel, 
            const WriteArchiveHelper::FileDetailsVector& files,  bool bCdBurn, CWnd* pParent = NULL);
           
        /**
          * ~BurnMediaDlg
          *
          * Standard destructor that calls terminateAndWait().
          */

        virtual ~BurnMediaDlg();
		
		virtual std::string getMyPModelName();
		virtual void setupPModel();
		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args);

    
        // Dialog Data
        //{{AFX_DATA(BurnMediaDlg)
	    enum { IDD = IDD_BURN_DVD_DIALOG };
	    CButton	m_buttonCancel;
	    CStatic	m_stageStatic;
	    CEdit	m_editLog;
        CProgressCtrl	m_writeProgress;
        CString	m_csStage;
        CString	m_csLog;
	    //}}AFX_DATA
        
        
        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(BurnMediaDlg)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
        //}}AFX_VIRTUAL
        
        // Implementation
    protected:
        
        // Generated message map functions
        //{{AFX_MSG(BurnMediaDlg)
        virtual BOOL OnInitDialog();
	    afx_msg void OnDestroy();
	    virtual void OnCancel();
	    //}}AFX_MSG
	    afx_msg LRESULT OnCloseDlg(WPARAM, LPARAM);
        DECLARE_MESSAGE_MAP()

    private:

		/**
          * appendStringToLogDisplay
          *
          * Adds a string to the log displayed on the dialog.
          *
          * @param      const std::string& sAdd
          *             The string to add.
          */

        void appendStringToLogDisplay( const std::string& sAdd );

		void showCloseButton();
		void enableCancelButton(bool enable);
		void setBurnFinish(bool success);
		 bool showFileDialog(TAUnicodeString& selectedPath);
		 void setProgress(DWORD dwProgressInPercent);

        //
        // Disable the copy constructor and assignment operator.
        //
		
		BurnMediaDlg( const BurnMediaDlg& );
        BurnMediaDlg& operator=( const BurnMediaDlg& );

      
        //
        // The files to write to CD in write order as passed to
        // this dialog and in a form ready for passing to Nero.
        //

        const WriteArchiveHelper::FileDetailsVector& m_files;

		//
        // The label to write on the CD manually (m_volumeLabel) and 
        // electronically (m_cdLabel) containing the archive date and part information.
        // KT 10 Mar 04: PW #3136 Added reformatted volume label for CD (m_cdLabel).
        // 

        std::string m_volumeLabel;
        std::string m_cdLabel;

		bool m_CDBurn;

		BurnMediaPModel* m_realPModel;

    };    
    
} // TA_Base_App

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_BurnMediaDlg_H__1468A462_430F_4418_BC92_334001E5F340__INCLUDED_)
