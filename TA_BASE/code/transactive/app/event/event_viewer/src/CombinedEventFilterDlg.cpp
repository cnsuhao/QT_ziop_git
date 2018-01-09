/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/event/event_viewer/src/CombinedEventFilterDlg.cpp $
 * @author:  Bradley Cleaver
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2015/10/09 15:42:35 $
 * Last modified by:  $Author: CM $
 * 
 * CombinedEventFilterDlg is an MFC class deriving from the FilterDlg header (which is in turn a CDialog derived class).
 * This dialog provides filtering capabilities on the data items displayed in the combined event view.
 */

#pragma warning(disable:4786)       // identifier truncated to 255 characters
#pragma warning(disable:4800)       // forcing value to bool 'true' or 'false' (performance warning)

//#include <afxtempl.h> // For the CArray
#include <algorithm>

#include "app/event/event_viewer/src/stdafx.h"
#include "app/event/event_viewer/src/EventViewer.h"
#include "app/event/event_viewer/src/EventViewerDlg.h"
#include "app/event/event_viewer/src/CombinedEventFilterDlg.h"
#include "app/event/event_viewer/src/SaveFilterDlg.h"
#include "app/event/event_viewer/src/EventViewerGUIConstants.h"
#include  "app/event/event_viewer/pmodel/src/ConstantItems.h"
#include "app/event/event_viewer/pmodel/src/GenericEventArgs.h"

#include "bus/generic_gui_view/src/TransActiveMessage.h"

#include "core/data_access_interface/src/NamedFilter.h"
#include "core/data_access_interface/src/NamedFilterAccessFactory.h"
#include "core/exceptions/src/DataException.h"

using TA_Base_Core::DataException;
using TA_Base_Core::NamedFilter;
using TA_Base_Core::NamedFilterAccessFactory;


#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CombinedEventFilterDlg dialog

const int CombinedEventFilterDlg::UNCHECKED = 0;
const int CombinedEventFilterDlg::CHECKED = 1;
const int CombinedEventFilterDlg::COMBO_LIMIT = 0;



CombinedEventFilterDlg::CombinedEventFilterDlg(/*CombinedEventFilter* filter,*/ CWnd* pParent /*=NULL*/)
: FilterDlg(CombinedEventFilterDlg::IDD, pParent) ,m_realPModel(NULL)
{
	//{{AFX_DATA_INIT(CombinedEventFilterDlg)
	//}}AFX_DATA_INIT
	CoInitialize(0);
}

CombinedEventFilterDlg::~CombinedEventFilterDlg()
{
	CoUninitialize();
	//detach this from its pmodel
	if(m_realPModel != NULL)
	{
		m_realPModel->detachView(this, INVOKE_POSTMESSGAE_NOTIFICATION.c_str());
		m_realPModel->detachView(this, CLEAR_SORT_MASK.c_str());
		m_realPModel->detachView(this, SET_TIME_CONTROLS.c_str());
		m_realPModel->detachView(this, SET_WINDOW_TEXT.c_str());
		m_realPModel->detachView(this, SET_SEVERITY_COMBOX_BOX.c_str());
		m_realPModel->detachView(this, GET_FILTER_DATA.c_str());
		m_realPModel->detachView(this, POPULATE_TMTYPE.c_str());
		m_realPModel->detachView(this, RESET_FILTER_TIME.c_str());	
		m_realPModel->detachView(this, SHOW_USER_MESSAGE_NOTIFY.c_str());	
	}
}


void CombinedEventFilterDlg::DoDataExchange(CDataExchange* pDX)
{
	FilterDlg::DoDataExchange(pDX);
	//{{AFX_DATA_MAP(CombinedEventFilterDlg)
	DDX_Control(pDX, IDC_ALARM_TYPE, m_alarmtypeEdit);
	DDX_Control(pDX, IDC_SYSTEM, m_systemEdit);
	DDX_Control(pDX, IDC_ALARM_STATE, m_alarmStateEdit);
	DDX_Control(pDX, IDC_OPERATOR, m_operatorEdit);
	DDX_Control(pDX, IDC_FROM, m_fromCheck);
	DDX_Control(pDX, IDOK, m_okButton);
	DDX_Control(pDX, IDCANCEL, m_cancelButton);
	DDX_Control(pDX, ID_DEFAULT_FILTER, m_defaultButton);
	DDX_Control(pDX, ID_APPLY, m_applyButton);
	DDX_Control(pDX, IDC_ALARM_ID, m_alarmIdEdit);
	DDX_Control(pDX, IDC_DESCRIPTION, m_descriptionEdit);
	DDX_Control(pDX, IDC_ASSET, m_assetEdit);
	DDX_Control(pDX, IDC_LOCATION, m_locationEdit);
	DDX_Control(pDX, IDC_SUBSYSTEM, m_subsystemEdit);
	DDX_Control(pDX, IDC_TO_TIME, m_toTime);
	DDX_Control(pDX, IDC_TO_DATE, m_toDate);
	DDX_Control(pDX, IDC_CURRENT, m_toCheck);
	DDX_Control(pDX, IDC_FROM_TIME, m_fromTime);
	DDX_Control(pDX, IDC_FROM_DATE, m_fromDate);
	DDX_Control(pDX, IDC_COMBO_NAMEDFILTER, m_namedFilterCombo);
	DDX_Control(pDX, IDC_BUTTON_SAVEFILTER, m_saveFilterButton);
	DDX_Control(pDX, IDC_BUTTON_DELETEFILTER, m_deleteFilterButton);
	//}}AFX_DATA_MAP
	DDX_Control(pDX, IDC_RADIO1, m_radio1);
	DDX_Control(pDX, IDC_RADIO2, m_radio2);
	DDX_Control(pDX, IDC_RADIO3, m_radio3);
	DDX_Control(pDX, IDC_RADIO4, m_radio4);
}


BEGIN_MESSAGE_MAP(CombinedEventFilterDlg, FilterDlg)
	//{{AFX_MSG_MAP(CombinedEventFilterDlg)
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_CURRENT, OnTo)
	ON_BN_CLICKED(IDC_SUBSYSTEM_EXT, OnSubsystemExt)
	ON_BN_CLICKED(IDC_LOCATION_EX, OnLocationExt)
	ON_BN_CLICKED(ID_APPLY, OnApply)
	ON_BN_CLICKED(ID_DEFAULT_FILTER, OnDefaultFilter)
	ON_EN_CHANGE(IDC_ASSET, OnChangeAsset)
	ON_EN_CHANGE(IDC_DESCRIPTION, OnChangeDescription)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TO_DATE, OnDatetimechangeToDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_TO_TIME, OnDatetimechangeToTime)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_FROM_DATE, OnDatetimechangeFromDate)
	ON_NOTIFY(DTN_DATETIMECHANGE, IDC_FROM_TIME, OnDatetimechangeFromTime)
	ON_LBN_KILLFOCUS(IDC_SEVERITY_LIST, OnKillfocusSeverityList)
	ON_CONTROL(CLBN_CHKCHANGE, IDC_SEVERITY_LIST, OnChkChange)
	ON_BN_CLICKED(IDC_FROM, OnFrom)
	ON_BN_CLICKED(IDC_EVENTTYPE_EXT, OnSystemExt)
	ON_BN_CLICKED(IDC_ALARMTYPE_EXT, OnAlarmtypeExt)
	ON_BN_CLICKED(IDC_OPERATOR_EXT, OnOperatorExt)
    ON_BN_CLICKED(IDC_BUTTON_SAVEFILTER, OnSaveFilter)
    ON_BN_CLICKED(IDC_BUTTON_DELETEFILTER, OnDeleteFilter)
	ON_CBN_SELCHANGE(IDC_COMBO_NAMEDFILTER, OnSelchangeComboNamedFilter)
	ON_CBN_EDITCHANGE(IDC_COMBO_NAMEDFILTER,OnTextchangeNamedFilter)
	//	ON_BN_CLICKED(IDC_RADIO1, &CombinedEventFilterDlg::OnBnClickedRadio1)
	//	ON_BN_CLICKED(IDC_RADIO2, &CombinedEventFilterDlg::OnBnClickedRadio2)
	//ON_NOTIFY(BCN_HOTITEMCHANGE, IDC_RADIO1, &CombinedEventFilterDlg::OnBnHotItemChangeRadio1)
	ON_BN_CLICKED(IDC_RADIO1, &CombinedEventFilterDlg::OnBnClickedRadio1)
	ON_BN_CLICKED(IDC_RADIO2, &CombinedEventFilterDlg::OnBnClickedRadio2)
	ON_BN_CLICKED(IDC_RADIO3, &CombinedEventFilterDlg::OnBnClickedRadio3)
	ON_BN_CLICKED(IDC_RADIO4, &CombinedEventFilterDlg::OnBnClickedRadio4)
	ON_BN_CLICKED(IDC_ALARMTYPE_EXT2, &CombinedEventFilterDlg::OnAlarmStateExt)
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CombinedEventFilterDlg message handlers

