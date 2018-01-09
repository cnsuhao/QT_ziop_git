/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/event/event_viewer/src/EventViewerDlg.cpp $
 * @author:  Nick Jardine
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2015/10/09 15:42:35 $
 * Last modified by:  $Author: CM $
 * 
 * EventViewer is the TransactiveWinApp derived main application class. This is the class that bootstraps the application.
 */

#pragma warning(disable:4800)       // 'long': forcing value to bool - this warning is generated for legitimate static casts

#include <afxpriv.h>    // Included for the common controls
#include <math.h>       // Included for log10
#include <sstream>
#include <sys/timeb.h>
#include <algorithm>
#include "app/event/event_viewer/src/stdafx.h"
#include "app/event/event_viewer/src/EventViewerDlg.h"
#include "app/event/event_viewer/src/EventViewerGUIConstants.h"
#include  "app/event/event_viewer/pmodel/src/ConstantItems.h"
#include "app/event/event_viewer/src/FilterDlg.h"
#include "app/event/event_viewer/src/EventViewer.h"
#include "app/event/event_viewer/src/CombinedEventFilterDlg.h"
#include "app/event/event_viewer/src/ATSComWrapper.h"

#include "app/event/event_viewer/pmodel/src/GenericEventArgs.h"
#include "app/event/event_viewer/pmodel/src/IATSComWrapper.h"

#include "bus/generic_gui_view/src/TransActiveMessage.h"
//TD17888 marc_bugfix for save as dialog
#include "bus/mfc_extensions/src/file_dialog_no_folder_change/FileDialogArialDisableFolders.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"

#include "core/data_access_interface/entity_access/src/EventViewer.h"
#include "core/data_access_interface/src/NamedFilterAccessFactory.h"
#include "core/exceptions/src/DataException.h"
#include "core/utilities/src/DebugUtil.h"


using TA_Base_Core::DebugUtil;
using TA_Base_Core::NamedFilterAccessFactory;
using TA_Base_Core::DataException;

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// EventViewerDlg dialog
EventViewerDlg::EventViewerDlg(TA_Base_Bus::ITransActiveWinApp * app)
    : TA_Base_Bus::AbstractTransActiveDialog(app, EventViewerDlg::IDD, NULL),
	m_realPModel(NULL),m_filterDlg(NULL),m_eventsToolbar(NULL),
	m_progressBarExists(false),m_listWidthOffset(0),m_listTopOffset(0),m_listBottomOffset(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_mainViewHandle = NULL;

    // Set the resizing properties
	TA_Base_Bus::ViewResizeProperty properties;
	properties.canMaximise = true;
	properties.maxHeight = -1;
	properties.maxWidth = -1;
	properties.minHeight = STANDARD_MIN_HEIGHT;
	properties.minWidth = STANDARD_MIN_WIDTH;
	m_resizeViewHelper->setViewResizeProperty(properties);

	CoInitialize(0);
}

EventViewerDlg::~EventViewerDlg()
{
	//IATSComWrapper::freeInstanceRealTime();
	//IATSComWrapper::freeInstanceHistorical();
	EVDisplayStrRes::ReleaseInstance();
	DeleteToolBarCtrl();
	CoUninitialize();
}


void EventViewerDlg::DoDataExchange(CDataExchange* pDX)
{
	AbstractTransActiveDialog::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(EventViewerDlg)
	DDX_Control(pDX, IDC_PAGE_DOWN, m_pageDown);
	DDX_Control(pDX, IDC_PAGE_UP, m_pageUp);
	DDX_Control(pDX, IDC_EVENT_LIST, m_eventList);
	//}}AFX_DATA_MAP
}

BEGIN_MESSAGE_MAP(EventViewerDlg, AbstractTransActiveDialog)
	//{{AFX_MSG_MAP(EventViewerDlg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_WM_CLOSE()
	ON_COMMAND(ID_FILE_EXIT, OnFileExit)
	ON_COMMAND(IDC_VIEW_FILTERS, OnViewFilters)
	ON_COMMAND(IDC_STATUS_BAR, OnStatusBar)
	ON_COMMAND(IDC_TOOL_BAR, OnToolBar)
    ON_COMMAND(IDC_SHOW_ALL, OnShowAll)
	ON_COMMAND(ID_APP_ABOUT, OnAppAbout)
    ON_COMMAND(ID_HELP, OnHelp)
	ON_WM_MENUSELECT()
	ON_BN_CLICKED(IDC_PAGE_UP, OnPageUp)
	ON_BN_CLICKED(IDC_PAGE_DOWN, OnPageDown)
	ON_WM_SHOWWINDOW()
	//}}AFX_MSG_MAP
	//zhiqiang++begin
	ON_MESSAGE(WM_USER_POPULATE_EVENT_LEVELS, OnPopulateEventLevels)
	ON_MESSAGE(WM_USER_APPLY_RIGHTS, OnApplyRights) 
    //zhiqiang--end
    ON_MESSAGE(WM_USER_LIST_UPDATE,OnListUpdate)
	ON_MESSAGE(WM_USER_LISTVIEW_UPDATE,OnListViewUpdate)
    ON_MESSAGE(WM_USER_LOAD_COUNT,OnLoadListCount)
    ON_MESSAGE(WM_USER_LOAD_UPDATE,OnLoadListUpdate)
    ON_MESSAGE(WM_USER_FILTER_DLG_CLOSED,OnFilterDlgClose)
    ON_MESSAGE(WM_USER_FILTER_DLG_APPLY,OnFilterApply)
    ON_MESSAGE(WM_USER_FILTER_DLG_CANCEL,OnFilterDlgCancel)
    ON_MESSAGE(WM_USER_FILTER_DLG_NAMED_FILTER_CHANGE,OnNamedFilterChange)
    ON_MESSAGE(WM_USER_CLOSE_MAIN_WINDOW,OnCloseMainWindow)
    ON_MESSAGE(WM_USER_LIST_SELECTION_UPDATE,OnListItemSelect)
    ON_COMMAND(IDC_NEXT, OnNext)
    ON_COMMAND(IDC_PREVIOUS, OnPrevious)
    ON_COMMAND(IDC_PRINT, OnPrint)
    ON_COMMAND(IDC_EXPORT, OnExport)
    ON_COMMAND(IDC_ALARM_MANAGER, OnAlarmManager)
    ON_COMMAND(IDC_VIEW_INCIDENTS, OnIncidentReport)
	ON_CBN_SELCHANGE(ID_NAMED_FILTER_COMBO, OnSelchangeComboNamedFilter)
    ON_CBN_SELCHANGE(ID_EVENT_LEVEL_COMBO, OnSelchangeComboEventLevel)
	ON_MESSAGE(WM_USER_UPDATE_EVENTFILTERS_RIGHTS, OnSetupEventFiltersAndRights) //TD17143
	//ON_MESSAGE(WM_USER_UPDATE_DATABASECONNECTION_STRING, OnSetupDatabaseConnectionString)
	ON_MESSAGE(WM_TASETLANG, OnSetLanguage)
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// EventViewerDlg message handlers

BOOL EventViewerDlg::OnInitDialog()
{
    FUNCTION_ENTRY("OnInitDialog");
	AbstractTransActiveDialog::OnInitDialog();
	
	initialiseWindowOffsets();

    // Set the icon for this dialog.  The framework does this automatically
    //  when the application's main window is not a dialog
    SetIcon(m_hIcon, TRUE);			// Set big icon
    SetIcon(m_hIcon, FALSE);		// Set small icon

	GetDlgItem(IDC_PAGER_CTRL)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PAGE_UP)->ShowWindow(SW_HIDE);
	GetDlgItem(IDC_PAGE_DOWN)->ShowWindow(SW_HIDE);	

    FUNCTION_EXIT;
    return TRUE;
}

void EventViewerDlg::init()
{
	IATSComWrapper::setInstanceRealTime(new ATSComWrapper());
	IATSComWrapper::setInstanceHistorical(new ATSComWrapper());
	m_mainViewHandle = AfxGetMainWnd()->GetSafeHwnd(); //for post message purpose
	getDisplayStringFromRes();
	m_realPModel->init();
}

void EventViewerDlg::getDisplayStringFromRes()
{
	EVDisplayStrRes::GetInstance()->COLUMN_DATETIME = TALoadString(IDS_COLUMN_DATETIME_STRING).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->COLUMN_SEVERITY = TALoadString(IDS_COLUMN__SEVERITY_STRING).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->COLUMN_ASSET = TALoadString(IDS_COLUMN_ASSET_STRING).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->COLUMN_ALARM_STATUS = TALoadString(IDS_COLUMN_ALARM_STATUS_STRING).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->COLUMN_DESCRIPTION = TALoadString(IDS_COLUMN_DESCRIPTION_STRING).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->COLUMN_VALUE = TALoadString(IDS_COLUMN_VALUE_STRING).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->COLUMN_T = TALoadString(IDS_COLUMN_T_STRING).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->COLUMN_MMS = TALoadString(IDS_COLUMN_MMS_STRING).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->COLUMN_DSS = TALoadString(IDS_COLUMN_DSS_STRING).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->COLUMN_AVL = TALoadString(IDS_COLUMN_AVL_STRING).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->COLUMN_OPERATOR = TALoadString(IDS_COLUMN_OPERATOR_STRING).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->COLUMN_COMMENT = TALoadString(IDS_COLUMN_COMMENT_STRING).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->STATUS_ATS_QUERYING = TALoadString(IDS_STATUS_ATS_QUERYING_STR).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->STATUS_ISCS_QUERYING = TALoadString(IDS_STATUS_ISCS_QUERYING).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->STATUS_ATS_READY = TALoadString(IDS_STATUS_ATS_READY).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->STATUS_ISCS_READY = TALoadString(IDS_STATUS_ISCS_READY).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->STATUS_ATS_UNHEALTHY = TALoadString(IDS_STATUS_ATS_UNHEALTHY).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->STATUS_ISCS_UNHEALTHY = TALoadString(IDS_STATUS_ISCS_UNHEALTHY).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->STATUS_ATS_DISABLED = TALoadString(IDS_STATUS_ATS_DISABLED).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->STATUS_ICSC_DISABLED= TALoadString(IDS_STATUS_ICSC_DISABLED).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->USER_DEFINED_NAMED_FILTER_NAME = TALoadString( IDS_USER_DEFINED_NAMED_FILTER_STR).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->ALARM_FILTER_MESSAGE = TALoadString(IDS_ALARM_FILTER_MESSAGE).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->ERROR_LOAD = TALoadString(IDS_ERROR_LOAD_STR).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->ERROR_LOAD = TALoadString(IDS_ERROR_LOADED_STR).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->ERROR_LOADED = TALoadString(IDS_ERROR_LOADED_STR).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->HIDDEN_COLUMN_ALARMID = TALoadString(IDS_HIDDEN_COLUMN_ALARMID).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->HIDDEN_COLUMN_STATUS = TALoadString(IDS_HIDDEN_COLUMN_STATUS).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->HIDDEN_COLUMN_ACKED = TALoadString(IDS_HIDDEN_COLUMN_ACKED).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->FILTER_APPLICATION_SUBSYSTEM = TALoadString(IDS_FILTER_APPLICATION_SUBSYSTEM).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->FILTER_PHYSICAL_SUBSYSTEM = TALoadString(IDS_FILTER_PHYSICAL_SUBSYSTEM).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->FILTER_SYSTEM_STR = TALoadString(IDS_FILTER_SYSTEM_STR).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->FILTER_SUBSYSTEM_STR = TALoadString(IDS_FILTER_SUBSYSTEM_STR).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->FILTER_LOCATION = TALoadString(IDS_FILTER_LOCATION).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->FILTER_OPERATOR = TALoadString(IDS_FILTER_OPERATOR).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->FILTER_OPERATOR_STR = TALoadString(IDS_FILTER_OPERATOR_STR).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->FILTER_LOCATION_STR = TALoadString(IDS_FILTER_LOCATION_STR).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->FILTER_EVENTTYPE = TALoadString(IDS_FILTER_EVENTTYPE).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->FILTER_ISCS_SYSTEM = TALoadString(IDS_FILTER_ISCS_SYSTEM).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->FILTER_TMTYPE = TALoadString(IDS_FILTER_TMTYPE).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->FILTER_ALARMSTATE = TALoadString(IDS_FILTER_ALARMSTATE).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->FILTER_ASSET = TALoadString(IDS_FILTER_ASSET).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->FILTER_DESCRIPTION = TALoadString(IDS_FILTER_DESCRIPTION).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->FILTER_SEVERITY = TALoadString(IDS_FILTER_SEVERITY).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->FILTER_ALARMTYPE = TALoadString(IDS_FILTER_ALARMTYPE).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->FILTER_ALARMID = TALoadString(IDS_FILTER_ALARMID).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->NAMEDFILTER_DEFAULT = TALoadString(IDS_NAMEDFILTER_DEFAULT).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->NAMEDFILTER_AVALANCHE = TALoadString(IDS_NAMEDFILTER_AVALANCHE).toMultiByteStdString();
	EVDisplayStrRes::GetInstance()->PRINT_HEADER_STR = TALoadString(IDS_PRINT_HEADER_STRING).toMultiByteStdString();
}

