/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/event/event_viewer/src/CombinedEventFilterDlg.h $
 * @author:  Bradley Cleaver
 * @version: $Revision: #1 $
 *
 * Last modification: $DateTime: 2015/10/09 15:42:35 $
 * Last modified by:  $Author: CM $
 * 
 * CombinedEventFilterDlg is an MFC class deriving from the FilterDlg header (which is in turn a CDialog derived class).
 * This dialog provides filtering capabilities on the data items displayed in the combined event view.
 */

#if !defined(_COMBINED_EVENT_FILTER_DLG_H_)
#define _COMBINED_EVENT_FILTER_DLG_H_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// CombinedEventFilterDlg.h : header file
//
#include "stdafx.h"

#include "app/event/event_viewer/src/FilterDlg.h"
#include "app/event/event_viewer/src/MultipleLoadDlg.h"
#include "app/event/event_viewer/src/resource.h"
#include "app/event/event_viewer/src/EventViewerGUIConstants.h"

#include  "app/event/event_viewer/pmodel/src/CombinedEventFilterPModel.h"

#include "bus/mfc_extensions/src/combo_temporary_item/ComboTempItem.h"


/////////////////////////////////////////////////////////////////////////////
// CombinedEventFilterDlg dialog

//class CombinedEventFilter; // Forward declaration
struct FilterData 
{
	TAUnicodeString m_strSubsystem;
	TAUnicodeString m_strLocation;
	TAUnicodeString m_strOperator;
	TAUnicodeString m_strSystem;
	TAUnicodeString m_strAsset;
	TAUnicodeString m_strDescription;
	TAUnicodeString m_strAlarmType;
	TAUnicodeString m_strAlarmID;
	bool	m_bFromTimeChecked;
	bool	m_bToTimeChecked;
	CTime	m_ctFromDate;
	CTime	m_ctFromTime;
	CTime	m_ctToDate;
	CTime	m_ctToTime;
	TAUnicodeString    m_TMType;   //added
};


class CombinedEventFilterDlg : public FilterDlg
{
// Construction
public:
	CombinedEventFilterDlg(/*CombinedEventFilter* filter,*/CWnd* pParent = NULL);   // standard constructor
	virtual ~CombinedEventFilterDlg();

// Dialog Data
	//{{AFX_DATA(CombinedEventFilterDlg)
	enum { IDD = IDD_COMBINED_EVENT_FILTER };
	CEdit	m_alarmtypeEdit;
	CEdit	m_systemEdit;
	CEdit   m_alarmStateEdit;  //for "Alarm State"
	CEdit	m_operatorEdit;
	CButton	m_fromCheck;
	CCheckListBox	m_severityList;
	CButton	m_okButton;
	CButton	m_cancelButton;
	CButton	m_defaultButton;
	CButton	m_applyButton;
	CEdit	m_alarmIdEdit;
	CEdit	m_descriptionEdit;
	CEdit	m_assetEdit;
	CEdit	m_locationEdit;
	CEdit	m_subsystemEdit;
	CDateTimeCtrl	m_toTime;
	CDateTimeCtrl	m_toDate;
	CButton	m_toCheck;
	CDateTimeCtrl	m_fromTime;
	CDateTimeCtrl	m_fromDate;
	TA_Base_Bus::ComboTempItem	m_namedFilterCombo;
    CButton m_saveFilterButton;
    CButton m_deleteFilterButton;

	CButton m_radio1;
	CButton m_radio2;
	CButton m_radio3;
	CButton m_radio4;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV support


// Implementation
protected:

