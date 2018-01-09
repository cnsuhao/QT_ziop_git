/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/data_management/archive_manager/src/RetrieveCompleteDlg.h $
  * @author:  Katherine Thomson
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Displayed if there are errors during the retrieval process.
  */

#if !defined(AFX_RETRIEVECOMPLETEDLG_H__CD3F001A_8535_4EB9_A640_E932A0DE3F4C__INCLUDED_)
#define AFX_RETRIEVECOMPLETEDLG_H__CD3F001A_8535_4EB9_A640_E932A0DE3F4C__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>

#include "app/data_management/archive_manager/pmodel/src/resource.h"
#include "app/data_management/archive_manager/pmodel/src/WriteArchivePModel.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"

namespace TA_Base_App
{
    class RetrieveCompleteDlg  : public TA_Base_Bus::AbstractDialog
    {
    public:
        /**
          * RetrieveCompleteDlg
          *
          * Standard constructor.
          *
          * @param      const TA_ArchiveManager::FileStatusMap& fileStatusMap
          *             Map of status of retrieval to associated log file.
          * @param      CWnd* pParent
          *             The parent window.
          */

        RetrieveCompleteDlg(CWnd* pParent = NULL);

        /**
          * ~RetrieveCompleteDlg
          *
          * Standard destructor.
          */

        virtual ~RetrieveCompleteDlg() {};

		virtual std::string getMyPModelName();
		virtual void setupPModel();

        // Dialog Data
	    //{{AFX_DATA(RetrieveCompleteDlg)
	    enum { IDD = IDD_RETRIEVE_COMPLETE_DIALOG };
	    CTreeCtrl	m_errorTree;
	    //}}AFX_DATA


        // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(RetrieveCompleteDlg)
	    protected:
	    virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	    //}}AFX_VIRTUAL

        // Implementation
    protected:

	    // Generated message map functions
	    //{{AFX_MSG(RetrieveCompleteDlg)
	    virtual BOOL OnInitDialog();
	    afx_msg void OnNo();
	    afx_msg void OnYes();
	    //}}AFX_MSG
	    DECLARE_MESSAGE_MAP()

    private:
        //
        // Disable copy constructor and assignment operator.
        //

		RetrieveCompleteDlg( const RetrieveCompleteDlg& );
		RetrieveCompleteDlg& operator=( const RetrieveCompleteDlg& );
        
        /**
          * insertItems
          *
          * Inserts the items with the given status into the tree under
          * a root node containing the given message.
          *
          * @param      TA_ArchiveManager::EErrorStatus status
          *             The status associated with the files to search for.
          * @param      const std::string& rootMsg
          *             The message to put in the root node for files with this status.
          */

        void insertItems( TA_ArchiveManager::EErrorStatus status, TCHAR* rootMsg );

        //
        // The map of status to log file name.
        //

        //const TA_ArchiveManager::FileStatusMap& m_fileStatusMap;

		WriteArchivePModel* m_realPModel;

    };

} // TA_Base_App

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RETRIEVECOMPLETEDLG_H__CD3F001A_8535_4EB9_A640_E932A0DE3F4C__INCLUDED_)
