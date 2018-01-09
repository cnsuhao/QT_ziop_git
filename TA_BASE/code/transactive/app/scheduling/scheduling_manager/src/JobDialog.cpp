/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/scheduling/scheduling_manager/src/JobDialog.cpp $
  * @author:  San Teo
  * @version: $Revision: #1 $
  *
  * Last modification: $DateTime: 2015/10/09 15:42:35 $
  * Last modified by:  $Author: CM $
  * 
  * Dialog that will be used to display details of a job.
  */

#include "app/scheduling/scheduling_manager/src/stdafx.h" 
#include "app/scheduling/scheduling_manager/src/SchedulingManager.h"
#include "app/scheduling/scheduling_manager/src/JobDialog.h"
#include "app/scheduling/scheduling_manager/src/SchedulingManagerView.h"
#include "app/scheduling/scheduling_manager/src/IJobDetailsPlugin.h"
#include "app/scheduling/scheduling_manager/plugin/model/src/SchedulingModelConstants.h"
#include "app/scheduling/scheduling_manager/plugin/model/src/JobDialogModel.h"
#include <algorithm>
#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/generic_gui_view/src/TransActiveConsts.h"
#include "bus/generic_gui_view/src/MFCControlHelper.h"
#include "bus/resources/resource.h"
#include "bus/generic_gui_view/src/HelpLauncher.h"
#include "bus/unicode_common_lib/src/TAUnicodeString.h"
#include "core/utilities/src/TAAssert.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/data_access_interface/src/IProfile.h"
#include "core/data_access_interface/src/ILocation.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/utilities/src/RunParams.h"
#include "core/utilities/src/DateFormat.h" // TD12474 
#include "core/exceptions/src/SchedulingManagerException.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

using namespace TA_Base_App::SchedulingManager;
using TA_Base_Bus::TransActiveMessage;
using TA_Base_Core::IProfile;
using TA_Base_Core::ILocation;
using TA_Base_Core::DebugUtil;

/////////////////////////////////////////////////////////////////////////////
// JobDialog dialog

#define checkRadio(itemId, check)       checkButton(FooButton::radio, itemId, check);
#define checkBox(itemId, check)         checkButton(FooButton::checkBox, itemId, check);
#define initialiseRadio(button, group)      \
    button.setType(FooButton::radio); \
    button.addToGroup(group);
// end #define initialiseRadio

namespace TA_Base_App
{
    const CString JobDialog::RECURRENCE_PATTERN_GROUP = "RecurrencePatternGroup";
    const CString JobDialog::RANGE_OF_RECURRENCE_GROUP = "RangeOfRecurrenceGroup";
    const int JobDialog::FIRST_COMMON_DAY_OF_MONTH = 1;
    const int JobDialog::LAST_COMMON_DAY_OF_MONTH = 31; //TD 12033
    const int JobDialog::MIN_RECURRENCE = 1;


    JobDialog::JobDialog(const unsigned long titleId, 
                         std::vector<std::string>& jobTypes, 
                         JobManager* jobManager, 
                         const bool readOnly, 
                         const bool hasAssociatedPlugin,
						 TA_Base_Bus::ITransActiveWinApp * pApp,
                         CWnd* pParent /*=NULL*/)
						 : TA_Base_Bus::AbstractTransActiveFormView(pApp),
						 CMultiDialog(JobDialog::IDD, pParent),
						   
        m_jobTypes(jobTypes),
        m_jobManager(jobManager),
        m_componentLibraryAccessor(jobTypes),
        m_readOnly(readOnly),
        m_hasAssociatedPlugin(hasAssociatedPlugin),
		//m_rightsLibrary(NULL),
		//m_planManagerKey(0),
		m_onceOffExecuteNow(true),
		m_onceOffFirstExecution(true)
    {
	    FUNCTION_ENTRY( "Constructor" );
        TA_ASSERT(jobTypes.size() > 0, "The Job Dialog must know the name of at least one job type");

		m_title = TALoadString(titleId);
        TA_VERIFY(!m_title.IsEmpty(), "String does not exist in string table");
	    //{{AFX_DATA_INIT(JobDialog)
	    //}}AFX_DATA_INIT

		m_resizeViewHelper = std::auto_ptr<TA_Base_Bus::ResizedViewHelper> ( new TA_Base_Bus::ResizedViewHelper(pApp, this) );
        //m_locationMap = m_doc.getAllLocations();
		m_locationMap = m_jobManager->getAllLocations();

	    FUNCTION_EXIT;
    }


    JobDialog::~JobDialog()
    {
	    FUNCTION_ENTRY( "Destructor" );

  //      if( m_rightsLibrary != NULL )
		//{
		//	delete m_rightsLibrary;
		//	m_rightsLibrary = NULL;
		//}

	    FUNCTION_EXIT;
    }


    BOOL JobDialog::DestroyWindow() 
    {
	    FUNCTION_ENTRY( "DestroyWindow" );
        // Remove the buttons from the group to free up memory.  Don't call FooButton::reset() here
        // as it will remove the buttons from other groups outside this class
	    m_onceoffRadio.removeFromGroup();
        m_dailyRadio.removeFromGroup();
        m_weeklyRadio.removeFromGroup();
        m_monthlyRadio.removeFromGroup();
        m_yearlyRadio.removeFromGroup();

        m_noEndDateRadio.removeFromGroup();
        m_endAfterDateRadio.removeFromGroup();
        m_endByDateRadio.removeFromGroup();
	    
	    FUNCTION_EXIT;
	    return CDialog::DestroyWindow();
    }


    void JobDialog::DoDataExchange(CDataExchange* pDX)
    {
	    CDialog::DoDataExchange(pDX);
	    //{{AFX_DATA_MAP(JobDialog)
	DDX_Control(pDX, IDC_JD_EDIT_JOBNAME, m_nameCtrl);
	    DDX_Control(pDX, IDC_JD_COMBO_LOCATION, m_locationCtrl);
	    DDX_Control(pDX, IDC_JD_COMBO_PROFILE, m_profileCtrl);
	    DDX_Control(pDX, IDCANCEL, m_closeCtrl);
	    DDX_Control(pDX, IDC_SAVE_NEW, m_saveNewCtrl);
	    DDX_Control(pDX, IDOK, m_saveCtrl);
	    DDX_Control(pDX, IDC_JD_BUTTON_SELECTJOB, m_selectJobButton);
	    DDX_Control(pDX, IDC_JD_BUTTON_JOBDETAILS, m_jobDetailsButton);
	    DDX_Control(pDX, IDC_JD_COMBO_JOBTYPE, m_jobTypeCtrl);
	    DDX_Control(pDX, IDC_JD_EDIT_JOBDESCRIPTION, m_descriptionCtrl);
	    DDX_Control(pDX, IDC_JD_RADIO_ONCEOFF, m_onceoffRadio);
	    DDX_Control(pDX, IDC_JD_RADIO_DAILY, m_dailyRadio);
	    DDX_Control(pDX, IDC_JD_RADIO_WEEKLY, m_weeklyRadio);
	    DDX_Control(pDX, IDC_JD_RADIO_MONTHLY, m_monthlyRadio);
	    DDX_Control(pDX, IDC_JD_RADIO_YEARLY, m_yearlyRadio);
	    DDX_Control(pDX, IDC_JD_RADIO_NO_END_DATE, m_noEndDateRadio);
	    DDX_Control(pDX, IDC_JD_RADIO_END_AFTER, m_endAfterDateRadio);
	    DDX_Control(pDX, IDC_JD_RADIO_END_BY, m_endByDateRadio);
	    DDX_Control(pDX, IDC_RANGE_PANEL_TEXT, m_rangePanelText);
	    DDX_Control(pDX, IDC_STATIC_RANGE_PANEL, m_rangePanel);
	    DDX_Control(pDX, IDC_JD_EDIT_END_AFTER, m_endAfterCtrl);
	    DDX_Control(pDX, IDC_JD_DATETIMEPICKER_END_BY, m_endByCtrl);
	    DDX_Control(pDX, IDC_JD_SPINNER_END_AFTER, m_endAfterSpinnerCtrl);
	    DDX_Control(pDX, IDC_JD_EDIT_SCHEDULE_SUMMARY, m_summaryCtrl);
	//}}AFX_DATA_MAP
    }


    BEGIN_MESSAGE_MAP(JobDialog, CDialog)
	    //{{AFX_MSG_MAP(JobDialog)
		ON_REGISTERED_MESSAGE(WM_SET_WINDOW_POSITION, OnSetWindowPosition)

	    ON_BN_CLICKED(IDC_JD_RADIO_ONCEOFF, onRadioNoRecurrence)
	    ON_BN_CLICKED(IDC_JD_RADIO_DAILY, onRadioDailyRecurrence)
	    ON_BN_CLICKED(IDC_JD_RADIO_WEEKLY, onRadioWeeklyRecurrence)
	    ON_BN_CLICKED(IDC_JD_RADIO_MONTHLY, onRadioMonthlyRecurrence)
	    ON_BN_CLICKED(IDC_JD_RADIO_YEARLY, onRadioYearlyRecurrence)
	    ON_BN_CLICKED(IDC_JD_RADIO_NO_END_DATE, onNoEndDate)
	    ON_BN_CLICKED(IDC_JD_RADIO_END_AFTER, onEndAfter)
	    ON_BN_CLICKED(IDC_JD_RADIO_END_BY, onEndBy)
        ON_NOTIFY(DTN_DATETIMECHANGE, IDC_ONCEOFF_DATETIMEPICKER_DATE, onChangeOnceoff)
        ON_NOTIFY(DTN_DATETIMECHANGE, IDC_DAILY_DATETIMEPICKER_TIME, onChangeDaily)

	    ON_BN_CLICKED(IDC_YEARLY_RADIO_ADAY, onChangeYearlySpecificDay)
	    ON_BN_CLICKED(IDC_YEARLY_RADIO_LASTDAY, onChangeYearlyLastDay)
	    ON_BN_CLICKED(IDC_SAVE_NEW, onSaveNew)
	    ON_BN_CLICKED(IDC_JD_BUTTON_SELECTJOB, onSelectJob)
	    ON_BN_CLICKED(IDC_JD_BUTTON_JOBDETAILS, onJobDetails)
	    ON_CBN_SELCHANGE(IDC_JD_COMBO_JOBTYPE, onSelchangeJobtype)
	    ON_EN_CHANGE(IDC_JD_EDIT_JOBDESCRIPTION, onChangeJobdescription)
	    ON_EN_UPDATE(IDC_JD_EDIT_END_AFTER, onEndAfter)
        ON_NOTIFY(DTN_DATETIMECHANGE, IDC_JD_DATETIMEPICKER_END_BY, onEndBy)
        ON_NOTIFY(DTN_DATETIMECHANGE, IDC_ONCEOFF_DATETIMEPICKER_TIME, onChangeOnceoff)
		ON_BN_CLICKED(IDC_WEEKLY_CHECK_MONDAY, onChangeWeekly)
	    ON_BN_CLICKED(IDC_WEEKLY_CHECK_TUESDAY, onChangeWeekly)
	    ON_BN_CLICKED(IDC_WEEKLY_CHECK_WEDNESDAY, onChangeWeekly)
	    ON_BN_CLICKED(IDC_WEEKLY_CHECK_THURSDAY, onChangeWeekly)
	    ON_BN_CLICKED(IDC_WEEKLY_CHECK_FRIDAY, onChangeWeekly)
	    ON_BN_CLICKED(IDC_WEEKLY_CHECK_SATURDAY, onChangeWeekly)
	    ON_BN_CLICKED(IDC_WEEKLY_CHECK_SUNDAY, onChangeWeekly)
        ON_NOTIFY(DTN_DATETIMECHANGE, IDC_WEEKLY_DATETIMEPICKER_TIME, onChangeWeekly)
	    ON_BN_CLICKED(IDC_MONTHLY_RADIO_ADAY, onChangeMonthlySpecificDay)
	    ON_EN_UPDATE(IDC_MONTHLY_EDIT_ADAY_DAY, onChangeMonthlySpecificDay)
        ON_NOTIFY(DTN_DATETIMECHANGE, IDC_MONTHLY_DATETIMEPICKER_ADAY_TIME, onChangeMonthlySpecificDay)
	    ON_BN_CLICKED(IDC_MONTHLY_RADIO_LASTDAY, onChangeMonthlyLastDay)
        ON_NOTIFY(DTN_DATETIMECHANGE, IDC_MONTHLY_DATETIMEPICKER_LASTDAY_TIME, onChangeMonthlyLastDay)
	    ON_BN_CLICKED(IDC_YEARLY_RADIO_ADAY, onChangeYearlySpecificDay)
        ON_NOTIFY(DTN_DATETIMECHANGE, IDC_YEARLY_DATETIMEPICKER_ADAY_TIME, onChangeYearlySpecificDay)
        ON_NOTIFY(DTN_DATETIMECHANGE, IDC_YEARLY_DATETIMEPICKER_ADAY_DATE, onChangeYearlySpecificDay)
	    ON_BN_CLICKED(IDC_YEARLY_RADIO_LASTDAY, onChangeYearlyLastDay)
        ON_NOTIFY(DTN_DATETIMECHANGE, IDC_YEARLY_DATETIMEPICKER_LASTDAY_TIME, onChangeYearlyLastDay)
	    ON_CBN_SELCHANGE(IDC_JD_COMBO_PROFILE, onSelchangeProfile)
		ON_MESSAGE(WM_TASETLANG, &JobDialog::OnSetLanguage)
	//}}AFX_MSG_MAP
    END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // JobDialog message handlers