void CombinedEventFilterDlg::OnClose() 
{
    FUNCTION_ENTRY("OnClose");
	
	m_realPModel->Close();

    // And close this dialog
    FUNCTION_EXIT;
    FilterDlg::OnClose();
}

void CombinedEventFilterDlg::OnApply()
{
    FUNCTION_ENTRY("OnApply");

	m_realPModel->setNamedFilter();
	
	// Enable and disable buttons as appropriate
	m_applyButton.EnableWindow(false);

	// Force the named filters to refresh.
    populateNamedFilters();

	m_realPModel->apply();

    FUNCTION_EXIT;
}


void CombinedEventFilterDlg::OnOK() 
{
    FUNCTION_ENTRY("OnOK");

	 m_realPModel->onOK();
    // And then actually close the dialog
    FUNCTION_EXIT;
    FilterDlg::OnOK();
}

void CombinedEventFilterDlg::OnCancel() 
{
    FUNCTION_ENTRY("OnCancel");

    // Dialog was canceled, so don't want to apply the filters. Do want to send the cancel message however
    OnClose();

    // Make sure the dialog closes (OnClose() should actually close it)
    FUNCTION_EXIT;
    FilterDlg::OnCancel();
}

BOOL CombinedEventFilterDlg::OnInitDialog() 
{
    FUNCTION_ENTRY("OnInitDialog");

    FilterDlg::OnInitDialog();

	m_realPModel->setFilterFromAppController();

    // Set max length to same as database field
    m_assetEdit.SetLimitText(80);
    m_descriptionEdit.SetLimitText(300);

	// Populate the dialog fields with correct data
	setUpSeverities( m_realPModel->m_alarmSeverities );

	EventViewerDlg* evDlg = dynamic_cast<EventViewerDlg *>(GetParent());

	m_realPModel->intialSetup(evDlg->getAtsSystemsVector(),evDlg->getISCSSubsystemVector());


	
	// Populate the named filter combo box.
	populateNamedFilters();

	// Enable and disable buttons as appropriate
    m_applyButton.EnableWindow(false);
    m_okButton.EnableWindow(true);

	// TD #12583
	// The Save button should be disabled upon startup
	m_saveFilterButton.EnableWindow(false);

	m_namedFilterCombo.LimitText(32); //TD15027 azenitha
	LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "TD15027: Set limit of text");

    FUNCTION_EXIT;
    return TRUE;  // return TRUE unless you set the focus to a control
                  // EXCEPTION: OCX Property Pages should return FALSE
}

void CombinedEventFilterDlg::OnFrom() 
{
    FUNCTION_ENTRY("OnFrom");

     // Called when the "From: checkbox state changes.
    if ( UNCHECKED == m_fromCheck.GetCheck() )
    {
        // The box is now unchecked, so need to disable the "from" filters
        m_fromTime.EnableWindow(false);
        m_fromDate.EnableWindow(false);

        // Get the current "to" time.
		offsetFromTimeTakeRefToTime(TWENTY_FOUR_HOURS);

        // If the value of "from" depends on "to" don't apply any restrictions
        // on the "to" range.
        // Need to cast to avoid ambiguous use of SetRange
        m_toTime.SetRange( static_cast<CTime*>(NULL), static_cast<CTime*>(NULL) );
        m_toDate.SetRange( static_cast<CTime*>(NULL), static_cast<CTime*>(NULL) );        
    }
    else
    {
		// The box is now checked, so need to enable the "from" filters
		m_fromTime.EnableWindow(true);
		m_fromDate.EnableWindow(true);

		CTime currentTime = CTime::GetCurrentTime();
		m_fromDate.SetRange(&EARLY_TIME_LIMIT,&currentTime);
		m_fromTime.SetRange(&EARLY_TIME_LIMIT,&currentTime);

		// TD 14914 Update the To-time only if it is unchecked
        // If the previous button is not checked we need to update its value.
        if ( UNCHECKED == m_toCheck.GetCheck() )
        {
			offsetToTimeTakeRefFromTime(TWENTY_FOUR_HOURS);
		}
    }

    // Enable Buttons as necessary
    enableButtons();

    // Now that the filter is changed it is user defined.
    selectUserFilter();

    // Update the named filter times.
    updateTimes();
    
    FUNCTION_EXIT;
}

void CombinedEventFilterDlg::OnTo() 
{
    FUNCTION_ENTRY("OnTo");

   // Called when the "To:" checkbox state changes
    if (UNCHECKED == m_toCheck.GetCheck())
    {
        // The box is now unchecked, so need to disable the "to" filters
        m_toTime.EnableWindow(false);
        m_toDate.EnableWindow(false);

        // Get the current "to" time.
        CTime toTime( CTime::GetCurrentTime() );

        // Set the value of the "to" fields.
        m_toDate.SetTime(&toTime);
        m_toTime.SetTime(&toTime);

        // Set valid time range for "from" field.
        m_fromTime.SetRange(&EARLY_TIME_LIMIT,&toTime);
        m_fromDate.SetRange(&EARLY_TIME_LIMIT,&toTime);        

        // If the previous button is not checked we need to update its value.
        if ( UNCHECKED == m_fromCheck.GetCheck() )
        {
			offsetFromTimeTakeRefToTime(TWENTY_FOUR_HOURS);
        }

    }
    else
    {
        // The box is now checked, so need to enable the "to" filters
        m_toTime.EnableWindow(true);
        m_toDate.EnableWindow(true);

		// Get the time from the From box and set to 24 hrs but not later than current time
        // If the previous button is not checked we need to update its value.
        if ( UNCHECKED == m_fromCheck.GetCheck() )
        {
			offsetFromTimeTakeRefToTime(TWENTY_FOUR_HOURS);
		}
  }

    // Enable Buttons as necessary
    enableButtons();

    // Now that the filter is changed it is user defined.
    selectUserFilter();

    // Update the filter buffers times.
    updateTimes();

    FUNCTION_EXIT;
}

void CombinedEventFilterDlg::OnSubsystemExt() 
{
     FUNCTION_ENTRY("OnSubsystemExt");
    // Called when the "..." next to the "Subsystem" edit box is clicked.

    // Create a MultipleLoadDlg - this will hold the list providing the selections.
    MultipleLoadDlg loadDlg(TALoadString(IDS_SUBSYSTEM_MULTI_DIALOG));

    // Populate the list with the loaded subsystems.
    loadDlg.setItems( m_realPModel->m_subsystems, true );

    // The selection is a combination of filters from application and physical subsystems.
    SelectedItems subsystems = m_realPModel->getSubSystemList();
	loadDlg.setSelection( subsystems );    

    // Display the dialog.
    if( loadDlg.DoModal() == IDOK )
    {
        // Get the current selection.
        SelectedItems selection = loadDlg.getSelection();

		m_realPModel->setFilterSubsystem(selection);

        // Populate the edit control with the new subsystems.
		TAUnicodeString displayStr = TAUnicodeString::CreateFromMultitByteString(m_realPModel->populateListStringFromSelection( selection, m_realPModel->m_subsystems).c_str());		
		m_subsystemEdit.SetWindowText(displayStr.c_str());

        // Now enable the buttons.
        enableButtons();

        // Now that the filter is changed it is user defined.
        selectUserFilter();
    } 
    FUNCTION_EXIT;
}

void CombinedEventFilterDlg::OnLocationExt() 
{
     FUNCTION_ENTRY("OnLocationExt");
    // Called when the "..." next to the "Location" edit box is clicked.

    // Create a MultipleLoadDlg - this will hold the list providing the selections.
    MultipleLoadDlg loadDlg(TALoadString(IDS_LOC_MULTI_DIALOG));

    // Populate the list with the loaded locations.	
    loadDlg.setItems( m_realPModel->m_locationKeyDisplayNameList, true );

    // Set the current selections.
    loadDlg.setSelection(m_realPModel->getNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_LOCATION ) );

    // Display the dialog.
    if ( loadDlg.DoModal() == IDOK )
    {
        // Get the list of selected items.
        SelectedItems selection = loadDlg.getSelection();

		m_realPModel->setFilterLocation(selection);

		TAUnicodeString displayStr = TAUnicodeString::CreateFromMultitByteString(m_realPModel->populateListStringFromSelection( selection, m_realPModel->m_locationKeyDisplayNameList ).c_str());		
		m_locationEdit.SetWindowText(displayStr.c_str());        

        // Now enable the buttons.
        enableButtons();

        // Now that the filter is changed it is user defined.
        selectUserFilter();
    }

    FUNCTION_EXIT;
}