void EventViewerDlg::InitEventListCtrl()
{
	m_eventList.SetExtendedStyle(LVS_EX_FULLROWSELECT       // Enable selecting a whole row when clicking anywhere in it
			| LVS_EX_HEADERDRAGDROP     // Enable moving the order of columns
			| LVS_EX_INFOTIP            // Enable tool tips when column text is too wide
			| LVS_EX_GRIDLINES|0x00010000 );       // Show gridlines.
	
		// Set the filter in the list component
		m_eventList.setFilter(); 

		// Set the colour coding behaviour
		m_eventList.setColourCoding(m_realPModel->getColorCodingEnabled());
		setWindowCaption();
		m_realPModel->getFilter()->setFilterID(IDD_COMBINED_EVENT_FILTER);
}

void EventViewerDlg::InitPagerCtrl()
{
		// Call this to set up the common controls - in this case Pager and CreateDialog
		INITCOMMONCONTROLSEX icex;
		icex.dwSize = sizeof(INITCOMMONCONTROLSEX);
		icex.dwICC = ICC_PAGESCROLLER_CLASS|ICC_BAR_CLASSES;
		::InitCommonControlsEx(&icex);

		GetDlgItem(IDC_PAGER_CTRL)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PAGE_UP)->ShowWindow(SW_SHOW);
		GetDlgItem(IDC_PAGE_DOWN)->ShowWindow(SW_SHOW);	

}

//TD17143 
void EventViewerDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	// call invalidate so that the list will be repainted
	CRect viewerRect;
    GetWindowRect(&viewerRect);
    ScreenToClient(&viewerRect);
	PostMessage(WM_SIZE,viewerRect.Height()-1, viewerRect.Width()-1 );
}

// If you add a minimize button to your dialog, you will need the code below
//  to draw the icon.  For MFC applications using the document/view model,
//  this is automatically done for you by the framework.

void EventViewerDlg::OnPaint()
{
    FUNCTION_ENTRY("OnPaint");
    if (IsIconic())
    {
        CPaintDC dc(this); // device context for painting

        SendMessage(WM_ICONERASEBKGND, (WPARAM) dc.GetSafeHdc(), 0);

        // Center icon in client rectangle
        int cxIcon = GetSystemMetrics(SM_CXICON);
        int cyIcon = GetSystemMetrics(SM_CYICON);
        CRect rect;
        GetClientRect(&rect);
        int x = (rect.Width() - cxIcon + 1) / 2;
        int y = (rect.Height() - cyIcon + 1) / 2;

        // Draw the icon
        dc.DrawIcon(x, y, m_hIcon);
    }
    else
    {
        AbstractTransActiveDialog::OnPaint();
    }
    FUNCTION_EXIT;
}

// The system calls this to obtain the cursor to display while the user drags
//  the minimized window.
HCURSOR EventViewerDlg::OnQueryDragIcon()
{
    FUNCTION_ENTRY("OnQueryDragIcon");
    FUNCTION_EXIT;
    return (HCURSOR) m_hIcon;
}

void EventViewerDlg::initialiseWindowOffsets()
{
    // TD2038: The code in this method was taken out of OnSize() because it
    // needs to be called prior OnSize() being called from a Control Station call.

    // Get the main window rectangle - it will be used when resizing most of the components
    CRect window;        
    GetWindowRect(&window);
    ScreenToClient(&window);

    CWnd* listCtrl = GetDlgItem(IDC_EVENT_LIST);
    if (NULL != listCtrl)
    {
        // Get the rectangle for the list control
        CRect list;
        listCtrl->GetWindowRect(&list);
        ScreenToClient(&list);

        if (0 == m_listWidthOffset && 0 == m_listTopOffset && 0 == m_listBottomOffset)
        {
            // if all three variables are 0, then the list control has not yet been adjusted

            // need to work out the offsets so they can be maintained, no matter how
            // the dialog is resized
            m_listWidthOffset = list.left - window.left;
            m_listTopOffset = list.top - window.top;
            m_listBottomOffset = window.bottom - list.bottom;
        }
    }
}


// Called when the viewer is re-sized
void EventViewerDlg::OnSize(UINT nType, int cx, int cy) 
{
    FUNCTION_ENTRY("OnSize");
    // Let the CDialog base class do any re-sizing processing it needs
	AbstractTransActiveDialog::OnSize(nType, cx, cy);

    // Get the main window rectangle - it will be used when resizing most of the components
    CRect window;        
    GetWindowRect(&window);
    ScreenToClient(&window);


    // Start by resizing the menu line
    CWnd* menuLine = GetDlgItem(IDC_MENU_LINE);
    if (NULL != menuLine)
    {
        // Get the rectangle for the menu line
        CRect line;
        menuLine->GetWindowRect(&line);
        ScreenToClient(&line);

        // Don't care about it's height - it's just a line. Set the line width bounds to the same as the main applicaiton window
        line.right = window.right;
        line.left = window.left;
        menuLine->MoveWindow(&line);
    }

    // Resize the event list
    CWnd* listCtrl = GetDlgItem(IDC_EVENT_LIST);
    if (NULL != listCtrl)
    {
        // Get the rectangle for the list control
        CRect list;
        listCtrl->GetWindowRect(&list);
        ScreenToClient(&list);

        // If the status bar is not visible, want to reduce the bottom offset
        unsigned int bottomOffset = m_listBottomOffset;
        if (NULL != m_statusBar.GetSafeHwnd())        
        {
            // Fix for TD2038: Check if the status bar is NULL before calling IsWindowVisible()
            if(!m_statusBar.IsWindowVisible())
            {
                bottomOffset = OFFSET_NO_STATUSBAR;
            }
            else
            {
                CRect status;
                m_statusBar.GetWindowRect(&status);
                ScreenToClient(&status);

                bottomOffset = m_listBottomOffset + status.Height();
            }
        }
        
        // If the tool bar is not visible, want to reduce the top offset
        unsigned int topOffset = m_listTopOffset;
        if (NULL != m_eventsToolbar->GetSafeHwnd() && !m_eventsToolbar->IsWindowVisible())            
        {
            topOffset = OFFSET_NO_TOOLBAR;
        }

        // And now resize the list to the offsets
        list.left = window.left + m_listWidthOffset;
        list.right = window.right - m_listWidthOffset;
        list.bottom = window.bottom - bottomOffset;
        list.top = window.top + topOffset;

        listCtrl->MoveWindow(list);
    }


    // Resize the pager control (with the toolbar in it)

    if(IsWindow(m_eventsToolbar->GetSafeHwnd()))
	{
        // The value provided by cx is not current as the left value seems
        // to be negative... manually calculate it instead.
        int realx = (( window.left < 0 )?-window.left:window.left) + window.right;

        // Get the rectangle fo the pager control
	    CRect pagerRect;
	    m_Pager.GetWindowRect(pagerRect);
	    ScreenToClient(pagerRect);

        // Reposition the page up and page down buttons.
        CRect pageUpRect;
        m_pageUp.GetWindowRect(pageUpRect);
        ScreenToClient(pageUpRect);

        pageUpRect.right = realx - 23;
        pageUpRect.left = pageUpRect.right - 26;
	    m_pageUp.MoveWindow(pageUpRect);
        
        CRect pageDownRect;
        m_pageDown.GetWindowRect(pageDownRect);
        ScreenToClient(pageDownRect);

        pageDownRect.right = realx - 23;
        pageDownRect.left = pageDownRect.right - 26;
        m_pageDown.MoveWindow(pageDownRect); 
        
        // Set the right limit of the pager (the left will always remain the same)
	    pagerRect.right = realx - 50;	    
	    m_Pager.MoveWindow(pagerRect);
    }
    
	// Resizing the status bar is automatically handled by the RepositionBars command
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST, 0);

    FUNCTION_EXIT;
}

// OnNotify is called when when a windows common control wants to send a message to the controling application
BOOL EventViewerDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult) 
{
    // This #define allows access to the header part of the lParam
    #define lpnm   ((LPNMHDR)lParam)


    if(lpnm->code == PGN_CALCSIZE)  // This message is requesting the size of the window (toolbar) contained in the pager
	{
        // cast the lParam to a pointer of the MPGCALCSIZE object
	    LPNMPGCALCSIZE   pCalcSize = (LPNMPGCALCSIZE)lParam;	

        // Determine the number of buttons contained in the toolbar
	    int buttonCount;		
		buttonCount = m_eventsToolbar->GetButtonCount();

		// Get the bonding rectangle for the far right toolbar button
	    CRect buttonRect;
		m_eventsToolbar->GetItemRect(buttonCount - 1, buttonRect);

        // Determine if the control is requesting hieght of width information
		switch(pCalcSize->dwFlag)
		{
		case PGF_CALCWIDTH:                                 // Width
			pCalcSize->iWidth = buttonRect.right;           // Set the width to the right hand side of the far-right button
			return 0;
			//break;

		case PGF_CALCHEIGHT:                                // Height
			pCalcSize->iHeight = buttonRect.bottom;         // Set the height to the bottom of the button
			return 0;
			//break;			
		}

		return FALSE;                                       // Unknown message request, so return FALSE (i.e. not valid data)
	}
    else if(lpnm->code == PGN_SCROLL)   // This message comes from the pager, and allows the application to set how far to scroll
    {
        LPNMPGSCROLL   pScroll = (LPNMPGSCROLL)lParam;

        switch(pScroll->iDir)
        {
        case PGF_SCROLLLEFT:
        case PGF_SCROLLRIGHT:
        case PGF_SCROLLUP:
        case PGF_SCROLLDOWN:
            pScroll->iScroll = 15;     // For any scroll direction, scroll 15 pixels at a time.
            break;
        }
    }
    else if (lpnm->code == TTN_GETDISPINFO) // This message comes from the toolbar when the mouse has "floated" over a button
    {                                       // for longer than the timeout period, and it wants to diaplay a tooltip.
        // Return the text associated with the currently "hot" button
        LPNMTTDISPINFO pDispInfo = (LPNMTTDISPINFO)lParam;

        // The NMTTDISPINFO struct has two items that need to be set - lpszText, which is the tooltip text to display, and
        // hinst which is the application instance that contains the toolbar requesting the tooltip.
        pDispInfo->lpszText = (LPTSTR) lpnm->idFrom;    // idFrom is the applicaiton ID of the control that sent the message -
                                                        // in this case, the button command ID iteslf, which is useful, as these
                                                        // have strings stored against them in the string table, so just cast
                                                        // it to a LPTSTR and the entry for the currently "hot" button in the
                                                        // string table will be displayed.
        pDispInfo->hinst = AfxGetInstanceHandle();
    }
  /*  else if (lpnm->code == TBN_HOTITEMCHANGE) // This message comes from the toolbar when the mouse moves into (or out of) a 
    {                                         // toolbar button. It is used here to put a message in the status bar about the 
                                              // button
        LPNMTBHOTITEM pHotItem = (LPNMTBHOTITEM)lParam;

        // This is the default message - if we can't get one, then display this.
        CString strMsg = IDLE_TEXT;

        // Bitwise OR - only want to load the string if the mouse is NOT leaving the toolbar (i.e. it is entering, or already
        // in). So bitwise AND the item flags with NOT(~) HICF_LEAVING, the flag indicating the mouse has left the button.
        if (pHotItem->dwFlags & ~HICF_LEAVING)
        {
            // Load the description from the string table - similar to the LTPSTR cast from above, but safer.
            strMsg.LoadString(pHotItem->idNew);
        }
        
        // If the "hot item" is a disabled button, don't show the text ('cause we won't be notified when leaving
        // it, meaning the text can't be erased).
        if(!m_eventsToolbar.IsButtonEnabled(pHotItem->idNew))
        {
            // Set it back to the idle text
            strMsg = IDLE_TEXT;
        }

        // If the string was not filled in (i.e. the string table is empty, or the ID was not found)
        if (strMsg.GetLength() == 0)
        {
            // Set it back to the idle text
            strMsg = IDLE_TEXT;
        }

        // And set the status bar to display the message
//        m_statusBar.SetWindowText(strMsg);
    }*/

	return AbstractTransActiveDialog::OnNotify(wParam, lParam, pResult);
}

