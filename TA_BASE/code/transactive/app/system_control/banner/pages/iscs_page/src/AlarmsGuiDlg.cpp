/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/banner/pages/iscs_page/src/AlarmsGuiDlg.cpp $
 * @author:  Andy Siow
 * @version: $Revision: #2 $
 *
 * Last modification: $DateTime: 2016/12/14 14:57:33 $
 * Last modified by:  $Author: Noel $
 * 
 * AlarmsGuiDlg.cpp : implementation file
 */

#include "app/system_control/banner/pages/iscs_page/src/AlarmsGuiDlg.h"
#include "app/system_control/banner/pages/iscs_page/src/Globals.h"
#include "app/system_control/banner/pages/iscs_page/src/IscsBannerPage_rc.h"
#include "app/system_control/banner/pages/iscs_page/pmodel/src/AlarmsGuiDlgPModel.h"
#include "app/system_control/banner/pages/iscs_page/pmodel/src/StringEventArgs.h"
#include "app\system_control\banner\pages\iscs_page\pmodel\src\PrefilterChangeEventArgs.h"

#include "bus/user_settings/src/SettingsMgr.h"

#include "bus/generic_gui_view/src/TransActiveMessage.h"
#include "bus/alarm/alarm_common/src/AlarmConstants.h"
#include "bus/alarm/alarm_list_control/src/AlarmActionFactory.h"
#include "bus/alarm/alarm_list_control/pmodel/src/Filter.h"

#include "bus/alarm/alarm_list_control/src/actions/ActionDisplayComment.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionSeparator.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionAcknowledgeAlarm.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionRelatedSchematic.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionCloseAndAckAlarm.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionIncidentLogSheet.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionJobRequest.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionPostponeDSS.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionRelatedPlanInstance.h" //TD15736
#include "bus/alarm/alarm_list_control/src/actions/ActionRelatedDSS.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionRelatedEvents.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionRelatedInspector.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionRelatedTrend.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionAcknowledgeAllAlarms.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionRelatedAvalancheEvents.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionSuppressAlarm.h"

#include "bus/alarm/alarm_list_control/src/columns/ColumnAck.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnAckBy.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnAsset.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnComment.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnAvalanche.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnDecisionSupport.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnDescription.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnAlarmOperationMode.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnIdentifiers.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnMms.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnSourceTimestamp.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnSeverity.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnState.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnValue.h"

#include "bus/banner_framework/src/BannerFrameworkDialog.h"
#include "bus/banner_framework/src/constants.h"
#include "bus/generic_gui_entity/src/GenericGUI.h"
#include "bus/resources/resource.h"

#include "core/data_access_interface/src/IProfile.h"
#include "core/data_access_interface/src/ProfileAccessFactory.h"

#include "core/exceptions/src/ApplicationException.h"
#include "core/exceptions/src/AssertException.h"
#include "core/exceptions/src/AuthenticationAgentException.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"

using TA_Base_Core::DebugUtil;
using TA_Base_Core::RunParams;
using TA_Base_Bus::Sort;

namespace TA_Base_App
{
	//TD15156++
	const std::string CAlarmsGuiDlg::RPARAM_USE_ALARM_ICON = "UseAlarmIcon";
	//++TD15156
   
    /////////////////////////////////////////////////////////////////////////////
    // CAlarmsGuiDlg dialog

    CAlarmsGuiDlg::CAlarmsGuiDlg(const std::string& buttonName, int xPos, int yPos, int width, int height, TA_Base_Bus::BannerFrameworkDialog* parentDialog)
		: BannerPage(buttonName, ViewType_IscsBannerPange, xPos, yPos, width, height, parentDialog)
		, m_silenceButton( CAlarmsGuiDlgBusLogic::ALARM_CATEGORY )
    {
        //{{AFX_DATA_INIT(CAlarmsGuiDlg)
        //m_numNotAckedString = "0";
	    //m_numTotalString = "0";
	    //}}AFX_DATA_INIT
        // Note that LoadIcon does not require a subsequent DestroyIcon in Win32

		ui.setupUi(this);
		this->setParent(parentDialog);
		ui.gridLayout->addWidget(&m_silenceButton);

		m_silenceButton.setGeometry(ui.silentBtnHolder->geometry());
		m_silenceButton.setParent(ui.silentBtnHolder);
		m_silenceButton.setVisible(true);

        //m_numAlarmsNotAcked = 0;
	    //m_numAlarmsTotal = 0;
	    m_exceededMaxDisplayAlarm = false;

		setupPModel();
    }


