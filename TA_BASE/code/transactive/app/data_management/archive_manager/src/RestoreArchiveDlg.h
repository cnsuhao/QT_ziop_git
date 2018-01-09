/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/data_management/archive_manager/src/RestoreArchiveDlg.h $
  * @author:  Alan Brims
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  *
  * Dialog for operator to select the archive files to be restored.
  *
  */

#if !defined(AFX_RESTOREARCHIVEDLG_H__3DA15816_CA7B_43CB_A66C_A735EBE85849__INCLUDED_)
#define AFX_RESTOREARCHIVEDLG_H__3DA15816_CA7B_43CB_A66C_A735EBE85849__INCLUDED_



#include "app/data_management/archive_manager/pmodel/src/resource.h"
#include "app/data_management/archive_manager/src/RestoredArchiveListComponent.h"
#include "app/data_management/archive_manager/pmodel/src/RestoreArchivePModel.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

    
namespace TA_Base_App
{
    class RestoreArchiveDlg : public TA_Base_Bus::AbstractDialog
    {
    public:
        /**
          * RestoreArchiveDlg
          *
          * Standard constructor.
          *
          * @param      CWnd* pParent
          *             The parent window.
          */

        RestoreArchiveDlg(CWnd* pParent = NULL);

        /**
          * ~RestoreArchiveDlg
          *
          * Standard destructor.
          */

        virtual ~RestoreArchiveDlg();
		
		virtual std::string getMyPModelName();
		virtual void setupPModel();
		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args);

        // Dialog Data
        //{{AFX_DATA(RestoreArchiveDlg)
	enum { IDD = IDD_RESTORE_ARCHIVE_DIALOG };
	    CComboBox	m_selectedDriveCombo;
	    CButton	m_closeButton;
	    CButton	m_restoreButton;
	    CButton	m_cancelButton;
	    CButton	m_selectAllButton;
        //CListCtrl	m_archiveList;
		RestoredArchiveListComponent m_archiveList;
	//}}AFX_DATA
        
        
        // Overrides
        // ClassWizard generated virtual function overrides
        //{{AFX_VIRTUAL(RestoreArchiveDlg)
	protected:
        virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support
	    //}}AFX_VIRTUAL
        
        // Implementation
    protected:
        
        // Generated message map functions
        //{{AFX_MSG(RestoreArchiveDlg)
        virtual BOOL OnInitDialog();
	    afx_msg void OnButtonSelectAll();
	    afx_msg void OnRestore();
	    virtual void OnCancel();
	    afx_msg void OnItemChangedArchiveList(NMHDR* pNMHDR, LRESULT* pResult);
		afx_msg void OnClose();
		afx_msg void OnSelchangeDriveCombo();
		//}}AFX_MSG
	    afx_msg LRESULT OnShowCompleteDlg(WPARAM, LPARAM);
        DECLARE_MESSAGE_MAP()

    private:

        //
        // Disable copy constructor and assignment operator.
        //

		RestoreArchiveDlg( const RestoreArchiveDlg& );
		RestoreArchiveDlg& operator=( const RestoreArchiveDlg& );

		/**
          * populateList
          *
          * Populates the archive list from the CD.
          */

        void populateList();

        /**
          * disableAllButCancelButton
          *
          * Disables everything except the Cancel button and
          * sets the focus to the Cancel button.
          */

        void disableAllButCancelButton();
    
        /**
          * resetButtons
          *
          * Resets the buttons to their original configuration.
          */

        void resetButtons();

        /**
          * enableCloseButton
          *
          * Hides all the other buttons and shows and enables
          * the Close button.
          */

        void enableCloseButton();
		
		/**
          * getTickedItems
          *
          * Gets the ticked items from the list.  These will determine
          * which dates are restored and the volume label on the CD
          * that will be used in the AR_INVENTORY table.
          *
          * @return     bool
          *             True if the process runs to completion.
          *             False if the user cancels midway through.
          *
          * @param      RestoreArchiveHelper::DatesIndexMap& restoredDatesMap
          *             Will be populated with the dates to restore in chronological order.
          * @param      std::string& volumeLabel
          *             Will be populated with the volume label to use in the AR_INVENTORY table.
          */
           
        bool getTickedItems( RestoreArchiveHelper::DatesIndexMap& restoredDatesMap, TAUnicodeString& volumeLabel );

		
		/**
		* restoreFiles
		*
		* Restores the files listed in the vector.  This includes adding an entry
		* to the AR_INVENTORY table.  The archives are restored in date order.
		* If the user cancels during the operation, the rest of the files
		* for the date that is being restored will be restored and then this method
		* will return.  This prevents data corruption.
		*
		* @return     bool
		*             True if the process runs to completion.
		*             False if the user cancels midway through.
		*
		* @param      const RestoreArchiveHelper::DatesIndexMap& restoredDatesMap
		*             The dates to restore in chronological order.
		* @param      const std::string& volumeLabel
		*             The volume label to use in the AR_INVENTORY table.
		*/

		bool restoreFiles( const RestoreArchiveHelper::DatesIndexMap& restoredDatesMap, const TAUnicodeString& volumeLabel );

		/**
		* processLogFiles
		*
		* If the log files from the restoration process indicate that
		* the process was successful, this method deletes the log files.
		* Otherwise it displays a Restore Complete dialog listing all
		* the files that contain problems.  The user is then
		* responsible for deleting the log files.
		*
		* Any attempts to Cancel by the user during this method will be
		* ignored (or at least a message box will be displayed, and then
		* the method will continue).
		*/
		void processLogFiles();


		void runThread();

		//
        // Used to disable the list ctrl updating until after the list has been populated.
        //

        bool m_isListPopulated;

		//
        // Will be populated with the archive files and their status'.
        //

        RestoreArchiveHelper::FileDetailsVector m_fileDetails;
		
		
		//
        // The value to return which records whether the user has
        // cancelled or if the process failed or if the process was successful.
        //

        int m_returnValue;

		RestoreArchivePModel* m_realPModel;

    };
    
} // TA_Base_App

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_RESTOREARCHIVEDLG_H__3DA15816_CA7B_43CB_A66C_A735EBE85849__INCLUDED_)