void CombinedEventFilterDlg::OnOperatorExt() 
{
    FUNCTION_ENTRY("OnOperatorExt");
    // Called when the "..." next to the "Operator" edit box is clicked.

    // Create a MultipleLoadDlg - this will hold the list providing the selections.
    MultipleLoadDlg loadDlg(TALoadString(IDS_OPT_MULTI_DIALOG));

    // Populate the list with the loaded locations.
    loadDlg.setItems( m_realPModel->m_operators, true );

    // Set the current selections.
    loadDlg.setSelection( m_realPModel->getNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_OPERATOR ) );

    // Display the dialog.
    if ( loadDlg.DoModal() == IDOK )
    {
        // Get the list of selected items.
        SelectedItems selection = loadDlg.getSelection();

		 m_realPModel->setFilterOperator(selection);

 		TAUnicodeString displayStr = TAUnicodeString::CreateFromMultitByteString(m_realPModel->populateListStringFromSelection( selection, m_realPModel->m_operators ).c_str());		
		m_operatorEdit.SetWindowText(displayStr.c_str());        

        // Now enable the buttons.
        enableButtons();

        // Now that the filter is changed it is user defined.
        selectUserFilter();
    }

    FUNCTION_EXIT;
}

void CombinedEventFilterDlg::OnSystemExt() 
{
    FUNCTION_ENTRY("OnSystemExt");
    // Called when the "..." next to the "Event Type" edit box is clicked.

    // Create a MultipleLoadDlg - this will hold the list providing the selections.
    MultipleLoadDlg loadDlg(TALoadString(IDS_SYSTEM_MULTI_DIALOG));

    // Populate the list with the loaded locations.
    loadDlg.setItems( m_realPModel->m_systems, true );
    
    // Set the current selections.
    loadDlg.setSelection(m_realPModel->getFilterSystemList() );

    // Display the dialog.
    if ( loadDlg.DoModal() == IDOK )
    {
        // Get the list of selected items.
        SelectedItems selection = loadDlg.getSelection();
		
		m_realPModel->setFilterSystem(selection);

		// Populate the edit control with the new subsystems.
		TAUnicodeString displayStr = TAUnicodeString::CreateFromMultitByteString( m_realPModel->populateListStringFromSelection( selection, m_realPModel->m_systems ).c_str());		
		m_systemEdit.SetWindowText(displayStr.c_str()); 

		//dynamic change subsystems after user selected systems:
		m_realPModel->setSubsystemsBySystemkeys ( selection );

        // Now enable the buttons.
        enableButtons();

        // Now that the filter is changed it is user defined.
        selectUserFilter();
    }

    FUNCTION_EXIT;
}

void CombinedEventFilterDlg::OnAlarmtypeExt() 
{
    FUNCTION_ENTRY("OnAlarmtypeExt");
    // Called when the "..." next to the "Alarm Type" edit box is clicked.

    // Create a MultipleLoadDlg - this will hold the list providing the selections.
    MultipleLoadDlg loadDlg(TALoadString(IDS_ALARM_TYPE_DIALOG));

    // Populate the list with the loaded locations.
    loadDlg.setItems( m_realPModel->m_alarmtypes, true );

    // Set the current selections.
    loadDlg.setSelection( m_realPModel->getNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_ALARMTYPE ) );

    // Display the dialog.
    if ( loadDlg.DoModal() == IDOK )
    {
        // Get the list of selected items.
        SelectedItems selection = loadDlg.getSelection();

        // If selection changed, update selection and repopulate edit control.
        m_realPModel->setNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_ALARMTYPE, selection );
		TAUnicodeString displayStr = TAUnicodeString::CreateFromMultitByteString(m_realPModel->populateListStringFromSelection( selection, m_realPModel->m_alarmtypes ).c_str());		
		m_alarmtypeEdit.SetWindowText(displayStr.c_str()); 

        // Now enable the buttons.
        enableButtons();

        // Now that the filter is changed it is user defined.
        selectUserFilter();
    }

    FUNCTION_EXIT;
}

void CombinedEventFilterDlg::OnDefaultFilter() 
{
    FUNCTION_ENTRY("OnDefaultFilter");

	TAUnicodeString oldFilterName;
	m_namedFilterCombo.GetWindowText( oldFilterName.str() );

	bool applyBtnEnable = true;	

	try
	{
		m_realPModel->loadDefaultFilter(oldFilterName.toMultiByteStdString(),applyBtnEnable);
	}
	catch ( DataException& ex )
	{
		// Log the exception.
		LOG_EXCEPTION_CATCH( SourceInfo, "DataException", ex.getWhichData() );

        // Display the load error message.
        TA_Base_Bus::TransActiveMessage userMsg;
        userMsg << EVDisplayStrRes::GetInstance()->ERROR_LOAD;
        userMsg << EVDisplayStrRes::GetInstance()->ERROR_LOADED;
        userMsg.showMsgBox(IDS_UE_040031);
		return;
	}
	
	// Now enable the buttons.
	enableButtons();

	m_applyButton.EnableWindow (applyBtnEnable);	
	
	// Now force the combo box to reflect the new state.
    populateNamedFilters();

    m_saveFilterButton.EnableWindow( FALSE );

    FUNCTION_EXIT;
}

void CombinedEventFilterDlg::OnChangeAsset() 
{
   FUNCTION_ENTRY("OnChangeAsset");

    if ( m_realPModel->isDisableEvents() )
    {
        return;
    }

	// Enable the buttons.
    enableButtons();

    // Now that the filter is changed it is user defined.
    selectUserFilter();

    TAUnicodeString text;
    m_assetEdit.GetWindowText( text.str() );	
    m_realPModel->setFilterText(EVDisplayStrRes::GetInstance()->FILTER_ASSET,text.toMultiByteStdString());

    FUNCTION_EXIT;
}

void CombinedEventFilterDlg::OnTextchangeNamedFilter()
{
   // Get the new name.
    TAUnicodeString filterName;
    m_namedFilterCombo.GetWindowText( filterName.str() );

    // Change filter button states.
    enableNamedFilterButtons( filterName );
	
	// TD #12583
	// The Save button should always be enabled if the name has changed
	m_saveFilterButton.EnableWindow(true);
}

void CombinedEventFilterDlg::OnChangeDescription() 
{
   FUNCTION_ENTRY("OnChangeDescription");

    if (  m_realPModel->isDisableEvents()  )
    {
        return;
    }

    // Enable the buttons.
    enableButtons();

    // Now that the filter is changed it is user defined.
    selectUserFilter();

    TAUnicodeString text;
    m_descriptionEdit.GetWindowText( text.str() );
	m_realPModel->setFilterText(EVDisplayStrRes::GetInstance()->FILTER_DESCRIPTION,text.toMultiByteStdString());

    FUNCTION_EXIT;
}

void CombinedEventFilterDlg::OnKillfocusSeverityList() 
{
    // Remove the selection highlighting
    m_severityList.SetCurSel(-1);
}

void CombinedEventFilterDlg::OnChkChange() 
{
    FUNCTION_ENTRY("OnChkChange");

    if ( m_realPModel->isDisableEvents() )
    {
        return;
    }

    // Enable the buttons.
    enableButtons();

    // Now that the filter is changed it is user defined.
    selectUserFilter();

    // Update the filter buffer.
	SelectedItems severityList;
	for( unsigned long i = 0; i < static_cast<unsigned long>(m_severityList.GetCount()); ++i )
	{
		if( m_severityList.GetCheck(i) == CHECKED)
		{
			severityList.push_back(m_severityList.GetItemData(i));
		}
	}
	m_realPModel->setNumericFilter(EVDisplayStrRes::GetInstance()->FILTER_SEVERITY,severityList);
    FUNCTION_EXIT;
} 


void CombinedEventFilterDlg::OnDatetimechangeToDate(NMHDR* pNMHDR, LRESULT* pResult) 
{
	FUNCTION_ENTRY("OnDatetimechangeToDate");

    // Enable the buttons.
    enableButtons();

    // Now that the filter is changed it is user defined.
    selectUserFilter();

    // Synchronise times between controls.
    CTime toDate;
    m_toDate.GetTime( toDate );
    m_toTime.SetTime( &toDate );

	// offset FROM time if it is unchecked
    if ( UNCHECKED == m_fromCheck.GetCheck() )
    {
		offsetFromTimeTakeRefToTime(TWENTY_FOUR_HOURS);
	}

    // Update the filter buffers times.
    updateTimes();

    *pResult = 0;

    FUNCTION_EXIT;
}


void CombinedEventFilterDlg::OnDatetimechangeToTime(NMHDR* pNMHDR, LRESULT* pResult) 
{
       FUNCTION_ENTRY("OnDatetimechangeToTime");

    // Enable the buttons.
    enableButtons();

    // Now that the filter is changed it is user defined.
    selectUserFilter();

    // Synchronise times between controls.
    CTime toTime;
    m_toTime.GetTime( toTime );
    m_toDate.SetTime( &toTime );

	// offset FROM time if it is unchecked
    if ( UNCHECKED == m_fromCheck.GetCheck() )
    {
		offsetFromTimeTakeRefToTime(TWENTY_FOUR_HOURS);
	}

    // Update the filter buffers times.
    updateTimes();

    *pResult = 0;

    FUNCTION_EXIT;
}