void EventViewerDlg::OnClose() 
{  
    FUNCTION_ENTRY("OnClose");
    // Need to handle closing of this dialog carefully. If the list is currently loading events, there is no way 
    // to stop it, and deleting a thread that is currently active gives undefined results, so just have to wait for it to 
    // finish.

    // In order that the user realises their "close me" request has been registered, pop up a dialog box to tell them to wait
    // while it closes down.

    // Use the Windows Platform SDK call CreateDialog here instead of creating a CDialog dialog, as getting a CDialog to
    // attach itself to the desktop is difficult. The advantage of attaching this window to the desktop is that even when this
    // method goes out of scope, it remains visible, up until the applications GUI resources are cleaned up, which means that
    // it will be visible essentially until the application has removed itself from memory, which with the number of database
    // threads and objects that are in the Event Viewer can be a number of seconds after the main dialog disapears from view.
    //
    // AL : 3/2/04 : Commented out for TD 1182
    //
    //HWND dlg = CreateDialog(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDD_WAIT_FOR_CLOSE),GetDesktopWindow()->GetSafeHwnd(),NULL);
    //::ShowWindow(dlg,SW_SHOW);

    // Attach the dialog to a CWnd object - that way we can centre it easily.
    //CWnd* dlgWindow = new CWnd();
    //dlgWindow->Attach(dlg);
    //dlgWindow->CenterWindow();

    // Disable the main applicaiton window - stops users from clicking close twice, or doing any other action once close is clicked
    EnableWindow(FALSE);

    m_eventList.clearList();
	
    // Delete the filter object (if it exists)
	m_realPModel->deleteFilter();

    // And close the dialog, and the applicaiton.
	//TD18095, jianghp, to fix the performance of showing manager application
	AbstractTransActiveDialog::DestroyWindow();

    FUNCTION_EXIT;
}

LRESULT EventViewerDlg::OnCloseMainWindow(WPARAM,LPARAM)
{
    FUNCTION_ENTRY("OnCloseMainWindow");
    // OnCloseMainWindow is called from the message map when the control station attempts to close the application. It is done 
    // this way, because the incoming CORBA thread from the control station can't be used to create dialog boxes, which is 
    // precisely what we want to do - so a two-stage process is necessitated.

    // As in OnClose, create a dialog box to notify the user the application is closing. See the comment in OnClose for why
    // the Windows SDK CreateDialog method was used.
    //
    // AL : 3/2/04 : Commented out for TD 1182
    //
    //HWND dlg = CreateDialog(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDD_WAIT_FOR_CLOSE),GetDesktopWindow()->GetSafeHwnd(),NULL);
    //::ShowWindow(dlg,SW_SHOW);
    //CWnd* dlgWindow = new CWnd();
    //dlgWindow->Attach(dlg);
    //dlgWindow->CenterWindow();

    EnableWindow(FALSE);

    m_eventList.clearList();
	
	// Delete the filter object (if it exists)
	m_realPModel->deleteFilter();
    
    // The only difference here is that we don't want to call OnClose() - as the dialog will be destroyed by the GenericGUI
    // framework, not us.

    FUNCTION_EXIT;
    return 0;
}


void EventViewerDlg::readyToClose()
{
    FUNCTION_ENTRY("isReadyToClose");
    // This is the second method in the 2-stage control-station initiated shutdown procedure. It lets the control station know
    // when it is safe to actually destroy the application.

    // It is safe to destroy the applicaiton ONLY when thre are no events in the event list, and the event list is not
    // currently in the process of loading more.
  //  FUNCTION_EXIT;
   // return (m_eventList.GetItemCount() == 0 );

	// Close the Event Viewer nicely - call OnClose()
	::SendMessage(m_mainViewHandle,WM_USER_CLOSE_MAIN_WINDOW,0,0);

	while (!(m_eventList.GetItemCount() == 0))
	{
		TA_Base_Core::Thread::sleep(100);
	}
	FUNCTION_EXIT;
}

void EventViewerDlg::OnFileExit() 
{
    FUNCTION_ENTRY("OnFileExit");
    // File->Exit has been selected. Treat that the same as clicking the "X" in the top-right corner.
	//TD18095, jianghp, to fix the performance of showing manager application
	AbstractTransActiveDialog::DestroyWindow();
    FUNCTION_EXIT;
}

void EventViewerDlg::OnMenuSelect(UINT nItemID,UINT nFlags, HMENU hSysMenu)
{
    FUNCTION_ENTRY("OnMenuSelect");
    // Want to display a helper message in the status bar for each menu item. Have to do that manually for 
    // a dialog, so here it is.

	if( !IsWindow(m_statusBar))
	{
		return;
	}
    // If nFlags == 0xFFFF and hSysMenu == 0, then the menu has been closed - i.e., set it back to "Ready"
    if (nFlags == 0xFFFF && hSysMenu == 0)
    { 
		m_statusBar.SetWindowText( TAUnicodeString::CreateFromMultitByteString(m_realPModel->getFilterStatusMsg().c_str()) );
    }
    // Or, if we're over a separator, or a in a popup, or any of these things, then want to keep displaying
    // what is currently displayed
	else if (nItemID == 0 ||
			 nFlags & (MF_SEPARATOR|MF_POPUP|MF_MENUBREAK|MF_MENUBARBREAK))
	{
		// Don't change what is currently displayed
	}
    // Or if the system menu is selected
	else if (nItemID >= 0xF000 && nItemID < 0xF1F0)
	{
        TAUnicodeString strMsg;
		// special string table entries for system commands
        if (strMsg.LoadString(ID_COMMAND_FROM_SC(nItemID)) != 0)
        {
			m_statusBar.SetWindowText(strMsg.str());
        }
	}
	else
	{
		// set the message string
        TAUnicodeString strMsg;
		if (strMsg.LoadString(nItemID) != 0)    // Load the string from the string table (a return of 0 is a failure)
        {
			m_statusBar.SetWindowText(strMsg.str());
        }
	}
    FUNCTION_EXIT;
}

void EventViewerDlg::PreSubclassWindow() 
{
    FUNCTION_ENTRY("PreSubclassWindow");
    // Called before OnInitDialog
	AbstractTransActiveDialog::PreSubclassWindow();
    FUNCTION_EXIT;
}

// to update statusbar
LRESULT  EventViewerDlg::OnListUpdate(WPARAM size,LPARAM loadFlags)
{
    FUNCTION_ENTRY("OnListUpdate");

    // Called when the event list notifies the main window the list has changed in size. The event list will not 
    // automatically update itself - this gives the application the ability to control when and how view updates occur

    // If the progress bar is currently active update its value.
    if ( m_progressBarExists )
    {
        m_statusBar.setProgressBarPos( size );
    }
	
	// Then set the text showing the event count
	std::ostringstream sizeStream;
    sizeStream << TALoadString(IDS_EVENT_COUNT_STR).toMultiByteStdString() << size;   
	TAUnicodeString sizeStr =	TAUnicodeString::CreateFromMultitByteString(sizeStream.str().c_str());
    if ( size >= EventHandlerManager::s_maxEvents)
    {
        sizeStr += PLUS_SYMBOL_STR;
    }
    m_statusBar.SetPaneText( m_statusBar.CommandToIndex(ID_INDICATOR_SIZE), sizeStr.str() );

    // And set the STRETCH property on the left-most pane - that will make the left pane take up all the status bar
    // not used by any other panes.
	m_statusBar.SetPaneInfo( STRETCH_PANE, m_statusBar.GetItemID(STRETCH_PANE), SBPS_STRETCH, NULL );

    // Resize the "list-size" status-bar pane to enable it to display the event count correctly
    m_statusBar.SetPaneInfo( m_statusBar.CommandToIndex(ID_INDICATOR_SIZE), // The index of the pane in the status bar
                             ID_INDICATOR_SIZE,  // The ID to assign to this (same as it is currently, but method requires it)
                             SBPS_NORMAL,        // Sytle of the pane - again, not change, but this method requires it
                          static_cast<int> (90 + ( log10( (double)size + 1.1 ) + 1 ) *9 ));// The size of the pane - 9 times the size of the number plus a constant

    // Force progress bar to resize.
    m_statusBar.resizeProgressBar();

    FUNCTION_EXIT;
    return 0;
}


LRESULT  EventViewerDlg::OnListViewUpdate(WPARAM size,LPARAM loadFlags)
{
	FUNCTION_ENTRY("OnListViewUpdate");
	// Then set the text showing the event count
	std::ostringstream sizeStream;
    sizeStream << TALoadString(IDS_EVENT_COUNT_STR).toMultiByteStdString() << size;   
	TAUnicodeString sizeStr =	TAUnicodeString::CreateFromMultitByteString(sizeStream.str().c_str());
	if ( size >= EventHandlerManager::s_maxEvents )
	{
		sizeStr += PLUS_SYMBOL_STR;
	}
	m_statusBar.SetPaneText( m_statusBar.CommandToIndex(ID_INDICATOR_SIZE), sizeStr.str() );

	// And set the STRETCH property on the left-most pane - that will make the left pane take up all the status bar
	// not used by any other panes.
	m_statusBar.SetPaneInfo( STRETCH_PANE, m_statusBar.GetItemID(STRETCH_PANE), SBPS_STRETCH, NULL );

	// Resize the "list-size" status-bar pane to enable it to display the event count correctly
	m_statusBar.SetPaneInfo( m_statusBar.CommandToIndex(ID_INDICATOR_SIZE), // The index of the pane in the status bar
		ID_INDICATOR_SIZE,  // The ID to assign to this (same as it is currently, but method requires it)
		SBPS_NORMAL,        // Sytle of the pane - again, not change, but this method requires it
		static_cast<int> (90 + ( log10( (double)size + 1.1 ) + 1 )*9) );  // The size of the pane - 9 times the size of the number plus a constant

	// Force progress bar to resize.
	m_statusBar.resizeProgressBar();
	unsigned long flags = LVSICF_NOSCROLL|LVSICF_NOINVALIDATEALL;
	flags &= ~LVSICF_NOINVALIDATEALL;
	m_eventList.SetItemCountEx( size);
	m_eventList.Invalidate();

	m_eventList.UpdateWindow();


	// Update the page status buttons - only if the update was from a load.
	if ( loadFlags & Filter::MASK_PAGEUP )
	{
		m_pageUp.EnableWindow(TRUE);
	}
	else
	{
		m_pageUp.EnableWindow(FALSE);
	}
	if ( loadFlags & Filter::MASK_PAGEDOWN )
	{
		m_pageDown.EnableWindow(TRUE);
	}
	else
	{
		m_pageDown.EnableWindow(FALSE);
	}

	FUNCTION_EXIT;
	return 0;
	
}

LRESULT  EventViewerDlg::OnLoadListCount(WPARAM count,LPARAM text)
{
    FUNCTION_ENTRY("OnLoadListCount");
    // This is called when the Event List notifies the main application that a load has begun, and approximately how many
    // events it is expecting to load.
    if(m_progressBarExists)
    {
        m_statusBar.removeProgressBar();
        m_progressBarExists = false;
    }

    if ( 0 < count )
    {
        // Start the progress bar in the SEPARATOR pane
        m_statusBar.startProgressBar(ID_SEPARATOR);
        m_progressBarExists = true;

        // Set its range from 0 to count
        m_statusBar.setProgressRange(0,count);
    }
    else
    {
        if ( NULL != text )
        {
			m_statusBar.SetWindowText( TAUnicodeString::CreateFromMultitByteString(reinterpret_cast<CHAR*>(text)) );
        }
    }

    FUNCTION_EXIT;
    return 0;
}

LRESULT  EventViewerDlg::OnLoadListUpdate(WPARAM count,LPARAM)
{
    // BACTODO: Remove this method entirely.
    FUNCTION_ENTRY("OnLoadListUpdate");
    // This is called when the Event List notifies the main application that "count" events have been loaded since the last
    // call to this method. So increment the progress bar accordingly.
    m_statusBar.incrementProgressBar(count);

    FUNCTION_EXIT;
    return 0;
}

