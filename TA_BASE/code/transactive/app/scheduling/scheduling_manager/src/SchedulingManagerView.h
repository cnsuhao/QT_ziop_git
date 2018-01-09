/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/scheduling/scheduling_manager/src/SchedulingManagerView.h $
  * @author:  San Teo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * Displays the list of jobs that are known to the Scheduling Agent.  It will pass user requests related to 
  * jobs to the document object.
  */

#if !defined(AFX_SCHEDULINGMANAGERVIEW_H__880DE812_3F92_49CF_88F5_B0449BE82537__INCLUDED_)
#define AFX_SCHEDULINGMANAGERVIEW_H__880DE812_3F92_49CF_88F5_B0449BE82537__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/scheduling/scheduling_manager/plugin/model/src/Job.h"
#include "app/scheduling/scheduling_manager/plugin/model/src/JobManager.h"
#include "app/scheduling/scheduling_manager/plugin/model/src/MainFrameModel.h"
#include <vector>
#include <string>
#include "bus/mfc_extensions/src/auto_sort_list_ctrl/AutoSortListCtrl.h"
#include "bus/mfc_extensions/src/auto_sort_list_ctrl/ISortingCallback.h"
#include "bus/generic_gui_view/src/TACView.h"

namespace TA_Base_Core
{
    class IProfile;
    class ILocation;
}

namespace TA_Base_App
{
    class JobDialog;
    class Job;