	// Generated message map functions
	//{{AFX_MSG(CombinedEventFilterDlg)
	afx_msg void OnClose();
	virtual void OnOK();
	virtual void OnCancel();
	virtual BOOL OnInitDialog();
	afx_msg void OnTo();
	afx_msg void OnSubsystemExt();
	afx_msg void OnLocationExt();
	afx_msg void OnApply();
	afx_msg void OnDefaultFilter();
	afx_msg void OnChangeAsset();
	afx_msg void OnChangeDescription();
	afx_msg void OnDatetimechangeToDate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDatetimechangeToTime(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDatetimechangeFromDate(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnDatetimechangeFromTime(NMHDR* pNMHDR, LRESULT* pResult);
	afx_msg void OnKillfocusSeverityList();
	afx_msg void OnChkChange();
	afx_msg void OnFrom();
	afx_msg void OnSystemExt();
	afx_msg void OnAlarmtypeExt();
	afx_msg void OnOperatorExt();
    afx_msg void OnSaveFilter();
    afx_msg void OnDeleteFilter();
	afx_msg void OnSelchangeComboNamedFilter();
	afx_msg void OnTextchangeNamedFilter();
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();
	afx_msg void OnBnClickedRadio3();
	afx_msg void OnBnClickedRadio4();
	afx_msg void OnAlarmStateExt();

	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:

	/**
     * updateFilter
     *
     * Notifies the filter dialog that the filter object has been changed. When this method is called, 
     * the filter object is re-read and the filter dialog updated to reflect its contents.
     */
    void updateFilter();

	virtual std::string getMyPModelName();
	virtual void setupPModel();
	virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args);

private:

	
    /**
     * populateNamedFilters
     *
     * Populates the named filter combo box with the current list of named filters.
     */
    void populateNamedFilters(bool reload=false);
	
	/**
     * selectUserFilter
     *
     * Selects the 'User Defined' filter.
     */
    void selectUserFilter();

	/**
     * setUpSeverities
     *
     * Sets up the severity list control.
     *
     * @param dataItems The severties to add to the list control.
     */
    void setUpSeverities(std::vector<std::pair<unsigned long,std::string> > dataItems);

	 /**
     * updateTimes
     *
     * Updates the times in filter buffer.
     */
    void updateTimes();

    /**
     * enableButtons
     *
     * Enables all buttons that can be greyed out. This is used when an object in the filter manipulated.
     */
    void enableButtons();

    /**
     * getCombinedTime
     *
     * Calculates and returns the "combined" time from the date and time components passed in.
     */
    CTime getCombinedTime(const CTime& date,const CTime& time);

	  /**
     * getNamedFilterState
     *
     * Returns the state of the specified named filter.
     *
     * @param namedFilter The filter to check.
     *
     * @return NamedFilterState The state of the specified named filter.
     */
    ENamedFilterState getNamedFilterState( const TAUnicodeString& namedFilter );

    /**
     * enableNamedFilterButtons
     *
     * Enables the 'Save' and 'Delete' buttons associated with the specified named filter as required.
     *
     * @param filterName The name of the filter to set the buttons for.
     */
    void enableNamedFilterButtons( const TAUnicodeString& filterName );

	void offsetToTimeTakeRefFromTime(unsigned long hours);
	void offsetFromTimeTakeRefToTime(unsigned long hours);

	void OnTMTypeRadioBoxChanged(TAUnicodeString tmtype);
	TAUnicodeString getOldFilterName (const TAUnicodeString& new_filterName );


	void SetTimeControls(bool isToCheck, time_t toTimet,bool isFromCheck, time_t fromTimet);
	void selectSeverities();
	void GetFilterData();
	bool IsFilterDataChanged();
	void populateTMTypeFromFilter (string tmType);
	void resetStartEndTimeDefined(FilterCriteria & filterCriteria);



	// Variable definitions to remove the use of literals
    static const int UNCHECKED; // Value returned by MFC when a checkbox is not checked
    static const int CHECKED;   // When it is checked  
    static const int COMBO_LIMIT;
 
	CombinedEventFilterPModel *m_realPModel;

	FilterData m_OldFilterData;
};

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_COMBINED_EVENT_FILTER_DLG_H_)