    BOOL JobDialog::OnInitDialog() 
    {
	    FUNCTION_ENTRY( "OnInitDialog" );
	    //CDialog::OnInitDialog();
		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to create presentation model");

		attachPModel();

		TA_ASSERT(m_pModel != NULL, "Presentation Model should be initialized sucessfully");

		CDialog::OnInitDialog();

		UINT idd = (LONG)(LONG_PTR)this->m_lpszTemplateName;
		TATranslateDialog( idd, this );

		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to init UI Control");

		initUIControl();

		//behavior similar to onInitGenericGUICompleted
		{
			LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to update view");

			CWaitCursor cur;
			LockWindowUpdate();
			updateView();
			UnlockWindowUpdate();
			//UpdateWindow();
		}

		LOG0(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "begin to sync data to presentation model");

		syncUIDataToPModel();

	    FUNCTION_EXIT;
	    return TRUE;  // return TRUE unless you set the focus to a control
	                  // EXCEPTION: OCX Property Pages should return FALSE
    }

    //////////////////////////////////////////////////////////////////////////////////
    //
    //
    // Handlers for the Recurrence Pattern Panel
    //
    //
    //////////////////////////////////////////////////////////////////////////////////


    void JobDialog::onRadioNoRecurrence()
    {
	    FUNCTION_ENTRY( "onRadioNoRecurrence" );
        initialiseRadioNoRecurrence();
	    FUNCTION_EXIT;
    }


    void JobDialog::onRadioDailyRecurrence()
    {
	    FUNCTION_ENTRY( "onRadioDailyRecurrence" );
        initialiseRadioDailyRecurrence();
	    FUNCTION_EXIT;
    }


    void JobDialog::onRadioWeeklyRecurrence()
    {
	    FUNCTION_ENTRY( "onRadioWeeklyRecurrence" );
        initialiseRadioWeeklyRecurrence();
	    FUNCTION_EXIT;
    }


    void JobDialog::onRadioMonthlyRecurrence()
    {
	    FUNCTION_ENTRY( "onRadioMonthlyRecurrence" );
        initialiseRadioMonthlyRecurrence();
	    FUNCTION_EXIT;
    }


    void JobDialog::onRadioYearlyRecurrence()
    {
	    FUNCTION_ENTRY( "onRadioYearlyRecurrence" );
        initialiseRadioYearlyRecurrence();
	    FUNCTION_EXIT;
    }


    // When an item specific to a recurrence pattern is selected, we have to
    // update the Details Summary.

	//jinhua++
    void JobDialog::onChangeOnceoff( NMHDR* pNMHDR, LRESULT* pResult )
    {
	    FUNCTION_ENTRY( "onChangeOnceoff" );

		changeOnceoff();
		*pResult = 0;
		
	    FUNCTION_EXIT;
    }

	void JobDialog::changeOnceoff()
	{
		FUNCTION_ENTRY( "ChangeOnceoff" );
		// TD #12042
		// if 'Once Off' has been selected as the recurrence pattern, without the date
		// or time being modified, then we can assume that the user wants to execute this
		// job now. we'll set the m_onceOffExecuteNow flag to true if this is the case.
		if(m_onceOffFirstExecution)
		{
			m_onceOffFirstExecution = false;
			m_onceOffExecuteNow = true;
		}
		else
		{
			m_onceOffExecuteNow = false;
		}

        // if the onceoff radio is disabled, then don't bother updating
        if (!m_onceoffRadio.IsWindowEnabled())
        {
    	    FUNCTION_EXIT;
            return;
        }

        // set the job's schedule
        COleDateTime theTime = getDateTime(IDC_ONCEOFF_DATETIMEPICKER_TIME);
        COleDateTime theDate = getDateTime(IDC_ONCEOFF_DATETIMEPICKER_DATE);

        // If read only, no need to update the range of date and time controls
        if (!m_readOnly)
        {
		    COleDateTime currTime = COleDateTime::GetCurrentTime();
		    CDateTimeCtrl* timeCtrl = reinterpret_cast<CDateTimeCtrl*>(GetDlgItem(IDC_ONCEOFF_DATETIMEPICKER_TIME));
		    TA_ASSERT(timeCtrl != NULL, "The time control cannot be found");

		    // if the date is today's date, then we need to restrict the time to a time after current time
		    // otherwise, it's ok to set the time to any time
		    if (theDate <= currTime)
		    {
			    timeCtrl->SetRange(&currTime, NULL);
			    if ((theTime.GetHour() < currTime.GetHour()) || 
				    ((theTime.GetHour() == currTime.GetHour()) && (theTime.GetMinute() < currTime.GetMinute())))
			    {
				    // TD #12042
				    // need to update 'theTime' so it gets displayed in the summary text
				    theTime = currTime;
				    timeCtrl->SetTime(currTime);
			    }

		    }
		    else
		    {
			    COleDateTime* nulltime = NULL;
			    timeCtrl->SetRange(nulltime, nulltime);
            }
            m_job.getSchedule().setOnceOff(theTime, theDate);    // update the schedule data
        }

		// display the schedule on the Summary Details panel
		CString display;
		TAUnicodeString tmpUniStrDate = m_pJobDialogModel->getDateString(theDate);
		std::string itemColDescDate = tmpUniStrDate.toMultiByteStdString();
		tmpUniStrDate = TAUnicodeString::CreateFromMultitByteString(itemColDescDate.c_str());
		TAUnicodeString tmpUniStrTime = m_pJobDialogModel->getTimeString(theTime);
		std::string itemColDescTime = tmpUniStrTime.toMultiByteStdString();
		tmpUniStrTime = TAUnicodeString::CreateFromMultitByteString(itemColDescTime.c_str());

		AfxFormatString2(display, IDS_RECURRENCE_FORMAT_NONE, tmpUniStrDate.c_str(), tmpUniStrTime.c_str());
		setFrequencySummary(display, OnceOff);

	    FUNCTION_EXIT;
    }


    void JobDialog::onChangeDaily( NMHDR* pNMHDR, LRESULT* pResult )
    {
	    FUNCTION_ENTRY( "onChangeDaily" );

		changeDaily();
		*pResult = 0;

	    FUNCTION_EXIT;
    }


	void JobDialog::changeDaily()
	{
		FUNCTION_ENTRY( "changeDaily" );
		m_onceOffFirstExecution = true;
		m_onceOffExecuteNow = false;

        // if the daily radio is disabled, then don't bother updating
        if (!m_dailyRadio.IsWindowEnabled())
        {
			FUNCTION_EXIT;
            return;
        }

        // set the job's schedule
        COleDateTime theTime = getDateTime(IDC_DAILY_DATETIMEPICKER_TIME);
        m_job.getSchedule().setDaily(theTime);    // update the schedule data

        // display the schedule on the Summary Details panel
        CString display;
		TAUnicodeString tmpUniStrTime = m_pJobDialogModel->getTimeString(theTime);
		std::string itemColDescTime = tmpUniStrTime.toMultiByteStdString();
		tmpUniStrTime = TAUnicodeString::CreateFromMultitByteString(itemColDescTime.c_str());

        AfxFormatString1(display, IDS_RECURRENCE_FORMAT_DAILY, tmpUniStrTime.c_str());
        setFrequencySummary(display, Daily);
	    FUNCTION_EXIT;
    }

    void JobDialog::onChangeWeekly( NMHDR* pNMHDR, LRESULT* pResult )
    {
	    FUNCTION_ENTRY( "onChangeWeekly" );

		onChangeWeekly();
		*pResult = 0;

	    FUNCTION_EXIT;
    }
	//++jinhua


    void JobDialog::onChangeWeekly()
    {
	    FUNCTION_ENTRY( "onChangeWeekly" );

		m_onceOffFirstExecution = true;
		m_onceOffExecuteNow = false;

        // if the weekly radio is disabled, then don't bother updating
        if (!m_weeklyRadio.IsWindowEnabled())
        {
            return;
        }

        TAUnicodeString dayStr;
		TAUnicodeString temp;
        unsigned short days = 0;
        unsigned short flag = 1;
        short index = 0;
        for (unsigned int day = IDC_WEEKLY_CHECK_MONDAY; day <= IDC_WEEKLY_CHECK_SUNDAY; day++)
        {
            FooButton* button = dynamic_cast<FooButton*>(GetDlgItem(day));
            TA_ASSERT(button != NULL, "The checkbox cannot be found");
            if (button->isChecked())
            {
                if (!dayStr.IsEmpty())
                {
                    dayStr += _T(", ");
                }
				temp = TALoadString(DAY[index]);
                TA_VERIFY(!temp.IsEmpty(), "String does not exist in string table");
                dayStr += temp;
                days |= flag;
            }
            flag <<= 1;
            index++;
        }

        // set the job's schedule
        COleDateTime theTime = getDateTime(IDC_WEEKLY_DATETIMEPICKER_TIME);
        m_job.getSchedule().setWeekly(theTime, days);

        // update the display
        CString display;
		TAUnicodeString tmpUniStrTime = m_pJobDialogModel->getTimeString(theTime);
		std::string itemColDescTime = tmpUniStrTime.toMultiByteStdString();
		tmpUniStrTime = TAUnicodeString::CreateFromMultitByteString(itemColDescTime.c_str());

        AfxFormatString2(display, IDS_RECURRENCE_FORMAT_WEEKLY, dayStr.c_str(), tmpUniStrTime.c_str());
        setFrequencySummary(display, Weekly);
	    FUNCTION_EXIT;
    }


	//jinhua++
    void JobDialog::onChangeMonthlySpecificDay( NMHDR* pNMHDR, LRESULT* pResult )
    {
	    FUNCTION_ENTRY( "onChangeMonthlySpecificDay" );
		
		onChangeMonthlySpecificDay();
		*pResult = 0;

	    FUNCTION_EXIT;
    }
	//++jinhua