void CombinedEventFilterDlg::OnDatetimechangeFromDate(NMHDR* pNMHDR, LRESULT* pResult) 
{
    FUNCTION_ENTRY("OnDatetimechangeFromDate");

    // Enable the buttons.
    enableButtons();

    // Now that the filter is changed it is user defined.
    selectUserFilter();

    // Synchronise times between controls.
    CTime fromDate;
    m_fromDate.GetTime( fromDate );
    m_fromTime.SetTime( &fromDate );

	// offset to time if it is unchecked
    if ( UNCHECKED == m_toCheck.GetCheck() )
    {
		offsetToTimeTakeRefFromTime(TWENTY_FOUR_HOURS);
	}

    // Update the filter buffers times.
    updateTimes();

    *pResult = 0;

    FUNCTION_EXIT;
}

void CombinedEventFilterDlg::OnDatetimechangeFromTime(NMHDR* pNMHDR, LRESULT* pResult) 
{
    FUNCTION_ENTRY("OnDatetimechangeFromTime");

    // Enable the ok, apply, and cancel buttons.
    enableButtons();

    // Now that the filter is changed it is user defined.
    selectUserFilter();

    // Synchronise times between controls.
    CTime fromTime;
    m_fromTime.GetTime( fromTime );
    m_fromDate.SetTime( &fromTime );

	// offset to time if it is unchecked
    if ( UNCHECKED == m_toCheck.GetCheck() )
    {
		offsetToTimeTakeRefFromTime(TWENTY_FOUR_HOURS);
	}

    // Update the filter buffers times.
    updateTimes();

    *pResult = 0;

    FUNCTION_EXIT;
}

void CombinedEventFilterDlg::OnSelchangeComboNamedFilter() 
{
   FUNCTION_ENTRY("OnSelchangeComboNamedFilter");

    // Get the currently selected item.
    int selIdx = m_namedFilterCombo.GetCurSel();
    if ( 0 <= selIdx )
    {
        // Get the name of the filter.
        TAUnicodeString filterName_c;
        m_namedFilterCombo.GetLBText( selIdx, filterName_c.str() );       
		bool enableButtonFlag = false;
		try
		{
			m_realPModel->loadSelectedFilter(filterName_c.toMultiByteStdString(),enableButtonFlag);
		}
		catch ( DataException& ex )
		{
			LOG_EXCEPTION_CATCH( SourceInfo, "DataException", ex.getWhichData() );
			// Display the load error message.
			 TA_Base_Bus::TransActiveMessage userMsg;
			 userMsg << EVDisplayStrRes::GetInstance()->ERROR_LOAD;
			 userMsg << EVDisplayStrRes::GetInstance()->ERROR_LOADED;
			 userMsg.showMsgBox(IDS_UE_040031);			
			// Reselect the current named filter.
			populateNamedFilters();
			return;
		}


		if(enableButtonFlag)
		{
			// Enable the ok, apply, and cancel buttons.
			enableButtons();
		}

        // Enable the named filter buttons as required.
        enableNamedFilterButtons( filterName_c );

		// TD #12583
		// The Save button should always be disabled after a saved named filter,
		// has been selected (including "Default Filter", etc)
		m_saveFilterButton.EnableWindow(false);
    }

    FUNCTION_EXIT;
}

void CombinedEventFilterDlg::OnSaveFilter()
{
    FUNCTION_ENTRY("OnSaveFilter");

    // Get the name that user wants to save the filter as.
    TAUnicodeString filterNameCStr;
    m_namedFilterCombo.GetWindowText( filterNameCStr.str() );   

	TAUnicodeString old_filterName = UNICODE_EMPTY_STR;
	ENamedFilterState namedFilterState = getNamedFilterState( filterNameCStr);
	if (NF_READWRITE == namedFilterState)
	{
		old_filterName = getOldFilterName(filterNameCStr);
	}

	filterNameCStr.str().MakeUpper();
	bool isUserORDefindORDefault = ( filterNameCStr.Find(TALoadString(IDS_USER_FILTER_NAME)) >= 0 
		|| filterNameCStr.Find(TALoadString(IDS_DEFAULT_FILTER_NAME)) >= 0 ||filterNameCStr.Find(TALoadString(IDS_DEFINED_FILTER_NAME)) >= 0 ) ? true : false;

	//bool isInComboList =  m_namedFilterCombo.FindStringExact(-1, filterNameCStr) >= 0 ? true : false;

	if ( NF_READONLY == namedFilterState 
		|| (NF_UNSAVED == namedFilterState && isUserORDefindORDefault) 
		||NF_USERDEFINED == namedFilterState ) // Can't save filters with these kind of names
	{
		// Display a message box to disallow these keywords as name
		TA_Base_Bus::TransActiveMessage userMsg;
		const TAUnicodeString saveEr(TALoadString(IDS_SAVE_ERR_STRING));
		const TAUnicodeString fileEr(TAAfxFormatString1(IDS_FILE_ERR_STRING,filterNameCStr));
		userMsg << saveEr;
		userMsg << fileEr;
		userMsg.showMsgBox(IDS_UE_040031);
		m_realPModel->setFilterName( NamedFilter::UNNAMED );
		return;
	}
	
	// This part is to delete old NamedFilter from database and create a new one
	if (  NF_READWRITE == namedFilterState ) {
		// TD12034: Should be prompted when overwriting an existing name.
		TA_Base_Bus::TransActiveMessage userMsg;
		userMsg << filterNameCStr;
		if ( IDNO == userMsg.showMsgBox(IDS_UW_010017) )
		{
			return;
		}

		try
		{
			// Load the filter.
			std::auto_ptr<NamedFilter> loadedFilter( TA_Base_Core::NamedFilterAccessFactory::getInstance().getNamedFilter( old_filterName.toMultiByteStdString() ) );
			loadedFilter->deleteNamedFilter();
		}
		catch(...)
		{
		}
	}
	

	//End-- TD14346
	try
	{
		m_realPModel->saveNamedFilter(filterNameCStr.toMultiByteStdString(),old_filterName.toMultiByteStdString()); 
	}
	catch ( TA_Base_Core::DataException& ex )
	{
		LOG_EXCEPTION_CATCH( SourceInfo, "DataException", ex.getWhichData() );

		// Display the save filter error message.
		 TA_Base_Bus::TransActiveMessage userMsg;
        userMsg << TALoadString(IDS_ERROR_SAVE_STR) << TALoadString(IDS_ERROR_SAVED_STR);
        UINT selectedButton = userMsg.showMsgBox(IDS_UE_040031);

		// Only an unnamed filter can be saved, so rename it back.
		m_realPModel->setFilterName( NamedFilter::UNNAMED );
		return;
	}
	catch (...)
	{
		LOG( SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError, "Failed to create and save filter");

		// Display the save filter error message.
		TA_Base_Bus::TransActiveMessage userMsg;
        userMsg << TALoadString(IDS_ERROR_SAVE_STR) << TALoadString(IDS_ERROR_SAVE_STR);
        UINT selectedButton = userMsg.showMsgBox(IDS_UE_040031);

		// Only an unnamed filter can be saved, so rename it back.
		m_realPModel->setFilterName( NamedFilter::UNNAMED );
		return;
	}

    // Refresh the state of the window controls.
    populateNamedFilters(true);
	
	GetFilterData();// TD14346
    m_okButton.EnableWindow(true);

	// TD #12583
	// On a save, make no change to the Apply button, and make
	// sure the Save button is disabled
    //m_applyButton.EnableWindow(true);
	m_saveFilterButton.EnableWindow(false);

    FUNCTION_EXIT;
}

void CombinedEventFilterDlg::OnDeleteFilter()
{
    FUNCTION_ENTRY("OnDeleteFilter");

    // Confirm the deletion.
    TA_Base_Bus::TransActiveMessage userMsg;
	TAUnicodeString errorStr = TALoadString(IDS_ERROR_DELETE_STR)+_T(" '")+TAUnicodeString::CreateFromMultitByteString( m_realPModel->getFilterName().c_str())+_T("'");
    userMsg << errorStr;

    if ( IDNO == userMsg.showMsgBox(IDS_UW_010003) )
    {
        return;
    }

	try
	{
		m_realPModel->deleteNamedFilter();
	}
	catch ( DataException& ex )
	{
		LOG_EXCEPTION_CATCH( SourceInfo, "DataException", ex.getWhichData() );

		// Display the load error message.
		 TA_Base_Bus::TransActiveMessage userMsg;
        userMsg << TALoadString(IDS_ERROR_DELETE_STR);
        userMsg << TALoadString(IDS_ERROR_DELETED_STR);
        userMsg.showMsgBox(IDS_UE_040031);
		return;
	}


    // TD12035: If filter is deleted it should revert to default.
    OnDefaultFilter();

    // Now update filter buttons.
	enableNamedFilterButtons( TAUnicodeString::CreateFromMultitByteString(m_realPModel->getFilterName().c_str()));

	populateNamedFilters(true);


    // The filter list has changed, so tell the main dialog to repopulate its control.
    //AfxGetMainWnd()->PostMessage( WM_USER_FILTER_DLG_NAMED_FILTER_CHANGE, 0, 0 ); //TD15032 azenitha
	LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "TD15032: No need to refresh main dialog");
    FUNCTION_EXIT;
}