void EventViewerDlg::OnViewFilters() 
{
    FUNCTION_ENTRY("OnViewFilters");	
    // Enbale the wait cursor - loading the information from the database could take a perceptable amount of time.
    CWaitCursor waitCursor;

    // The show filters is a toggle button.
    // If it has been toggled on, then need to create the filter dialog based on what filter we currently have selected.
    // If the button has just been toggled on, the filter dialog member variable will be null, so test on that.
    if (NULL == m_filterDlg)
    {
        // If the current filter is for audit events, then create the dialog for that type.
        if (IDD_COMBINED_EVENT_FILTER == m_realPModel->getFilter()->getFilterID())
        {
            // Construct it as a CombinedEventFilterDlg, allowing access to the dialog creation methods
            CombinedEventFilterDlg* filterDlg = new CombinedEventFilterDlg(); //this);
            
            // Create and show the window
            filterDlg->Create(IDD_COMBINED_EVENT_FILTER);
            filterDlg->ShowWindow(SW_SHOW);
            filterDlg->CenterWindow(this);

            // Assign the filter dialog to the member variable. Need to cast it down to the base type.
            m_filterDlg = dynamic_cast<FilterDlg*> (filterDlg);
        }

        if (NULL != m_filterDlg)
        {
            // If the filter box was created successfully, check the menu and toolbar as necessary
	        GetMenu()->CheckMenuItem(IDC_VIEW_FILTERS,MF_CHECKED);
	        m_eventsToolbar->CheckButton(IDC_VIEW_FILTERS, true);
        }
        else
        {
            // If not, make sure they stay unchecked
	        GetMenu()->CheckMenuItem(IDC_VIEW_FILTERS,MF_UNCHECKED);
	        m_eventsToolbar->CheckButton(IDC_VIEW_FILTERS, false);
        }

    }
    else
    {
        // The button has been toggled off, so need to destroy the filter window. 
        // FilterDlg does not acutally subclass from CDialog, so need to cast the filter back to a CDialog
        // This requires a reinterpret cast, but it should be safe, as the assigning code is in this method as well.
        CDialog* filterDlg = reinterpret_cast<CDialog*> (m_filterDlg);
		//Begin-- TD7268 LiDan 29.5.2007

        // TD11568: Make sure the dialog is not destroyed if it is disabled.
        if ( filterDlg->IsWindowEnabled() )
        {
            filterDlg->ShowWindow(SW_HIDE);

            delete m_filterDlg;
            m_filterDlg = NULL;

	        GetMenu()->CheckMenuItem(IDC_VIEW_FILTERS,MF_UNCHECKED);
	        m_eventsToolbar->CheckButton(IDC_VIEW_FILTERS, false);

            // And enable or disable the updating button and combo box, and the "Refresh" button as necessary
            if (m_realPModel->IsCurrentTimeInFilter())
            {
                m_eventsToolbar->EnableButton(IDC_NEXT,false);
                /*m_eventList.setRefreshPeriod( m_refreshTime );*/
            }
            else
            {
                m_eventsToolbar->EnableButton(IDC_NEXT);
                /*m_eventList.setRefreshPeriod(0);*/
            }
        }
        else
        {
            // Window is disabled so was not destroyed, back sure the button state is consistent.
            GetMenu()->CheckMenuItem(IDC_VIEW_FILTERS,MF_CHECKED);
            m_eventsToolbar->CheckButton(IDC_VIEW_FILTERS,true);
        }
// 		//Begin-- TD7268 LiDan 29.5.2007
// 		//Resume the loading
// 		m_filter->resumeCurrentListDataLoad();
// 		m_filter->resumeRefresh();
    }

    setWindowCaption();

    FUNCTION_EXIT;
}

void EventViewerDlg::clearSortMark()
{
	m_eventList.clearSortMark();
}

LRESULT  EventViewerDlg::OnFilterDlgClose(WPARAM,LPARAM)
{
    FUNCTION_ENTRY("OnFilterDlgClose");
    // When the filter dialog closes, clear the event list, because we are loading a new filter
    //m_eventList.clearList();

    // Disable the filter menu checkmark, and uncheck the button.
    OnViewFilters();

    // Update the named filter combo box.
    populateNamedFilters();

    FUNCTION_EXIT;
    return 0;
}


LRESULT  EventViewerDlg::OnFilterApply(WPARAM,LPARAM)
{
    FUNCTION_ENTRY("OnFilterApply");

    // Enable or disable the updating button and combo box, and the "Refresh" button as necessary
    if (m_realPModel->IsFilterTOTimeEmpty())
    {
        m_eventsToolbar->EnableButton(IDC_NEXT,false);
    }
    else
    {
        m_eventsToolbar->EnableButton(IDC_NEXT);
    }

	setWindowCaption();
	// ++yanrong CL-15055

    // Update the named filter combo box.
    populateNamedFilters();

    FUNCTION_EXIT;
    return 0;
}

LRESULT  EventViewerDlg::OnFilterDlgCancel(WPARAM,LPARAM)
{
    FUNCTION_ENTRY("OnFilterDlgCancel");

    // Close the fitler dialog, disable the menu checkmark, and uncheck the toolbar button
    OnViewFilters();

    FUNCTION_EXIT;
    return 0;
}

LRESULT EventViewerDlg::OnNamedFilterChange(WPARAM,LPARAM)
{
    FUNCTION_ENTRY("OnNamedFilterChange");

    // A named filter has been saved/deleted so we need to refresh the contents.
    populateNamedFilters();


    FUNCTION_EXIT;
    return 0;
}

LRESULT EventViewerDlg::OnListItemSelect(WPARAM wParam,LPARAM lParam)
{
    FUNCTION_ENTRY("OnListItemSelect");

    // Retrieve the parameters
    unsigned int numSelectedItems = wParam;
    unsigned int index = lParam;
    
  	bool enableButton = false;

    // First test that only one event is selected
    if(numSelectedItems == 1)
    {
        enableButton = m_realPModel->IsSelectedAlarmEvent(index);
    }
    
    m_eventsToolbar->EnableButton(IDC_VIEW_INCIDENTS, enableButton);

    FUNCTION_EXIT;
    return 0;
}
void EventViewerDlg::OnNext()
{
    LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo,"OnNext come in");

	m_realPModel->next();
	
    LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo,"OnNext come out");
     
}

// set searching span before current. will keep the latest record if need to cut the size
void EventViewerDlg::OnPrevious()
{
    LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo,"OnPrevious come in");
    // The "previous" button has been clicked

		m_realPModel->previous();
    LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo,"OnPrevious come out");
}


void EventViewerDlg::OnPrint()
{
    FUNCTION_ENTRY("OnPrint");
    // Notify the event list printing was requested
    m_eventList.print();

    FUNCTION_EXIT;
}

void EventViewerDlg::OnExport()
{
    FUNCTION_ENTRY("OnExport");

	EventViewerApp * pApp = dynamic_cast<EventViewerApp*> (AfxGetApp());
    TA_Base_Core::EventViewer* guiEntity = dynamic_cast<TA_Base_Core::EventViewer*>( pApp->getApplicationController()->getEntity());

    TAUnicodeString exportLocation(UNICODE_EMPTY_STR);
    try
    {

        //exportLocation = guiEntity->getDefaultExportLocation().c_str();
		//TD17888  marc_bugfix
		exportLocation = TAUnicodeString::CreateFromMultitByteString(guiEntity->getExportDirectory().c_str());

        // ++ AZ : TD 13826, no err message for invalid path
        exportLocation.str().TrimRight();
        if (exportLocation != UNICODE_EMPTY_STR)
        {
            // check if the path is valid
			TAUnicodeString fileName = UNTITLED_LBL_STR;
			CFile outFile;

			if ( 0 == outFile.Open(exportLocation + fileName, CFile::modeCreate | CFile::modeReadWrite) )
			{
				TCHAR szMaxpath[MAX_PATH];		
				TAUnicodeString msg = exportLocation + fileName + _T(".\n");
				msg += TALoadString(IDS_EXPORT_MSG1_STR); 
				TA_Base_Bus::TransActiveMessage userMsg;
				userMsg << msg;
				UINT selectedButton = userMsg.showMsgBox(IDS_UI_040034);
				//++TD15037
                exportLocation.Empty();
				if (SHGetSpecialFolderPath(NULL, szMaxpath, CSIDL_PERSONAL, FALSE))
				{
					 exportLocation = szMaxpath;

				}
			}
			else
			{
				// remove the file if it is created
				if (outFile != NULL) 
				{	
					outFile.Close();
					CFile::Remove(exportLocation + fileName);
				}
			}

        }
		else
		{
			//TD17888 marc_bugfix
			TCHAR szMaxpath[MAX_PATH];

		    if (SHGetSpecialFolderPath(NULL, szMaxpath, CSIDL_PERSONAL, FALSE))
			{
				exportLocation = szMaxpath;
				
			}
		}
        // AZ ++ TD 13826
    }
    catch(...)
    {
        exportLocation.Empty();
    }

	m_realPModel->pauseLoad();

	TA_Base_Bus::FileDialogArialDisableFolders dlg(FALSE,            // We want a Save dialog not an Open one
					_T(".csv"),           // The default file extension
					UNTITLED_LBL_STR,       // This is the initial file name
					OFN_HIDEREADONLY| // Hide the readonly check box
					OFN_OVERWRITEPROMPT| // Prompt to ask the user if they want to overwrite existing files
					OFN_PATHMUSTEXIST |	 // Check that the path entered is correct and warn if it is not
					OFN_NOCHANGEDIR,
					_T("CSV (Comma delimited) (*.csv)|*.csv||"),	// Only allow them to save in csv format
					this);			  // Parent window is the actual event viewer

    dlg.m_ofn.lpstrInitialDir = exportLocation;

	if (dlg.DoModal() == IDOK)
	{
		try
		{
			TAUnicodeString fileName = dlg.GetPathName(); //todo check conversion
			m_realPModel->exportEvents(fileName.toMultiByteStdString(), m_eventList.GetItemCount());
		}
		catch ( ... )
		{
			TA_Base_Bus::TransActiveMessage userMsg;
			userMsg << TAUnicodeString(dlg.GetPathName());
			UINT selectedButton = userMsg.showMsgBox(IDS_UE_040018);
			return;
		}

		TA_Base_Bus::TransActiveMessage userMsg;
		UINT selectedButton = userMsg.showMsgBox(IDS_UI_040018);		
	}
	else
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo,"Export was cancelled");
	}	

	m_realPModel->resumeLoad();
    FUNCTION_EXIT;
}

void EventViewerDlg::OnAlarmManager()
{
    FUNCTION_ENTRY("OnAlarmManager");

	m_realPModel->launchAlarmManager();

    FUNCTION_EXIT;
}

void EventViewerDlg::OnIncidentReport()
{
    FUNCTION_ENTRY("OnIncidentReport");

    // Generate an incident report on any selected alarm events.
  //  m_eventList.executeAction( ACTION_CREATE_INCIDENT_REPORT ); 
	m_realPModel->executeAction(ACTION_CREATE_INCIDENT_REPORT);
    FUNCTION_EXIT;
}

void EventViewerDlg::OnSelchangeComboNamedFilter()
{
    FUNCTION_ENTRY("OnSelchangeComboNamedFilter");

    // Get the currently selected item.
    int selIdx = m_namedFiltersComboBox.GetCurSel();
    if ( 0 <= selIdx )
    {
        // Get the name of the filter.
        TAUnicodeString filterName_c;
        m_namedFiltersComboBox.GetLBText( selIdx, filterName_c.str() );

		if(m_realPModel->IsNotSameFilterSelected(filterName_c.toMultiByteStdString()))
		{
			try
			{
				m_realPModel->loadSelectedFilter(filterName_c.toMultiByteStdString());
			}			
			catch ( DataException& )
			{
				// Display the load error message.
                TA_Base_Bus::TransActiveMessage userMsg;
                userMsg << EVDisplayStrRes::GetInstance()->ERROR_LOAD;
                userMsg << EVDisplayStrRes::GetInstance()->ERROR_LOADED;
                userMsg.showMsgBox(IDS_UE_040031);

				// Reselect the old named filter.
				populateNamedFilters();				
				return;
			}
			
			// Update the window title.
            setWindowCaption();

            // TD11343: Update any button states that depend on the current filter.
            m_eventsToolbar->EnableButton( IDC_NEXT,!(m_realPModel->IsCurrentTimeInFilter()));

            // Update the filter dialog if required.
            if ( NULL != m_filterDlg )
            {
                m_filterDlg->updateFilter();
            }
		}                       
	}      

    FUNCTION_EXIT;
}