    void JobDialog::onChangeMonthlySpecificDay()
    {
	    FUNCTION_ENTRY( "onChangeMonthlySpecificDay" );

		m_onceOffFirstExecution = true;
		m_onceOffExecuteNow = false;

        // if the monthly radio is disabled, then don't bother updating
        if (!m_monthlyRadio.IsWindowEnabled())
        {
			FUNCTION_EXIT;
            return;
        }

        // Get the day from the edit box
        CEdit* ctrl = reinterpret_cast<CEdit*>(GetDlgItem(IDC_MONTHLY_EDIT_ADAY_DAY));
        TA_ASSERT(ctrl != NULL, "The date edit control cannot be found");

        TAUnicodeString dayStr(_T(""));
        ctrl->GetWindowText(dayStr.str());
        unsigned short day = atoi(dayStr.toMultiByteStdString().c_str());
        // if it is an invalid day in the month, then make it take a valid value
        if (day > LAST_COMMON_DAY_OF_MONTH)
        {
            day = LAST_COMMON_DAY_OF_MONTH;
            dayStr.str().Format(_T("%d"), day);
            ctrl->SetWindowText(dayStr.GetString());
        }
        else if (day < FIRST_COMMON_DAY_OF_MONTH)
        {
            day = FIRST_COMMON_DAY_OF_MONTH;
            dayStr.str().Format(_T("%d"), day);
            ctrl->SetWindowText(dayStr.GetString());
        }

        // set the job's schedule
        COleDateTime theTime = getDateTime(IDC_MONTHLY_DATETIMEPICKER_ADAY_TIME);
        m_job.getSchedule().setMonthly(theTime, day);

        // update the display
        CString display;
		TAUnicodeString tmpUniStrTime = m_pJobDialogModel->getTimeString(theTime);
		std::string itemColDescTime = tmpUniStrTime.toMultiByteStdString();
		tmpUniStrTime = TAUnicodeString::CreateFromMultitByteString(itemColDescTime.c_str());

        AfxFormatString2(display, IDS_RECURRENCE_FORMAT_MONTHLY_SPECIFIC, dayStr.c_str(), tmpUniStrTime.c_str());
        setFrequencySummary(display, Monthly);

        checkRadio(IDC_MONTHLY_RADIO_LASTDAY, false);

        enableControl(IDC_MONTHLY_EDIT_ADAY_DAY);
        enableControl(IDC_MONTHLY_DATETIMEPICKER_ADAY_TIME);
        enableControl(IDC_MONTHLY_DATETIMEPICKER_LASTDAY_TIME, false);
	    FUNCTION_EXIT;
    }


	//jinhua++
    void JobDialog::onChangeMonthlyLastDay( NMHDR* pNMHDR, LRESULT* pResult )
    {
	    FUNCTION_ENTRY( "onChangeMonthlyLastDay" );
		
		onChangeMonthlyLastDay();
		*pResult = 0;

	    FUNCTION_EXIT;
    }
	//Jinhua++


    void JobDialog::onChangeMonthlyLastDay()
    {
	    FUNCTION_ENTRY( "onChangeMonthlyLastDay" );

		m_onceOffFirstExecution = true;
		m_onceOffExecuteNow = false;

        // if the monthly radio is disabled, then don't bother updating
        if (!m_monthlyRadio.IsWindowEnabled())
        {
    	    FUNCTION_EXIT;
            return;
        }

        // set the job's schedule
        COleDateTime theTime = getDateTime(IDC_MONTHLY_DATETIMEPICKER_LASTDAY_TIME);
        m_job.getSchedule().setMonthly(theTime, LAST_DAY_OF_MONTH); 

        // update the display
        CString display;
		TAUnicodeString tmpUniStrTime = m_pJobDialogModel->getTimeString(theTime);
		std::string itemColDescTime = tmpUniStrTime.toMultiByteStdString();
		tmpUniStrTime = TAUnicodeString::CreateFromMultitByteString(itemColDescTime.c_str());

        AfxFormatString1(display, IDS_RECURRENCE_FORMAT_MONTHLY_LAST, tmpUniStrTime.c_str());
        setFrequencySummary(display, Monthly);

        checkRadio(IDC_MONTHLY_RADIO_ADAY, false);

        enableControl(IDC_MONTHLY_EDIT_ADAY_DAY, false);
        enableControl(IDC_MONTHLY_DATETIMEPICKER_ADAY_TIME, false);
        enableControl(IDC_MONTHLY_DATETIMEPICKER_LASTDAY_TIME);
	    FUNCTION_EXIT;
    }

	//jinhua++
    void JobDialog::onChangeYearlySpecificDay( NMHDR* pNMHDR, LRESULT* pResult )
    {
	    FUNCTION_ENTRY( "onChangeYearlySpecificDay" );

		onChangeYearlySpecificDay();
		*pResult = 0;

	    FUNCTION_EXIT;
    }
	//++jinhua


    void JobDialog::onChangeYearlySpecificDay()
    {
	    FUNCTION_ENTRY( "onChangeYearlySpecificDay" );

		m_onceOffFirstExecution = true;
		m_onceOffExecuteNow = false;

        // if the yearly radio is disabled, then don't bother updating
        if (!m_yearlyRadio.IsWindowEnabled())
        {
			FUNCTION_EXIT;
            return;
        }

        // set the job's schedule
        COleDateTime theTime = getDateTime(IDC_YEARLY_DATETIMEPICKER_ADAY_TIME);
        COleDateTime theDate = getDateTime(IDC_YEARLY_DATETIMEPICKER_ADAY_DATE);
        m_job.getSchedule().setYearly(theTime, theDate.GetMonth(), theDate.GetDay()); 

        // update the display
        CString display;
		TAUnicodeString tmpUniStrDate = m_pJobDialogModel->getDateString(theDate,false);
		std::string itemColDescDate = tmpUniStrDate.toMultiByteStdString();
		tmpUniStrDate = TAUnicodeString::CreateFromMultitByteString(itemColDescDate.c_str());
		TAUnicodeString tmpUniStrTime = m_pJobDialogModel->getTimeString(theTime);
		std::string itemColDescTime = tmpUniStrTime.toMultiByteStdString();
		tmpUniStrTime = TAUnicodeString::CreateFromMultitByteString(itemColDescTime.c_str());

        AfxFormatString2(display, IDS_RECURRENCE_FORMAT_YEARLY_SPECIFIC, tmpUniStrDate.c_str(), tmpUniStrTime.c_str());
        setFrequencySummary(display, Yearly);

        checkRadio(IDC_YEARLY_RADIO_LASTDAY, false);

        enableControl(IDC_YEARLY_DATETIMEPICKER_ADAY_TIME);
        enableControl(IDC_YEARLY_DATETIMEPICKER_ADAY_DATE);
        enableControl(IDC_YEARLY_DATETIMEPICKER_LASTDAY_TIME, false);
	    FUNCTION_EXIT;
    }
	

	//Jinhua++
    void JobDialog::onChangeYearlyLastDay( NMHDR* pNMHDR, LRESULT* pResult )
    {
	    FUNCTION_ENTRY( "onChangeYearlyLastDay" );

		onChangeYearlyLastDay();
		*pResult = 0;

	    FUNCTION_EXIT;
    }
	//jinhua++


    void JobDialog::onChangeYearlyLastDay()
    {
	    FUNCTION_ENTRY( "onChangeYearlyLastDay" );

		m_onceOffFirstExecution = true;
		m_onceOffExecuteNow = false;

        // if the yearly radio is disabled, then don't bother updating
        if (!m_yearlyRadio.IsWindowEnabled())
        {
    	    FUNCTION_EXIT;
            return;
        }

        // set the job's schedule
        COleDateTime theTime = getDateTime(IDC_YEARLY_DATETIMEPICKER_LASTDAY_TIME);
        m_job.getSchedule().setYearly(theTime, FEBRUARY, LAST_DAY_OF_MONTH); // Last day of February

        // update the display
        CString display;
		TAUnicodeString tmpUniStrTime = m_pJobDialogModel->getTimeString(theTime);
		std::string itemColDescTime = tmpUniStrTime.toMultiByteStdString();
		tmpUniStrTime = TAUnicodeString::CreateFromMultitByteString(itemColDescTime.c_str());

        AfxFormatString1(display, IDS_RECURRENCE_FORMAT_YEARLY_LAST, tmpUniStrTime.c_str());
        setFrequencySummary(display, Yearly);

        checkRadio(IDC_YEARLY_RADIO_ADAY, false);

        enableControl(IDC_YEARLY_DATETIMEPICKER_ADAY_TIME, false);
        enableControl(IDC_YEARLY_DATETIMEPICKER_ADAY_DATE, false);
        enableControl(IDC_YEARLY_DATETIMEPICKER_LASTDAY_TIME);
	    FUNCTION_EXIT;
    }


    //////////////////////////////////////////////////////////////////////////////////
    //
    //
    // Handlers for the Range of Recurrence Panel
    //
    //
    //////////////////////////////////////////////////////////////////////////////////

    void JobDialog::onNoEndDate()
    {
	    FUNCTION_ENTRY( "onNoEndDate" );
        m_job.getSchedule().setEndDate(NULL);
        TA_VERIFY(!TALoadString(IDS_RANGE_NONE).IsEmpty(), "String does not exist in string table");
        // TD #12083
		// IDS_RANGE_NONE is a single space, which is being appended to the first
		// line of the summary text when no end date has been specified.
		// This is causing troubles with the automated tests, so just set it to an empty string instead.
        //setPeriodSummary(str, NoEndDate);
		setPeriodSummary("", NoEndDate);

        // disable the controls for the 
        m_endAfterCtrl.EnableWindow(FALSE);
        m_endByCtrl.EnableWindow(FALSE);
	    FUNCTION_EXIT;
    }


    void JobDialog::onEndAfter()
    {
	    FUNCTION_ENTRY( "onEndAfter" );
        TA_ASSERT(m_scheduleType != OnceOff, "Shouldn't get in here. Once off shouldn't have an end date");

        TAUnicodeString display, endAfterStr;
        m_endAfterCtrl.GetWindowText(endAfterStr.str());

        long numRepeats = atol(endAfterStr.toMultiByteStdString().c_str());

        if ((numRepeats < MIN_RECURRENCE) || (numRepeats > MAX_RECURRENCE))
        {
            if (numRepeats < MIN_RECURRENCE)
            {
                numRepeats = MIN_RECURRENCE;
            }
            else
            {
                numRepeats = MAX_RECURRENCE;
            }
            endAfterStr.str().Format(_T("%d"), numRepeats);
            m_endAfterCtrl.SetWindowText(endAfterStr.c_str());
        }
        Schedule& schedule = m_job.getSchedule();
        schedule.setEndDate(numRepeats);

        // update display.  
        TAUnicodeString numRepeatStr;
        numRepeatStr.str().Format(_T("%d"), numRepeats);
		TAUnicodeString tmpUniStrDate = m_pJobDialogModel->getDateString(*schedule.getEndDate());
		std::string itemColDescDate = tmpUniStrDate.toMultiByteStdString();
		tmpUniStrDate = TAUnicodeString::CreateFromMultitByteString(itemColDescDate.c_str());
        AfxFormatString2(display.str(), IDS_RANGE_END_AFTER, numRepeatStr.str(), tmpUniStrDate.c_str());
        setPeriodSummary(display.str(), EndAfterNumTimes);

        // enableControl/disable the controls for the 
        m_endAfterCtrl.EnableWindow(TRUE);
        m_endByCtrl.EnableWindow(FALSE);
	    FUNCTION_EXIT;
    }

	//jinhua++
	void JobDialog::onEndBy( NMHDR* pNMHDR, LRESULT* pResult )
	{
		FUNCTION_ENTRY( "onEndBy" );

		onEndBy();
		*pResult = 0;

		FUNCTION_EXIT;
	}
	//++jinhua


    void JobDialog::onEndBy()
    {
	    FUNCTION_ENTRY( "onEndBy" );
        COleDateTime theDate;
        m_endByCtrl.GetTime(theDate);

        m_job.getSchedule().setEndDate(theDate);

        // update display
        CString display;
		TAUnicodeString tmpUniStrDate = m_pJobDialogModel->getDateString(theDate);
		std::string itemColDescDate = tmpUniStrDate.toMultiByteStdString();
		tmpUniStrDate = TAUnicodeString::CreateFromMultitByteString(itemColDescDate.c_str());
;
        AfxFormatString1(display, IDS_RANGE_END_BEFORE, tmpUniStrDate.c_str());
        setPeriodSummary(display, EndByDate);

        // enableControl/disable the controls for the 
        m_endAfterCtrl.EnableWindow(FALSE);
        m_endByCtrl.EnableWindow(TRUE);
	    FUNCTION_EXIT;
    }