void CombinedEventFilterDlg::OnBnClickedRadio1()
{
	OnTMTypeRadioBoxChanged( TAUnicodeString::CreateFromMultitByteString(TA_Base_App::TMType_1.c_str()));
}

void CombinedEventFilterDlg::OnBnClickedRadio2()
{
	OnTMTypeRadioBoxChanged( TAUnicodeString::CreateFromMultitByteString(TA_Base_App::TMType_2.c_str()));
}

void CombinedEventFilterDlg::OnBnClickedRadio3()
{
	OnTMTypeRadioBoxChanged( TAUnicodeString::CreateFromMultitByteString(TA_Base_App::TMType_3.c_str()));
}

void CombinedEventFilterDlg::OnBnClickedRadio4()
{
	OnTMTypeRadioBoxChanged( TAUnicodeString::CreateFromMultitByteString(TA_Base_App::TMType_4.c_str()));	
}

void CombinedEventFilterDlg::OnAlarmStateExt()
{
	FUNCTION_ENTRY("OnAlarmStateExt");
	// Called when the "..." next to the "Alarm Type" edit box is clicked.

	// Create a MultipleLoadDlg - this will hold the list providing the selections.
	MultipleLoadDlg loadDlg(TALoadString(IDS_ALARM_STATE_DIALOG));

	// Populate the list with the loaded locations.
	loadDlg.setItems( m_realPModel->m_alarmState, true );

	// Set the current selections.
	loadDlg.setSelection( m_realPModel->getNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_ALARMSTATE ) );

	// Display the dialog.
	if ( loadDlg.DoModal() == IDOK )
	{
		// Get the list of selected items.
		SelectedItems selection = loadDlg.getSelection();

		// If selection changed, update selection and repopulate edit control.
		m_realPModel->setNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_ALARMSTATE, selection );
		TAUnicodeString displayStr = TAUnicodeString::CreateFromMultitByteString(m_realPModel->populateListStringFromSelection( selection, m_realPModel->m_alarmState ).c_str());		
		m_alarmStateEdit.SetWindowText(displayStr.c_str()); 

		// Now enable the buttons.
		enableButtons();

		// Now that the filter is changed it is user defined.
		selectUserFilter();
	}

	FUNCTION_EXIT;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
///////////HELPER Functions

void CombinedEventFilterDlg::populateNamedFilters(bool reload)
{
   // TAUnicodeString str;

    // Make sure the combo box is empty.
    m_namedFilterCombo.ResetContent();

    try
    {
		NamedFilterAccessFactory::FilterNameList filters;

		EventViewerDlg * eventViewerDlg = dynamic_cast<EventViewerDlg*> (GetParent());
		if(reload == true)
		{
			eventViewerDlg->setNamedFilter(NamedFilterAccessFactory::getInstance().getAllFilterNames());			
		}
		filters = eventViewerDlg->getNamedFilter();		
		
        // Now add each to the combo box.
        NamedFilterAccessFactory::FilterNameList::iterator namedFilterIt;
        for ( namedFilterIt= filters.begin(); namedFilterIt!=filters.end(); namedFilterIt++ )
        {
			int index = m_namedFilterCombo.AddString( TAUnicodeString::CreateFromMultitByteString(namedFilterIt->first.c_str() ));
            if ( 0 <= index )
            {
                // If the data was inserted correctly store whether it is read only.
                m_namedFilterCombo.SetItemData( index, static_cast<int>(namedFilterIt->second) );

                // If it is the current filter then select it.
                if ( namedFilterIt->first == m_realPModel->getFilterName() )
                {
                    m_namedFilterCombo.SetCurSel( index );
                }
            }
        }
    }
    catch ( DataException& )
    {
        LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Could not get a list of named filters!");
    }

    // If the filter is custom it won't appear in the combo box, so add its text
    // manually.
    if ( NamedFilter::UNNAMED == m_realPModel->getFilterName() )
    {
        m_namedFilterCombo.AddTempString( NamedFilter::UNNAMED.c_str() );
    }

    // Now set the filter button states.
    enableNamedFilterButtons( TAUnicodeString::CreateFromMultitByteString(m_realPModel->getFilterName().c_str()));
}

ENamedFilterState CombinedEventFilterDlg::getNamedFilterState( const TAUnicodeString& namedFilter )
{
    ENamedFilterState state = NF_UNSAVED;
	TAUnicodeString unNamedFilter = TAUnicodeString::CreateFromMultitByteString(NamedFilter::UNNAMED.c_str());
    if ( namedFilter == unNamedFilter )
    {
        state = NF_USERDEFINED;
    }
    else
    {
        // Search the combo box for the specified string.
        int index = m_namedFilterCombo.FindStringExact( 0, namedFilter.c_str() );
        if ( CB_ERR != index )
        {
            if ( static_cast<bool>(m_namedFilterCombo.GetItemData( index )) )
            {
                state = NF_READWRITE;
            }
            else
            {
                state = NF_READONLY;
            }
        }
    }

    return state;
}

void CombinedEventFilterDlg::enableNamedFilterButtons( const TAUnicodeString& filterName )
{
    switch ( getNamedFilterState( filterName ) )
    {
    case NF_UNSAVED:
        m_saveFilterButton.EnableWindow( TRUE );
        m_deleteFilterButton.EnableWindow( FALSE );
        break;
    case NF_READWRITE:
		// TD #12583
		// Don't enable the Save button until a change has been made..
        m_saveFilterButton.EnableWindow( FALSE );
        m_deleteFilterButton.EnableWindow( TRUE );
        break;
    default:
        m_saveFilterButton.EnableWindow( FALSE );
        m_deleteFilterButton.EnableWindow( FALSE );
        break;
    };
}

void CombinedEventFilterDlg::enableButtons()
{
    // Enable buttons as required.
    m_okButton.EnableWindow(true);
    m_applyButton.EnableWindow(true);
    m_cancelButton.EnableWindow(true);
	m_saveFilterButton.EnableWindow(true);
}

void CombinedEventFilterDlg::selectUserFilter()
{
    // Get the currently selected name.
    TAUnicodeString filterName;
    m_namedFilterCombo.GetWindowText( filterName.str() );

    // Only select user defined filter if the current filter is read only or unnamed.
    ENamedFilterState state = getNamedFilterState( filterName );
    if ( ( NF_USERDEFINED == state ) || ( NF_READONLY == state ) )
    {
        // Whenever the operator modifies the filter criteria the filter changes to 'User Defined'.
        m_namedFilterCombo.AddTempString( NamedFilter::UNNAMED.c_str() );

        // 'User Defined' cannot be saved or deleted.
        m_saveFilterButton.EnableWindow( FALSE );
        m_deleteFilterButton.EnableWindow( FALSE );
    }
}

CTime CombinedEventFilterDlg::getCombinedTime(const CTime& date,const CTime& time)
{
    // Convert the date and time into a single time entity.
    return CTime( date.GetYear(), date.GetMonth(), date.GetDay(), time.GetHour(), time.GetMinute(), time.GetSecond() );
}