/*zhiqiang--begin
//TD17143
LRESULT EventViewerDlg::OnSetupEventFiltersAndRights(WPARAM,LPARAM)
{
	//zhiqiang-- FUNCTION_ENTRY("OnRefreshRights");
	FUNCTION_ENTRY("EventViewerDlg::OnSetupEventFiltersAndRights"); //zhiqiang++
	
    // Populate it.
    populateNamedFilters();
 
    // Populate event levels
    populateEventLevels();

	applyRights();
	FUNCTION_EXIT;
	return 0;
}
//*/ //zhiqiang--end

//zhiqiang++begin
LRESULT EventViewerDlg::OnSetupEventFiltersAndRights(WPARAM, LPARAM)
{
	FUNCTION_ENTRY("EventViewerDlg::OnSetupEventFiltersAndRights");
 	
	populateNamedFilters();
	m_realPModel->startRightsAndEventLevelThreads();

	FUNCTION_EXIT;
	return 0;
}
//zhiqiang--end
void EventViewerDlg::OnSelchangeComboEventLevel()
{
    FUNCTION_ENTRY("OnSelchangeComboEventLevel");

//     // Get the currently selected item.
//     int index = m_eventLevelsComboBox.GetCurSel();
//     if ( CB_ERR != index )
//     {
//         // Get the new event level.
//         unsigned long newLevel = m_eventLevelsComboBox.GetItemData( index );
//         if ( newLevel != m_filter->getEventLevel() )
//         {
//             // Set the new level and trigger event loading.
//             m_filter->setEventLevel( newLevel );
//             reloadEvents();
//         }
//     }

    FUNCTION_EXIT;
}

/*void EventViewerDlg::enableDisableColumn(const std::string& columnName,const unsigned long menuId)
{
    FUNCTION_ENTRY("enableDisableColumn");
    // Get the header control (need this to figure out which column ID coresponds to the supplied column name).
    CHeaderCtrl* header = m_eventList.GetHeaderCtrl();

    // In order to get a column name from a header control, need to get header items, so set one up.
    HDITEM hdi;
    TCHAR  stringBuffer[256];   // To get the name in a header control, need a string buffer - which is what we have here

    hdi.mask = HDI_TEXT;        // Set the mask to HDI_TEXT, telling the header we want to retrieve the name
    hdi.pszText = stringBuffer; // Set a string pointer for the header control to write the text to (the string buffer)
    hdi.cchTextMax = 256;       // Tell the header control how long the buffer is (max size)

    // Loop for the number of items in the header
    int count = header->GetItemCount();
    for (int i=0;i < count;i++)
    {
       // Get the item, and put the name in a string
       header->GetItem(i, &hdi);
       std::string headerName(stringBuffer);
   
       if (0 == headerName.compare(columnName))
       {
           // If this is the column we're looking for...
           if (m_eventList.GetColumnWidth(i) == 0)
           {
               // ... and it is currently not visible, make it visible
               m_eventList.SetColumnWidth(i,150);

               // And check its menu item
               GetMenu()->CheckMenuItem(menuId,MF_CHECKED);
           }
           else
           {
               // ... and it is currently visible, hide it
               m_eventList.SetColumnWidth(i,0);

               // And uncheck its menu item
               GetMenu()->CheckMenuItem(menuId,MF_UNCHECKED);
           }
           return;
       }
    }

    FUNCTION_EXIT;
}*/ //------------Not used

void EventViewerDlg::OnStatusBar() 
{ 
    FUNCTION_ENTRY("OnStatusBar");   

    // Toggle the visibility of the status bar
    if (m_statusBar.IsWindowVisible())
    {
        // If it's currently visible, then hide it
        m_statusBar.ShowWindow(SW_HIDE);
        // Uncheck the item in the menu
        GetMenu()->CheckMenuItem(IDC_STATUS_BAR,MF_UNCHECKED);
    }
    else
    {
        // If it's currently not visible, then show it
        m_statusBar.ShowWindow(SW_SHOW);
        // Check the item in the menu
        GetMenu()->CheckMenuItem(IDC_STATUS_BAR,MF_CHECKED);
    }

    // Reposition the status bar
	CRect rcClientNow;
    RepositionBars(AFX_IDW_CONTROLBAR_FIRST, AFX_IDW_CONTROLBAR_LAST,
				   0, reposQuery, rcClientNow);    

    // Send a "Size" message to repaint the main window (invalidate doesn't work here for some reason)
    CRect viewerRect;
    GetWindowRect(&viewerRect);
    ScreenToClient(&viewerRect);
    this->PostMessage(WM_SIZE,viewerRect.Height(),viewerRect.Width());

    FUNCTION_EXIT;
}

void EventViewerDlg::OnToolBar() 
{
    FUNCTION_ENTRY("OnToolBar");
    // Toggle the visibility of the tool bar
    if (m_eventsToolbar->IsWindowVisible())
    {
        // If it's currently visible, hide it (and the pager it's in)
        m_eventsToolbar->ShowWindow(SW_HIDE);
        CWnd* pager = GetDlgItem(IDC_PAGER_CTRL);
        pager->ShowWindow(SW_HIDE);
        m_pageUp.ShowWindow(SW_HIDE);
        m_pageDown.ShowWindow(SW_HIDE);
        // Check the item in the menu
        GetMenu()->CheckMenuItem(IDC_TOOL_BAR,MF_UNCHECKED);
    }
    else
    {
        // If it's currently not visible, show it (and the pager it's in)
        m_eventsToolbar->ShowWindow(SW_SHOW);
        CWnd* pager = GetDlgItem(IDC_PAGER_CTRL);
        pager->ShowWindow(SW_SHOW);
        m_pageUp.ShowWindow(SW_SHOW);
        m_pageDown.ShowWindow(SW_SHOW);
        // Check the item in the menu
        GetMenu()->CheckMenuItem(IDC_TOOL_BAR,MF_CHECKED);
    }  

    // Send a "Size" message to repaint the toolbar (invalidate doesn't work here for some reason)
    CRect viewerRect;
    GetWindowRect(&viewerRect);
    ScreenToClient(&viewerRect);
    this->SendMessage(WM_SIZE,viewerRect.Height(),viewerRect.Width());	

    // Invalidate the event list to cause it to redraw
    m_eventList.Invalidate();
    FUNCTION_EXIT;
}


void EventViewerDlg::OnShowAll()
{
    FUNCTION_ENTRY("OnShowAll");

    // Get the current state.
    MENUITEMINFO info;
    info.cbSize = sizeof (MENUITEMINFO); // must fill up this field
    info.fMask = MIIM_STATE;             // get the state of the menu item
    GetMenu()->GetMenuItemInfo( IDC_SHOW_ALL, &info );

    // Change to the new state.
    if ( info.fState & MF_CHECKED )
    {
        // Uncheck the menu item.
        GetMenu()->CheckMenuItem( IDC_SHOW_ALL, MF_UNCHECKED|MF_BYCOMMAND );
		m_realPModel->setFilterDuty(true);
    }
    else
    {
        // Check the menu item.
        GetMenu()->CheckMenuItem( IDC_SHOW_ALL, MF_CHECKED|MF_BYCOMMAND );
		m_realPModel->setFilterDuty(false);
    }

    // Reload the current filter.
    m_realPModel->reloadEvents();

    FUNCTION_EXIT;
}


void EventViewerDlg::OnAppAbout() 
{
    FUNCTION_ENTRY("OnAppAbout");
    // Display the about box
	TA_Base_Bus::HelpLauncher::getInstance().displayAboutBox();
    FUNCTION_EXIT;
}


void EventViewerDlg::OnHelp() 
{
    FUNCTION_ENTRY("OnHelp");
    // Display the about box
	TA_Base_Bus::HelpLauncher::getInstance().displayHelp();	
    FUNCTION_EXIT;
}


/*int EventViewerDlg::findMenuItem(CMenu* Menu, LPCTSTR MenuString)
{
    FUNCTION_ENTRY("findMenuItem");
    // Ensure the menu is valid (and is actually a menu).
   ASSERT(Menu);
   ASSERT(::IsMenu(Menu->GetSafeHmenu()));

   // Loop for each item in the menu
   int count = Menu->GetMenuItemCount();
   for (int i = 0; i < count; i++)
   {
      CString str;
      // If the menu name matches the search string, return it
      if (Menu->GetMenuString(i, str, MF_BYPOSITION) &&
         (strcmp(str, MenuString) == 0))
         return i;
   }

   // If not found, return -1
   FUNCTION_EXIT;
   return -1;
}*/ ///----------------NOT used

void EventViewerDlg::setWindowCaption()
{
	FUNCTION_ENTRY("setWindowCaption");
	TAUnicodeString windowText = TALoadString(IDS_APP_NAME_STRING);
	TAUnicodeString filterStr =  TAUnicodeString::CreateFromMultitByteString(m_realPModel->getCurrentFilterText().c_str());
	 if (0 < filterStr.GetLength())
    {
		windowText += TALoadString(IDS_SLASH_FILTER_ON_STR) + filterStr;       
    }
	SetWindowText(windowText);	
    FUNCTION_EXIT;
}

void EventViewerDlg::OnPageUp() 
{	
    LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo,"OnPageUp come in");
	// Trigger the page change.
     m_realPModel->previousPage();
	if( TA_Base_Core::RunParams::getInstance().isSet(EventConstants::RPARAM_NO_VISIBILITY_RULE.c_str()))
	{
		// Disable any other page changes.
		m_pageUp.EnableWindow(FALSE);
		m_pageDown.EnableWindow(FALSE);
	}
	LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo,"OnPageUp come out");
}

void EventViewerDlg::OnPageDown() 
{
	LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo,"OnPageDown come in");
    // Trigger the page change.
    //m_eventList.nextPage();
	 m_realPModel->nextPage();

// 	if( TA_Base_Core::RunParams::getInstance().isSet(EventConstants::RPARAM_NO_VISIBILITY_RULE.c_str()))
// 	{
// 		// Disable any other page changes.
// 		m_pageUp.EnableWindow(FALSE);
// 		m_pageDown.EnableWindow(FALSE);
// 	}
	LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo,"OnPageDown come out");
}

//zhiqiang++begin
LRESULT EventViewerDlg::OnPopulateEventLevels(WPARAM wParam, LPARAM lParam)
{
	FUNCTION_ENTRY("OnPopulateEventLevels");
	
// 	// Get the number of configured event levels.
//     unsigned long numLevels = wParam; //TA_Base_Core::ConfigurationEditor::getEventLevels();
// 	
//     // Get the current event level.
//     unsigned long currLevel = m_filter->getEventLevel();
// 	
//     // Add them to the combo box.
//     for ( ; numLevels > 0; numLevels-- )
//     {
//         std::stringstream numToStr;
//         numToStr << numLevels;
//         int index = m_eventLevelsComboBox.AddString( numToStr.str().c_str() );
// 		
//         if ( CB_ERR != index )
//         {
//             // Store the level number to save conversion later.
//             m_eventLevelsComboBox.SetItemData( index, numLevels );
// 			
//             // Select it if its the current.
//             if ( currLevel == numLevels )
//             {
//                 m_eventLevelsComboBox.SetCurSel( index );
//             }
//         }
//     }
//     
	FUNCTION_EXIT;
	return 0;
}