    //////////////////////////////////////////////////////////////////////////////////
    //
    //
    // Recurrence Pattern panel Helper methods
    //
    //
    //////////////////////////////////////////////////////////////////////////////////


    // When the radio dials on the left of Recurrence Pattern Panel are selected, we need 
    // to initialise the date time controls so that they are displaying and restricting 
    // the data as desired 
    void JobDialog::initialiseRadioNoRecurrence(COleDateTime* theTime /*=NULL*/, COleDateTime* theDate /*=NULL*/) 
    {
	    FUNCTION_ENTRY( "initialiseRadioNoRecurrence" );
        AddDialog(IDD_NO_RECURRENCE, IDC_JD_RECURRENCE_PATTERN);
        initialiseDateTimeCtrl(IDC_ONCEOFF_DATETIMEPICKER_DATE, theDate);
        initialiseDateTimeCtrl(IDC_ONCEOFF_DATETIMEPICKER_TIME, theTime);
        changeOnceoff();
        enableRangePanel(false);
	    FUNCTION_EXIT;
    }

    void JobDialog::initialiseRadioDailyRecurrence(COleDateTime* theTime /*= NULL*/) 
    {
	    FUNCTION_ENTRY( "initialiseRadioDailyRecurrence" );
        AddDialog(IDD_DAILY_RECURRENCE, IDC_JD_RECURRENCE_PATTERN);
        initialiseDateTimeCtrl(IDC_DAILY_DATETIMEPICKER_TIME, theTime);
        changeDaily();
        enableRangePanel(true);
	    FUNCTION_EXIT;
    }

    void JobDialog::initialiseRadioWeeklyRecurrence(COleDateTime* theTime /*= NULL*/, 
                                            unsigned long days /*= 0*/) 
    {
	    FUNCTION_ENTRY( "initialiseRadioWeeklyRecurrence" );
        AddDialog(IDD_WEEKLY_RECURRENCE, IDC_JD_RECURRENCE_PATTERN);
        initialiseDateTimeCtrl(IDC_WEEKLY_DATETIMEPICKER_TIME, theTime);
    
        // check the corresponding check box if the day is selected
        checkBox(IDC_WEEKLY_CHECK_MONDAY, (days&Monday)!=0);
	    checkBox(IDC_WEEKLY_CHECK_TUESDAY, (days&Tuesday)!=0);
	    checkBox(IDC_WEEKLY_CHECK_WEDNESDAY, (days&Wednesday)!=0);
	    checkBox(IDC_WEEKLY_CHECK_THURSDAY, (days&Thursday)!=0);
	    checkBox(IDC_WEEKLY_CHECK_FRIDAY, (days&Friday)!=0);
	    checkBox(IDC_WEEKLY_CHECK_SATURDAY, (days&Saturday)!=0);
	    checkBox(IDC_WEEKLY_CHECK_SUNDAY, (days&Sunday)!=0);

        onChangeWeekly();
        enableRangePanel(true);
	    FUNCTION_EXIT;
    }

    void JobDialog::initialiseRadioMonthlyRecurrence(COleDateTime* theTime /*= NULL*/, 
                                             unsigned long theDay /*= 1*/) 
    {
	    FUNCTION_ENTRY( "initialiseRadioMonthlyRecurrence" );
        AddDialog(IDD_MONTHLY_RECURRENCE, IDC_JD_RECURRENCE_PATTERN);

        // initialise the day specific controls
        initialiseDateTimeCtrl(IDC_MONTHLY_DATETIMEPICKER_ADAY_TIME, theTime);

        // initialise the controls for last day
        initialiseDateTimeCtrl(IDC_MONTHLY_DATETIMEPICKER_LASTDAY_TIME, theTime);

        enableRangePanel(true);

        if (theDay == LAST_DAY_OF_MONTH)
        {
            initialiseEditAndSpinner(IDC_MONTHLY_EDIT_ADAY_DAY, IDC_MONTHLY_SPINNER_DAY, FIRST_COMMON_DAY_OF_MONTH, LAST_COMMON_DAY_OF_MONTH, FIRST_COMMON_DAY_OF_MONTH);
            checkRadio(IDC_MONTHLY_RADIO_LASTDAY, true);
            onChangeMonthlyLastDay();
        }
        else
        {
            initialiseEditAndSpinner(IDC_MONTHLY_EDIT_ADAY_DAY, IDC_MONTHLY_SPINNER_DAY, FIRST_COMMON_DAY_OF_MONTH, LAST_COMMON_DAY_OF_MONTH, theDay);
            checkRadio(IDC_MONTHLY_RADIO_ADAY, true);
            onChangeMonthlySpecificDay();
        }
	    FUNCTION_EXIT;
    }


    void JobDialog::initialiseRadioYearlyRecurrence(COleDateTime* theTime /*= NULL*/, 
                                                    unsigned short theDay /*= USHRT_MAX */, 
                                                    unsigned short theMonth /*= USHRT_MAX */)
    {
	    FUNCTION_ENTRY( "initialiseRadioYearlyRecurrence" );
        AddDialog(IDD_YEARLY_RECURRENCE, IDC_JD_RECURRENCE_PATTERN);

        // initialise time controls
        initialiseDateTimeCtrl(IDC_YEARLY_DATETIMEPICKER_ADAY_TIME, theTime);
        initialiseDateTimeCtrl(IDC_YEARLY_DATETIMEPICKER_LASTDAY_TIME, theTime);

        enableRangePanel(true);

        if (theDay == LAST_DAY_OF_MONTH)
        {
            // last day
            initialiseDateTimeCtrl(IDC_YEARLY_DATETIMEPICKER_ADAY_DATE, NULL, NULL, NULL, true);
            checkRadio(IDC_YEARLY_RADIO_LASTDAY, true);
            onChangeYearlyLastDay();
        }
        else
        {
            COleDateTime currentDate = COleDateTime::GetCurrentTime();
            if (theDay == USHRT_MAX)
            {
                theMonth = currentDate.GetMonth();
                theDay = currentDate.GetDay();
            }
            COleDateTime theDate(currentDate.GetYear(), theMonth, theDay, 0, 0, 0);
            initialiseDateTimeCtrl(IDC_YEARLY_DATETIMEPICKER_ADAY_DATE, &theDate, NULL, NULL, true);
            checkRadio(IDC_YEARLY_RADIO_ADAY, true);
            onChangeYearlySpecificDay();
        }
	    FUNCTION_EXIT;
    }


    //////////////////////////////////////////////////////////////////////////////////
    //
    //
    // Generic Helper methods
    //
    //
    //////////////////////////////////////////////////////////////////////////////////


