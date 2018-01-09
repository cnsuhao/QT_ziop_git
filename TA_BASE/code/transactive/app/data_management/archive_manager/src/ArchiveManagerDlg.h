/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/data_management/archive_manager/src/ArchiveManagerDlg.h $
  * @author:  Alan Brims
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Dialog for the operator to select the archive operation to perform.
  * Inherits from TransActiveDialog.
  */

#if !defined(AFX_ARCHIVEMANAGERDLG_H__B1FC56F1_91F3_4680_9193_F5B9A34FE350__INCLUDED_)
#define AFX_ARCHIVEMANAGERDLG_H__B1FC56F1_91F3_4680_9193_F5B9A34FE350__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/data_management/archive_manager/pmodel/src/resource.h"
#include "app/data_management/archive_manager/pmodel/src/ArchiveManagerDlgPModel.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"


namespace TA_Base_App
{
    class CArchiveManagerDlg : public TA_Base_Bus::AbstractTransActiveDialog                               
    {
    public:
        /**
          * CArchiveManagerDlg
          *
          * Constructor. The server status is initialised to true (i.e. up).
          *
          */
        
        CArchiveManagerDlg( TA_Base_Bus::ITransActiveWinApp * app );

        /**
          * ~CArchiveManagerDlg
          *
          * Standard destructor.
          */

        virtual ~CArchiveManagerDlg();
        
        /**
          * setServerStatus
          *
          * Sets the server status, so the buttons can 
          * be enabled/disabled accordingly. This is called from the
          * ArchiveManagerGUI's serverUp() and serverDown() methods.
          *
          * @param      bool isServerUp
          *             True if the server is up.
          *             False if the server is down.
          */

        void setServerStatus( bool isServerUp );
      
       
        // Dialog Data
        //{{AFX_DATA(CArchiveManagerDlg)
	    enum { IDD = IDD_ARCHIVEMANAGER_DIALOG };
	    CButton	m_automaticButton;
	    CButton	m_restoreButton;
	    CButton	m_manualButton;
	    CButton	m_deleteButton;
	    //}}AFX_DATA
        
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(CArchiveManagerDlg)
    protected:
        virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
        //}}AFX_VIRTUAL
        
        // Implementation
    protected:
        // Generated message map functions
        //{{AFX_MSG(CArchiveManagerDlg)
        virtual BOOL OnInitDialog();
        virtual void OnOK();
	    afx_msg void OnButtonSchedule();
	    afx_msg void OnButtonManual();
	    afx_msg void OnButtonRestore();
	    afx_msg void OnButtonDisplay();
		afx_msg void OnClose();
		afx_msg LRESULT OnSetLanguage(WPARAM wParam, LPARAM lParam);
	//}}AFX_MSG
	    afx_msg LRESULT OnServerStatusChange(WPARAM, LPARAM);
		afx_msg LRESULT OnSessionIdChange(WPARAM, LPARAM);
		DECLARE_MESSAGE_MAP()


		////AbstractTransActiveDialog Interface methods
		//interface virtual method
		virtual std::string getMyPModelName();
		void setupPModel();
		virtual void initUIControl();
		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL);

    private:
        //
        // Disable the copy constructor and assignment operator.
        //

        CArchiveManagerDlg( const CArchiveManagerDlg& );
        CArchiveManagerDlg& operator=( const CArchiveManagerDlg& );

        /**
          * enableButtons
          * 
          * Enables the buttons (except for Close) based on the
          * operator's rights and the server status.
          * If the server is down, all the buttons will be disabled.
          * If the server is up, the buttons will be enabled based on the operator's rights.
          */

        void enableButtons();

        //
        // Icon for the dialog. 
        //
        
        HICON m_hIcon;

		ArchiveManagerDlgPModel* m_realPModel;
		
		CMenu m_menuMain; 

    };
    
} // TA_Base_App

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ARCHIVEMANAGERDLG_H__B1FC56F1_91F3_4680_9193_F5B9A34FE350__INCLUDED_)