    CAlarmsGuiDlg::~CAlarmsGuiDlg()
    {
        
    }


    void CAlarmsGuiDlg::onButtonDoubleClick()
    {
		// Launch AlarmManager
		try
		{
			m_pRealModel->launchAlarmManager(getXPosition());
		}
		catch(TA_Base_Core::ApplicationException&)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "ApplicationException", "Could not launch Alarm Manager viewer");

			QString applicationName = tr("Alarm Manager");
			TA_Base_Bus::TransActiveMessage userMsg;
			userMsg << applicationName.toStdString();
			UINT selectedButton = userMsg.showMsgBox(IDS_UE_010002);
		}
		catch(...)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "GeneralException", "General Exception: Could not launch Alarm Manager viewer");

			QString applicationName = tr("Alarm Manager");
			TA_Base_Bus::TransActiveMessage userMsg;
			userMsg << applicationName.toStdString();
			UINT selectedButton = userMsg.showMsgBox(IDS_UE_010002);
		}
    }


    void CAlarmsGuiDlg::addBannerPage()
    {
		m_alarmList = new TA_Base_Bus::AlarmListCtrl(false, false);
		m_alarmList->setParent(this);
		m_alarmList->setVisible(true);
		ui.alarmListLayout->addWidget(m_alarmList);

        TA_Base_Bus::BannerIcon bannerIcon;

	    if (TA_Base_Core::RunParams::getInstance().isSet(RPARAM_USE_ALARM_ICON.c_str()))
	    {
			bannerIcon.onIcon = BANNER_CONST::BTN_ICO_ALARMKRTC_ON;
			bannerIcon.offIcon = BANNER_CONST::BTN_ICO_ALARMKRTC_OFF;
			bannerIcon.onDownIcon = BANNER_CONST::BTN_ICO_ALARMKRTC_ON_DOWN;
			bannerIcon.offDownIcon = BANNER_CONST::BTN_ICO_ALARMKRTC_OFF_DOWN;
	    }
	    else
	    {
			bannerIcon.onIcon = BANNER_CONST::BTN_ICO_ALARM_ON;
			bannerIcon.offIcon = BANNER_CONST::BTN_ICO_ALARM_OFF;
			bannerIcon.onDownIcon = BANNER_CONST::BTN_ICO_ALARM_ON_DOWN;
			bannerIcon.offDownIcon = BANNER_CONST::BTN_ICO_ALARM_OFF_DOWN;
	    }

		initUIControl();
		getParentDialog()->AddBannerPage(ViewType_IscsBannerPange, getButtonName(), bannerIcon, this);
    }

    const int CAlarmsGuiDlg::ALARM_SOUND_TIMER = 1;


  //  BEGIN_MESSAGE_MAP(CAlarmsGuiDlg, TStackedPage)
  //      //{{AFX_MSG_MAP(CAlarmsGuiDlg)
	 //   ON_WM_SYSCOMMAND()
  //      ON_WM_PAINT()
  //      ON_WM_QUERYDRAGICON()
  //      ON_WM_SIZE()
	 //   ON_WM_GETMINMAXINFO()
  //      ON_WM_TIMER()
  //      ON_WM_CLOSE()
	 //   ON_WM_COMPAREITEM()
	 //   ON_WM_DESTROY()
	 //   ON_BN_CLICKED(IDC_SILENCE, OnSilence)
  //      ON_COMMAND(IDOK, OnOK)
	 //   ON_WM_COMPAREITEM() 
  //      ON_MESSAGE(WM_ON_SYSTEM_DEGRADED, OnSystemInDegradedMode)
  //      ON_MESSAGE(WM_ON_SYSTEM_RECOVERED, OnSystemInNormalMode)
		//ON_MESSAGE(WM_ON_SESSION_CHANGED, OnSessionChanged)
		//ON_MESSAGE(WM_ON_NOTIFY_UNMUTE, OnNotifyUnMute)
	 //   //}}AFX_MSG_MAP
	 //   ON_MESSAGE(WM_UPDATE_ALARMS_MSG, OnUpdateAlarms)
  //  END_MESSAGE_MAP()

    /////////////////////////////////////////////////////////////////////////////
    // CAlarmsGuiDlg message handlers


	void CAlarmsGuiDlg::OnUpdateAlarmCount(int totalUnAcked, int totalAlarms)
    {
		if (m_exceededMaxDisplayAlarm)
		{
			TAUtfString str = TAUtfString::number(totalAlarms) + TAUtfString("+");
			ui.txtTotalAlarms->setText(str.q_str());
		}
		else
			ui.txtTotalAlarms->setText(TAUtfString::number(totalAlarms).q_str());

		ui.txtNotAcked->setText(TAUtfString::number(totalUnAcked).q_str());
    }

  //  void CAlarmsGuiDlg::OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI)
  //  {	
	 //   // Set a minimum size for the Alarm Viewer window
	 //   lpMMI->ptMinTrackSize.x = 500;
	 //   lpMMI->ptMinTrackSize.y = 300;
	 //   
	 //   TStackedPage::OnGetMinMaxInfo(lpMMI);
  //  }

  //  void CAlarmsGuiDlg::OnClose()
  //  {
  //      //
  //      // Start terminiating the application.  
  //      //
  //      LOG_GENERIC( SourceInfo, DebugUtil::DebugDebug, "OnClose()");
		//// Todo: Add getSettingsMgr 
		//
	 //   //CPD Save the user settings
		//TA_Base_Bus::SettingsMgr*ptrSettingsMgr = m_pRealModel->getSettingsMgr();
	 //   if(ptrSettingsMgr != 0)
	 //   {
		//    try
		//    {
		//	    // This method will save the placement of windowPtr (retrieved via CWnd::GetWindowPlacement()) 
		//	    //to the database. See RestoreWindowPlacement() for why you might want to do this.
		//	    //Will throw UserSettings::DataBaseEx& if the write to the database fails.
		//	    //Throws UserSettings::AssertEx& if windowPtr or the handle to windowPtr is NULL
		//	    //Note that if you call this on more than one CWnd only the last call will be saved. In other
		//	    //words, this method is only useful for saving the placement of only one window per application
		//	    WINDOWPLACEMENT lpwndpl;
		//		GetWindowPlacement(&lpwndpl);
		//		ptrSettingsMgr->saveWindowPlacementToDB(lpwndpl);

		//    }
		//    catch(TA_Base_Core::AssertException& ex) //use the default if the SettingsMgr can't restore the placement
		//    {
		//	    LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "SettingsMgr could not save Window placement because: %s",
  //                  ex.what() );
		//    }
		//    catch(TA_Base_Core::DatabaseException& ex) //use the default if the SettingsMgr can't restore the placement
		//    {
		//	    LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "SettingsMgr could not save Window placement because: %s",
  //                  ex.what() );
  //          }
	 //   }
  //  }


 /*   BOOL CAlarmsGuiDlg::PreTranslateMessage(MSG* pMsg) 
    {
         if (WM_KEYFIRST <= pMsg->message && pMsg->message <= WM_KEYLAST)
         {
             HACCEL hAccel = m_hAccel;
             if (hAccel && ::TranslateAccelerator(AfxGetMainWnd()->m_hWnd, hAccel, pMsg))
             {
                 return TRUE;
             }
         }

	     return TStackedPage::PreTranslateMessage(pMsg);
    }*/


    void CAlarmsGuiDlg::OnSilence() 
    {
        m_silenceButton.toggleState();
    }

    std::vector<TA_Base_Bus::IAlarmAction*> CAlarmsGuiDlg::initListControlActions()
    {
	    FUNCTION_ENTRY("initListControlActions");

        std::vector<TA_Base_Bus::IAlarmAction*> actions;

	    // Add actions available from the context menu.
	    // TD #7550
	    // Context menu changed to match the alarm list control in the Alarm Manager
        actions.push_back( new TA_Base_Bus::ActionAcknowledgeAlarm() );
	    //actions.push_back( new TA_Base_Bus::ActionDisplayComment() );
        actions.push_back( new TA_Base_Bus::ActionSeparator() );
        actions.push_back( new TA_Base_Bus::ActionRelatedSchematic() );
        actions.push_back( new TA_Base_Bus::ActionRelatedInspector() );
        actions.push_back( new TA_Base_Bus::ActionRelatedEvents() );
        actions.push_back( new TA_Base_Bus::ActionSeparator() );
        actions.push_back( new TA_Base_Bus::ActionRelatedDSS() );
	    actions.push_back( new TA_Base_Bus::ActionRelatedPlanInstance() ); 	//TD15736
        actions.push_back( new TA_Base_Bus::ActionPostponeDSS() );
        actions.push_back( new TA_Base_Bus::ActionSeparator() );
        actions.push_back( new TA_Base_Bus::ActionJobRequest() );
        //actions.push_back( new TA_Base_Bus::ActionRelatedTrend() );
        actions.push_back( new TA_Base_Bus::ActionRelatedAvalancheEvents() );
        actions.push_back( new TA_Base_Bus::ActionIncidentLogSheet() );
        //actions.push_back( new TA_Base_Bus::ActionSeparator() );
        //actions.push_back( new TA_Base_Bus::ActionCloseAndAckAlarm() );
	   // actions.push_back( new TA_Base_Bus::ActionSuppressAlarm() );

        FUNCTION_EXIT;
	    return actions;
    }


    void CAlarmsGuiDlg::initListControlColumns()
    {
	    FUNCTION_ENTRY("initListControlColumns");

	    TA_Base_Bus::Sort descSort;
	    TA_Base_Bus::Sort ascSort;

	    if( RunParams::getInstance().isSet(RPARAM_KRTCALARMSORTING) )
	    {
		    m_alarmColumns.push_back(new TA_Base_Bus::ColumnState( 1, true, SORT_BY_DEFAULT, NULL));
	    }
	   
	   else
	    {
		    /*// Add disabled state and ack columns so that default sort will work.
		    ascSort.addSort(Sort::SORT_ACK_NOT_ACK, Sort::SORT_ASCENDING); //TD15689
		    ascSort.addSort(Sort::SORT_OPEN_CLOSE,  Sort::SORT_ASCENDING); //TD15689
		    ascSort.addSort(Sort::SORT_SEVERITY,    Sort::SORT_ASCENDING);
		    ascSort.addSort(Sort::SORT_TIME,        Sort::SORT_DESCENDING);*/
		    m_alarmColumns.push_back(new TA_Base_Bus::ColumnState(1, false,  SORT_BY_DEFAULT, NULL));
	    }
		
  	    // TD11777 / TD3354: This col is added to list but hidden from user to remove the black line affecting 1st col 
	    m_alarmColumns.push_back(new TA_Base_Bus::ColumnAck(30, false, SORT_BY_ACK, NULL));

	    // Add the rest of the columns.
	    //m_alarmColumns.push_back(new TA_Base_Bus::ColumnSourceTimestamp(130, true));
	    unsigned long ulTimestampWidth = TA_Base_Bus::AlarmConstants::DEFAULT_TIMESTAMPCOLWIDTH;
	    if (RunParams::getInstance().isSet(TA_Base_Bus::AlarmConstants::RPARAM_TIMESTAMPCOLWIDTH.c_str()))
	    {
		    ulTimestampWidth = strtoul(RunParams::getInstance().get(TA_Base_Bus::AlarmConstants::RPARAM_TIMESTAMPCOLWIDTH.c_str()).c_str(), NULL, 0);
	    }

	    /* if( RunParams::getInstance().isSet(RPARAM_KRTCALARMSORTING) )
	    {
		   // ascSort.addSort(Sort::SORT_TIME, Sort::SORT_DESCENDING);
		    m_alarmColumns.push_back(new TA_Base_Bus::ColumnSourceTimestamp(ulTimestampWidth, true,SORT_BY_DATETIME, NULL));
	    }
	    else
	    {
		    m_alarmColumns.push_back(new TA_Base_Bus::ColumnSourceTimestamp(ulTimestampWidth, true));
	    }
		*/
		m_alarmColumns.push_back(new TA_Base_Bus::ColumnSourceTimestamp(ulTimestampWidth, true,SORT_BY_DATETIME, NULL));

	    m_alarmColumns.push_back(new TA_Base_Bus::ColumnSeverity(40, true, SORT_BY_SEVERITY, NULL)); //TD15005
	    m_alarmColumns.push_back(new TA_Base_Bus::ColumnAsset(210, true, SORT_BY_ASSET, NULL)); //TD15005
	    m_alarmColumns.push_back(new TA_Base_Bus::ColumnDescription(1, true, SORT_BY_DESCRIPTION, NULL)); //TD15005
	    m_alarmColumns.push_back(new TA_Base_Bus::ColumnValue(140, true, SORT_BY_VALUE, NULL));
		m_alarmColumns.push_back(new TA_Base_Bus::ColumnAlarmOperatorType(24, true, SORT_BY_ALARMOPERATORTYPE, NULL));
	    // TD #11287
	    // Changed column order from DAM to MDA
	    m_alarmColumns.push_back(new TA_Base_Bus::ColumnMms(24, true, SORT_BY_MMS, NULL));
	    m_alarmColumns.push_back(new TA_Base_Bus::ColumnDecisionSupport(22, true, SORT_BY_DSS, NULL));
	    m_alarmColumns.push_back(new TA_Base_Bus::ColumnAvalanche(22, true, SORT_BY_AVALANCHE, NULL));
	    
	    FUNCTION_EXIT;
    }

    void CAlarmsGuiDlg::updateAlarmCounts(unsigned int totAlarms, unsigned int totUnacknowledgedAlarms, bool exceededMaxDisplayAlarm, bool isLoadingAlarms)
    {
	    //We must send a message to itself to update the total Alarms counter. 
	    //Handling it here directly will cause an unnecessary assert to occur when
	    //UpdateData(false) is called. This is an MFC bug, Microsoft Knowledge Base Article - 192853.
		m_pRealModel->setExceededMaxDisplay(exceededMaxDisplayAlarm);
	    
	    if (isLoadingAlarms == true)
	    {
			m_isLoadingAlarms = true;
		    // Display wait ie. hour-glass cursor while loading
			setCursor(QCursor(Qt::WaitCursor));
	    }
	    else
	    {
			m_isLoadingAlarms = false;
		    // Display normal ie. Arrow cursor after loading
			setCursor(QCursor(Qt::ArrowCursor));
		    //SetCursor(AfxGetApp()->LoadStandardCursor(IDC_ARROW));
	    }

		emit alarmCountUpdate(totUnacknowledgedAlarms, totAlarms);
	    
	    if(totUnacknowledgedAlarms == 0)
        {
            disableButtonFlash();
        }
        else
        {
            enableButtonFlash();
        }
	    //OnChange(true, true, getHighestSeverity(), true);
    }

	void CAlarmsGuiDlg::updateAlarmSound(unsigned long severity, bool wereAlarmsAdded)
	{
		m_pRealModel->updateAlarmSound(severity, wereAlarmsAdded);
	}

    void CAlarmsGuiDlg::notifyNoMatchingAlarms()
    {
    }

	void CAlarmsGuiDlg::notifyFilterReset()
	{
	}


    void CAlarmsGuiDlg::updateAvailableAlarmActions(const ItemAvailability& availability)
    {
    }


	void CAlarmsGuiDlg::setCurrentDescriptionText(const TAUtfString& description)
    {
    }


    void CAlarmsGuiDlg::setWindowTitle(const std::string& windowText) 
    {
	    //DO nothing for the alarm banner container as it has no title bar. 
	    //Important not to delete this method. 
    }

	void CAlarmsGuiDlg::setUpConnectionPoints()
	{
		// Custom connection
		connect(this, SIGNAL(alarmCountUpdate(int, int)), this, SLOT(OnUpdateAlarmCount(int, int)));
	}

	void CAlarmsGuiDlg::setupPModel()
	{
		m_pModel = new CAlarmsGuiDlgPModel(this->getTransActiveWinApp()->getApplicationController(), PMODEL_ALARMS_GUI_DLG);
		m_pRealModel = dynamic_cast<CAlarmsGuiDlgPModel*>(m_pModel);
		TA_ASSERT(m_pRealModel != NULL, "PModel should be valid");

		m_pRealModel->attachView(this, PMODEL_SESSION_CHANGE);
		m_pRealModel->attachView(this, PMODEL_SYSTEM_STATUS);
		m_pRealModel->attachView(this, PMODEL_NOTIFY_UNMUTE);
	
	}

	void CAlarmsGuiDlg::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args/*=NULL*/)
	{		
		if (_stricmp(updateType, PMODEL_SYSTEM_STATUS) == 0 || _stricmp(updateType, PMODEL_SESSION_CHANGE) == 0)
		{
			PrefilterChangeEventArgs* evArgs = dynamic_cast<PrefilterChangeEventArgs*>(args);
			m_alarmList->updatePreFilterLocationsAndSession(*evArgs->getPreFilter(), evArgs->getLocations(), evArgs->getSessionID());
		}
		else if (_stricmp(updateType, PMODEL_NOTIFY_UNMUTE) == 0)
		{ // Post Message
			m_silenceButton.unsilence();
			/*PostMessage(
				WM_ON_NOTIFY_UNMUTE, 
				wParam, 
				lparam);*/
		}

	}
    
	void CAlarmsGuiDlg::initUIControl()
	{
		m_silenceButton.initialise(false);

		//// Added for Manila - Disable the Close button in the title bar
		//HMENU hmenu = ::GetSystemMenu(AfxGetMainWnd()->m_hWnd, FALSE);
		//::EnableMenuItem(hmenu, SC_CLOSE, MF_GRAYED | MF_BYCOMMAND);


		//// Set the icon for this dialog.  The framework does this automatically
		////  when the application's main window is not a dialog
		//SetIcon(m_hIcon, TRUE);            // Set big icon
		//SetIcon(AfxGetApp()->LoadIcon(IDI_SM_BELL), FALSE);        // Set small icon

		//// Load the accelerator keys
		//m_hAccel = ::LoadAccelerators(AfxGetResourceHandle(),MAKEINTRESOURCE(IDR_ACCELERATOR1));

		// Initialize Model
		m_pRealModel->initialize();

		// Initialise the Alarm List Control
		TA_Base_Bus::Filter preFilter;
		std::vector<unsigned long> locations;    
		m_pRealModel->initPreFiltering(preFilter, locations);

		// TD 6950: Check if the system is currently in degraded mode. If so, clear the pre-filter.
		m_pRealModel->updatePreFilterAccordingToSystemState(preFilter);

		TA_Base_Bus::Filter filter;

		TA_Base_Bus::AlarmActionFactory* factory = 
			new TA_Base_Bus::AlarmActionFactory(initListControlActions());

		initListControlColumns();
		m_alarmList->setupAlarmList(*this, m_alarmColumns, true);
		long sortColumn = 0;
		if( RunParams::getInstance().isSet(RPARAM_KRTCALARMSORTING))
		{
			sortColumn = TA_Base_Bus::COL_SOURCETIMESTAMP;
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "sort by COL_SOURCETIMESTAMP");
			//m_alarmList->startAlarmList(*this, *factory, m_alarmColumns, preFilter, locations, TA_Base_Bus::COL_SOURCETIMESTAMP, false, filter, m_pRealModel->getSession(), true);
		}
		else
		{
			sortColumn = TA_Base_Bus::COL_STATE;
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "sort by COL_STATE");
			//m_alarmList->startAlarmList(*this, *factory, m_alarmColumns, preFilter, locations, TA_Base_Bus::COL_STATE, true, filter, m_pRealModel->getSession(), true);
		}
		std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
		m_alarmList->startAlarmList(*factory, locations, preFilter, filter, sortColumn, false, sessionId);

		int left = 140;
		int top = 0;
		int width = getWidth() - 2 - left;
		int height = getHeight() - 2;
		m_alarmList->setGeometry(left, top, width, height);
		m_alarmList->move(left, top);

		//TD14896++
		//From the m_alarmColumns.push_back() calls, COL_DESCRIPTION is 5th (or 4th???) item
		//TA_Base_Bus::FixedHeaderCtrl::expandColumnToFitListWidth(*m_alarmList, 4, true); //TD15005
		//++TD14896

		setUpConnectionPoints();
	}
}