LRESULT EventViewerDlg::OnApplyRights(WPARAM wParam, LPARAM lParam)
{
	FUNCTION_ENTRY("OnApplyRights"); 
    // Flags to set visibility - default to visible.
    /*bool eventLevelVisible = true;
	bool showAllEventsEnabled = false;
	SrtActionPermittedInfo *pSrtActionPermittedInfo = reinterpret_cast<SrtActionPermittedInfo *>(wParam);
	
	try
	{
        std::string reason;
        TA_Base_Bus::EDecisionModule decisionModule = TA_Base_Bus::UNDEFINED_DM;
		
		eventLevelVisible = m_rightsLibrary->isActionPermittedOnResourceAndLocSub( pSrtActionPermittedInfo->sessionId, pSrtActionPermittedInfo->resourceId,
			pSrtActionPermittedInfo->locationKey, pSrtActionPermittedInfo->subsystemKey, TA_Base_Bus::aca_CHANGE_EVENT_LEVEL, reason, decisionModule );
		
		showAllEventsEnabled = m_rightsLibrary->isActionPermittedOnResourceAndLocSub( pSrtActionPermittedInfo->sessionId, pSrtActionPermittedInfo->resourceId,
            pSrtActionPermittedInfo->locationKey, pSrtActionPermittedInfo->subsystemKey,  TA_Base_Bus::aca_SHOW_ALL_EVENTS, reason, decisionModule );
		
		LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "isActionPermittedOnResourceAndLocSub End.");

	}
	catch( ... )
	{
		LOG_EXCEPTION_CATCH( SourceInfo, "DataException", "Errors occurred while trying to get rights.");
	}
	
//     // Now apply them.
//     if ( eventLevelVisible )
//     {
// 
//         m_eventsToolbar.HideButton( EVENT_LEVEL_SEPARATOR_ID, FALSE );
// 
//         m_eventLevelsStatic.ShowWindow( SW_SHOW );
// 
//         m_eventLevelsComboBox.ShowWindow( SW_SHOW );
// 		// TD 14765 enable the window if not
// 		if (m_eventLevelsStatic.IsWindowEnabled() == false) 
// 		{
// 			m_eventLevelsStatic.EnableWindow(TRUE);
// 		}
// 
// 		if (m_eventLevelsComboBox.IsWindowEnabled() == false) 
// 		{
// 			m_eventLevelsComboBox.EnableWindow(TRUE);
// 		}
//     }
//     else
//     {
// 		// TD 14765 instead of hiding the box and text, disable them
//         //m_eventsToolbar.HideButton( EVENT_LEVEL_SEPARATOR_ID, TRUE );
//         //m_eventLevelsStatic.ShowWindow( SW_HIDE );
//         //m_eventLevelsComboBox.ShowWindow( SW_HIDE );
//         m_eventsToolbar.HideButton( EVENT_LEVEL_SEPARATOR_ID, FALSE );
//         m_eventLevelsComboBox.ShowWindow( SW_SHOW );
//         m_eventLevelsStatic.ShowWindow( SW_SHOW );
// 		m_eventLevelsStatic.EnableWindow(FALSE);
//         m_eventLevelsComboBox.EnableWindow(FALSE);
//     }
	
	// Get the current state.
	MENUITEMINFO info;
	info.cbSize = sizeof (MENUITEMINFO); // must fill up this field
	info.fMask = MIIM_STATE;             // get the state of the menu item
	GetMenu()->GetMenuItemInfo( IDC_SHOW_ALL, &info );
	// Enable the menu command for show all events
	if ( showAllEventsEnabled )
	{
		if ( info.fState == MFS_GRAYED )
		{
			// enable the menu item.
			GetMenu()->EnableMenuItem( IDC_SHOW_ALL, MF_ENABLED|MF_BYCOMMAND );
		}
	}
	else
	{
		if ( info.fState == MFS_ENABLED )
		{
			// gray the menu item.
			GetMenu()->EnableMenuItem( IDC_SHOW_ALL, MF_GRAYED|MF_BYCOMMAND );
		}
	}
	
	//release memory space
	delete pSrtActionPermittedInfo; */

	m_realPModel->applyRights();
	
	FUNCTION_EXIT;

	return 0;	
}

/*LRESULT EventViewerDlg::OnSetupDatabaseConnectionString(WPARAM, LPARAM)
{
	FUNCTION_ENTRY("EventViewerDlg::OnSetupDatabaseConnectionString");
 	
	m_currentDbString = getCurrentDatabaseString();

	// Update the filter dialog if it exists.
    if ( NULL != m_filterDlg )
    {
        m_filterDlg->updateFilter();
    }
	
	// set the window title to the changed filter
	setWindowCaption();

    // Trigger a database load.
    reloadEvents();
	
	FUNCTION_EXIT;
	return 0;
}*/ //////////////NOT in USE

//zhiqiang++end
void EventViewerDlg::populateNamedFilters()
{
	FUNCTION_ENTRY("populateNamedFilters");
    // Make sure the combo box is empty.
    m_namedFiltersComboBox.ResetContent();

    try
    {

		TA_Base_Core::NamedFilterAccessFactory::FilterNameList filterNameList =  m_realPModel->getFilterNameList();
		

        // Now add each to the combo box.
        NamedFilterAccessFactory::FilterNameList::iterator namedFilterIt;
        for ( namedFilterIt=filterNameList.begin(); namedFilterIt!=filterNameList.end(); namedFilterIt++ )
        {
			int index = m_namedFiltersComboBox.AddString( TAUnicodeString::CreateFromMultitByteString(namedFilterIt->first.c_str() ));
            if ( 0 <= index )
            {
                // If the data was inserted correctly store whether it is read only.
                m_namedFiltersComboBox.SetItemData( index, static_cast<int>(namedFilterIt->second) );

                // If it is the current filter then select it.
                if ( namedFilterIt->first == m_realPModel->getNameFilterName() )
                {
                    m_namedFiltersComboBox.SetCurSel( index );
                }
            }
        }
    }
    catch ( DataException& )
    {
        LOG_GENERIC(SourceInfo, DebugUtil::DebugError, "Could not get a list of named filters!");
    }

    // TD11626: If nothing is currently selected than it is a temporary name.
    if ( CB_ERR == m_namedFiltersComboBox.GetCurSel() )
    {
		m_namedFiltersComboBox.AddTempString( TAUnicodeString::CreateFromMultitByteString(m_realPModel->getNameFilterName().c_str() ));
    }
	m_namedFiltersComboBox.Invalidate();
	FUNCTION_EXIT;//zhiqiang++
}






/// PModel Notification Handler Function
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void EventViewerDlg::setUpStatusBar()
{
    FUNCTION_ENTRY("setUpStatusBar");

    // Create the status bar
	if (m_statusBar.Create(this)) 
    {
        // If successful, set the indicators.
	    m_statusBar.SetIndicators(indicators,sizeof(indicators)/sizeof(UINT));


        // Set the default text display
       // m_statusBar.SetWindowText(IDLE_TEXT);
        
        // And set the STRETCH property on the left-most pane - that will make the left pane take up all the status bar
        // not used by any other panes.
		m_statusBar.SetPaneInfo(STRETCH_PANE, m_statusBar.GetItemID(STRETCH_PANE),
			SBPS_STRETCH, NULL );

        // Set the size of the event count pane
        int pane = m_statusBar.CommandToIndex(ID_INDICATOR_SIZE);
        m_statusBar.SetPaneInfo(pane, m_statusBar.GetItemID(pane),
		    SBPS_NORMAL, 130 );

		//check the statusbar menu
		 GetMenu()->CheckMenuItem(IDC_STATUS_BAR,MF_CHECKED);

    }   


    // Ensure the statusbar is displayed in the 
    CFont font;
    font.CreatePointFont(FONT_SIZE,TALoadString(IDS_FONT_NAME_STR),NULL); // 10pt Arial Bold
    m_statusBar.SetFont(&font,FALSE);

    
	// Get the size of the application window
	CRect clientRectStart;
	CRect clientRectCurrent;
	GetClientRect(clientRectStart);

    // And reposition the status bar across the bottom
	RepositionBars(AFX_IDW_CONTROLBAR_FIRST, // These two specify the range in which the default windows 
                   AFX_IDW_CONTROLBAR_LAST,  // control bars reside, which includes the status bar
                   0, // The item ID to resize to fill the space not occupied by the status bar - set to 0, because resizing is manual
                   reposQuery, // Tell the method not to re-size the main window
                   clientRectCurrent); // A rectangle object which will be set to the size of the client area after the method has completed

	// Now move all the controls so they are in the same relative
	// position within the remaining client area as they would be
	// with no control bars.

    // So first get the offset by which everything needs to be moved
	CPoint offset(clientRectCurrent.left - clientRectStart.left,
					clientRectCurrent.top - clientRectStart.top);

    // Get the first child of the main applicaiton window
    CWnd* childWindow = GetWindow(GW_CHILD);
	CRect  childRect;
	while (NULL != childWindow)
	{
        // Get the current client rectangle for the window
		childWindow->GetWindowRect(childRect);
		ScreenToClient(childRect);

        // Adjust is by the offest
		childRect.OffsetRect(offset);

        // And move the window accordingly
		childWindow->MoveWindow(childRect, FALSE);

        // Then get the next child window
		childWindow = childWindow->GetNextWindow();
	}

	// Adjust the dialog window dimensions

    // Get the current main applicaiton window
	CRect windowRect;
	GetWindowRect(windowRect);

    // And resize it by the offset from adding the control bars
	windowRect.right += clientRectStart.Width() - clientRectCurrent.Width();
	windowRect.bottom += clientRectStart.Height() - clientRectCurrent.Height();
	MoveWindow(windowRect, FALSE);
    FUNCTION_EXIT;
}

void EventViewerDlg::setUpPager()
{
	// Create the standard ("enabled") image list, and the highlighted ("hot") image list
    m_eventsTbImageListEnabled.Create(16,16, ILC_MASK, 10, 2);
	m_eventsTbImageListHot.Create(16,16, ILC_MASK, 10, 2);

    // Get the bounds of the pager static
	CRect m_pagerRect;
	GetDlgItem(IDC_PAGER_CTRL)->GetWindowRect(m_pagerRect);
	ScreenToClient(m_pagerRect);
    m_pagerRect.right -= 50; // This is needed to correctly draw pager the first time.

    // And then delete it (we'll replace it with the actual pager next)
	GetDlgItem(IDC_PAGER_CTRL)->DestroyWindow();

	// Create a Pager Control in the bounds of the old pager static, and give it the same ID
	m_Pager.Create(WC_PAGESCROLLER, UNICODE_EMPTY_STR ,WS_CHILD | WS_VISIBLE | WS_TABSTOP , m_pagerRect, this, IDC_PAGER_CTRL);

}

