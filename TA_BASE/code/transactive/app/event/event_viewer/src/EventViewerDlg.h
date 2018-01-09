/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/event/event_viewer/src/EventViewerDlg.h $
 * @author:  Nick Jardine
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2015/10/09 15:42:35 $
 * Last modified by:  $Author: CM $
 * 
 * EventViewerDlg is the main dialog class. This is the main window of the application, and is the container that holds the 
 * Event List Component.
 */

#if !defined(AFX_EVENTVIEWERDLG_H__482E140A_F7F4_40DF_8499_9F47F3114230__INCLUDED_)
#define AFX_EVENTVIEWERDLG_H__482E140A_F7F4_40DF_8499_9F47F3114230__INCLUDED_


#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "app/event/event_viewer/src/StatusProgressBar.h"
#include "app/event/event_viewer/EventListComponent/src/EventListComponent.h"
#include "app/event/event_viewer/src/MultipleLoadDlg.h"

#include  "app/event/event_viewer/pmodel/src/EventViewerDlgPModel.h"

#include "bus/generic_gui_view/src/TransActiveDialog.h"

#include "bus/mfc_extensions/src/combo_temporary_item/ComboTempItem.h"

#include <map>

/////////////////////////////////////////////////////////////////////////////
// EventViewerDlg dialog

class FilterDlg;    //forward declarations

class EventViewerDlg : public TA_Base_Bus::AbstractTransActiveDialog
{
// Construction
public:
	EventViewerDlg(TA_Base_Bus::ITransActiveWinApp * app);	// standard constructor
	virtual ~EventViewerDlg(); //zhiqiang++

// Dialog Data
	//{{AFX_DATA(EventViewerDlg)
	enum { IDD = IDD_EVENTVIEWER_DIALOG };
	CButton	m_pageDown;
	CButton	m_pageUp;
	EventListComponent	m_eventList;
	
	//}}AFX_DATA

	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(EventViewerDlg)
	

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV support
	virtual BOOL OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult);
	virtual void PreSubclassWindow();
	//}}AFX_VIRTUAL

// Implementation
protected:
	HICON m_hIcon;

	// Generated message map functions
	//{{AFX_MSG(EventViewerDlg)
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnClose();
	afx_msg void OnFileExit();
	afx_msg void OnViewFilters();
	afx_msg void OnStatusBar();
	afx_msg void OnToolBar();
    afx_msg void OnShowAll();
	afx_msg void OnAppAbout();
    afx_msg void OnHelp();
	afx_msg void OnPageUp();
	afx_msg void OnPageDown();	
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus); //TD17143
	//}}AFX_MSG
	//zhiqiang++begin
	afx_msg LRESULT OnPopulateEventLevels(WPARAM,LPARAM);
	afx_msg LRESULT OnApplyRights(WPARAM,LPARAM);
	//zhiqiang++end
    afx_msg LRESULT  OnListUpdate(WPARAM,LPARAM);
    afx_msg LRESULT  OnLoadListCount(WPARAM,LPARAM);
    afx_msg LRESULT  OnLoadListUpdate(WPARAM,LPARAM);
	afx_msg LRESULT  OnListViewUpdate(WPARAM,LPARAM);
    afx_msg LRESULT  OnFilterDlgClose(WPARAM,LPARAM);
    afx_msg LRESULT  OnFilterApply(WPARAM,LPARAM);
    afx_msg LRESULT  OnFilterDlgCancel(WPARAM,LPARAM);
    afx_msg LRESULT  OnNamedFilterChange(WPARAM,LPARAM);
    afx_msg LRESULT  OnListItemSelect(WPARAM wParam,LPARAM lParam);
    afx_msg LRESULT  OnCloseMainWindow(WPARAM,LPARAM);
	afx_msg LRESULT OnSetupEventFiltersAndRights(WPARAM,LPARAM); //TD17143
	//afx_msg LRESULT OnSetupDatabaseConnectionString(WPARAM,LPARAM);
	afx_msg LRESULT OnSetLanguage(WPARAM wParam, LPARAM lParam);
	afx_msg void OnMenuSelect(UINT nItemID, UINT nFlags, HMENU hSysMenu);
    afx_msg void OnNext();
    afx_msg void OnPrevious();
    afx_msg void OnPrint();
    afx_msg void OnExport();
    afx_msg void OnAlarmManager();
    afx_msg void OnIncidentReport();
	afx_msg void OnSelchangeComboNamedFilter();
    afx_msg void OnSelchangeComboEventLevel();
    DECLARE_MESSAGE_MAP()


	////AbstractTransActiveDialog Interface methods
	//interface virtual method
    virtual std::string getMyPModelName();
    void setupPModel();

	virtual void initUIControl();
	
	virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL);


public:

	void init();
	
	/**
     * readyToClose
     *
     * Determines if the event list is in a state that is ready to close and 
	 * wait till it becomes ready to close state.     
     * @return 
     */
    void readyToClose();

 
    /**
     * initialiseWindowOffsets
     *
     * Initialises the offsets that are used when the dialog is resized. 
     *
     */
    void initialiseWindowOffsets();

	void clearSortMark();

	std::vector<TA_Base_App::ATSSystemItem> getAtsSystemsVector();
	std::vector<ISCSSubsystemItem> getISCSSubsystemVector();
	void setNamedFilter(TA_Base_Core::NamedFilterAccessFactory::FilterNameList filterList);
	TA_Base_Core::NamedFilterAccessFactory::FilterNameList getNamedFilter();


private:

	void InitPagerCtrl();


    /**
     * setUpStatusBar
     *
     * Creates and positions the status bar, and re-adjusts the main window dimensions to account for it.
     */
    void setUpStatusBar();

	void setUpPager();

    /**
     * setUpToolBar
     *
     * Creates and positions the tool bar and pager control that contains it.
     */
    void setUpToolBar();

    /**
     * setUpMenu
     *
     * Sets default menu checks, and adds the default columns menu
     */
    void setUpMenu();

    /**
     * setWindowCaption
     *
     * Sets the window caption depending on the current filter.
     */
    void setWindowCaption();

	void InitEventListCtrl();

	void populateNamedFilters();

	void showAllEventsMenu(bool showAllEventsEnabled);

	void PostMessageToUIThread(UINT messageId, WPARAM wParam, LPARAM lParam);

	void getDisplayStringFromRes();

	void DeleteToolBarCtrl();

    // The status bar member variable
    StatusProgressBar m_statusBar;

    // The image lists for the toolbar - enabled and "hot" (mouse over)
    CImageList m_eventsTbImageListEnabled;
    CImageList m_eventsTbImageListHot;

    // The toolbar and the pager that contains it
    CToolBarCtrl * m_eventsToolbar;
    CWnd m_Pager;
	CRect m_pagerRect;

    // Timer ID used with the standard application timer
    unsigned int m_nIDTimer;

    // Offests of the list from the top bottom and sides of the main window
    unsigned int m_listWidthOffset;
    unsigned int m_listTopOffset;
    unsigned int m_listBottomOffset;

    // The filter dialog. Will either be NULL or have a valid filter dialog
    FilterDlg* m_filterDlg;

    bool m_progressBarExists;


    // The combo box for storing named filters.
    TA_Base_Bus::ComboTempItem m_namedFiltersComboBox;

    // The controls for selecting event levels.
    CStatic m_eventLevelsStatic;
    CComboBox m_eventLevelsComboBox;   

	EventViewerDlgPModel* m_realPModel;
	HWND m_mainViewHandle;


};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_EVENTVIEWERDLG_H__482E140A_F7F4_40DF_8499_9F47F3114230__INCLUDED_)