    void JobDialog::initialiseDateTimeCtrl(const int itemId, const COleDateTime* theTime, const COleDateTime* earliest /*=NULL*/, const COleDateTime* latest /*=NULL*/, const bool yearly /* = false */)
    {
	    FUNCTION_ENTRY( "initialiseDateTimeCtrl" );
        CDateTimeCtrl* dateTimeCtrl = reinterpret_cast<CDateTimeCtrl*>(GetDlgItem(itemId));
        TA_ASSERT(dateTimeCtrl != NULL, "The dateTimeCtrl control cannot be found");

        // if it is not read-only then just display it as it is
        if (theTime != NULL)
        {
            dateTimeCtrl->SetTime(*theTime);
        }
        else
        {
            COleDateTime nullTime = COleDateTime::GetCurrentTime();
            nullTime.SetTime(0, 0, 0);
            dateTimeCtrl->SetTime(nullTime);
        }
        // is it a time-only control, or a dateTimeCtrl-only control?
        if ((dateTimeCtrl->GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT)
        {
            dateTimeCtrl->SetFormat(_T("HH:mm"));
        }
        else
        {
            if (yearly)
            {
                dateTimeCtrl->SetFormat(_T("dd-MMM"));
                return;
            }        
            dateTimeCtrl->SetFormat(_T("dd-MMM-yy"));
            dateTimeCtrl->SetRange((earliest==NULL) ? &COleDateTime::GetCurrentTime() : earliest, latest);
        }
	    FUNCTION_EXIT;
    }


    void JobDialog::initialiseEditAndSpinner(const int editId, const int spinnerId, const int min, const int max, const int select /*= UINT_MAX*/)
    {
	    FUNCTION_ENTRY( "initialiseEditAndSpinner" );
        // initialise the spinner
        CSpinButtonCtrl* spinner = reinterpret_cast<CSpinButtonCtrl*>(GetDlgItem(spinnerId));
        TA_ASSERT(spinner != NULL, "The spinner control cannot be found");

        CEdit* edit = reinterpret_cast<CEdit*>(GetDlgItem(editId));
        TA_ASSERT(edit != NULL, "The edit control cannot be found");

        // associate spinner with the edit box so that we can limit the number
        spinner->SetBuddy(edit);        
        spinner->SetRange(min, max);    // set range

        // limit it to the number of characters max is equivalent to
        int temp = max;
		int i = 1;
        for (; (temp = temp/10) > 0; i++);
        edit->SetLimitText(i);

        if (select != UINT_MAX)
        {
            // adjust the value of select if it is outside the range
            spinner->SetPos(select);
            TAUnicodeString curr;
            curr.str().Format(_T("%d"), select);
            edit->SetWindowText(curr.GetString());
        }
	    FUNCTION_EXIT;
    }


    void JobDialog::checkButton(const FooButton::Type type, const int itemId, const bool wantCheck)
    {
	    FUNCTION_ENTRY( "checkButton" );
        FooButton* button = reinterpret_cast<FooButton*>(GetDlgItem(itemId));
        TA_ASSERT(button != NULL, "The button cannot be found");

        button->setType(type);
        button->check(wantCheck);
	    FUNCTION_EXIT;
    }


    void JobDialog::enableControl(const int itemId, const int enable /*= TRUE */)
    {
	    FUNCTION_ENTRY( "enableControl" );
        CWnd* wnd = reinterpret_cast<CWnd*>(GetDlgItem(itemId));
        TA_ASSERT(wnd != NULL, "The control cannot be found");
        wnd->EnableWindow(enable);
	    FUNCTION_EXIT;
    }


    void JobDialog::setReadOnly(const int itemId)
    {
	    FUNCTION_ENTRY( "setReadOnly" );
        CWnd* wnd = reinterpret_cast<CWnd*>(GetDlgItem(itemId));
        TA_ASSERT(wnd != NULL, "The control cannot be found");

        // edit box
        CEdit* edit = dynamic_cast<CEdit*>(wnd);
        if (edit != NULL)
        {
            edit->SetReadOnly(TRUE);
    	    FUNCTION_EXIT;
            return;
        }

        // id of the resource, can be any number since we're not going to do anything with it apart from
        // displaying it as read-only
        int newResourceId = 1; 

        // combo box
        CComboBox* combo = dynamic_cast<CComboBox*>(wnd);
        if (combo != NULL)
        {
			CString strControlText;
            combo->GetWindowText(strControlText);

	        // Get edit control which happens to be the first child window
	        CEdit* edit = dynamic_cast<CEdit*>(combo->GetWindow(GW_CHILD));
            if (edit != NULL)
            {
	            // Set read only is combo box is disabled
	            edit->SetReadOnly(TRUE);
        	    FUNCTION_EXIT;
                return;
            }

            // otherwise this is a drop down list, which has a static control rather than edit control, so
            // it's difficult for us to make it read-only.  So we'll just hide this control and put a new
            // drop down combo in its place

			// TD #12425
			// SOLUTION 1:
			// Disable the combo box, and overlay a CStatic label to display the text.
			// Issue with this: unsure how to get the bounds of the combo box's 3d border,
			// so needed to reduce the size of the label using hardcoded values.
			combo->EnableWindow(false);
            CRect rect;
            combo->GetWindowRect(&rect);
	        ScreenToClient(&rect);
			rect.top += 4;
			rect.bottom -= 2;
			rect.left +=4;
			rect.right -= 18;
            CStatic* comboText = new CStatic;
            CString text;
            combo->GetWindowText(text);
            comboText->Create(text, WS_CHILD|WS_VISIBLE, rect, this, newResourceId);

			// TD #12425
			// SOLUTION 2:
			// Replace the combo box with a CEdit textbox set to read only.
			// Issue with this: the drop-down arrow is no longer visible.
			/*edit = new CEdit;
			CRect rect;
            combo->GetWindowRect(&rect);
	        ScreenToClient(&rect);
            CString text;
            combo->GetWindowText(text);
			edit->CreateEx(WS_EX_CLIENTEDGE, "Edit", text, WS_CHILD|WS_VISIBLE|WS_TABSTOP, rect, this, newResourceId);
			edit->SetReadOnly(TRUE);
			combo->ShowWindow(SW_HIDE);*/

			// ORIGINAL:
			// Replace the combo box with an editable combo box set to read only.
			// Issue with this: the drop-down arrow remains active, and pressing it
			// results in a black line to apprear below the control.
			// (i.e. a drop-down list of height 1)
            /*CRect rect;
            combo->GetWindowRect(&rect);
	        ScreenToClient(&rect);
            CComboBox* newCombo = new CComboBox;
            newCombo->Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWN, rect, this, newResourceId);
            CString text;
            combo->GetWindowText(text);
            newCombo->SetWindowText(text);
            // Set edit control which happens to be the first child window to read-only
	        edit = reinterpret_cast<CEdit*>(newCombo->GetWindow(GW_CHILD));
            TA_ASSERT(edit != NULL, "The edit cannot be found");
    	    edit->SetReadOnly(m_readOnly);
            // hide the actual combo
            combo->ShowWindow(SW_HIDE);*/

    	    FUNCTION_EXIT;
            return;
        }

        // check box or radio
        FooButton* buttonEx = dynamic_cast<FooButton*>(wnd);
        if (buttonEx != NULL)
        {
            buttonEx->setReadOnly(true);
            buttonEx->enable(false);
    	    FUNCTION_EXIT;
            return;
        }

        CDateTimeCtrl* dateTimeCtrl = reinterpret_cast<CDateTimeCtrl*>(GetDlgItem(itemId));
        if (dateTimeCtrl != NULL)
        {
            // otherwise it is read-only.  As we can't make the CDateTimeCtrl read-only, we have to substitute
            // it with a spinner edit (if it's time) or combo box (if it's date).

            // get the size of the actual control
            CRect rect;
            dateTimeCtrl->GetWindowRect(&rect);
	        ScreenToClient(&rect);

            COleDateTime theTime;
            DWORD dwResult = dateTimeCtrl->GetTime(theTime);
            bool validTime = (dwResult != FALSE);

            if ((dateTimeCtrl->GetStyle() & DTS_TIMEFORMAT) == DTS_TIMEFORMAT) // time -> change to edit with spinner
            {
                // dynamically create an edit control on the view
                CEdit* edit = new CEdit;
				
				// TD #11900
				// Use CreateEx() so we can get the 3d border with WS_EX_CLIENTEDGE.
				// Create() with WS_BORDER resulted in a flat, 1 pixel border
                edit->CreateEx(WS_EX_CLIENTEDGE, _T("Edit"), _T(""), WS_CHILD|WS_VISIBLE|WS_TABSTOP, rect, this, newResourceId);
//                edit->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|WS_BORDER, rect, this, newResourceId);
                if (validTime)
                {
                    edit->SetWindowText(theTime.Format(_T("%H:%M")));
                }
                edit->SetReadOnly(TRUE);

                CSpinButtonCtrl* spinner = new CSpinButtonCtrl;
                spinner->Create(WS_CHILD|WS_VISIBLE|WS_TABSTOP|UDS_SETBUDDYINT|UDS_ALIGNRIGHT, rect, this, newResourceId);
                spinner->SetBuddy(edit);        
                spinner->EnableWindow(FALSE);

				// hide the actual control
				dateTimeCtrl->ShowWindow(SW_HIDE);    
            }
            else    // date -> change to combo
			{
				// TD #12425
				// SOLUTION 1:
				// (see above)
				dateTimeCtrl->EnableWindow(false);
				rect.top += 4;
				rect.bottom -= 2;
				rect.left +=4;
				rect.right -= 18;
				CStatic* comboText = new CStatic;

				// TD12474 ++
				//comboText->Create(theTime.Format("%d-%b-%y"), WS_CHILD|WS_VISIBLE, rect, this, newResourceId);

				TA_Base_Core::DateFormat dateFormat; 
				std::string dateFormatStr = dateFormat.getDateFormat(TA_Base_Core::DateFormat::DateFormat1a);

				TAUnicodeString tmpUniStrForDate = TAUnicodeString::CreateFromMultitByteString(dateFormatStr.c_str());
				TAUnicodeString tmpUniStr = theTime.Format(tmpUniStrForDate.c_str());
				std::string itemColDesc = tmpUniStr.toMultiByteStdString();
				tmpUniStr = TAUnicodeString::CreateFromMultitByteString(itemColDesc.c_str());
				comboText->Create(tmpUniStr.c_str(), WS_CHILD|WS_VISIBLE, rect, this, newResourceId);
				// ++ TD12474 

				// TD #12425
				// SOLUTION 2:
				// (see above)
                /*CEdit* edit = new CEdit;
				edit->CreateEx(WS_EX_CLIENTEDGE, "Edit", theTime.Format("%d-%b-%y"), WS_CHILD|WS_VISIBLE|WS_TABSTOP, rect, this, newResourceId);
				edit->SetReadOnly(TRUE);
				dateTimeCtrl->ShowWindow(SW_HIDE);*/

				// TD #12425
				// ORIGINAL:
				// (see above)
/*				CComboBox* combo = new CComboBox;
                combo->Create(WS_CHILD|WS_VISIBLE|CBS_DROPDOWN, rect, this, newResourceId);
                if (validTime)
                {
                    combo->SetWindowText(theTime.Format("%d-%b-%y"));
                }

                // Set edit control which happens to be the first child window to read-only
	            CEdit* edit = reinterpret_cast<CEdit*>(combo->GetWindow(GW_CHILD));
                TA_ASSERT(edit != NULL, "The edit control cannot be found");
    	        edit->SetReadOnly(m_readOnly);*/
            }

    	    FUNCTION_EXIT;
            return;
        }

        TA_ASSERT(false, "Don't know how to make the control read only");
    }


    void JobDialog::setFrequencySummary(const CString& str, const ERecurrenceType type)
    {
	    FUNCTION_ENTRY( "setFrequencySummary" );
        m_frequencyStr = str;
        m_scheduleType = type;

        // if the End After x occurrences radio dial is checked in the Recurrence
        // Range panel, then we need to recalculate the new end date
        if (m_range == EndAfterNumTimes && m_scheduleType != OnceOff)
        {
            onEndAfter();
        }

        CString text = m_frequencyStr;
        if (m_scheduleType != OnceOff)
        {
             text += m_periodStr;
        }
        m_summaryCtrl.SetWindowText(text);
	    FUNCTION_EXIT;
    }


    void JobDialog::setPeriodSummary(const CString& str, const ERecurrenceRange range)
    {
	    FUNCTION_ENTRY( "setPeriodSummary" );
        m_periodStr = str;
        m_range = range;
        CString text = m_frequencyStr + m_periodStr;
        m_summaryCtrl.SetWindowText(text);
	    FUNCTION_EXIT;
    }

    COleDateTime JobDialog::getDateTime(const int itemId) const
    {
	    FUNCTION_ENTRY( "getDateTime" );
        // Gain a pointer to the control
        CDateTimeCtrl* dateTimeCtrl = reinterpret_cast<CDateTimeCtrl*>(GetDlgItem(itemId));
        TA_ASSERT(dateTimeCtrl != NULL, "The dateTimeCtrl control cannot be found");

        COleDateTime theTime;
        dateTimeCtrl->GetTime(theTime);
	    FUNCTION_EXIT;
        return theTime;
    }


    void JobDialog::enableRangePanel(const bool enableControl)
    {
	    FUNCTION_ENTRY( "enableRangePanel" );
	    m_noEndDateRadio.enable(enableControl);
	    m_endAfterDateRadio.enable(enableControl);
	    m_endByDateRadio.enable(enableControl);

	    m_rangePanel.EnableWindow(enableControl);
	    m_endAfterSpinnerCtrl.EnableWindow(enableControl);
        m_rangePanelText.EnableWindow(enableControl);
        if (!enableControl)
        {
            m_endAfterCtrl.EnableWindow(FALSE);
            m_endByCtrl.EnableWindow(FALSE);
        }
	    FUNCTION_EXIT;
    }


    bool JobDialog::prepareDataToSave() 
    {
	    FUNCTION_ENTRY( "prepareDataToSave" );
        // save any data that we didn't save earlier
        TAUnicodeString description;
        m_descriptionCtrl.GetWindowText(description.str());
        m_job.setDescription(description.toMultiByteStdString());

        TransActiveMessage userMsg;
        TAUnicodeString itemName;
		TAUnicodeString actionName = TALoadString(IDS_SAVE_JOB);
        TA_VERIFY(!actionName.IsEmpty(), "String does not exist in string table");
        userMsg << actionName;

        // save the profile and location
        try
        {
            int sel = m_profileCtrl.GetCurSel();
            if (sel == CB_ERR)
            {
				itemName = TALoadString(IDS_PROFILE);
                TA_VERIFY(!itemName.IsEmpty(), "String does not exist in string table");
                userMsg << itemName;
                userMsg.showMsgBox(IDS_UE_250014);
                return false;
            }

            IProfile* profile = reinterpret_cast<IProfile*>(m_profileCtrl.GetItemData(sel));
            if (profile == NULL)
            {
				itemName = TALoadString(IDS_PROFILE);
				TA_VERIFY(!itemName.IsEmpty(), "String does not exist in string table");
                userMsg << itemName;
                userMsg.showMsgBox(IDS_UE_250014);
                return false;
            }

            m_job.setProfileId(profile->getKey());

            if (profile->requiresLocationToBeSelected())
            {
                sel = m_locationCtrl.GetCurSel();
                if (sel == CB_ERR)
                {
					itemName = TALoadString(IDS_LOCATION);
                    TA_VERIFY(!itemName.IsEmpty(), "String does not exist in string table");
                    userMsg << itemName;
                    userMsg.showMsgBox(IDS_UE_250014);
            	    FUNCTION_EXIT;
                    return false;
                }

                m_job.setLocationId(m_locationCtrl.GetItemData(sel));
            }
    	    FUNCTION_EXIT;
            return true;
        }
        catch (...)
        {
            UINT selectedButton = userMsg.showMsgBox(IDS_UE_250015);
        }
	    FUNCTION_EXIT;
        return false;
    }


    void JobDialog::onSaveNew() 
    {
	    FUNCTION_ENTRY( "onSaveNew" );

		// TD #12042
		// The user wants to execute this job now.
		// Unsure how to do this, so just reschedule the job to be
		// executed at the turn of the next minute.
		if(m_onceOffExecuteNow)
		{
			// get the current time + 1 minute
	        COleDateTime theTime = COleDateTime::GetCurrentTime() + COleDateTimeSpan(0, 0, 1, 0);
			m_job.getSchedule().setOnceOff(theTime, theTime);

			// the dialog stays open, so update the date & time displayed
			// in the controls and in the summary text
			CDateTimeCtrl* dateCtrl = reinterpret_cast<CDateTimeCtrl*>(GetDlgItem(IDC_ONCEOFF_DATETIMEPICKER_DATE));
			TA_ASSERT(dateCtrl != NULL, "The date control cannot be found");
			dateCtrl->SetTime(theTime);

			CDateTimeCtrl* timeCtrl = reinterpret_cast<CDateTimeCtrl*>(GetDlgItem(IDC_ONCEOFF_DATETIMEPICKER_TIME));
			TA_ASSERT(timeCtrl != NULL, "The time control cannot be found");
			timeCtrl->SetTime(theTime);

			CString display;
			TAUnicodeString tmpUniStrDate = m_pJobDialogModel->getDateString(theTime);
			std::string itemColDescDate = tmpUniStrDate.toMultiByteStdString();
			tmpUniStrDate = TAUnicodeString::CreateFromMultitByteString(itemColDescDate.c_str());
			TAUnicodeString tmpUniStrTime = m_pJobDialogModel->getTimeString(theTime);
			std::string itemColDescTime = tmpUniStrTime.toMultiByteStdString();
			tmpUniStrTime = TAUnicodeString::CreateFromMultitByteString(itemColDescTime.c_str());
			AfxFormatString2(display, IDS_RECURRENCE_FORMAT_NONE, tmpUniStrDate.c_str(), tmpUniStrTime.c_str());
			setFrequencySummary(display, OnceOff);
		}

        if (prepareDataToSave() && saveNewJob(m_job))
        {
            // disable it once we've saved it as the operator must enter a new description before saving another one
            m_saveNewCtrl.EnableWindow(FALSE); 
        }
	    FUNCTION_EXIT;
    }


    void JobDialog::OnOK() 
    {
	    FUNCTION_ENTRY( "OnOK" );

		if(m_onceOffExecuteNow)
		{
	        COleDateTime theTime = COleDateTime::GetCurrentTime() + COleDateTimeSpan(0, 0, 1, 0);
			m_job.getSchedule().setOnceOff(theTime, theTime);
		}

        // if we didn't manage to save successfully, then don't close the dialog so that the operator 
        // could fix the problem
        if (prepareDataToSave() && saveJob(m_job))
        {
    	    CDialog::OnOK();
        }
	    FUNCTION_EXIT;
    }


    void JobDialog::disableAllControls()
    {
	    FUNCTION_ENTRY( "disableAllControls" );

        m_selectJobButton.EnableWindow(FALSE);

		setReadOnly(IDC_JD_COMBO_PROFILE);
        setReadOnly(IDC_JD_COMBO_LOCATION);

		setReadOnly(IDC_JD_COMBO_JOBTYPE);
        setReadOnly(IDC_JD_EDIT_JOBNAME);
        setReadOnly(IDC_JD_EDIT_JOBDESCRIPTION);
		
        setReadOnly(IDC_JD_RADIO_ONCEOFF);
        setReadOnly(IDC_JD_RADIO_DAILY);
        setReadOnly(IDC_JD_RADIO_WEEKLY);
        setReadOnly(IDC_JD_RADIO_MONTHLY);
        setReadOnly(IDC_JD_RADIO_YEARLY);

        Schedule& schedule = m_job.getSchedule();

        setReadOnly(IDC_JD_RADIO_NO_END_DATE);
        setReadOnly(IDC_JD_RADIO_END_AFTER);
        setReadOnly(IDC_JD_RADIO_END_BY);
        if (schedule.hasEndDate())
        {
            setReadOnly(IDC_JD_DATETIMEPICKER_END_BY);
        }

        switch (schedule.getType())
        {
        case OnceOff:
            setReadOnly(IDC_ONCEOFF_DATETIMEPICKER_DATE);
            setReadOnly(IDC_ONCEOFF_DATETIMEPICKER_TIME);
            break;
        case Daily:
            setReadOnly(IDC_DAILY_DATETIMEPICKER_TIME);
            break;
        case Weekly:
            setReadOnly(IDC_WEEKLY_DATETIMEPICKER_TIME);
            setReadOnly(IDC_WEEKLY_CHECK_MONDAY);
            setReadOnly(IDC_WEEKLY_CHECK_TUESDAY);
            setReadOnly(IDC_WEEKLY_CHECK_WEDNESDAY);
            setReadOnly(IDC_WEEKLY_CHECK_THURSDAY);
            setReadOnly(IDC_WEEKLY_CHECK_FRIDAY);
            setReadOnly(IDC_WEEKLY_CHECK_SATURDAY);
            setReadOnly(IDC_WEEKLY_CHECK_SUNDAY);
            break;
        case Monthly:
            setReadOnly(IDC_MONTHLY_RADIO_ADAY);
            setReadOnly(IDC_MONTHLY_RADIO_LASTDAY);
            if (schedule.isOnLastDay())
            {
                setReadOnly(IDC_MONTHLY_DATETIMEPICKER_LASTDAY_TIME);
            }
            else
            {
                setReadOnly(IDC_MONTHLY_DATETIMEPICKER_ADAY_TIME);
                setReadOnly(IDC_MONTHLY_EDIT_ADAY_DAY);
            }
            break;
        case Yearly:
            setReadOnly(IDC_YEARLY_RADIO_ADAY);
            setReadOnly(IDC_YEARLY_RADIO_LASTDAY);

            if (schedule.isOnLastDay())
            {
                setReadOnly(IDC_YEARLY_DATETIMEPICKER_LASTDAY_TIME);
            }
            else
            {
                setReadOnly(IDC_YEARLY_DATETIMEPICKER_ADAY_DATE);
                setReadOnly(IDC_YEARLY_DATETIMEPICKER_ADAY_TIME);
            }

            break;
        default:
            TA_ASSERT(false, "Unknown type");
        }
	    FUNCTION_EXIT;
    }


    void JobDialog::onSelectJob() 
    {
	    FUNCTION_ENTRY( "onSelectJob" );
        try
        {
            IJobDetailsPlugin* plugin = m_componentLibraryAccessor.getComponent(m_job.getType());
            if (plugin != NULL)
            {
                TAUnicodeString jobDescription;
                m_descriptionCtrl.GetWindowText(jobDescription.str());

                IJobDetailsPlugin::JobData jobData;
                jobData.description = jobDescription;
                jobData.parameters = m_job.getParameters();

                plugin->setData(jobData);
                plugin->showSelectJobDialog(m_readOnly);

                if (!m_readOnly) 
                {
                    // Get updated parameters. Name and description may also be updated if
                    // the plugin has determined that the old ones were auto generated as well.
                    // If manually set by the operator, the plugin should leave them alone.
                    jobData = plugin->getUpdatedData(false);

                    // Disable the Details button if the job details is not specified
                    if (!jobData.parameters.empty())
                    {
                        m_job.setParameters(jobData.parameters);

                        m_nameCtrl.SetWindowText(jobData.name.str());
                        m_descriptionCtrl.SetWindowText(jobData.description.str());
                    }

					// TD #12412
					// only enable the Details button if the user has the rights to use it
					m_jobDetailsButton.EnableWindow(m_pJobDialogModel->isJobDetailsButtonEnabled() && m_nameCtrl.GetWindowTextLength () != 0);
                    onChangeJobdescription();
                }
            }
            else
            {
                TransActiveMessage userMsg;
                //userMsg << m_job.getType().c_str();
                UINT selectedButton = userMsg.showMsgBox(IDS_UE_250013);
            }
        }
        catch (...)
        {
            TAUnicodeString action = TALoadString(IDS_SHOW_JOB_DETAIL);;

            TransActiveMessage userMsg;
            userMsg << action;
            userMsg.showMsgBox(IDS_UE_250014);
        }
	    FUNCTION_EXIT;
    }


    void JobDialog::onJobDetails() 
    {
	    FUNCTION_ENTRY( "onJobDetails" );
        try
        {
            IJobDetailsPlugin* plugin = m_componentLibraryAccessor.getComponent(m_job.getType());
            if (plugin != NULL)
            {
                TAUnicodeString jobDescription;
                m_descriptionCtrl.GetWindowText(jobDescription.str());

                IJobDetailsPlugin::JobData jobData;
                jobData.description = jobDescription;
                jobData.parameters = m_job.getParameters();

                plugin->setData(jobData);
                plugin->showJobDetailsDialog();
            }
            else
            {
                TransActiveMessage userMsg;
                //userMsg << m_job.getType().c_str();
                UINT selectedButton = userMsg.showMsgBox(IDS_UE_250013);
            }
        }
        catch (...)
        {
            TAUnicodeString action = TALoadString(IDS_SHOW_JOB_DETAIL);

            TransActiveMessage userMsg;
            userMsg << action;
            userMsg.showMsgBox(IDS_UE_250015);
        }
	    FUNCTION_EXIT;
    }

    
    void JobDialog::onSelchangeJobtype() 
    {
	    FUNCTION_ENTRY( "onSelchangeJobtype" );
        TAUnicodeString jobType;
        m_jobTypeCtrl.GetLBText(m_jobTypeCtrl.GetCurSel(), jobType.str());
        m_job.setType(jobType.toMultiByteStdString());
	    FUNCTION_EXIT;
    }


    void JobDialog::onChangeJobdescription() 
    {
	    FUNCTION_ENTRY( "onChangeJobdescription" );
        TAUnicodeString description;
        m_descriptionCtrl.GetWindowText(description.str());
        m_saveNewCtrl.EnableWindow(description.toMultiByteStdString().compare(m_job.getDescription().c_str()) == 0 ? FALSE : TRUE);
	    FUNCTION_EXIT;
    }


    void JobDialog::initialiseProfileCombo()
    {
	    FUNCTION_ENTRY( "initialiseProfileCombo" );

        // TODO: This method should be called when there is a change in session so that the
        // combo gets repopulated

        // delete previous data if it is previously populated
        m_profileCtrl.ResetContent();

        unsigned long existingProfileId = m_job.getProfileId();

        if (m_readOnly)
        {
            //ProfileMap profileMap = m_doc.getAllSchedulableProfiles();
			ProfileMap profileMap = m_jobManager->getAllSchedulableProfiles();
            IProfile* profile = profileMap[existingProfileId];
            try
            {
                if (profile != NULL)
                {
					TAUnicodeString tmpUniStr = TAUnicodeString::CreateFromMultitByteString(profile->getName().c_str());
                    m_profileCtrl.AddString(tmpUniStr.c_str());
                    m_profileCtrl.SetItemData(0, reinterpret_cast<DWORD>(profile));
                    m_profileCtrl.SetCurSel(0);
                }
            }
            catch (TA_Base_Core::DatabaseException& ex)
            {
                LOG_EXCEPTION_CATCH(SourceInfo, "DatabaseException", ex.what());
                TransActiveMessage userMsg;
                TAUnicodeString actionName = TALoadString(IDS_SHOW_PROFILE);
                userMsg << actionName;
                UINT selectedButton = userMsg.showMsgBox(IDS_UE_250016);
            }
            catch (...)
            {
                LOG_EXCEPTION_CATCH(SourceInfo, "Unknown exception", "Thrown while attempting to populate profile combo");
                TransActiveMessage userMsg;
                TAUnicodeString actionName = TALoadString(IDS_SHOW_PROFILE);
                userMsg << actionName;
                UINT selectedButton = userMsg.showMsgBox(IDS_UE_250015);
            }
        }
        else
        {
            bool isNew = m_job.isNew();
            //ProfileVector profiles = m_doc.getAllOperatorProfiles();
			ProfileVector profiles = m_jobManager->getAllOperatorProfiles();
            CString selectedName("");
            try
            {
                if (profiles.empty())
                {
                    TA_THROW(TA_Base_Core::DatabaseException("Rethrowing DatabaseException"));
                }

                std::vector<unsigned long> sessionProfiles;
                if (isNew)
                {
                    //sessionProfiles = m_doc.getSessionProfiles();
					sessionProfiles = m_jobManager->getSessionProfiles();
                }

                bool foundSelection = false;
                for(unsigned int i = 0; i < profiles.size(); ++i)
                {
                    try
                    {
						TAUnicodeString tmpUniStr = TAUnicodeString::CreateFromMultitByteString(profiles[i]->getName().c_str());
                        int pos = m_profileCtrl.AddString(tmpUniStr.c_str());
                        m_profileCtrl.SetItemData(pos, reinterpret_cast<DWORD>(profiles[i]));
                        unsigned long profileKey = profiles[i]->getKey();

                        // we just want to mark which item we're going to select by default
                        if (!foundSelection &&
                            ((!isNew && (existingProfileId == profileKey)) ||
                             (isNew && (std::find(sessionProfiles.begin(), sessionProfiles.end(), profileKey) != sessionProfiles.end()))))
                        {
                            selectedName = profiles[i]->getName().c_str();
                            foundSelection = true;
                        }
                    }
                    catch (TA_Base_Core::DataException& ex)
                    {
                        LOG_EXCEPTION_CATCH(SourceInfo, "DataException", ex.what());
                    }
                }
            }
            catch (TA_Base_Core::DatabaseException& ex)
            {
                LOG_EXCEPTION_CATCH(SourceInfo, "DatabaseException", ex.what());
                TransActiveMessage userMsg;
                TAUnicodeString actionName = TALoadString(IDS_SHOW_PROFILE);
                userMsg << actionName;
                UINT selectedButton = userMsg.showMsgBox(IDS_UE_250016);
            }
            catch (...)
            {
                LOG_EXCEPTION_CATCH(SourceInfo, "Unknown exception", "Thrown while attempting to populate profile combo");
            }
            // if the job already has a profile associated with it i.e. it is an existing job
            // then set the profile to that one  Otherwise just select the first item.
            int sel = m_profileCtrl.FindStringExact(-1,selectedName);
            m_profileCtrl.SetCurSel(sel == CB_ERR ? 0 : sel);
        }
        onSelchangeProfile();
	    FUNCTION_EXIT;
    }


    void JobDialog::onSelchangeProfile() 
    {
	    FUNCTION_ENTRY( "onSelchangeProfile" );
	    // see whether the profile is associated with locations.  Populate the location combo if necessary.
        int index = m_profileCtrl.GetCurSel();
        DWORD data = (index != CB_ERR) ? m_profileCtrl.GetItemData(index) : 0;
        IProfile* profile = (data != CB_ERR) ? reinterpret_cast<IProfile*>(data) : NULL;
        try
        {
            if ((profile == NULL) || !profile->requiresLocationToBeSelected())
            {
                LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "Either profile is not specified or there is no location associated with the selected profile");
                // disable the location combo if no profile is selected or if the profile does not
                // need a location to be selected
                m_locationCtrl.SetCurSel(CB_ERR);
                m_locationCtrl.EnableWindow(FALSE);
                return;
            }
        
            // clear the location control and rebuild it
	        m_locationCtrl.ResetContent();
            std::vector<unsigned long> locations = profile->getAssociatedLocationKeys();
            for (unsigned int i = 0; i < locations.size(); i++)
            {
				TAUnicodeString tmpUniStr = TAUnicodeString::CreateFromMultitByteString(m_locationMap[locations[i]]->getDisplayName().c_str());
                int pos = m_locationCtrl.AddString(tmpUniStr.c_str());
                m_locationCtrl.SetItemData(pos, locations[i]); // use the location key as the data
            }

            // use the session location if it is new, otherwise, use the one that is associated with the job
            //unsigned long locationId = (m_job.isNew()) ? m_doc.getSessionLocation() : m_job.getLocationId();

			unsigned long locationId = (m_job.isNew()) ? m_jobManager->getSessionLocation() : m_job.getLocationId();

            for (int j = 0; j < m_locationCtrl.GetCount(); j++)
            {
                if (locationId == m_locationCtrl.GetItemData(j))
                {
                    m_locationCtrl.SetCurSel(j);
                    break;
                }
            }
            m_locationCtrl.EnableWindow(TRUE);
            return;
        }
        catch (TA_Base_Core::DataException& ex)
        {
            LOG_EXCEPTION_CATCH(SourceInfo, "DataException", ex.what());
        }
        catch (TA_Base_Core::DatabaseException& ex)
        {
            LOG_EXCEPTION_CATCH(SourceInfo, "DatabaseException", ex.what());
        }

        TransActiveMessage userMsg;
        TAUnicodeString actionName = TALoadString(IDS_SHOW_LOCATION);
        userMsg << actionName;
        UINT selectedButton = userMsg.showMsgBox(IDS_UE_250016);

	    FUNCTION_EXIT;
    }