// update time picker in GUI(m_fromDate ,m_fromTIme , m_toDate, m_toTime) and reset m_namedfilter (Totime, FromTime)
void CombinedEventFilterDlg::updateTimes() 
{
    FUNCTION_ENTRY("updateTimes");

    CTime date;
    CTime time;

    // Get the current time.
    CTime currentTime = CTime::GetCurrentTime();

    // TD11082: 'to' time cannot be more than current time.
    m_toDate.SetRange( &EARLY_TIME_LIMIT, &currentTime );
    m_toTime.SetRange( &EARLY_TIME_LIMIT, &currentTime );

    // Get the to time - it should now be less than current time.
    m_toDate.GetTime(date);
    m_toTime.GetTime(time);
    CTime toTime = getCombinedTime( date, time );

	// Get the from time - it should now be less than 'to' time.
	m_fromDate.GetTime(date);
	m_fromTime.GetTime(time);
	CTime fromTime = getCombinedTime( date, time );

    //m_namedFilter.removeNumericFilter( Filter::FILTER_TOTIME );
	//m_namedFilter.removeNumericFilter( Filter::FILTER_FROMTIME );
	// TD 14914 not to get the current time but the 24hrs time set when the "from" check box is checked
	// ie. set the default current time, when both are unchecked
    if (UNCHECKED == m_toCheck.GetCheck())
    {
		if ( UNCHECKED == m_fromCheck.GetCheck())
		{
			// If its not checked make sure the 'to' time is the current time.
			toTime = currentTime;
			fromTime = CTime( toTime.GetTime() - TWENTY_FOUR_HOURS );

			m_fromDate.SetTime( &fromTime );
			m_fromTime.SetTime( &fromTime );
			m_toDate.SetTime( &toTime );
			m_toTime.SetTime( &toTime );

			m_realPModel->setFilterTime(static_cast<unsigned long>(Filter::CURRENT_TIME),static_cast<unsigned long>(Filter::PREVIOUS_DAY));

			//m_namedFilter.addNumericFilter( Filter::FILTER_TOTIME,  );
			//m_namedFilter.addNumericFilter( Filter::FILTER_FROMTIME,  );
		}
		else
		{
// 			toTime = CTime( fromTime.GetTime() + TWENTY_FOUR_HOURS);
// 			if( toTime.GetTime() > currentTime.GetTime())
// 			{
// 				toTime = currentTime;
// 			}
			toTime = currentTime;  // according to SWDS, when "from" selected but "To" not selected, should retrieve events from startTime to current time.

			//m_namedFilter.addNumericFilter( Filter::FILTER_TOTIME, static_cast<unsigned long>(toTime.GetTime()) );
			//m_namedFilter.addNumericFilter( Filter::FILTER_FROMTIME, static_cast<unsigned long>(fromTime.GetTime()) );
			m_realPModel->setFilterTime(static_cast<unsigned long>(toTime.GetTime()),static_cast<unsigned long>(fromTime.GetTime()));
			
			m_toDate.SetTime( &toTime );
			m_toTime.SetTime( &toTime );
			
		}

    }
    else   // CHECKED == m_toCheck.GetCheck()
    {   
		if(UNCHECKED == m_fromCheck.GetCheck())
		{
			
			fromTime = CTime( toTime.GetTime() - TWENTY_FOUR_HOURS );
			m_fromDate.SetTime( &fromTime );
			m_fromTime.SetTime( &fromTime );
		}

		//m_namedFilter.addNumericFilter( Filter::FILTER_FROMTIME, static_cast<unsigned long>(fromTime.GetTime()) );		
		//m_namedFilter.addNumericFilter( Filter::FILTER_TOTIME, static_cast<unsigned long>(toTime.GetTime()) );
		m_realPModel->setFilterTime(static_cast<unsigned long>(toTime.GetTime()),static_cast<unsigned long>(fromTime.GetTime()));

    }

    // 'from' time cannot be more than 'to' time.
	m_fromDate.SetRange(&EARLY_TIME_LIMIT,&toTime);
	m_fromTime.SetRange(&EARLY_TIME_LIMIT,&toTime);


    FUNCTION_EXIT;
}

TAUnicodeString CombinedEventFilterDlg::getOldFilterName (const TAUnicodeString& new_filterName )
{
	TAUnicodeString oldName= UNICODE_EMPTY_STR;
	int index = m_namedFilterCombo.FindStringExact( -1, new_filterName.c_str() );
	if(index>=0)
	{
		//TAUnicodeString szname;
		m_namedFilterCombo.GetLBText(index,oldName.str());
		//oldName = (LPCTSTR)szname;
	}
	return oldName;
}

void CombinedEventFilterDlg::OnTMTypeRadioBoxChanged(TAUnicodeString tmtype)
{
	if ( m_realPModel->isDisableEvents() )
	{
		return;
	}
	// Enable the buttons.
	enableButtons();

	// Now that the filter is changed it is user defined.
	selectUserFilter();

	m_realPModel->setFilterText(EVDisplayStrRes::GetInstance()->FILTER_TMTYPE,tmtype.toMultiByteStdString());

}

void CombinedEventFilterDlg::updateFilter()
{
    FUNCTION_ENTRY("updateFilter");

    // Load the new filter.
    m_realPModel->loadFilter( true );

    // Make sure the combo box matches.
    populateNamedFilters();

    FUNCTION_EXIT;
}

void CombinedEventFilterDlg::setUpSeverities(std::vector<std::pair<unsigned long,std::string> > dataItems)
{
    FUNCTION_ENTRY("setUpSeverities");

    CRect rect;
    GetDlgItem(IDC_LIST_PLACEHOLDER)->GetWindowRect(rect);
    ScreenToClient(rect);
    if(!m_severityList.CreateEx(WS_EX_CLIENTEDGE, _T("LISTBOX"), _T("Severity List"), 
        LBS_OWNERDRAWFIXED | LBS_HASSTRINGS | WS_VISIBLE | WS_CHILD | WS_VSCROLL | LBS_DISABLENOSCROLL,
        rect, this, IDC_SEVERITY_LIST))
    {
        LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Could not create Severity List Box");
        GetDlgItem(IDC_LIST_PLACEHOLDER)->ShowWindow(SW_HIDE);
        return;
    }

    GetDlgItem(IDC_LIST_PLACEHOLDER)->ShowWindow(SW_HIDE);
	
	// TD #12567
	// Hide the 'severity' list and the border frame around it
	// if NO_SEVERITY_FILTER has been defined
	// Now using RUN_PARAM instead
	if (TA_Base_Core::RunParams::getInstance().isSet( EventConstants::RPARAM_NOSEVERITY.c_str()))
	{
		    m_severityList.ShowWindow(SW_HIDE);
			GetDlgItem(IDC_STATIC_SEVERITY)->ShowWindow(SW_HIDE);
	}
	else
	{
		m_severityList.ShowWindow(SW_SHOW);
	}
	
    for (unsigned int i = 0; i < dataItems.size();i++)
    {
		m_severityList.InsertString(i, TAUnicodeString::CreateFromMultitByteString(dataItems[i].second.c_str()));
        m_severityList.SetItemData(i, dataItems[i].first);
    }

    // Insert "Event" into the list
    m_severityList.InsertString(dataItems.size(), TALoadString(IDS_EVENT_ONLY_STR));
    m_severityList.SetItemData(dataItems.size(), EVENT_ONLY_KEY);

    FUNCTION_EXIT;
}

void CombinedEventFilterDlg::offsetToTimeTakeRefFromTime(unsigned long hours)
{
	// Get the time from time and set it "Hours" hrs earlier but not later than current time
	// Get the current "to" time.
	CTime time;
	m_fromTime.GetTime( time );
	// Now adjust "from" to be "Hours" hours after "from".
	CTime toTime(time.GetTime() + hours);

	// Check current time and set to this if exceed the current time
	CTime curTime( CTime::GetCurrentTime() );
	if (toTime > curTime)
	{
		// Set the value of the "to" fields.
		m_toDate.SetRange(&EARLY_TIME_LIMIT,&curTime);
		m_toTime.SetRange(&EARLY_TIME_LIMIT,&curTime);

		// And set the box to have adjusted from+24hr time
		m_toDate.SetTime( &curTime );
		m_toTime.SetTime( &curTime );
	}
	else
	{
		// And set the box to have adjusted from+24hr time
		m_toDate.SetTime( &toTime );
		m_toTime.SetTime( &toTime );
	}
}