    class CSchedulingManagerView :  public TA_Base_Bus::TACListView,
                                    public TA_Base_Bus::ISortingCallback
    {
    public: // create from serialization only
	    CSchedulingManagerView();
	    DECLARE_DYNCREATE(CSchedulingManagerView)

    // Operations
    public:

        /** 
          * getSelections
          *
          * Returns all jobs that are selected on the view
          *
          *
          * @return all jobs that are selected on the view
          */
        void getSelections(JobPtrVector& selections);
		bool getIsProfileFilterOn();
		void setMainFrameModel(MainFrameModel* pModel);

    // Overrides
	    // ClassWizard generated virtual function overrides
	    //{{AFX_VIRTUAL(CSchedulingManagerView)
	    public:
	    virtual void OnDraw(CDC* pDC);  // overridden to draw this view
	    virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
	    virtual void OnInitialUpdate();
	    virtual void OnUpdate(CView* pSender, LPARAM lHint, CObject* pHint);
		virtual void PostNcDestroy();
	    //}}AFX_VIRTUAL

    // Implementation
    public:
	    virtual ~CSchedulingManagerView();
        void initialise(TA_Base_Bus::ITransActiveWinApp * winApp, JobManager* jobManager);

    #ifdef _DEBUG
	    virtual void AssertValid() const;
	    virtual void Dump(CDumpContext& dc) const;
    #endif

    protected:

    // Generated message map functions
    protected:
	    //{{AFX_MSG(CSchedulingManagerView)
	    afx_msg void onViewJobdetails();
	    afx_msg void onJobAdd();
	    afx_msg void onJobDelete();
	    afx_msg void onJobDisable();
	    afx_msg void onJobEdit();
	    afx_msg void onJobEnable();
	    afx_msg void onItemchanged(NMHDR* pNMHDR, LRESULT* pResult);
	    afx_msg void onFilePrint();
	    //}}AFX_MSG
        afx_msg LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
        afx_msg LRESULT onCreateJob(WPARAM theJob, LPARAM);
        afx_msg LRESULT onUpdateJobList(WPARAM, LPARAM);
        afx_msg LRESULT onJobDeleted(WPARAM, LPARAM);
        afx_msg LRESULT onJobUpdated(WPARAM, LPARAM);
        afx_msg LRESULT onShowExceedMaxJobDlg(WPARAM, LPARAM);

		afx_msg void onViewAlljobs();
		afx_msg void onUpdateViewAlljobs(CCmdUI* pCmdUI);
		afx_msg void OnAppAbout();
		afx_msg void OnHelpSchedulingmanager();
        afx_msg LRESULT OnSetLanguage(WPARAM wParam, LPARAM lParam);

	    DECLARE_MESSAGE_MAP()

    private: // methods
		
        /**
         * sortCallback
         *
         * User defined sorting function
         *
         * @return positive value if item lParam2 before lParam1, negative if lParam1 should
         *      come first
         *
         * @param lParam1, lParam2 user defined values passed in from sort operation (app specific)
         * @param columnIndex the index of the column being sorted
         * @param currentSort defines the type of sorting specified (ie ascending or descending)
         */
        virtual int sortCallback(LPARAM lParam1, LPARAM lParam2, int columnIndex, TA_Base_Bus::AutoSortListCtrl::ECurrentSort currentSort);

		//TD16352++
		/** 
          * isJobEditLockedForSelection
          *
          * Checks if current selected job is edit locked
          *
          * @param jobId 
          */
        bool isJobEditLockedForSelection(const std::string& jobId);

		/**
		  * updateEditLock
		  *
          * Locks or releases job for edit preventing edits or removal while another operator is editing
          *
          * @param jobId
		  * @param lock 
          */
		void updateEditLock(const std::string& jobId, bool lock);
		//++TD16352


        /** 
          * enable
          *
          * Common method that will be used to enable/disable the selected items
          *
          * @param isEnable true if intended to enable the selected items
          */
        void enable(const bool isEnable);


        /** 
          * updateNumJobsProcessed
          *
          * Update the status on the jobs that being processed
          *
          * @param resourceId
          * @param numProcessed
          * @param totalNum
          *
          */
        void updateNumJobsProcessed(int resourceId, int numProcessed, int totalNum);


        /** 
          * updateNumJobsStatus
          *
          * Update the status on the number of jobs left in the list
          *
          */
        void updateNumJobsStatus();


        /** 
          * populateItem
          *
          * Populates the specified row with the details of the given job.
          * NOTE: This method doesn't insert the row, it only populates it.  
          *
          * @param index Where it is going to be inserted
          * @param job The job that is going to be displayed here
          *
          */
        void populateItem(int index, Job& job);

		bool canLoadDLL(); //TD16070
		//jinhua++ TD20487
		// C830 agreed to use geographical location as the primary sort followed by status, type and schedule start.
		int compareStringContent( int rowIndex1, int rowIndex2, int columnIndex );
		int sortByPrimaryAndSecondary( int rowIndex1, int rowIndex2 );
		//++jinhua TD20487

		JobPtrMap getAllJobs();
        unsigned long getJobCount();

        // A reference to the list control for efficiency so that we don't have to call 
        // GetListCtrl() each time it's needed
	    CListCtrl& m_listCtrl;
		int * m_printoutColWidths;//cl-16406-lkm
        std::vector<std::string> m_jobTypes;

        // Maintains the current sort column and order
        TA_Base_Bus::AutoSortListCtrl::SortInfo m_sortInfo;

        enum ColumnType
        {
            Status,
            Type, 
            Description,
            ScheduleStart,
            Recurrence,
            ScheduleEnd,
            Profile,
            Location,
            NumColumnType
        };

        // Maintains the information for each column
        struct ColumnInfo
        {
            ColumnType type;        // Column Type
            unsigned int width;     // The width of the column
            unsigned int name;      // The string Id in the resource
            TA_Base_Bus::AutoSortListCtrl::ESortType sortBy; // The way that the column is sorted by
        };

        // Contains the information regarding all columns
        static const ColumnInfo COLUMN_INFO[NumColumnType];

        // Using this flag to indicate whether we're in the process of updating the view
        // If it is, then don't tell the main frame of the change (otherwise onItemchange will
        // keep calling on the main frame and crashes)
        bool m_isUpdating;

        // Holds the job types that do not need specialised plugins for configuring that type of job
        std::vector<std::string> m_typesWithoutPlugin;

        // The tooltip control so we can set the display time of the tooltip
        CToolTipCtrl m_toolTip;

        // The common strings to be displayed on the list
        TAUnicodeString INHIBITED_STRING; 
        TAUnicodeString UNINHIBITED_STRING;
        TAUnicodeString RECUR_NONE_STRING;
        TAUnicodeString RECUR_DAILY_STRING;
        TAUnicodeString RECUR_WEEKLY_STRING;
        TAUnicodeString RECUR_MONTHLY_STRING;
        TAUnicodeString RECUR_YEARLY_STRING;

		JobManager* m_jobManager;
		TA_Base_Bus::ITransActiveWinApp* m_pWinApp;

		bool m_isProfileFilterOn;
		MainFrameModel* m_pMainFrmModel;
    };

    #ifndef _DEBUG  // debug version in SchedulingManagerView.cpp
    //inline CSchedulingManagerDoc* CSchedulingManagerView::GetDocument()
    //   { return (CSchedulingManagerDoc*)m_pDocument; }
    #endif

};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_SCHEDULINGMANAGERVIEW_H__880DE812_3F92_49CF_88F5_B0449BE82537__INCLUDED_)