void EventViewerDlg::setUpToolBar()
{
    FUNCTION_ENTRY("setUpToolBar");
    // Get the CWinApp object of this application (need it for the image lists)
    CWinApp*	pApp = AfxGetApp();
    
    // Set the tool bar child
    UINT		tbarStyle;	
	tbarStyle = WS_VISIBLE | WS_CHILD;          // Standard windows sytles
	tbarStyle = tbarStyle | CCS_NODIVIDER;      // Don't draw a divider line at the top of the toolbar (we have a different line for that)
	tbarStyle = tbarStyle | CCS_NOPARENTALIGN;  // Don't align the toolbar with the top of the window (i.e. manual placement)
	tbarStyle = tbarStyle | CCS_NORESIZE;       // Don't resize the toolbar automatically (we'll do it ourselves)
	tbarStyle = tbarStyle | TBSTYLE_TOOLTIPS;   // Enable tooltip notification when floating
	tbarStyle = tbarStyle | TBSTYLE_FLAT;       // Enable the "flat" style (buttons with text under them, and hot tracking enabled)	
    tbarStyle = tbarStyle | TBSTYLE_TRANSPARENT;
   
    // Create the toolbar, with the pager set as its parent window
	m_eventsToolbar = new CToolBarCtrl();
	m_eventsToolbar->Create(tbarStyle, m_pagerRect, &m_Pager, IDC_EVENTS_TBAR );

    // Tell the pager that the toolbar is its child
	Pager_SetChild(m_Pager.m_hWnd, m_eventsToolbar->m_hWnd);
	m_Pager.ModifyStyle(0,PGS_HORZ); //  See horizontal scrolling
	Pager_SetButtonSize(m_Pager.m_hWnd, 15);     // And set the default button size

	m_eventsToolbar->SetExtendedStyle(TBSTYLE_EX_DRAWDDARROWS);  // Notify the event toolbar that drop-down buttons can be used (if needed)
    m_eventsToolbar->SetButtonWidth(15, 200); // Set the min and max button widths

    // Ensure the toolbar is displayed in the correct font
    CFont font;
    font.CreatePointFont(FONT_SIZE,TALoadString(IDS_FONT_NAME_STR),NULL); // 10pt Arial Bold
    m_eventsToolbar->SetFont(&font,FALSE);

    // The TBBUTTON struct is used to define the buttons to be added to the toolbar.
	TBBUTTON		tbButton;

	// Add Print button

    // Add a string to the toolbar's string list. AddStrings() returns the index at which it was added, and that is set in the
    // TBBUTTON struct iString member (tells the toolbar which index to use for this button)
	VERIFY((tbButton.iString = m_eventsToolbar->AddStrings(TALoadString(IDS_PRINT_STRING))) != -1);

    // Set the image for this button. As with the string, the image is actually added to an image list (and loaded from the 
    // IDI_REFRESH resource), and the TBBUTTON struct is set to the index in the image list to look up.
	tbButton.iBitmap = m_eventsTbImageListEnabled.Add(pApp->LoadIcon(IDI_PRINT));

    // Same for the "hot" image. By toolbar constraints, the two indexes need to be the same (that's why this doesn't
    // set another index value)
	m_eventsTbImageListHot.Add(pApp->LoadIcon(IDI_PRINT_HOT));

    // Set the button style - enabled, button (not divider, etc), and autosizing.
	tbButton.fsState = TBSTATE_ENABLED;
	tbButton.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;

    // Set the data associated with the button - nothing
	tbButton.dwData = 0;

    // Set the command that will be sent to the app when the button is clicked
	tbButton.idCommand = IDC_PRINT;

    // and finally add the button to the toolbar (the 1 tells the toolbar how many buttons are being added in this command - 
    // if you want to be advanced you can define many toolbar buttons, and add them all to the list at once, but that isn't as
    // clear)
	VERIFY(m_eventsToolbar->AddButtons(1, &tbButton));

	// Add Export button

	VERIFY((tbButton.iString = m_eventsToolbar->AddStrings(TALoadString(IDS_EXPORT_STR))) != -1);
	tbButton.iBitmap = m_eventsTbImageListEnabled.Add(pApp->LoadIcon(IDI_SAVE));
	m_eventsTbImageListHot.Add(pApp->LoadIcon(IDI_SAVE_HOT));
	tbButton.fsState = TBSTATE_ENABLED;
	tbButton.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;
	tbButton.dwData = 0;
	tbButton.idCommand = IDC_EXPORT;
	VERIFY(m_eventsToolbar->AddButtons(1, &tbButton));    

    // Add a separator

    tbButton.iBitmap = NULL;
    tbButton.fsState = TBSTATE_ENABLED;
    tbButton.fsStyle = TBSTYLE_SEP; // This style sets the button as a separator
    tbButton.dwData = 0;
    tbButton.idCommand = 0;
    VERIFY(m_eventsToolbar->AddButtons(1, &tbButton));


	// Add Filter button - a check button

	VERIFY((tbButton.iString = m_eventsToolbar->AddStrings(TALoadString(IDS_FILTER_STR))) != -1);
	tbButton.iBitmap = m_eventsTbImageListEnabled.Add(pApp->LoadIcon(IDI_FILTERS));
	m_eventsTbImageListHot.Add(pApp->LoadIcon(IDI_FILTERS_HOT));
	tbButton.fsState = TBSTATE_ENABLED; // The CHECK style (below) means this is a toggle button
	tbButton.fsStyle = TBSTYLE_BUTTON | TBSTYLE_CHECK | TBSTYLE_AUTOSIZE;
	tbButton.dwData = 0;
	tbButton.idCommand = IDC_VIEW_FILTERS;
	VERIFY(m_eventsToolbar->AddButtons(1, &tbButton));

	// Add Previous button

	VERIFY((tbButton.iString = m_eventsToolbar->AddStrings(TALoadString(IDS_PREVIOUS_STR))) != -1);
	tbButton.iBitmap = m_eventsTbImageListEnabled.Add(pApp->LoadIcon(IDI_PREVIOUS));
	m_eventsTbImageListHot.Add(pApp->LoadIcon(IDI_PREVIOUS_HOT));
	tbButton.fsState = TBSTATE_ENABLED;
	tbButton.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;
	tbButton.dwData = 0;
	tbButton.idCommand = IDC_PREVIOUS;
	VERIFY(m_eventsToolbar->AddButtons(1, &tbButton));

	// Add Next button

	VERIFY((tbButton.iString = m_eventsToolbar->AddStrings(TALoadString(IDS_NEXT_STR))) != -1);
	tbButton.iBitmap = m_eventsTbImageListEnabled.Add(pApp->LoadIcon(IDI_NEXT));
	m_eventsTbImageListHot.Add(pApp->LoadIcon(IDI_NEXT_HOT));
	tbButton.fsState = TBSTATE_ENABLED;
	tbButton.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;
	tbButton.dwData = 0;
	tbButton.idCommand = IDC_NEXT;
	VERIFY(m_eventsToolbar->AddButtons(1, &tbButton));
    
    // Add separator

	tbButton.iBitmap = NULL;
    tbButton.fsState = TBSTATE_ENABLED;
    tbButton.fsStyle = TBSTYLE_SEP;
    tbButton.dwData = 0;
	tbButton.idCommand = 0;
	VERIFY(m_eventsToolbar->AddButtons(1, &tbButton));

    // Add Incident Report button

	VERIFY((tbButton.iString = m_eventsToolbar->AddStrings(TALoadString(IDS_INCIDENT_REPORT_STR))) != -1);
	tbButton.iBitmap = m_eventsTbImageListEnabled.Add(pApp->LoadIcon(IDI_INCIDENT));
	m_eventsTbImageListHot.Add(pApp->LoadIcon(IDI_INCIDENT_HOT));
	tbButton.fsState = TBSTATE_HIDDEN ; //TBSTATE_ENABLED ;  //hide this button("Incident Report") for C955
	tbButton.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;
	tbButton.dwData = 0;
	tbButton.idCommand = IDC_VIEW_INCIDENTS;
	VERIFY(m_eventsToolbar->AddButtons(1, &tbButton));

    // Add Alarm Manager button.

	VERIFY((tbButton.iString = m_eventsToolbar->AddStrings(TALoadString(IDS_ALARM_MANAGER_STR))) != -1);
	tbButton.iBitmap = m_eventsTbImageListEnabled.Add(pApp->LoadIcon(IDI_ALARMMANAGER));
	m_eventsTbImageListHot.Add(pApp->LoadIcon(IDI_ALARMMANAGER_HOT));
	tbButton.fsState = TBSTATE_ENABLED;
	tbButton.fsStyle = TBSTYLE_BUTTON | TBSTYLE_AUTOSIZE;
	tbButton.dwData = 0;
	tbButton.idCommand = IDC_ALARM_MANAGER;
	VERIFY(m_eventsToolbar->AddButtons(1, &tbButton));

    ////////////////////////////
    // Named Filter Combo Box //
    ////////////////////////////
    {
        // Add Space

        tbButton.iBitmap = NULL;
        tbButton.fsState = TBSTATE_ENABLED;
        tbButton.fsStyle = TBSTYLE_SEP;
        tbButton.dwData = 0;
        tbButton.idCommand = 0;
        VERIFY(m_eventsToolbar->AddButtons(1, &tbButton));

        // Add Combo box label placeholder. You can't actually add separate label to a toolbar, so we're just adding 
        // a wide "separator" button on top of which we can place the label (in a bit)

	    tbButton.iBitmap = NAMED_FILTER_LABEL_SEPARATOR_WIDTH;
        tbButton.fsState = TBSTATE_ENABLED;
        tbButton.fsStyle = TBSTYLE_SEP;
        tbButton.dwData = 0;
	    tbButton.idCommand = NAMED_FILTER_LABEL_SEPARATOR_ID;
	    VERIFY(m_eventsToolbar->AddButtons(1, &tbButton));

        // Add Combo box placeholder. As with the label, can't directly add a combo box to a toolbar, so again, make a large
        // spearator button, and cover it with the combo box later.

	    tbButton.iBitmap = NAMED_FILTER_COMBO_SEPARATOR_WIDTH;
        tbButton.fsState = TBSTATE_ENABLED;
        tbButton.fsStyle = TBSTYLE_SEP;
        tbButton.dwData = 0;
	    tbButton.idCommand = NAMED_FILTER_COMBO_SEPARATOR_ID;
	    VERIFY(m_eventsToolbar->AddButtons(1, &tbButton));

        // Use two different rects when positioning the label and combo box.
        // rect will be the 'const' version, while rectCopy will contain a copy
        // of rect. This will save excessive calls to GetItemRect.
	    CRect rect, rectCopy;
	    int nIndex = m_eventsToolbar->CommandToIndex(NAMED_FILTER_LABEL_SEPARATOR_ID);
        m_eventsToolbar->GetItemRect( nIndex, &rect );

        // Determine the size and position for the static that hides label separator.
        rectCopy = rect;
        rectCopy.left += NAMED_FILTER_LABEL_OFFSET;
        CStatic* hideSeparator = new CStatic();
        hideSeparator->Create(UNICODE_EMPTY_STR, WS_VISIBLE, rectCopy, m_eventsToolbar );

        // Determine size and position for label.
        rectCopy = rect;
        rectCopy.left += NAMED_FILTER_LABEL_OFFSET; // move it right a bit to make way for the previous box
        rectCopy.top = NAMED_FILTER_LABEL_TOP;

        // Insert the label.
        CStatic* namedFilter = new CStatic();
        namedFilter->Create( TALoadString(IDS_NAMED_FILTER_LABEL_STR), WS_VISIBLE|SS_RIGHT, rectCopy, m_eventsToolbar );

        nIndex = m_eventsToolbar->CommandToIndex(NAMED_FILTER_COMBO_SEPARATOR_ID);
        m_eventsToolbar->GetItemRect(nIndex, &rect);

        // Determine size and position for the static that hides combo separator.
        rectCopy = rect;
        rectCopy.left += NAMED_FILTER_COMBO_OFFSET; // move it right a bit to make way for the previous label
        hideSeparator = new CStatic();
        hideSeparator->Create(UNICODE_EMPTY_STR , WS_VISIBLE, rectCopy, m_eventsToolbar );

        // Determine size and position for combo box.
        rectCopy = rect;
        rectCopy.left += NAMED_FILTER_COMBO_OFFSET;
        rectCopy.top = NAMED_FILTER_COMBO_TOP;
	    rectCopy.bottom = NAMED_FILTER_COMBO_DROP_HEIGHT;

        // Insert the combo box.
	    m_namedFiltersComboBox.Create(CBS_DROPDOWNLIST | WS_VISIBLE |
		    WS_TABSTOP | WS_VSCROLL | CBS_SORT, rectCopy, m_eventsToolbar, ID_NAMED_FILTER_COMBO);
		m_namedFiltersComboBox.Invalidate();
	    
		//TD17143 - must be called onrefreshrights
        // Populate it.
        //populateNamedFilters();
    }

    ///////////////////////////
    // Event Level Combo Box //
    ///////////////////////////
    //{
         // Add Space

        tbButton.iBitmap = NULL;
        tbButton.fsState = TBSTATE_ENABLED;
        tbButton.fsStyle = TBSTYLE_SEP;
        tbButton.dwData = 0;
        tbButton.idCommand = EVENT_LEVEL_SEPARATOR_ID;
        VERIFY(m_eventsToolbar->AddButtons(1, &tbButton));
/*
        // Add Combo box label placeholder. You can't actually add separate label to a toolbar, so we're just adding 
        // a wide "separator" button on top of which we can place the label (in a bit)

	    tbButton.iBitmap = EVENT_LEVEL_LABEL_SEPARATOR_WIDTH;
        tbButton.fsState = TBSTATE_ENABLED;
        tbButton.fsStyle = TBSTYLE_SEP;
        tbButton.dwData = 0;
	    tbButton.idCommand = EVENT_LEVEL_LABEL_SEPARATOR_ID;
	    VERIFY(m_eventsToolbar.AddButtons(1, &tbButton));

        // Add Combo box placeholder. As with the label, can't directly add a combo box to a toolbar, so again, make a large
        // spearator button, and cover it with the combo box later.

	    tbButton.iBitmap = EVENT_LEVEL_COMBO_SEPARATOR_WIDTH;
        tbButton.fsState = TBSTATE_ENABLED;
        tbButton.fsStyle = TBSTYLE_SEP;
        tbButton.dwData = 0;
	    tbButton.idCommand = EVENT_LEVEL_COMBO_SEPARATOR_ID;
	    VERIFY(m_eventsToolbar.AddButtons(1, &tbButton));

        // Use two different rects when positioning the label and combo box.
        // rect will be the 'const' version, while rectCopy will contain a copy
        // of rect. This will save excessive calls to GetItemRect.
	    CRect rect, rectCopy;
	    int nIndex = m_eventsToolbar.CommandToIndex(EVENT_LEVEL_LABEL_SEPARATOR_ID);
        m_eventsToolbar.GetItemRect( nIndex, &rect );

        // Determine the size and position for the static that hides label separator.
        rectCopy = rect;
        rectCopy.left += EVENT_LEVEL_LABEL_OFFSET;
        CStatic* hideSeparator = new CStatic();
        hideSeparator->Create( "", WS_VISIBLE, rectCopy, &m_eventsToolbar );

        // Determine size and position for label.
        rectCopy = rect;
        rectCopy.left += EVENT_LEVEL_LABEL_OFFSET; // move it right a bit to make way for the previous box
        rectCopy.top = EVENT_LEVEL_LABEL_TOP;

        // Insert the label.
        m_eventLevelsStatic.Create( EVENT_LEVEL_LABEL_TEXT, WS_VISIBLE|SS_RIGHT, rectCopy, &m_eventsToolbar );

        nIndex = m_eventsToolbar.CommandToIndex(EVENT_LEVEL_COMBO_SEPARATOR_ID);
        m_eventsToolbar.GetItemRect(nIndex, &rect);

        // Determine size and position for the static that hides combo separator.
        rectCopy = rect;
        rectCopy.left += EVENT_LEVEL_COMBO_OFFSET; // move it right a bit to make way for the previous label
        hideSeparator = new CStatic();
        hideSeparator->Create( "", WS_VISIBLE, rectCopy, &m_eventsToolbar );

        // Determine size and position for combo box.
        rectCopy = rect;
        rectCopy.left += EVENT_LEVEL_COMBO_OFFSET;
        rectCopy.top = EVENT_LEVEL_COMBO_TOP;
	    rectCopy.bottom = EVENT_LEVEL_COMBO_DROP_HEIGHT;

        // Insert the combo box.
	    m_eventLevelsComboBox.Create( CBS_DROPDOWNLIST|WS_VISIBLE|WS_TABSTOP|WS_VSCROLL|CBS_SORT,
                                      rectCopy, &m_eventsToolbar, ID_EVENT_LEVEL_COMBO);
	    
		//TD17143 - moved 
        // Populate it.
        // populateEventLevels();
    }*/

    // Set the image lists in the view
   	m_eventsToolbar->SetImageList(&m_eventsTbImageListEnabled);
	m_eventsToolbar->SetHotImageList(&m_eventsTbImageListHot);

    // Disable the buttons that are only enabled when one event is selected
    m_eventsToolbar->EnableButton(IDC_VIEW_INCIDENTS, false);
    
    // Disable the tool-bar buttons that are not yet implemented
    m_eventsToolbar->EnableButton(IDC_NEXT,false);

    // Prepare the page navigation buttons that arent actually part of the toolbar.
    HICON buttonIcon;
    buttonIcon = AfxGetApp()->LoadIcon(IDI_PAGE_UP);
    m_pageUp.SetIcon(buttonIcon);
    m_pageUp.EnableWindow(FALSE);
	m_pageUp.ShowWindow(SW_HIDE);  // Hide this CButton for C955
	
    buttonIcon = AfxGetApp()->LoadIcon(IDI_PAGE_DOWN);
    m_pageDown.SetIcon(buttonIcon);
    m_pageDown.EnableWindow(FALSE);
	m_pageDown.ShowWindow(SW_HIDE); // Hise this CButton for C955

	// Check the item in the menu
    GetMenu()->CheckMenuItem(IDC_TOOL_BAR,MF_CHECKED);

    // Re-draw the tool bar (just to make sure everything shows up fine).
    m_eventsToolbar->RedrawWindow();
	
	//TATranslateToolBar(IDC_TOOL_BAR, &m_eventsToolbar );

    // Now make sure rights are applied.
	//TD17143++ - call is moved to the windows message handler
    //applyRights();
	//++TD17143	
    FUNCTION_EXIT;
}