void CombinedEventFilterDlg::offsetFromTimeTakeRefToTime(unsigned long hours)
{
	// Get the current "to" time.
	CTime time;
	m_toTime.GetTime( time );

	// Now adjust "from" to be 24 hours before "to".
	CTime fromTime(time.GetTime() - hours);

	// And set the box to have the "current" time
	m_fromDate.SetTime( &fromTime );
	m_fromTime.SetTime( &fromTime );
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// PMODEL Notification handler
void CombinedEventFilterDlg::SetTimeControls(bool isToCheck, time_t toTimet,bool isFromCheck, time_t fromTimet)
{
	CTime toTime(toTimet);
	CTime fromTime(fromTimet);
	if(!isToCheck)
	{
		// The "to" time filter is disabled - that is, the filter is in "current" mode. Need to disable the to date and
		// time selection boxes, and check the "current" box
		m_toCheck.SetCheck(UNCHECKED);
		m_toDate.EnableWindow(false);
		m_toTime.EnableWindow(false);

		// And set the box to have the "current" time
		toTime = CTime::GetCurrentTime();
		m_toDate.SetTime(&toTime);
		m_toTime.SetTime(&toTime);

		// And set the valid time range
		m_fromTime.SetRange(&EARLY_TIME_LIMIT,&toTime);
		m_fromDate.SetRange(&EARLY_TIME_LIMIT,&toTime);
	}
	else
	{
		// The to time filter is enabled - so uncheck the "current" box, and set the date and time selection boxes accordingly.
		m_toCheck.SetCheck(CHECKED);
		m_toDate.SetTime(&toTime);
		m_toTime.SetTime(&toTime);

		// And set the valid time range
		m_fromTime.SetRange(&EARLY_TIME_LIMIT,&toTime);
		m_fromDate.SetRange(&EARLY_TIME_LIMIT,&toTime);

		// Also need to enable the date and time slection boxes in case they were previously disabled
		m_toDate.EnableWindow(true);
		m_toTime.EnableWindow(true);
	}
	if(!isFromCheck)
	{
		// The "from" time filter is disabled - that is, the filter is only retrieving
		// events from the last 24 hours.
		m_fromCheck.SetCheck(UNCHECKED);
		m_fromDate.EnableWindow(false);
		m_fromTime.EnableWindow(false);

		// Set the boxes to have values 24 hours before "to" time.
		fromTime = toTime.GetTime() - TWENTY_FOUR_HOURS;
		m_fromTime.SetTime(&fromTime);
		m_fromDate.SetTime(&fromTime);

		// Use the from time to set the lower bound for the to time
		m_toTime.SetRange(&fromTime,NULL);
		m_toDate.SetRange(&fromTime,NULL);
	}
	else
	{
		// The to time filter is enabled - so uncheck the "current" box, and set the date and time selection boxes accordingly.
		m_fromCheck.SetCheck(CHECKED);
		m_fromDate.SetTime(&fromTime);
		m_fromTime.SetTime(&fromTime);

		// And set the valid time range
		m_toTime.SetRange(&fromTime,NULL);
		m_toDate.SetRange(&fromTime,NULL);

		// Also need to enable the date and time slection boxes in case they were previously disabled
		m_fromDate.EnableWindow(true);
		m_fromTime.EnableWindow(true);
	}

	// Wenguang++
	// Fix for TD14987
	// It might because the SetRange() cause the SetTime() not working properly for m_toTime
	// Add this part to make sure the time display correctly when load default filter
	if (UNCHECKED == m_toCheck.GetCheck() && UNCHECKED == m_fromCheck.GetCheck() )
	{
		// If its not checked make sure the 'to' time is the current time.
		toTime = CTime::GetCurrentTime();
		m_toDate.SetTime( &toTime );
		m_toTime.SetTime( &toTime );

		m_fromDate.SetRange( &EARLY_TIME_LIMIT, &toTime );
		m_fromTime.SetRange( &EARLY_TIME_LIMIT, &toTime );
		fromTime = CTime( toTime.GetTime() - TWENTY_FOUR_HOURS );
		m_fromDate.SetTime( &fromTime );
		m_fromTime.SetTime( &fromTime );
	}

	// ++Wenguang
}

void CombinedEventFilterDlg::selectSeverities()
{
	// Severity
	std::vector<unsigned long> ids = m_realPModel->getNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_SEVERITY );
	// AnitaL : PW2970 : New filter requirements have severities as checkboxes
	// rather than combo box.
	for( int i = 0; i < m_severityList.GetCount(); ++i )
	{
			std::vector<unsigned long>::iterator it = std::find( ids.begin(), ids.end(), m_severityList.GetItemData(i) );
			if ( it != ids.end() )
			{
				m_severityList.SetCheck( i, 1 );
			}
			else
			{
				m_severityList.SetCheck( i, 0 );
			}
		}
}

//Begin-- TD14346
void CombinedEventFilterDlg::GetFilterData()
{
	m_OldFilterData.m_strSubsystem.Empty();
	m_OldFilterData.m_strLocation.Empty();
	m_OldFilterData.m_strOperator.Empty();
	m_OldFilterData.m_strSystem.Empty();
	m_OldFilterData.m_strAsset.Empty();
	m_OldFilterData.m_strDescription.Empty();
	m_OldFilterData.m_strAlarmType.Empty();
	m_OldFilterData.m_strAlarmID.Empty();
	m_OldFilterData.m_bFromTimeChecked = false;
	m_OldFilterData.m_bToTimeChecked = false;
	m_OldFilterData.m_TMType = TAUnicodeString::CreateFromMultitByteString(TA_Base_App::TMType_4.c_str());

	GetDlgItem(IDC_SUBSYSTEM)->GetWindowText(m_OldFilterData.m_strSubsystem.str());
	GetDlgItem(IDC_LOCATION)->GetWindowText(m_OldFilterData.m_strLocation.str());
	GetDlgItem(IDC_OPERATOR)->GetWindowText(m_OldFilterData.m_strOperator.str());
	GetDlgItem(IDC_SYSTEM)->GetWindowText(m_OldFilterData.m_strSystem.str());
	GetDlgItem(IDC_ASSET)->GetWindowText(m_OldFilterData.m_strAsset.str());
	GetDlgItem(IDC_DESCRIPTION)->GetWindowText(m_OldFilterData.m_strDescription.str());
	GetDlgItem(IDC_ALARM_TYPE)->GetWindowText(m_OldFilterData.m_strAlarmType.str());
	GetDlgItem(IDC_ALARM_ID)->GetWindowText(m_OldFilterData.m_strAlarmID.str());
	m_OldFilterData.m_bFromTimeChecked =  UNCHECKED == m_fromCheck.GetCheck()?false:true;
	m_OldFilterData.m_bToTimeChecked = UNCHECKED == m_toCheck.GetCheck()?false:true;
	if (true == m_OldFilterData.m_bFromTimeChecked) {
		m_fromDate.GetTime(m_OldFilterData.m_ctFromDate);
		m_fromTime.GetTime(m_OldFilterData.m_ctFromTime);
	}
	if (true == m_OldFilterData.m_bToTimeChecked) {
		m_toDate.GetTime(m_OldFilterData.m_ctToDate);
		m_toTime.GetTime(m_OldFilterData.m_ctToTime);
	}

	// set TM type to m_oldFilter:
	if( m_radio1.GetCheck() == TRUE )
	{
		m_OldFilterData.m_TMType = TAUnicodeString::CreateFromMultitByteString(TA_Base_App::TMType_1.c_str());
	}else if( m_radio2.GetCheck() == TRUE )
	{
		m_OldFilterData.m_TMType = TAUnicodeString::CreateFromMultitByteString(TA_Base_App::TMType_2.c_str());
	}else if( m_radio3.GetCheck() == TRUE )
	{
		m_OldFilterData.m_TMType = TAUnicodeString::CreateFromMultitByteString(TA_Base_App::TMType_3.c_str());
	}else if( m_radio4.GetCheck() == TRUE )
	{
		m_OldFilterData.m_TMType = TAUnicodeString::CreateFromMultitByteString(TA_Base_App::TMType_4.c_str());
	}

}
//End-- TD14346

//Begin--   TD14346
bool CombinedEventFilterDlg::IsFilterDataChanged()
{
	TAUnicodeString strTemp;
	GetDlgItem(IDC_SUBSYSTEM)->GetWindowText(strTemp.str());
	if(m_OldFilterData.m_strSubsystem != strTemp)
		return true;
	GetDlgItem(IDC_LOCATION)->GetWindowText(strTemp.str());
	if(m_OldFilterData.m_strLocation != strTemp)
		return true;
	GetDlgItem(IDC_OPERATOR)->GetWindowText(strTemp.str());
	if(m_OldFilterData.m_strOperator != strTemp)
		return true;
	GetDlgItem(IDC_SYSTEM)->GetWindowText(strTemp.str());
	if(m_OldFilterData.m_strSystem != strTemp)
		return true;
	GetDlgItem(IDC_ASSET)->GetWindowText(strTemp.str());
	if(m_OldFilterData.m_strAsset != strTemp)
		return true;
	GetDlgItem(IDC_DESCRIPTION)->GetWindowText(strTemp.str());
	if(m_OldFilterData.m_strDescription != strTemp)
		return true;
	GetDlgItem(IDC_ALARM_TYPE)->GetWindowText(strTemp.str());
	if(m_OldFilterData.m_strAlarmType != strTemp)
		return true;
	GetDlgItem(IDC_ALARM_ID)->GetWindowText(strTemp.str());
	if(m_OldFilterData.m_strAlarmID != strTemp)
		return true;
	bool bTemp;
	
	bTemp =  UNCHECKED == m_fromCheck.GetCheck()?false:true;
	if (m_OldFilterData.m_bFromTimeChecked != bTemp) {
		return true;
	}
	else
	{
		CTime tmDate,tmTime;
		m_fromDate.GetTime(tmDate);
		m_fromTime.GetTime(tmTime);
		if (m_OldFilterData.m_ctFromDate != tmDate || m_OldFilterData.m_ctFromTime != tmTime) {
			return true;
		}
	}
	bTemp = UNCHECKED == m_toCheck.GetCheck()?false:true;
	if (m_OldFilterData.m_bToTimeChecked != bTemp) {
		return true;
	}
	else
	{
		CTime tmDate,tmTime;
		m_toDate.GetTime(tmDate);
		m_toTime.GetTime(tmTime);
		if (m_OldFilterData.m_ctToDate != tmDate || m_OldFilterData.m_ctToTime != tmTime) {
			return true;
		}
	}

	// add for TM type comparing:
	if( m_radio1.GetCheck() == TRUE &&   TAUnicodeString::CreateFromMultitByteString(TA_Base_App::TMType_1.c_str()) != m_OldFilterData.m_TMType )
	{
		return true;
	}else if( m_radio2.GetCheck() == TRUE &&   TAUnicodeString::CreateFromMultitByteString(TA_Base_App::TMType_2.c_str()) !=  m_OldFilterData.m_TMType)
	{
		return true;
	}else if( m_radio3.GetCheck() == TRUE &&  TAUnicodeString::CreateFromMultitByteString(TA_Base_App::TMType_3.c_str()) !=  m_OldFilterData.m_TMType )
	{
		return true;
	}else if( m_radio4.GetCheck() == TRUE &&   TAUnicodeString::CreateFromMultitByteString(TA_Base_App::TMType_4.c_str()) !=  m_OldFilterData.m_TMType)
	{
		return true;
	}

	return false;
}
//End-- TD14346