	void JobDialog::OnDestroy()
	{
		//detach from the presentation model
		if (m_pModel != NULL)
		{
			m_pModel->detachView(this);
		}
		CDialog::OnDestroy();
	}
	void JobDialog::OnSysCommand(UINT nID, LPARAM lParam)
	{
		// In WM_SYSCOMMAND messages, the four low-order bits of the nID parameter 
		// are used internally by Windows CE. When an application tests the value of 
		// nID, it must combine the value 0xFFF0 with the nID value by using the 
		// bitwise-AND operator to obtain the correct result.
		if ((nID & APPCOMMAND) == IDM_ABOUTBOX)
		{
			TA_Base_Bus::HelpLauncher::getInstance().displayAboutBox();
		}
		else
		{
			CDialog::OnSysCommand(nID, lParam);
		}
	}

	void JobDialog::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI) 
	{
		CDialog::OnGetMinMaxInfo(lpMMI);

		if (m_pModel == NULL)
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugWarn, "Main PModel has not been initialized, ignore GetMinMaxInfo message");
			return; 
		}
		LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "--------OnGetMinMaxInfo");


		// Maximum size is restricted by the size of the available workstation screen (i.e. the parts not
		// taken up by banner and/or control station bar)
		// To work out that space, need to know which screen the current window is in.
		int currentScreen = -1;
		RECT rcNormalPosition;

		m_resizeViewHelper->getViewPosition(rcNormalPosition, currentScreen);
		m_resizeViewHelper->msgProcessMinMaxInfo(lpMMI, currentScreen, rcNormalPosition);

	}


	void JobDialog::OnWindowPosChanging(WINDOWPOS FAR* lpwndpos) 
	{
		CDialog::OnWindowPosChanging(lpwndpos);
		if (m_pModel == NULL)
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugWarn, "Main PModel has not been initialized, ignore WindowPosChanging message");
			return; 
		}
		LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "----------OnWindowPosChanging");


		m_resizeViewHelper->msgProcessWindowPosChanging(lpwndpos);
	}


	LRESULT JobDialog::OnSetWindowPosition(WPARAM wParam, LPARAM lParam)
	{
		if (m_pModel == NULL)
		{
			LOG_GENERIC(SourceInfo, DebugUtil::DebugWarn, "Main PModel has not been initialized, ignore SetWindowPosition message");
			return true; 
		}
		LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "--------OnSetWindowPosition");
		return TA_Base_Bus::MFCControlHelper::setWindowPosition(this, wParam, lParam);
	}

	void JobDialog::initUIControl()
	{
		m_pJobDialogModel->initRightLib();

		SetWindowText(m_title.c_str());

		TAUnicodeString uniStr;
		for (unsigned int i = 0; i < m_jobTypes.size(); i++)
		{
			// TD 8692: Alarm generator job type has been added for archiving but we don't want
			// operators to be able to configure more alarm generation jobs, only editing the
			// archiving one, so don't allow this type for configuring a new job.
			const std::string ALARM_GENERATOR_TYPE("AlarmGeneratorJob");
			if(m_jobTypes[i] == ALARM_GENERATOR_TYPE && m_job.getType() != ALARM_GENERATOR_TYPE)
			{
				continue;
			}

			uniStr = TAUnicodeString::CreateFromMultitByteString(m_jobTypes[i].c_str());
			m_jobTypeCtrl.AddString(uniStr.c_str());
		}

		uniStr = TAUnicodeString::CreateFromMultitByteString(m_job.getType().c_str());
		if (m_jobTypeCtrl.SelectString(0, uniStr.c_str()) == CB_ERR)
		{
			// if the job is new, then just select the first job type by default
			m_jobTypeCtrl.SetCurSel(0);
			onSelchangeJobtype();
		}
		TAUnicodeString jobName = TALoadString(IDS_UNDEFINED_JOB);
		TA_VERIFY(!jobName.IsEmpty(), "Undefined Job is not in string table");
		TAUnicodeString jobDescription = TAUnicodeString::CreateFromMultitByteString(m_job.getDescription().c_str());

		try
		{
			IJobDetailsPlugin* plugin = m_componentLibraryAccessor.getComponent(m_job.getType());
			if (plugin != NULL)
			{
				IJobDetailsPlugin::JobData jobData;
				jobData.description = jobDescription.c_str();
				jobData.parameters = m_job.getParameters();

				plugin->setData(jobData);
				jobData = plugin->getUpdatedData(true); // get updated name and description

				jobName = jobData.name;
				std::string strJobName = jobName.toMultiByteStdString();
				jobName = TAUnicodeString::CreateFromMultitByteString(strJobName.c_str());

				// Don't reset the description on existing jobs.
				if (m_job.isNew())
				{
					jobDescription = jobData.description;
					m_job.setDescription(jobDescription.toMultiByteStdString());
					//m_jobDetailsButton.EnableWindow(FALSE);
				}
			}
			else
			{
				TransActiveMessage userMsg;
				//userMsg << m_job.getType().c_str();
				UINT selectedButton = userMsg.showMsgBox(IDS_UE_250013);
			}
		}
		catch (...)
		{
			// Just ignore.  We don't want the error message to pop out to inform us of
			// any error with the plugin since the operator might not want to change the
			// job details.  Operator will get an error when he/she tries to click on Select button anyway
		}

		m_descriptionCtrl.SetLimitText(MAX_DESCRIPTION_LENGTH);
		m_descriptionCtrl.SetWindowText(jobDescription.GetString());
		m_nameCtrl.SetWindowText(jobName.GetString());
		m_jobDetailsButton.EnableWindow(m_pJobDialogModel->isJobDetailsButtonEnabled() && m_nameCtrl.GetWindowTextLength () != 0);

		// set the FooButton types 
		initialiseRadio(m_onceoffRadio, RECURRENCE_PATTERN_GROUP);
		initialiseRadio(m_dailyRadio, RECURRENCE_PATTERN_GROUP);
		initialiseRadio(m_weeklyRadio, RECURRENCE_PATTERN_GROUP);
		initialiseRadio(m_monthlyRadio, RECURRENCE_PATTERN_GROUP);
		initialiseRadio(m_yearlyRadio, RECURRENCE_PATTERN_GROUP);

		initialiseRadio(m_noEndDateRadio, RANGE_OF_RECURRENCE_GROUP);
		initialiseRadio(m_endAfterDateRadio, RANGE_OF_RECURRENCE_GROUP);
		initialiseRadio(m_endByDateRadio, RANGE_OF_RECURRENCE_GROUP);

		// initialise and format control items in the Range of Recurrence panel
		initialiseEditAndSpinner(IDC_JD_EDIT_END_AFTER, IDC_JD_SPINNER_END_AFTER, MIN_RECURRENCE, MAX_RECURRENCE);

		COleDateTime theTime = COleDateTime::GetCurrentTime();
		// Initialise with today's date
		initialiseDateTimeCtrl(IDC_JD_DATETIMEPICKER_END_BY, &theTime, &theTime);

		Schedule& schedule = m_job.getSchedule();
		// initialise second to 0 as it is not going to be shown anyway
		theTime.SetTime(schedule.getHour(), schedule.getMinute(), 0);
		switch (schedule.getType())
		{
		case OnceOff:
			m_onceoffRadio.check(true);
			if (m_job.isNew())
			{
				initialiseRadioNoRecurrence();
			}
			else
			{
				// just set the date, set the time to 00:00:00 since we're not using it anyway
				COleDateTime theDate(schedule.getYear(), schedule.getMonthOfYear(), schedule.getDayOfMonth(), 0, 0, 0);
				initialiseRadioNoRecurrence(&theTime, &theDate);
			}
			break;
		case Daily:
			m_dailyRadio.check(true);
			initialiseRadioDailyRecurrence(&theTime);
			break;
		case Weekly:
			m_weeklyRadio.check(true);
			initialiseRadioWeeklyRecurrence(&theTime, schedule.getDaysOfWeek());
			break;
		case Monthly:
			m_monthlyRadio.check(true);
			initialiseRadioMonthlyRecurrence(&theTime, schedule.getDayOfMonth());
			break;
		case Yearly:
			m_yearlyRadio.check(true);
			initialiseRadioYearlyRecurrence(&theTime, schedule.getDayOfMonth(), schedule.getMonthOfYear());
			break;
		}

		COleDateTime* endDate = schedule.getEndDate();
		if (endDate != NULL)
		{
			m_endByCtrl.SetTime(*endDate);
			delete endDate;
			m_endByDateRadio.check(true);
			onEndBy();
		}
		else
		{
			m_noEndDateRadio.check(true);
			onNoEndDate();
		}

		// get the session id
		std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);

		// Load the available profiles
		initialiseProfileCombo();

		if (m_readOnly)
		{
			disableAllControls();

			// remove the save and save new buttons, reposition the close button and change its label to "OK"
			m_saveNewCtrl.ShowWindow(SW_HIDE);
			m_saveCtrl.ShowWindow(SW_HIDE);
			CRect dialogRect, buttonRect;
			GetWindowRect(&dialogRect);
			m_closeCtrl.GetWindowRect(&buttonRect);
			ScreenToClient(&buttonRect);
			int width = buttonRect.Width();
			buttonRect.left = (dialogRect.Width() - width) / 2;
			buttonRect.right = buttonRect.left + width;
			m_closeCtrl.MoveWindow(&buttonRect);
			TAUnicodeString buttonName = TALoadString(IDS_VIEW_DIALOG_OK_BUTTON_NAME);
			TA_VERIFY(!buttonName.IsEmpty(), "String does not exist in string table");
			m_closeCtrl.SetWindowText(buttonName.c_str());
		}
		else
		{
			// otherwise, if it is editing an existing job, don't enable the save new button
			if (!m_job.isNew())
			{
				m_saveNewCtrl.EnableWindow(FALSE);
			}

			// TD 8692: An alarm can be scheduled to be submitted periodically for archiving. Allow the
			// timing to be configured, but nothing else, hence we don't need a plugin. Disable the 
			// Settings button if there is no associated plugin.
			if(!m_hasAssociatedPlugin)
			{
				m_selectJobButton.EnableWindow(false);
			}
		}

		// Add "About..." menu item to system menu.

		CMenu* pSysMenu = GetSystemMenu(FALSE);
		if (pSysMenu != NULL)
		{
			std::string strAboutMenu = "About ";

			std::string tmpUniStr = TA_Base_Core::RunParams::getInstance().get(RPARAM_APPNAME).c_str();
			strAboutMenu += tmpUniStr;

			strAboutMenu += "...";
			pSysMenu->AppendMenu(MF_SEPARATOR);

			TAUnicodeString strUni = TAUnicodeString::CreateFromMultitByteString(strAboutMenu.c_str());
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strUni.c_str());
		}
	}

	std::string JobDialog::getMyPModelName()
	{
		return PMODEL_JOB_DLG;
	}

	void JobDialog::setupPModel(void)
	{
        m_pJobDialogModel = dynamic_cast<TA_Base_App::JobDialogModel*> (m_pModel);
	}

	void JobDialog::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args)
	{

	}

	bool JobDialog::saveJob(const Job& job)
	{
		FUNCTION_ENTRY( "saveJob" );
		TA_ASSERT(m_jobManager != NULL, "JobManager is NULL, cannot continue");
		try
		{
			m_jobManager->saveJob(job);
			return true;
		}
		catch (const TA_Base_Core::SchedulingManagerException& ex)
		{
			TransActiveMessage userMsg;
			unsigned long errorId;
			TAUnicodeString action, agent, itemName;
			action = TALoadString(IDS_SAVE_JOB);
			TA_VERIFY(!action.IsEmpty(), "String does not exist in string table");
			if (ex.getFailType() != TA_Base_Core::SchedulingManagerException::ACCESS_DENIED)
			{
				userMsg << action;
			}

			switch(ex.getFailType())
			{
			case TA_Base_Core::SchedulingManagerException::SCHEDULING_AGENT_FAILURE: // Couldn't contact the scheduling agent
				agent = TALoadString(IDS_SCHEDULING_AGENT);
				TA_VERIFY(!agent.IsEmpty(), "String does not exist in string table");
				userMsg << agent;
				errorId = IDS_UE_030080;
				break;
			case TA_Base_Core::SchedulingManagerException::RIGHTS_AGENT_FAILURE:   // Couldn't contact the rights agent
                agent = TALoadString(IDS_RIGHTS_AGENT);
				TA_VERIFY(!agent.IsEmpty(), "String does not exist in string table");
				userMsg << agent;
				errorId = IDS_UE_030080;
				break;
			case TA_Base_Core::SchedulingManagerException::UNKNOWN_EXCEPTION:      // Unknown exception is thrown
				errorId = IDS_UE_250015;
				break;
			case TA_Base_Core::SchedulingManagerException::ACCESS_DENIED:          // Session has insufficient rights to perform action
				{
					errorId = IDS_UE_250017;
					//std::string userName;
					//std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
					//TA_Base_Bus::AuthenticationLibrary authLibrary(true);
					//TA_Base_Bus::SessionInfo sessionInfo = authLibrary.getSessionInfo(sessionId, sessionId);
					//std::auto_ptr<TA_Base_Core::IOperator> iOperator(TA_Base_Core::OperatorAccessFactory::getInstance().getOperator(sessionInfo.UserId, false));
					//userName = iOperator->getName();
					//userMsg << userName;
					//userMsg << action;
					break;
				}
			case TA_Base_Core::SchedulingManagerException::DATABASE_ERROR:         // Unable to use the database or error in database
				errorId = IDS_UE_250016;
				break;
			case TA_Base_Core::SchedulingManagerException::JOB_IN_PAST:            // Schedule is set to run in the past
				errorId = IDS_UE_250018;
				break;
			case TA_Base_Core::SchedulingManagerException::NO_JOB_PARAMETER:       // No job specified details are specified
				itemName = TALoadString(IDS_JOB_SETTING);
				TA_VERIFY(!itemName.IsEmpty(), "String does not exist in string table");
				userMsg << itemName;
				errorId = IDS_UE_250014;
				break;
			case TA_Base_Core::SchedulingManagerException::NO_DAY_OF_WEEK:         // Job contain bad dates etc, output the error returned by agent
				errorId = IDS_UE_250019;
				break;
			case TA_Base_Core::SchedulingManagerException::BAD_SCHEDULE:           // Schedule is invalid, just assert as the job dialog supposed to stop it
				TA_ASSERT(false, "Job dialog supposed to prevent bad schedules");
			case TA_Base_Core::SchedulingManagerException::INVALID_JOB_PARAMETER:  // Job parameters are not configured properly
				errorId = IDS_UE_250020;
				break;
			case TA_Base_Core::SchedulingManagerException::MISSING_JOB_PARAMETER:  // Job parameters are missing
				errorId = IDS_UE_250021;
				break;
			case TA_Base_Core::SchedulingManagerException::JOB_NOT_EXIST:          // Job does not exist
				errorId = IDS_UE_250022;
				break;
			case TA_Base_Core::SchedulingManagerException::TOO_MANY_JOBS:          // Too many jobs are scheduled to start on the same time
				errorId = IDS_UE_250023;
				break;
			default:
				TA_ASSERT(false, "Unknown exception type");
			}

			userMsg.showMsgBox(errorId);
			return false;
		}
		FUNCTION_EXIT;
	}

	bool JobDialog::saveNewJob(Job& job)
	{
		FUNCTION_ENTRY( "saveNewJob" );
		TA_ASSERT(m_jobManager != NULL, "JobManager is NULL, cannot continue");
		try
		{
			m_jobManager->saveNewJob(job);
			return true;
		}
		catch (const TA_Base_Core::SchedulingManagerException& ex)
		{
			TransActiveMessage userMsg;
			unsigned long errorId;
			TAUnicodeString action, agent, itemName;
			action = TALoadString(IDS_SAVE_NEW_JOB);
			TA_VERIFY(!action.IsEmpty(), "String does not exist in string table");
			if (ex.getFailType() != TA_Base_Core::SchedulingManagerException::ACCESS_DENIED)
			{
				userMsg << action;
			}

			switch(ex.getFailType())
			{
			case TA_Base_Core::SchedulingManagerException::SCHEDULING_AGENT_FAILURE: // Couldn't contact the scheduling agent
				agent = TALoadString(IDS_SCHEDULING_AGENT);
				TA_VERIFY(!agent.IsEmpty(), "String does not exist in string table");
				userMsg << agent;
				errorId = IDS_UE_030080;
				break;
			case TA_Base_Core::SchedulingManagerException::RIGHTS_AGENT_FAILURE:   // Couldn't contact the rights agent
				agent = TALoadString(IDS_RIGHTS_AGENT);
				TA_VERIFY(!agent.IsEmpty(), "String does not exist in string table");
				userMsg << agent;
				errorId = IDS_UE_030080;
				break;
			case TA_Base_Core::SchedulingManagerException::UNKNOWN_EXCEPTION:      // Unknown exception is thrown
				errorId = IDS_UE_250015;
				break;
			case TA_Base_Core::SchedulingManagerException::ACCESS_DENIED:          // Session has insufficient rights to perform action
				{
					errorId = IDS_UE_250017;
					//std::string userName;
					//std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
					//TA_Base_Bus::AuthenticationLibrary authLibrary(true);
					//TA_Base_Bus::SessionInfo sessionInfo = authLibrary.getSessionInfo(sessionId, sessionId);
					//std::auto_ptr<TA_Base_Core::IOperator> iOperator(TA_Base_Core::OperatorAccessFactory::getInstance().getOperator(sessionInfo.UserId, false));
					//userName = iOperator->getName();
					//userMsg << userName;
					//userMsg << action;
					break;
				}
			case TA_Base_Core::SchedulingManagerException::DATABASE_ERROR:         // Unable to use the database or error in database
				errorId = IDS_UE_250016;
				break;
			case TA_Base_Core::SchedulingManagerException::JOB_IN_PAST:            // Schedule is set to run in the past
				errorId = IDS_UE_250018;
				break;
			case TA_Base_Core::SchedulingManagerException::NO_JOB_PARAMETER:       // No job specified details are specified
				itemName = TALoadString(IDS_JOB_SETTING);
				TA_VERIFY(!itemName.IsEmpty(), "String does not exist in string table");
				userMsg << itemName;
				errorId = IDS_UE_250014;
				break;
			case TA_Base_Core::SchedulingManagerException::NO_DAY_OF_WEEK:         // Job contain bad dates etc, output the error returned by agent
				errorId = IDS_UE_250019;
				break;
			case TA_Base_Core::SchedulingManagerException::BAD_SCHEDULE:           // Schedule is invalid, just assert as the job dialog supposed to stop it
				TA_ASSERT(false, "Job dialog supposed to prevent bad schedules");
			case TA_Base_Core::SchedulingManagerException::INVALID_JOB_PARAMETER:  // Job parameters are not configured properly
				errorId = IDS_UE_250020;
				break;
			case TA_Base_Core::SchedulingManagerException::MISSING_JOB_PARAMETER:  // Job parameters are missing
				errorId = IDS_UE_250021;
				break;
			case TA_Base_Core::SchedulingManagerException::JOB_NOT_EXIST:          // Job does not exist
				errorId = IDS_UE_250022;
				break;
			case TA_Base_Core::SchedulingManagerException::TOO_MANY_JOBS:          // Too many jobs are scheduled to start on the same time
				errorId = IDS_UE_250023;
				break;
			case TA_Base_Core::SchedulingManagerException::JOB_EXIST:              // Job is identical to original
				errorId = IDS_UE_250011;
				break;
			default:
				TA_ASSERT(false, "Unknown exception type");
			}
			userMsg.showMsgBox(errorId);
			return false;
		}
		FUNCTION_EXIT;
	}

	LRESULT JobDialog::OnSetLanguage(WPARAM wParam, LPARAM lParam)
	{
		UINT idd = (LONG)(LONG_PTR)this->m_lpszTemplateName;
		TATranslateDialog( idd, this );
		return TRUE;
	}
};