void EventViewerDlg::setUpMenu()
{
    FUNCTION_ENTRY("setUpMenu");
    // Get the main menu
    CMenu* theMenu = GetMenu();
	CMenu* submenu = theMenu->GetSubMenu(1);
	submenu->DeleteMenu(5,MF_BYPOSITION);  //for deleting the menuItem "View->Show All Events"
	submenu->DeleteMenu(4,MF_BYPOSITION);  //for deleting the menuItem "----" before "View->Show All Events"


    // Determine if the close button and file->exit are to be disabled
	 EventViewerApp * pApp = dynamic_cast<EventViewerApp*> (AfxGetApp());
    TA_Base_Core::EventViewer* guiEntity = dynamic_cast<TA_Base_Core::EventViewer*>( pApp->getApplicationController()->getEntity());

    bool disableClose(false);
    try
    {
        disableClose = guiEntity->isCloseDisabled();
    }
    catch(...)
    {
        disableClose = false;
    }   

    if(disableClose)
    {
        // Disable the close button
        HMENU hmenu = ::GetSystemMenu(AfxGetMainWnd()->m_hWnd, FALSE);
        ::EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED | MF_BYCOMMAND);

        // Now disable the exit button
        theMenu->EnableMenuItem(ID_FILE_EXIT,MF_GRAYED);
    }
    FUNCTION_EXIT;
}

void EventViewerDlg::PostMessageToUIThread(UINT messageId, WPARAM wParam, LPARAM lParam)
{
	//AfxGetMainWnd()->PostMessage(messageId,wParam,lParam);
	::PostMessage(m_mainViewHandle,  messageId, wParam, lParam);
}

std::vector<TA_Base_App::ATSSystemItem> EventViewerDlg::getAtsSystemsVector()
{
	return m_realPModel->m_ATSSystemsVector;
}

std::vector<ISCSSubsystemItem> EventViewerDlg::getISCSSubsystemVector()
{
	return m_realPModel->m_ISCSsubsystemsVector;
}

void EventViewerDlg::setNamedFilter(TA_Base_Core::NamedFilterAccessFactory::FilterNameList filterList)
{
	m_realPModel->setNamedFilter(filterList);
}

TA_Base_Core::NamedFilterAccessFactory::FilterNameList EventViewerDlg::getNamedFilter()
{
	return m_realPModel->getFilterNameList();
}
void EventViewerDlg::showAllEventsMenu(bool showAllEventsEnabled)
{
	MENUITEMINFO info;
	info.cbSize = sizeof (MENUITEMINFO); // must fill up this field
	info.fMask = MIIM_STATE;             // get the state of the menu item
	GetMenu()->GetMenuItemInfo( IDC_SHOW_ALL, &info );
	

	// Enable the menu command for show all events
	if ( showAllEventsEnabled )
	{
		if ( info.fState == MFS_GRAYED )
		{
			// enable the menu item.
			GetMenu()->EnableMenuItem( IDC_SHOW_ALL, MF_ENABLED|MF_BYCOMMAND );
		}
	}
	else
	{
		if ( info.fState == MFS_ENABLED )
		{
			// gray the menu item.
			GetMenu()->EnableMenuItem( IDC_SHOW_ALL, MF_GRAYED|MF_BYCOMMAND );
		}
	}
}

/*void EventViewerDlg::eventLevelToolbar(bool eventLevelVisible)
{
	if ( eventLevelVisible )
    {		
		m_eventsToolbar.HideButton( EVENT_LEVEL_SEPARATOR_ID, FALSE );
	}
    else
    {
		m_eventsToolbar.HideButton( EVENT_LEVEL_SEPARATOR_ID, FALSE );
    }
}*/
void EventViewerDlg::DeleteToolBarCtrl()
{
	delete m_eventsToolbar;	
	m_eventsToolbar = NULL;
}

LRESULT EventViewerDlg::OnSetLanguage(WPARAM wParam, LPARAM lParam)
{	
    TATranslateMenu(IDR_EV_MAIN_MENU , GetMenu() );
    DrawMenuBar();
	getDisplayStringFromRes();
	DeleteToolBarCtrl();
	setUpToolBar();
	//change listCtrl Caption
	m_eventList.ChangeColumnsCaption();	
	m_realPModel->reloadEvents();
	populateNamedFilters();
	LRESULT ret = AbstractTransActiveDialog::OnSetLanguage( wParam, lParam );
	setWindowCaption();
	return ret;
}

//////////////////////////////////////////////////////////////////////////////////////////
/// AbstractTransActiveDialog member functions

void EventViewerDlg::initUIControl()
{
	TAAppendLangMenu( GetMenu() );
	TATranslateMenu( IDR_EV_MAIN_MENU, GetMenu() );
	DrawMenuBar();
}

std::string EventViewerDlg::getMyPModelName()
{
	return EVENT_VIEWER_DLG_PMODEL;
}

void EventViewerDlg::setupPModel()
{
	m_realPModel = dynamic_cast<EventViewerDlgPModel*> (m_pModel);
	TA_ASSERT(m_realPModel != NULL, "PModel has been initialized successfully");
	m_realPModel->attachView(this, INIT_PAGER_CTRL.c_str());
	m_realPModel->attachView(this, SETUP_STATUS_BAR.c_str());
	m_realPModel->attachView(this, SETUP_TOOLBAR.c_str());
	m_realPModel->attachView(this, SETUP_MENU.c_str());
	m_realPModel->attachView(this, INIT_EVENT_LIST_COMPONENT.c_str());
	m_realPModel->attachView(this, UPDATE_PREVIOUS_TOOLBAR.c_str());
	m_realPModel->attachView(this, UPDATE_NEXT_TOOLBAR.c_str());
	m_realPModel->attachView(this, UPDATE_FILTER_DLG.c_str());
	m_realPModel->attachView(this, POPULATE_NAMED_FILTERS.c_str());
	m_realPModel->attachView(this, SETUP_ALLEVENTS_MENU.c_str());
	m_realPModel->attachView(this, POST_MESSAGE_TO_UI.c_str());
	m_realPModel->attachView(this, SHOW_USER_MESSAGE_NOTIFY.c_str());
	m_realPModel->attachView(this, INVOKE_READY_TO_CLOSE.c_str());	
}

void EventViewerDlg::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
{
	std::string type(updateType);
	if( INIT_PAGER_CTRL == type)
	{
		InitPagerCtrl();
	}	
	else if(SETUP_STATUS_BAR == type)
	{	
		setUpStatusBar();
	}
	else if(SETUP_TOOLBAR == type)
	{	
		setUpPager();
		setUpToolBar();
	}
	else if(SETUP_MENU == type)
	{	
		setUpMenu();
	}
	else if(INIT_EVENT_LIST_COMPONENT == type)
	{
		InitEventListCtrl();
	}
	else if(UPDATE_PREVIOUS_TOOLBAR == type)
	{
		TA_Base_App::GenericEventArgs * gargs = dynamic_cast<TA_Base_App::GenericEventArgs *>(args);
		bool update = *((bool *) gargs->Param1);
		m_eventsToolbar->EnableButton(IDC_PREVIOUS,update);		
	}
	else if(UPDATE_NEXT_TOOLBAR == type)
	{
		TA_Base_App::GenericEventArgs * gargs = dynamic_cast<TA_Base_App::GenericEventArgs *>(args);
		bool update = *((bool *) gargs->Param1);
		m_eventsToolbar->EnableButton(IDC_NEXT,update);
		m_eventsToolbar->SetHotItem(5);		
	}
	else if(UPDATE_FILTER_DLG == type)
	{
		if (m_filterDlg != NULL)
		{
			// If there is a currently active filter dialog, notify it so it can update its to and from times
			m_filterDlg->updateFilter(); 
		}
	}
	else if (SHOW_WINDOW_TEXT == type)
	{
		setWindowCaption();
	}
	else if(POPULATE_NAMED_FILTERS == type)
	{		
		populateNamedFilters();
	}
	else if(SETUP_ALLEVENTS_MENU == type)
	{
		TA_Base_App::GenericEventArgs * gargs = dynamic_cast<TA_Base_App::GenericEventArgs *>(args);
		bool update = *((bool *) gargs->Param1);
		//delete gargs;
		//gargs = NULL;
		showAllEventsMenu(update);
	}
	else if(POST_MESSAGE_TO_UI == type)
	{
		TA_Base_App::GenericEventArgs * gargs = dynamic_cast<TA_Base_App::GenericEventArgs *>(args);
		UINT messageID = *((UINT *) (gargs->Param1));
		WPARAM wParam = *((WPARAM *)gargs->Param2);
		LPARAM lParam = *((LPARAM *)gargs->Param3);
		PostMessageToUIThread(messageID,wParam,lParam);
	}
	else if(SHOW_USER_MESSAGE_NOTIFY == type)
	{
		TA_Base_App::GenericEventArgs * gargs = dynamic_cast<TA_Base_App::GenericEventArgs *>(args);
		int message = *((int *) (gargs->Param1));
		std::string param1,param2;
		if(gargs->Param2 != NULL)
		{
			std::string *str1 = static_cast<std::string*>(gargs->Param2);
			param1 = *str1;
		}
		if(gargs->Param3 != NULL)
		{
			std::string *str2 = static_cast<std::string*>(gargs->Param3);
			param2 = *str2;
		}
		TA_Base_Bus::TransActiveMessage userMsg;
		if(param1.length() != 0)
		{
			userMsg << param1;
		}
		if(param2.length() != 0)
		{
			userMsg << param2;
		}
		UINT selectedButton = userMsg.showMsgBox(message);
	}
	else if(INVOKE_READY_TO_CLOSE == type)
	{
		readyToClose();
	}

}