void CombinedEventFilterDlg::populateTMTypeFromFilter (string tmType)
{
	//clear the check first:
	m_radio1.SetCheck(0);
	m_radio2.SetCheck(0);
	m_radio3.SetCheck(0);
	m_radio4.SetCheck(0);

	if(tmType.size() > 0)
	{		
		if(tmType == "O"){
			m_radio1.SetCheck(1);
		}else if(tmType == "M"){
			m_radio2.SetCheck(1);
		}else if(tmType == "B"){
			m_radio3.SetCheck(1);
		}
		else 
		{
			m_radio4.SetCheck(1);
		}
	}
	else
	{
		m_radio4.SetCheck(1);
	}
}

void CombinedEventFilterDlg::resetStartEndTimeDefined(FilterCriteria & filterCriteria)
{

	filterCriteria.StartTimePeriodDefined = false ;
	filterCriteria.EndTimePeriodDefined = false ;
	// 	if( CHECKED == m_fromCheck.GetCheck() )
	// 	{
	// 		filterCriteria.StartTimePeriodDefined = true ;
	// 	}
	// 	if( CHECKED == m_toCheck.GetCheck())
	// 	{
	// 		filterCriteria.EndTimePeriodDefined = true ;
	// 	}
	if( CHECKED == m_fromCheck.GetCheck() || CHECKED == m_toCheck.GetCheck() )
	{
		filterCriteria.StartTimePeriodDefined = true ;
		filterCriteria.EndTimePeriodDefined = true ;
	}
}


//////////////////////////////////////////////////////////////////////////////////////////
/// AbstractTransActiveView member functions

std::string CombinedEventFilterDlg::getMyPModelName()
{
	return COMBINED_EVENT_FILTER_PMODEL;
}

void CombinedEventFilterDlg::setupPModel()
{
	m_realPModel = dynamic_cast<CombinedEventFilterPModel*> (m_pModel);
	TA_ASSERT(m_realPModel != NULL, "PModel has been initialized successfully");
	m_realPModel->attachView(this, INVOKE_POSTMESSGAE_NOTIFICATION.c_str());
	m_realPModel->attachView(this, CLEAR_SORT_MASK.c_str());
	m_realPModel->attachView(this, SET_TIME_CONTROLS.c_str());
	m_realPModel->attachView(this, SET_WINDOW_TEXT.c_str());
	m_realPModel->attachView(this, SET_SEVERITY_COMBOX_BOX.c_str());
	m_realPModel->attachView(this, GET_FILTER_DATA.c_str());
	m_realPModel->attachView(this, POPULATE_TMTYPE.c_str());
	m_realPModel->attachView(this, RESET_FILTER_TIME.c_str());	
	m_realPModel->attachView(this, SHOW_USER_MESSAGE_NOTIFY.c_str());	
	
}

void CombinedEventFilterDlg::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
{
	std::string type(updateType);
	if(INVOKE_POSTMESSGAE_NOTIFICATION == type)
	{
		TA_Base_App::GenericEventArgs * gargs = dynamic_cast<TA_Base_App::GenericEventArgs *>(args);
		unsigned int messageId = *((unsigned int*) gargs->Param1);
		//delete gargs;
		//gargs = NULL;
		AfxGetMainWnd()->PostMessage(messageId);
	}
	else if(CLEAR_SORT_MASK == type)
	{
		EventViewerDlg * dlg = dynamic_cast<EventViewerDlg *>(GetParent());
		// This will only delete the sorting icon in event viewer dialog
		dlg->clearSortMark();  
	}
	else if(SET_TIME_CONTROLS == type)
	{
		TA_Base_App::GenericEventArgs * gargs = dynamic_cast<TA_Base_App::GenericEventArgs *>(args);
		bool toCheck = *((bool *) gargs->Param1);
		time_t totime = *((time_t *)gargs->Param2);
		bool fromCheck = *((bool *) gargs->Param3);
		time_t fromtime =*((time_t *)gargs->Param4);
		//delete gargs;
		//gargs = NULL;
		SetTimeControls(toCheck,totime,fromCheck,fromtime);
	}
	else if(SET_WINDOW_TEXT == type)
	{
		TA_Base_App::GenericEventArgs * gargs = dynamic_cast<TA_Base_App::GenericEventArgs *>(args);
		std::string *str =  static_cast<std::string*>(gargs->Param1);
		std::string editControl = *str;
		std::string *str1 = static_cast<std::string*>(gargs->Param2);
		std::string displayStr = *str1;
		//delete gargs;
		//gargs = NULL;
		if(editControl == SYSTEM_EDIT)
		{
			m_systemEdit.SetWindowText(TAUnicodeString::CreateFromMultitByteString(displayStr.c_str()));
		}
		else if(SUB_SYSTEM_EDIT == editControl)
		{
			m_subsystemEdit.SetWindowText(TAUnicodeString::CreateFromMultitByteString(displayStr.c_str()));
		}
		else if(EVDisplayStrRes::GetInstance()->FILTER_LOCATION == editControl)
		{
			m_locationEdit.SetWindowText(TAUnicodeString::CreateFromMultitByteString(displayStr.c_str()));
		}
		else if(EVDisplayStrRes::GetInstance()->FILTER_OPERATOR == editControl)
		{
			m_operatorEdit.SetWindowText(TAUnicodeString::CreateFromMultitByteString(displayStr.c_str()));
		}
		else if(EVDisplayStrRes::GetInstance()->FILTER_ALARMSTATE == editControl)
		{
			m_alarmStateEdit.SetWindowText(TAUnicodeString::CreateFromMultitByteString(displayStr.c_str()));
		}
		else if(EVDisplayStrRes::GetInstance()->FILTER_ASSET == editControl)
		{
			 m_assetEdit.SetWindowText(TAUnicodeString::CreateFromMultitByteString(displayStr.c_str()));
		}
		else if(EVDisplayStrRes::GetInstance()->FILTER_DESCRIPTION == editControl)
		{
			m_descriptionEdit.SetWindowText(TAUnicodeString::CreateFromMultitByteString(displayStr.c_str()));
		}
		else if(EVDisplayStrRes::GetInstance()->FILTER_ALARMTYPE == editControl)
		{
			m_alarmtypeEdit.SetWindowText(TAUnicodeString::CreateFromMultitByteString(displayStr.c_str()));
		}
		else if(EVDisplayStrRes::GetInstance()->FILTER_ALARMID == editControl)
		{
			m_alarmIdEdit.SetWindowText(TAUnicodeString::CreateFromMultitByteString(displayStr.c_str()));
		}
	}
	else if(type == SET_SEVERITY_COMBOX_BOX)
	{
		selectSeverities();
	}
	else if(type == GET_FILTER_DATA)
	{
		GetFilterData();
	}
	else if(type == POPULATE_TMTYPE)
	{
		TA_Base_App::GenericEventArgs * gargs = dynamic_cast<TA_Base_App::GenericEventArgs *>(args);
		std::string *str =  static_cast<std::string*>(gargs->Param1);
		std::string tmtype = *str;
		//delete gargs;
		//gargs = NULL;
		populateTMTypeFromFilter(tmtype);
	}
	else if(type == RESET_FILTER_TIME)
	{
		TA_Base_App::GenericEventArgs * gargs = dynamic_cast<TA_Base_App::GenericEventArgs *>(args);
		TA_Base_App::FilterCriteria* filterCriteria = static_cast<TA_Base_App::FilterCriteria*>(gargs->Param1);
		//DONOT delete args as filtercriteria is used by app;
		resetStartEndTimeDefined(*filterCriteria);
	}	
	else if(SHOW_USER_MESSAGE_NOTIFY == type)
	{
		TA_Base_App::GenericEventArgs * gargs = dynamic_cast<TA_Base_App::GenericEventArgs *>(args);
		int message = *((int *) (gargs->Param1));
		std::string *str1 = static_cast<std::string*>(gargs->Param2);
		std::string param1 = *str1;
		std::string *str2 = static_cast<std::string*>(gargs->Param3);
		std::string param2 = *str2;
		//delete gargs;
		//gargs = NULL;
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
}



	