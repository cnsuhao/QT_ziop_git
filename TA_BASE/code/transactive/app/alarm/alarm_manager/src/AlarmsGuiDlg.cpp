/**
 * The source code in this file is the property of 
 * Ripple Systems and is not for redistribution
 * in any form.
 *
 * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/alarm/alarm_manager/src/AlarmsGuiDlg.cpp $
 * @author:  Andy Siow
 * @version: $Revision: #9 $
 *
 * Last modification: $DateTime: 2016/12/06 12:21:32 $
 * Last modified by:  $Author: Noel $
 * 
 * AlarmsGuiDlg.cpp : implementation file
 */

 
#include <cmath>
#include <process.h>
#include <cstdlib>

#include "core\alarm\IDL\src\AlarmAgentCorbaDef.h"
#include "core\corba\src\CorbaUtil.h"
#include "core\utilities\src\DebugUtil.h" 
#include "core\utilities\src\RunParams.h"
#include "core\naming\src\Naming.h"
#include "core\data_access_interface\src\Operator.h"
#include "core\data_access_interface\src\OperatorAccessFactory.h"
#include "core\data_access_interface\entity_access\src\ConfigEntity.h"
#include "core\data_access_interface\entity_access\src\ConsoleAccessFactory.h"
#include "core\data_access_interface\entity_access\src\Console.h"
#include "core\data_access_interface\entity_access\src\EntityData.h"
#include "core\data_access_interface\entity_access\src\EntityAccessFactory.h"
#include "core\data_access_interface\src\ProfileAccessFactory.h"
#include "core\data_access_interface\src\IProfile.h"
#include "core\data_access_interface\src\AlarmTypeData.h"
#include "core\data_access_interface\src\AlarmTypeAccessFactory.h"
#include "core/data_access_interface/src/NamedFilterAccessFactory.h"
#include "core/data_access_interface/src/NamedFilter.h"
#include "core\exceptions\src\DatabaseException.h"
#include "core\exceptions\src\DataException.h"
#include "core\exceptions\src\AssertException.h"
#include "core\exceptions\src\AuthenticationAgentException.h"
#include "core\exceptions\src\UserSettingsException.h"
#include "core\exceptions\src\ApplicationException.h"
#include "core\exceptions\src\ObjectResolutionException.h"
#include "core\synchronisation\src\ThreadGuard.h"
#include "core/utilities/src/TAAssert.h"
#include "core\data_access_interface\entity_access\src\AlarmGUI.h"
#include "core\utilities\src\Timer.h"
#include "core\message\src\MessageConfig.h"
#include "core/utilities/src/DateFormat.h" // TD12474 


//#include "bus/generic_gui/src/HelpLauncher.h"
//#include "bus/generic_gui/src/TransActiveMessage.h"
//#include "bus/generic_gui/src/SystemControllerGroupUpdateSubscriber.h"
#include "bus\application_types\src\apptypes.h"
//#include "bus\generic_gui\src\GenericGUI.h"
#include "bus\common_control\src\common\TA_String.h"
//#include "bus\generic_gui\src\AppLauncher.h"
#include "bus\security\authentication_library\src\AuthenticationLibrary.h"

#include "bus/alarm/alarm_common/src/AlarmConstants.h"
#include "bus/alarm/alarm_list_control/src/AlarmActionFactory.h"
#include "bus/alarm/alarm_list_control/pmodel/src/Filter.h"
#include "bus/alarm/alarm_list_control/pmodel/src/Sort.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnIdentifiers.h"
#include "bus/alarm/alarm_list_control/src/IAlarmAction.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionDisplayComment.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionSeparator.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionAcknowledgeAlarm.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionRelatedSchematic.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionCloseAndAckAlarm.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionIncidentLogSheet.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionJobRequest.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionPostponeDSS.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionRelatedDSS.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionRelatedPlanInstance.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionRelatedEvents.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionRelatedInspector.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionRelatedTrend.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionAcknowledgeAllAlarms.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionRelatedAvalancheEvents.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionSuppressAlarm.h"
#include "bus/alarm/alarm_list_control/src/actions/ActionEventsViewer.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnAck.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnState.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnSourceTimestamp.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnSeverity.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnAsset.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnAlarmOperationMode.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnDescription.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnValue.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnDecisionSupport.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnAvalanche.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnMms.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnComment.h"
#include "bus/alarm/alarm_list_control/src/columns/ColumnAckBy.h"

#include "bus\alarm\alarm_list_control\src\AlarmListCtrl.h"

#include "app/alarm/alarm_manager/pmodel/src/AlarmManagerConsts.h"
#include "app/alarm/alarm_manager/pmodel/src/AlarmGUIDlgModel.h"
#include "app/alarm/alarm_manager/pmodel/src/AlarmFilterService.h"
#include "app/alarm/alarm_manager/pmodel/src/AlarmManagerService.h"
#include "app/alarm/alarm_manager/pmodel/src/AlarmFilterDlgPModel.h"
#include "app/alarm/alarm_manager/pmodel/src/FilterEventArgs.h"
#include "app\alarm\alarm_manager\pmodel\src\PrefilterChangeEventArgs.h"
#include "app\alarm\alarm_manager\pmodel\src\ServerStatusEventArgs.h"
#include "bus\generic_gui_view\src\HelpLauncher.h"

//#include "app/alarm/alarm_manager/src/ExportAlarms.h"
 
#include "app/alarm/alarm_manager/src/Globals.h"
#include "app\alarm\alarm_manager\src\AlarmsGuiDlg.h"
#include "app\alarm\alarm_manager\src\AlarmFilterDlg.h"
#include "app\alarm\alarm_manager\src\ExportAlarms.h"
#include "app\alarm\alarm_manager\src\ListCtrlPrint.h"
#include "bus\common_control\src\printdialog\CtlPrintDialog.h"
#include "bus/generic_gui_view/resource.h" // TD14164 azenitha
#include "bus\generic_gui_view\src\TransActiveMessage.h"
#include <algorithm>
#include <QtPrintSupport\qprinter.h>

using TA_Base_Core::RunParams;
using TA_Base_Bus::AbstractTransActiveDialog;
using TA_Base_Bus::Sort;

namespace TA_Base_App
{

 

	/////////////////////////////////////////////////////////////////////////////
	// AlarmsGuiDlg dialog

	//AlarmsGuiDlg::AlarmsGuiDlg(CWnd* pParent /*=NULL*/)
	AlarmsGuiDlg::AlarmsGuiDlg(TA_Base_Bus::ITransActiveApp* app)
		:TA_Base_Bus::AbstractTransActiveDialog(app, ViewType_AlarmManager)
		, m_exceededMaxDisplayAlarm(false)
		, m_isLoadingAlarms(false)
	{
		 
		ui.setupUi(this);
		m_alarmList=new AlarmListCtrl(true,true);
		//Add alarm count and alarm filter into toolbar
		ui.wAlarmCount->setLayout(ui.layAlarmCount);
		ui.wAlarmFilter->setLayout(ui.layFilter);
		ui.toolBar->addWidget(ui.wAlarmFilter);
		//add space layer
		QWidget* spacer = new QWidget();
		spacer->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
		// toolBar is a pointer to an existing toolbar
		ui.toolBar->addWidget(spacer);
		//add number of alarm information
		ui.toolBar->addWidget(ui.wAlarmCount);
		//maximum alarm list
		ui.layAlarmList->addWidget(m_alarmList);
		ui.centralwidget->setLayout(ui.layAlarmList);
	
		initilization();
 
	}

	// Destructor
	AlarmsGuiDlg::~AlarmsGuiDlg()
	{
		// Refer to OnDestroy. Most cleanup code is in there.
		//ITransActivePModel* ptempPModel = this->getTransActiveWinApp()->getApplicationController()->getPModel(PMODEL_AlarmFilterDlg);
		//ptempPModel->detachView(this, EV_SET_ALARM_FILTER);
		m_pModel->detachView(this, EV_SYSTEM_STATUS);
		m_pModel->detachView(this, EV_SERVER_STATUS);
		m_pModel->detachView(this, EV_FILTER_ON_ASSET);
		m_pModel->detachView(this, EV_FILTER_ON_ENTITY);
		m_pModel->detachView(this, EV_SESSION_CHANGED);
	}

	void AlarmsGuiDlg::initUIControl()
	{
		ui.cboFilter->blockSignals(true);
		std::list<std::string > filterList = m_realPModel->getFilterList();
		std::list<std::string >::iterator itr = filterList.begin();
		for (itr; itr != filterList.end(); itr++)
		{
			//QString temp = QString::fromStdString(*itr);
			ui.cboFilter->addItem(*itr);
		}
		ui.cboFilter->blockSignals(false);
	}

	/*bool AlarmsGuiDlg::init()
	{
		return true;
	}*/


	//----IAlarmListObserver----------------------------------------------------------------------------------
	void AlarmsGuiDlg::updateAvailableAlarmActions(const ItemAvailability& availability)
	{
		 for (ItemAvailability::const_iterator iter = availability.begin(); 
			iter != availability.end(); iter++)
		{
	 
			bool isEnabled = (iter->second == TA_Base_Bus::IAlarmListObserver::ENABLED);
			switch (iter->first)
			{
			case ACKNOWLEDGE_ALARM:
			
				ui.actionAck->setEnabled(isEnabled); // Tool Bar
				ui.m_actionAcknowledge->setEnabled(isEnabled);
				break;
			case VIEW_COMMENT:
				ui.actionComments->setEnabled(isEnabled);
				ui.m_actionComments->setEnabled(isEnabled);
				break;

			case CLOSE_AND_ACK_ALARM:
				ui.actionClose_Alarm->setEnabled(isEnabled);
				//menu->EnableMenuItem(ID_AM_ALARM_MANUAL_CLOSE, menuState);
				break;

			case RELATED_INSPECTOR:
				ui.m_actionRelated_Inspector->setEnabled(isEnabled);
				break;

			case RELATED_SCHEMATIC:
				ui.m_actionRelated_Schematic->setEnabled(isEnabled);
				break;

			case RELATED_TREND:
				//menu->EnableMenuItem(ID_AM_ALARM_TREND, menuState);
				break;

			case RELATED_AVALANCHE_EVENTS:
				ui.m_actionRelated_Avalanche_Events->setEnabled(isEnabled);
				break;

			case RELATED_EVENTS:
				ui.m_actionRelated_Events->setEnabled(isEnabled);
				//TD 11783 Event Viewer button is always enabled via toolbar, and will launch EventViewer
				// m_alarmsToolbar.EnableButton(IDC_RELATED_EVENTS,isEnabled);
				break;

			case RELATED_PLAN_INSTANCE:
				ui.m_actionRelated_Plan_Instance->setEnabled(isEnabled);
				//TD 11783 Event Viewer button is always enabled via toolbar, and will launch EventViewer
				// m_alarmsToolbar.EnableButton(IDC_RELATED_EVENTS,isEnabled);
				break;
			case JOB_REQUEST:
				ui.m_actionRaise_Service_Request->setEnabled(isEnabled);
				break;

			case RELATED_DSS: 
				ui.m_actionRelated_Plan->setEnabled(isEnabled);
				//menu->EnableMenuItem(ID_AM_ALARM_PLAN, menuState);
				break;

			case POSTPONE_DSS:
				ui.m_actionPostpone_Plan->setEnabled(isEnabled);
				break;

			case INCIDENT_LOG_SHEET: 
				ui.m_actionCreate_Incident_Report->setEnabled(isEnabled);
				//menu->EnableMenuItem(ID_AM_ALARM_EVENT_LOG, menuState);
				break;

			case ACKNOWLEDGE_ALL:
				ui.actionAck_All->setEnabled(isEnabled);
				ui.m_actionAcknowledge->setEnabled(isEnabled);
				break;

			case INHIBIT_ALARM:
				ui.actionInhibit->setEnabled(isEnabled);
				//menu->EnableMenuItem(ID_AM_INHIBIT_BY_TYPE, menuState);
				//menu->EnableMenuItem(ID_AM_INHIBIT_BY_DESCRIPTION, menuState);
				break;
			}
		}
	}


	void AlarmsGuiDlg::notifyNoMatchingAlarms()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "notifyNoMatchingAlarms" );
		//m_alarmList->Invalidate(); //17081
		TAUtfString str=tr("No matched alarms");
		ui.statusbar->setWindowTitle(str);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "notifyNoMatchingAlarms" );
	}

	void AlarmsGuiDlg::notifyFilterReset()
	{
		//refresh the filter combo box with default
		populateNamedFilters(TA_Base_App::AlarmFilter::FILTER_DEFAULT);
	
	 	// Reset the caption to default
	 	std::string windowText(RunParams::getInstance().get(RPARAM_APPNAME));
		setWindowTitle(windowText.c_str());
	
	}

	void AlarmsGuiDlg::setCurrentDescriptionText(const TAUtfString& description)
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "setCurrentDescriptionText" );
		 
		ui.statusbar->setWindowTitle(description);
			LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "setCurrentDescriptionText" );
	}


	void AlarmsGuiDlg::updateAlarmCounts(unsigned int totAlarms, unsigned int totUnacknowledgedAlarms, bool exceededMaxDisplayAlarm, bool isLoadingAlarms)
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "updateAlarmCounts" );
		m_exceededMaxDisplayAlarm = exceededMaxDisplayAlarm;
		if (isLoadingAlarms == true)
		{
			m_isLoadingAlarms = true;
			// Display wait ie. hour-glass cursor while loading
			setCursor(Qt::WaitCursor);
		}
		else
		{
			m_isLoadingAlarms = false;
			// Display normal ie. Arrow cursor after loading
			setCursor(Qt::ArrowCursor);
		}
	 
		emit alarmCountUpdate(totUnacknowledgedAlarms, totAlarms);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "updateAlarmCounts" );
	}

	void AlarmsGuiDlg::updateAlarmSound(unsigned long severity, bool wereAlarmsAdded)
	{
		// Alarm manager plays no sound.
	}
	//----AbstractTransActiveDialog----------------------------------------------------------------------------------

	std::string  AlarmsGuiDlg::getMyPModelName(void)
	{
		return  PMODEL_AlarmGUIDlg;
	}
	void AlarmsGuiDlg::setupPModel(void)
	{
		m_realPModel = dynamic_cast<AlarmGUIDlgModel*> (m_pModel);

		// attached to filter view
		//ITransActivePModel* ptempPModel = this->getTransActiveWinApp()->getApplicationController()->getPModel(PMODEL_AlarmFilterDlg);
		//ptempPModel->attachView(this, EV_SET_ALARM_FILTER);
		m_pModel->attachView(this, EV_SYSTEM_STATUS);
		m_pModel->attachView(this, EV_SERVER_STATUS);
		m_pModel->attachView(this, EV_FILTER_ON_ASSET);
		m_pModel->attachView(this, EV_FILTER_ON_ENTITY);
		m_pModel->attachView(this, EV_SESSION_CHANGED);
	}

	void AlarmsGuiDlg::translateUI()
	{
		ui.retranslateUi(this);
		m_alarmList->translateUI();
	}
 
	//BOOL AlarmsGuiDlg::OnNotify(WPARAM wParam, LPARAM lParam, LRESULT* pResult)
	//{
	//
	//    #define lpnm   ((LPNMHDR)lParam)
	//    #define lpnmTB ((LPNMTOOLBAR)lParam)
	//	LPNMPGCALCSIZE   pCalcSize = (LPNMPGCALCSIZE)lParam;
	//
	//	CRect buttonRect;
	//	int buttonCount;
	//
	//    if ( (lpnm->code == HDN_DIVIDERDBLCLICKA) ||
	//		      (lpnm->code == HDN_DIVIDERDBLCLICKW) )
	//    {
	//		TRACE("Double Click on Column Divider. Column = %d\n", lpnmTB->iItem);
	//		//ResizeColumn(lpnmTB->iItem);
	//		return FALSE;
	//	}
	//	// This needs to be here for the arrows of the pager control to appear
	//	else if(lpnm->code == PGN_CALCSIZE)
	//	{
	//		
	//		buttonCount = m_alarmsToolbar.GetButtonCount();
	//
	//		// Determine size of child
	//		m_alarmsToolbar.GetItemRect(buttonCount - 1, buttonRect);
	//
	//		switch(pCalcSize->dwFlag)
	//		{
	//		case PGF_CALCWIDTH:
	//			pCalcSize->iWidth = buttonRect.right;
	//			break;
	//
	//		case PGF_CALCHEIGHT:
	//			pCalcSize->iHeight = buttonRect.bottom;
	//			break;			
	//		}
	//
	//		return FALSE;
	//	}
	//	else if(lpnm->code == PGN_SCROLL)
	//	{
	//		LPNMPGSCROLL   pScroll = (LPNMPGSCROLL)lParam;
	//
	//		switch(pScroll->iDir)
	//		{
	//		case PGF_SCROLLLEFT:
	//			pScroll->iScroll = 15;
	//			return FALSE;
	//			
	//		case PGF_SCROLLRIGHT:
	//			pScroll->iScroll = 15;
	//			return FALSE;
	//			
	//		case PGF_SCROLLUP:
	//			break;
	//
	//		case PGF_SCROLLDOWN:
	//			break;
	//		}
	//	}
	//
	//    return TransActiveDialog::OnNotify(wParam, lParam, pResult);
	//
	//}
	//

	//LRESULT AlarmsGuiDlg::OnFilterApply(WPARAM wParam, LPARAM lParam)
	//{
	//	LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnFilterApply" );
	//
	//	std::string filterName;
	//	CString curName;
	//
	//	//refresh the filter combo box with default
	//	populateNamedFilters(TA_Base_App::AlarmFilterDlg::FILTER_DEFAULT);
	//
	//	// Get the filter index type
	//	TA_Base_App::AlarmFilterDlg::FilterIndex fIndex = static_cast<TA_Base_App::AlarmFilterDlg::FilterIndex> (wParam);
	//	switch (fIndex)
	//	{
	//		case (TA_Base_App::AlarmFilterDlg::INDEX_DEFAULT):
	//			filterName = TA_Base_App::AlarmFilterDlg::FILTER_DEFAULT;
	//			break;
	//
	//		case (TA_Base_App::AlarmFilterDlg::INDEX_USER_DEFINED):
	//			filterName = TA_Base_Core::NamedFilter::UNNAMED;
	//			break;
	//		
	//		case (TA_Base_App::AlarmFilterDlg::INDEX_LIST):
	//			//Get the index of the Default Filter which is either first or last
	//			int defaultIndex = m_currentFilterListCtrl.FindString(-1, TA_Base_App::AlarmFilterDlg::FILTER_DEFAULT.c_str() );
	//
	//			int index = lParam;
	//			//if(defaultIndex<=index)
	//			// index = index + 1;
	//            //++ Hu Wenguang
	//            // TD12728
	//			if(index>=0)
	//            // TD12728
	//            //Hu Wenguang ++ 
	//			m_currentFilterListCtrl.GetLBText(index, curName);
	//			filterName = curName;
	//			break;
	//	}
	//	
	//	// Populate the filter combo box with the current filter name		
	//	populateNamedFilters(filterName);
	//
	//	// Reset the caption to show the filter param
	//    std::string windowText(RunParams::getInstance().get(RPARAM_APPNAME));
	//    std::string filterText(m_alarmFilterDlg->getCurrentFilterString());
	//    
	//	if (0 < filterText.length() && filterName != TA_Base_App::AlarmFilterDlg::FILTER_DEFAULT)//TD15055
	//
	//    {
	//        windowText += filterText;
	//        SetWindowText(CString(windowText.c_str()));
	//    }
	//	else // This is a case of empty filter string so set the text to just the application name
	//	{
	//        SetWindowText(CString(windowText.c_str()));
	//	}
	//
	//	LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnFilterApply" );
	//
	//	return NULL;	// This line must be included. Everything above is application specific
	//}
	//
	//
	//LRESULT AlarmsGuiDlg::OnFilterDelete(WPARAM wParam, LPARAM lParam)
	//{
	//	LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnFilterDelete" );
	//
	//	// Confirm there is delete of the filter
	//	TA_Base_App::AlarmFilterDlg::FilterIndex fIndex = static_cast<TA_Base_App::AlarmFilterDlg::FilterIndex> (wParam);
	//	if ( fIndex == TA_Base_App::AlarmFilterDlg::INDEX_LIST )
	//	{
	//		// Refresh the filter combo box
	//		// Always go back to Default when a filter is deleted
	//		populateNamedFilters(TA_Base_App::AlarmFilterDlg::FILTER_DEFAULT); 
	//	}
	//	
	//	// Otherwise do nothing
	//	LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnFilterDelete" );
	//	return NULL;	// This line must be included. Everything above is application specific
	//}
	//
	//
	//LRESULT	AlarmsGuiDlg::OnFilterUpdate(WPARAM wParam, LPARAM lParam)
	//{
	//	LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnFilterUpdate" );
	//
	//	// Refresh the ComboBox
	//	CString strFilterName;
	//	GetDlgItemText(IDC_NAMED_FILTER, strFilterName);	
	//
	//	populateNamedFilters(strFilterName.GetBuffer(0));
	//
	//	//m_currentFilterListCtrl.SetCurSel()
	//
	//	return 0;
	//}

 
	// BJR 17/8/04: This function appears to apply to menus as well as buttons.
	void AlarmsGuiDlg::DisableButtons()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "DisableButtons" );

		ItemAvailability availability;
		availability[ACKNOWLEDGE_ALARM]			= TA_Base_Bus::IAlarmListObserver::DISABLED;
		availability[VIEW_COMMENT]				= TA_Base_Bus::IAlarmListObserver::DISABLED;

		availability[RELATED_SCHEMATIC]			= TA_Base_Bus::IAlarmListObserver::DISABLED;
		availability[RELATED_INSPECTOR]			= TA_Base_Bus::IAlarmListObserver::DISABLED;
		availability[RELATED_EVENTS]			= TA_Base_Bus::IAlarmListObserver::DISABLED;

		availability[RELATED_DSS]				= TA_Base_Bus::IAlarmListObserver::DISABLED; 
		availability[POSTPONE_DSS]				= TA_Base_Bus::IAlarmListObserver::DISABLED; 

		availability[JOB_REQUEST]				= TA_Base_Bus::IAlarmListObserver::DISABLED;
		availability[RELATED_TREND]				= TA_Base_Bus::IAlarmListObserver::DISABLED;
		availability[RELATED_AVALANCHE_EVENTS]	= TA_Base_Bus::IAlarmListObserver::DISABLED;
		availability[INCIDENT_LOG_SHEET]		= TA_Base_Bus::IAlarmListObserver::DISABLED;

		availability[CLOSE_AND_ACK_ALARM]		= TA_Base_Bus::IAlarmListObserver::DISABLED;

		availability[INHIBIT_ALARM]				= TA_Base_Bus::IAlarmListObserver::DISABLED;

		// The following are not in the main and right-click menus, but they are button actions
		availability[EDIT_COMMENT]				= TA_Base_Bus::IAlarmListObserver::DISABLED;
		availability[ACKNOWLEDGE_ALL]			= TA_Base_Bus::IAlarmListObserver::DISABLED;
		availability[RELATED_PLAN_INSTANCE]		= TA_Base_Bus::IAlarmListObserver::DISABLED;

		updateAvailableAlarmActions(availability);

		setCurrentDescriptionText("");	// Clear the status bar text

		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "DisableButtons" );
		return;
	}


	void AlarmsGuiDlg::Print()
	{
		FUNCTION_ENTRY( "Print" );

		std::string oper = "";
		std::string console = "";
		TA_Base_Core::IOperator* p_operator = NULL;
		TA_Base_Core::IConsole* p_console = NULL;
		try
		{
			std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
			p_operator = TA_Base_Core::OperatorAccessFactory::getInstance().getOperatorFromSession(sessionId, false);
			oper = p_operator->getDescription();
			p_console = TA_Base_Core::ConsoleAccessFactory::getInstance().getConsoleFromSession(sessionId, false);
			console = p_console->getName();
		}
		catch (...)
		{
			if (p_operator != NULL)
			{
				delete p_operator;
				p_operator = 0;
			}
			if (p_console != NULL)
			{
				delete p_console;
				p_console = 0;
			}
			TA_Base_Bus::TransActiveMessage userMsg;
			UINT selectedButton = userMsg.showMsgBox(IDS_UI_040027);
		}

		delete p_operator;
		delete p_console;

		// TD12474 ++
		/*CString header = "Alarms : " + CTime::GetCurrentTime().Format("%a %#d/%#m/%Y %#H:%M:%S")
		+ " - " + oper.c_str() + " on console " + console.c_str();*/

		TA_Base_Core::DateFormat dateFormat;
		std::string dateFormatStr = dateFormat.getDateFormat(TA_Base_Core::DateFormat::DateTimeFormat3);

		//TD14929
		QString header = "Alarms Manager: " + QDateTime::currentDateTime().toString(Qt::SystemLocaleShortDate)
			+ " - " + oper.c_str() + "  on console " + console.c_str();
		// ++ TD12474 

		// Do not delete this structure. It will be deleted inside the thread
		CListCtrlPrint::t_ParamsToReceive* params = new CListCtrlPrint::t_ParamsToReceive;
		params->csPageHead = header;
		params->csPage = "Page %d of %d";
		params->list = m_alarmList;
		
		QPrinter* printer = new QPrinter();
		printer->setPageMargins(5, 10, 5, 10, QPrinter::Millimeter);
		printer->setOrientation(QPrinter::Landscape);
		printer->setPageSize(QPrinter::A4);

		CtlPrintDialog* printDialog = new CtlPrintDialog(printer, NULL);
		if (printDialog->doModal() != QDialog::Accepted)
		{
			return;
		}
		params->printer = printer;
		
		CListCtrlPrint* printList = new CListCtrlPrint();
		printList->print(params);
	
		FUNCTION_EXIT;
	}


	void AlarmsGuiDlg::Save()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "Save" );   
		
		// Get the GUI entity from which we can get the default export path.
		TA_Base_Core::AlarmGUI* alarmGUI = dynamic_cast<TA_Base_Core::AlarmGUI*>(this->getTransActiveWinApp()->getApplicationController()->getEntity());

		// Do not delete this structure. It will be deleted inside the thread
		CExportAlarms::t_ParamsToReceive* params;
		params = new CExportAlarms::t_ParamsToReceive;
		params->alarmList = m_alarmList;

		try
		{
			//params->defaultExportPath = alarmGUI->getExportAlarmDirectory().c_str();
			//TD17888 marc_bugfix 
			params->defaultExportPath = alarmGUI->getExportDirectory().c_str();
		}
		catch (...)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "DatabaseError", "There was an error while attempting to retrieve the default export path. Using default.");
			params->defaultExportPath = "";
		}

		TAUtfString fileExtension = ".csv";
		TAUtfString fileName = tr("Untitled");
		TAUtfString caption = tr("Save File");
		TAUtfString filters = tr("CSV (Comma delimited) (*.csv)|*.csv||");

		CtlFileDilaog* pFileDlg = new CtlFileDilaog(0, // Parent
			caption.q_str(),
			params->defaultExportPath.q_str(),
			filters.q_str());

		pFileDlg->setFileMode(QFileDialog::AnyFile);
		pFileDlg->setViewMode(QFileDialog::List);
		pFileDlg->setAcceptMode(QFileDialog::AcceptSave);  // We want a Save dialog not an Open one
		pFileDlg->setLabelText(QFileDialog::FileName, fileName);

		params->fileDlg = pFileDlg;
		CExportAlarms* pExportAlarms = new CExportAlarms();
		pExportAlarms->saveNow(params);

		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "Save" );
	}

	//----Menu and Toobar----------------------------------------------------------------------------------
	void AlarmsGuiDlg::OnAppExit()
	{
		// Todo: call PModel to unregister param user
		close();
	}

	void AlarmsGuiDlg::OnPrint() 
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnPrint" );
		try
		{
			Print();
		}
		catch(...)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugDebug, "OnPrint failed" );
		}
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnPrint" );
	}

	void AlarmsGuiDlg::OnExport() 
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnExport" );
		Save();
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnExport" );
	}

	void AlarmsGuiDlg::OnAppAbout() 
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnAppAbout" );
		TA_Base_Bus::HelpLauncher::getInstance().displayAboutBox();
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnAppAbout" );
	}

	void AlarmsGuiDlg::OnHelp()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnHelp" );
		TA_Base_Bus::HelpLauncher::getInstance().displayHelp();
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnHelp" );
	}


	void AlarmsGuiDlg::OnAlarmAcknowledge() 
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnAlarmAcknowledge" );
		m_alarmList->executeAction(ACKNOWLEDGE_ALARM);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnAlarmAcknowledge" );
	}


	void AlarmsGuiDlg::OnAlarmAllAcknowledge() 
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnAlarmAllAcknowledge" );
		m_alarmList->executeAction(ACKNOWLEDGE_ALL);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnAlarmAllAcknowledge" );
	} 


	void AlarmsGuiDlg::OnAlarmClose()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnAlarmClose" );
		m_alarmList->executeAction(CLOSE_AND_ACK_ALARM);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnAlarmClose" );
	}

	void AlarmsGuiDlg::OnAlarmDisplayComment() 
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnAlarmDisplayComment" );
		m_alarmList->executeAction(VIEW_COMMENT);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnAlarmDisplayComment" );
	}


	void AlarmsGuiDlg::OnAlarmSchematic()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnAlarmSchematic" );
		m_alarmList->executeAction(RELATED_SCHEMATIC);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnAlarmSchematic" );
	}


	void AlarmsGuiDlg::OnAlarmPlan()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnAlarmPlan" );
		m_alarmList->executeAction(RELATED_DSS);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnAlarmPlan" );
	}


	void AlarmsGuiDlg::OnAlarmInspector()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnAlarmInspector" );
		m_alarmList->executeAction(RELATED_INSPECTOR);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnAlarmInspector" );
	}


	void AlarmsGuiDlg::OnAlarmTrend()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnAlarmTrend" );
		m_alarmList->executeAction(RELATED_TREND);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnAlarmTrend" );
	}


	void AlarmsGuiDlg::OnRelatedAvalancheEvents()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnRelatedAvalancheEvents" );
		m_alarmList->executeAction(RELATED_AVALANCHE_EVENTS);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnRelatedAvalancheEvents" );
	}


	void AlarmsGuiDlg::OnAlarmJobRequest()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnAlarmJobRequest" );
		m_alarmList->executeAction(JOB_REQUEST);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnAlarmJobRequest" );
	}

	// TD 13825 Postpone dss function not added to menu
	void AlarmsGuiDlg::OnAlarmCancelPlan()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnAlarmCancelPlan" );
		m_alarmList->executeAction(POSTPONE_DSS);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnAlarmCancelPlan" );
	}

	void AlarmsGuiDlg::OnAlarmEventLog()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnAlarmEventLog" );
		m_alarmList->executeAction(INCIDENT_LOG_SHEET);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnAlarmEventLog" );
	}


	void AlarmsGuiDlg::OnAlarmEvents()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnAlarmEvents" );
		m_alarmList->executeAction(RELATED_EVENTS);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnAlarmEvents" );
	}

	void AlarmsGuiDlg::OnAlarmPlanInstance()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnAlarmPlanInstance" );
		m_alarmList->executeAction(RELATED_PLAN_INSTANCE);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnAlarmPlanInstance" );
	}

	void AlarmsGuiDlg::OnEventsViewer()
	{
		FUNCTION_ENTRY("OnEventsViewer");

		m_alarmList->executeAction(EVENTS_VIEWER);

		FUNCTION_EXIT;
	}

	void AlarmsGuiDlg::OnViewInhibitAlarms()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnViewInhibitAlarms" );
		m_alarmList->executeAction(INHIBIT_ALARM);
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnViewInhibitAlarms" );
	}

	void AlarmsGuiDlg::OnViewInhibitAlarmByType()
	{
		FUNCTION_ENTRY("OnViewInhibitAlarmByType");

		m_alarmList->executeAction(INHIBIT_ALARM_BY_TYPE);

		FUNCTION_EXIT;
	}

	void AlarmsGuiDlg::OnViewInhibitAlarmByDescription()
	{
		FUNCTION_ENTRY("OnViewInhibitAlarmByDescription");

		m_alarmList->executeAction(INHIBIT_ALARM_BY_DESCRIPTION);

		FUNCTION_EXIT;
	}

	void AlarmsGuiDlg::OnViewInhibitions()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnViewInhibitions()" );
		TA_Base_Bus::ITransActiveView* pView = this->m_pWinApp->getViewManager()->getView(ViewType_SuppressionRule, "");
		pView->doModal();

		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnViewInhibitions()" );
	}

	void AlarmsGuiDlg::OnDefaultSort()
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnDefaultSort" );
		m_alarmList->defaultSort();
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnDefaultSort" );
	}
	void AlarmsGuiDlg::OnSelchangeNamedFilter(int index)
	{
		FUNCTION_ENTRY("OnSelchangeNamedFilter");
		// Get the currently selected item.
		int selIdx = ui.cboFilter->currentIndex();
		if (0 <= selIdx)
		{
			// Get the selected filter name
			std::string filterName = ui.cboFilter->itemText(selIdx).toStdString();

			// Make sure the string is not empty
			if (filterName != "")
			{
				// Reset the filter via the AlarmFilterDlg object
				ITransActivePModel* ptempPModel = this->getTransActiveWinApp()->getApplicationController()->getPModel(PMODEL_AlarmFilterDlg);
				AlarmFilterDlgPModel *pFilterPModel = dynamic_cast<AlarmFilterDlgPModel*> (ptempPModel);

				pFilterPModel->setNamedFilter(filterName);

				// Reset the filter combo box to selected name
				populateNamedFilters(filterName);

				// Reset the caption to show the filter param
				std::string windowText(RunParams::getInstance().get(RPARAM_APPNAME));
				std::string filterText(pFilterPModel->getCurrentFilterString());

				if (0 < filterText.length() && filterName != TA_Base_App::AlarmFilter::FILTER_DEFAULT)//TD15055

				{
					windowText += filterText;
					setWindowTitle(windowText.c_str());
				}
				else // This is a case of empty filter string so set the text to just the application name
				{
					setWindowTitle(windowText.c_str());
				}
			}

			bool isUserDefine = filterName.compare("User Defined") ? false : true;
			if (!isUserDefine)
			{ // Check whether we have this filter on the combobox
				int idx = ui.cboFilter->findText("User Defined");
				if (idx != -1)
				{ // we have it then we need to remove it because this because this one is only temporary 
					ui.cboFilter->blockSignals(true);
					ui.cboFilter->removeItem(idx);
					ui.cboFilter->blockSignals(false);
				}
			}
		}

		FUNCTION_EXIT;	
	}  

	void AlarmsGuiDlg::OnViewShowFilters()
	{
		FUNCTION_ENTRY("OnViewShowFilters");
		TA_Base_Bus::ITransActiveView* pView = this->m_pWinApp->getViewManager()->getView(ViewType_AlarmFilter, AlarmFilter_ViewID);
		AlarmFilterDlg* pFilterDlg = dynamic_cast<AlarmFilterDlg*>(pView);

		pFilterDlg->setFromDateTime(m_alarmList->getEarliestAlarmTime());
		pFilterDlg->setToDateTime(m_alarmList->getLatestAlarmTime());

		pFilterDlg->showView(false);

		std::string strCurFilter = ui.cboFilter->currentText().toStdString();
		if (!strCurFilter.empty())
		{
			pFilterDlg->showFilter(strCurFilter);
		}

		FUNCTION_EXIT;
	}

	void AlarmsGuiDlg::initListControlColumns()
	{
		FUNCTION_ENTRY("initListControlColumns");
 
		//as per TD 12743/12744
		//we now use nuber of characters to specify initial width of coulmns
		//for Ack By and Asset columns
	 
		//m_alarmColumns.push_back(new TA_Base_Bus::ColumnState(0, true, SORT_BY_DEFAULT, NULL));
		// Source timestamp
		unsigned long ulTimestampWidth = TA_Base_Bus::AlarmConstants::DEFAULT_TIMESTAMPCOLWIDTH;	
		m_alarmColumns.push_back(new TA_Base_Bus::ColumnSourceTimestamp(ulTimestampWidth, true,SORT_BY_DATETIME, NULL));	        		
		// Severity
		m_alarmColumns.push_back(new TA_Base_Bus::ColumnSeverity(40, true, SORT_BY_SEVERITY, NULL)); //TD15005
		// Asset
		m_alarmColumns.push_back(new TA_Base_Bus::ColumnAsset(210, true, SORT_BY_ASSET, NULL)); //TD15005
		// Description	
		m_alarmColumns.push_back(new TA_Base_Bus::ColumnDescription(1, true, SORT_BY_DESCRIPTION, NULL)); 
		//++ TD12744 / TD12743 / TD15005(4th)
		// Value	
		m_alarmColumns.push_back(new TA_Base_Bus::ColumnValue(140, true, SORT_BY_VALUE, NULL));		
		// Alarm Operator type
		m_alarmColumns.push_back(new TA_Base_Bus::ColumnAlarmOperatorType(24, true, SORT_BY_ALARMOPERATORTYPE, NULL));		
		// MMS	
		m_alarmColumns.push_back(new TA_Base_Bus::ColumnMms(26, true, SORT_BY_MMS, NULL));
		// DSS		
		m_alarmColumns.push_back(new TA_Base_Bus::ColumnDecisionSupport(26, true, SORT_BY_DSS, NULL));
		// Avalanche		
		m_alarmColumns.push_back(new TA_Base_Bus::ColumnAvalanche(26, true, SORT_BY_AVALANCHE, NULL));
		// Acknowledged by	
		m_alarmColumns.push_back(new TA_Base_Bus::ColumnAckBy(10, true, SORT_BY_ACK, NULL));	

		FUNCTION_EXIT;
	}


	void AlarmsGuiDlg::initPreFiltering(TA_Base_Bus::Filter& preFilter, std::vector<unsigned long>& locations)
	{
		locations.clear();

		TA_Base_Bus::SessionInfo sessionInfo;
		std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
		try
		{
			// Get some info about the current session.
			TA_Base_Bus::AuthenticationLibrary authLibrary(true);
			sessionInfo = authLibrary.getSessionInfo(sessionId, sessionId);
		}
		catch(TA_Base_Core::AuthenticationAgentException&)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "AuthenticationAgentException", "Could not retrieve session information");
			return;
		}
		catch(TA_Base_Core::ObjectResolutionException&)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "ObjectResolutionException", "Could not resolve Authentication Agent");
			return;
		}

		// The prefilter requires the session id.
		preFilter.addFilter(TA_Base_Bus::Filter::FILTER_PROFILE, 
			TA_Base_Bus::Filter::COMPARE_EQUAL, sessionId);

		bool isCentralProfile = false;
		TA_Base_Core::IProfile* profile = NULL;

		for (std::vector<unsigned long>::iterator iter = sessionInfo.ProfileId.begin();
			iter != sessionInfo.ProfileId.end(); iter++)
		{
			try
			{
				profile = TA_Base_Core::ProfileAccessFactory::getInstance().getProfile(*iter);

				if (profile->getType() == PROFILE_LOCAL)
				{
					locations.push_back(sessionInfo.LocationId);
				}
				else  // PROFILE_CENTRAL
				{
					isCentralProfile = true;
				}

				delete profile;
			}
			catch(TA_Base_Core::DataException&)
			{
				if (NULL != profile)
				{
					delete profile;
				}

				LOG_EXCEPTION_CATCH(SourceInfo, "DataException", "Could not retrieve profile information");
			}
			catch(TA_Base_Core::DatabaseException&)
			{
				if (NULL != profile)
				{
					delete profile;
				}
				LOG_EXCEPTION_CATCH(SourceInfo, "DatabaseException", "Could not retrieve profile information");
			}
		}

		if (isCentralProfile)
		{
			// Central profiles require all locations.
			locations.clear();
		}
	}

	std::vector<TA_Base_Bus::IAlarmAction*> AlarmsGuiDlg::initListControlActions()
	{
		FUNCTION_ENTRY("initListControlActions");

		std::vector<TA_Base_Bus::IAlarmAction*> actions;

		// Add actions available from the context menu.
		actions.push_back( new TA_Base_Bus::ActionAcknowledgeAlarm() );
		//Disable [View Comment] for 955 project [lwk]
		actions.push_back( new TA_Base_Bus::ActionDisplayComment() );
		actions.push_back( new TA_Base_Bus::ActionSeparator() );
		actions.push_back( new TA_Base_Bus::ActionRelatedSchematic() );
		actions.push_back( new TA_Base_Bus::ActionRelatedInspector() );
		actions.push_back( new TA_Base_Bus::ActionRelatedEvents() );
		actions.push_back( new TA_Base_Bus::ActionSeparator() );
		actions.push_back( new TA_Base_Bus::ActionRelatedDSS() );
		actions.push_back( new TA_Base_Bus::ActionRelatedPlanInstance() ); 	
		actions.push_back( new TA_Base_Bus::ActionPostponeDSS() );
		actions.push_back( new TA_Base_Bus::ActionSeparator() );
		actions.push_back( new TA_Base_Bus::ActionJobRequest() );
		//Disable [Related Trend] [lwk]
		actions.push_back( new TA_Base_Bus::ActionRelatedTrend() );
		actions.push_back( new TA_Base_Bus::ActionRelatedAvalancheEvents() );
		actions.push_back( new TA_Base_Bus::ActionIncidentLogSheet() );
		actions.push_back( new TA_Base_Bus::ActionSeparator() );
		//Disable [Close And Ack Selection]  [lwk]
		actions.push_back( new TA_Base_Bus::ActionCloseAndAckAlarm() );
		actions.push_back( new TA_Base_Bus::ActionSuppressAlarm() );

		// Add actions not available from the context menu.
		actions.push_back( new TA_Base_Bus::ActionAcknowledgeAllAlarms(false) );
		// TD 11783 Add an Events Viewer Action to launch events viewer
		actions.push_back( new TA_Base_Bus::ActionEventsViewer(false) );

		FUNCTION_EXIT;
		return actions;
	}


	void AlarmsGuiDlg::initilization()
	{
		initListControlColumns();	
		m_alarmList->setupAlarmList(*this, m_alarmColumns, true);	 
		initializeAlarmList(); 
		DisableButtons();
		setUpConnectionPoints();

		// Alarm Filter
		
		//m_alarmFilterDlg = (AlarmFilterDlg*)pView;

	}

	void AlarmsGuiDlg::setUpConnectionPoints()
	{
		// Toolbar actions
		connect(ui.actionAck, SIGNAL(triggered()), this, SLOT(OnAlarmAcknowledge()));
		connect(ui.actionDefault_Sort, SIGNAL(triggered()), this, SLOT(OnDefaultSort()));
		connect(ui.actionClose_Alarm, SIGNAL(triggered()), this, SLOT(OnAlarmClose()));
		connect(ui.actionFilter, SIGNAL(triggered()), this, SLOT(OnViewShowFilters()));
		connect(ui.actionInhibit, SIGNAL(triggered()), this, SLOT(OnViewInhibitAlarms()));
		connect(ui.actionShow_Inhibits, SIGNAL(triggered()), this, SLOT(OnViewInhibitions()));
		connect(ui.actionAck_All, SIGNAL(triggered()), this, SLOT(OnAlarmAllAcknowledge()));
		connect(ui.actionPrint, SIGNAL(triggered()), this, SLOT(OnPrint()));
		connect(ui.actionExport, SIGNAL(triggered()), this, SLOT(OnExport()));
		connect(ui.actionComments, SIGNAL(triggered()), this, SLOT(OnAlarmDisplayComment()));
		connect(ui.actionEvent_Viewer, SIGNAL(triggered()), this, SLOT(OnEventsViewer()));

		//Menu actions
		connect(ui.m_actionPrint, SIGNAL(triggered()), this, SLOT(OnPrint()));
		connect(ui.m_actionExport, SIGNAL(triggered()), this, SLOT(OnExport()));
		connect(ui.m_actionExit, SIGNAL(triggered()), this, SLOT(OnAppExit()));
		connect(ui.m_actionFilter, SIGNAL(triggered()), this, SLOT(OnViewShowFilters()));
		//connect(ui.m_actionToolBar, SIGNAL(triggered()), this, SLOT(OnExport())); 
		//connect(ui.m_actionStatus_Bar, SIGNAL(triggered()), this, SLOT(OnExport()));
		connect(ui.m_actionAcknowledge, SIGNAL(triggered()), this, SLOT(OnAlarmAcknowledge()));
		connect(ui.m_actionRelated_Schematic, SIGNAL(triggered()), this, SLOT(OnAlarmSchematic()));
		connect(ui.m_actionRelated_Inspector, SIGNAL(triggered()), this, SLOT(OnAlarmInspector()));
		connect(ui.m_actionRelated_Events, SIGNAL(triggered()), this, SLOT(OnAlarmEvents()));
		connect(ui.m_actionRelated_Plan, SIGNAL(triggered()), this, SLOT(OnAlarmPlan()));
		connect(ui.m_actionRelated_Plan_Instance, SIGNAL(triggered()), this, SLOT(OnAlarmPlanInstance()));
		connect(ui.m_actionPostpone_Plan, SIGNAL(triggered()), this, SLOT(OnAlarmCancelPlan()));
		connect(ui.m_actionRaise_Service_Request, SIGNAL(triggered()), this, SLOT(OnAlarmJobRequest()));
		connect(ui.m_actionRelated_Avalanche_Events, SIGNAL(triggered()), this, SLOT(OnRelatedAvalancheEvents()));
		connect(ui.m_actionCreate_Incident_Report, SIGNAL(triggered()), this, SLOT(OnAlarmEventLog()));
		connect(ui.m_actionAbout_Alarm_Manager_Help, SIGNAL(triggered()), this, SLOT(OnHelp()));
		connect(ui.m_actionAbout_Alarm_Manager, SIGNAL(triggered()), this, SLOT(OnAppAbout()));
		connect(ui.m_actionComments, SIGNAL(triggered()), this, SLOT(OnAlarmDisplayComment()));

		connect(ui.cboFilter, SIGNAL(currentIndexChanged(int)), this, SLOT(OnSelchangeNamedFilter(int))); // named filter

		// Custom connection
		connect(this, SIGNAL(alarmCountUpdate(int, int)), this, SLOT(OnUpdateAlarmCount(int, int)));
	}
 
	void AlarmsGuiDlg::initializeAlarmList()
	{
		setCurrentDescriptionText(tr("Loading of alarms is in progress."));

		TA_Base_Bus::Filter preFilter;
		std::vector<unsigned long> locations;    
		initPreFiltering(preFilter, locations); 

		TA_Base_Bus::AlarmActionFactory* factory = 
		new TA_Base_Bus::AlarmActionFactory(initListControlActions());

		TA_Base_Bus::Filter filter;
		long sortColumn=0;
		if(RunParams::getInstance().isSet(RPARAM_KRTCALARMSORTING))
		{
			sortColumn=TA_Base_Bus::COL_SOURCETIMESTAMP;
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "sort by COL_SOURCETIMESTAMP");		
		}
		else
		{
			sortColumn=TA_Base_Bus::COL_STATE;
			//m_alarmList->startAlarmList(*this, m_alarmColumns, TA_Base_Bus::COL_STATE, true);
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugInfo, "sort by COL_STATE");		
		}	 
		std::string sessionId = TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID);
		m_alarmList->startAlarmList( *factory,locations,preFilter,filter, sortColumn, false,sessionId);
	 
	}

	void AlarmsGuiDlg::populateNamedFilters(std::string curName)
	{
		// Make sure the combo box is empty.
		ui.cboFilter->blockSignals(true);
		ui.cboFilter->clear();
		
		std::list<std::string> filterList = m_realPModel->getFilterList();
		// Always put the Default Filter name as first item
		std::list<std::string>::iterator namedFilterIt;
		for (namedFilterIt = filterList.begin(); namedFilterIt != filterList.end(); namedFilterIt++)
		{
			ui.cboFilter->addItem((*namedFilterIt));
		}
		// Check for present of curName for Default & User Defined
		if (curName != TA_Base_App::AlarmFilter::FILTER_DEFAULT && curName != TA_Base_Core::NamedFilter::UNNAMED)
		{
			//++TD16109
			if (RunParams::getInstance().get(AlarmManagerService::RPARAM_FILTERONASSET.c_str()) != "")
			{
				ui.cboFilter->addItem(curName);
			}
			else if (RunParams::getInstance().get(AlarmManagerService::RPARAM_FILTERONENTITY.c_str()) != "")
			{
				ui.cboFilter->addItem(curName);
			}
			//TD16109++

			//Set the curName as the name in the combo box
			int index = ui.cboFilter->findText(curName.c_str());
			ui.cboFilter->setCurrentIndex(index);
		}
		// Set the name in the combox box as Default Filter
		else if (curName == TA_Base_App::AlarmFilter::FILTER_DEFAULT)
		{
			int index = ui.cboFilter->findText(TA_Base_App::AlarmFilter::FILTER_DEFAULT.c_str());
			ui.cboFilter->setCurrentIndex(index);
		}
		// Otherwise this must be a User Defined as it is not in the Named Filters database
		else if (curName == TA_Base_Core::NamedFilter::UNNAMED)
		{
			ui.cboFilter->addItem(TA_Base_Core::NamedFilter::UNNAMED.c_str());
			int index = ui.cboFilter->findText(TA_Base_Core::NamedFilter::UNNAMED.c_str());
			ui.cboFilter->setCurrentIndex(index);
		}
		ui.cboFilter->blockSignals(false);
	}

	void AlarmsGuiDlg::OnFilterApply(TRA_WPARAM  wParam, TRA_LPARAM lParam)
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnFilterApply");
		char* pszFilterName = reinterpret_cast<char*>(lParam);
		std::string filterName;
		
		//refresh the filter combo box with default
		populateNamedFilters(TA_Base_App::AlarmFilter::FILTER_DEFAULT);

		// Get the filter index type
		TA_Base_App::AlarmFilter::FilterIndex fIndex = static_cast<TA_Base_App::AlarmFilter::FilterIndex> (wParam);
		switch (fIndex)
		{
		case (TA_Base_App::AlarmFilter::INDEX_DEFAULT) :
			filterName = TA_Base_App::AlarmFilter::FILTER_DEFAULT;
			break;

		case (TA_Base_App::AlarmFilter::INDEX_USER_DEFINED) :
			filterName = TA_Base_Core::NamedFilter::UNNAMED;
			break;

		case (TA_Base_App::AlarmFilter::INDEX_LIST) :
			//Get the index of the Default Filter which is either first or last
			filterName = pszFilterName;
			break;
		}

		// Populate the filter combo box with the current filter name		
		populateNamedFilters(filterName);

		// Reset the caption to show the filter param
		std::string windowText(RunParams::getInstance().get(RPARAM_APPNAME));
		ITransActivePModel* ptempPModel = this->getTransActiveWinApp()->getApplicationController()->getPModel(PMODEL_AlarmFilterDlg);
		AlarmFilterDlgPModel *pFilterPModel = dynamic_cast<AlarmFilterDlgPModel*> (ptempPModel);
		std::string filterText(pFilterPModel->getCurrentFilterString());

		if (0 < filterText.length() && filterName != TA_Base_App::AlarmFilter::FILTER_DEFAULT)//TD15055

		{
			windowText += filterText;
		}
		
		this->setWindowTitle(windowText.c_str());
		if (pszFilterName != 0)
		{
			delete [] pszFilterName;
			pszFilterName = 0;
		}
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnFilterApply");
	}

	void AlarmsGuiDlg::OnFilterDelete(TRA_WPARAM  wParam, TRA_LPARAM lParam)
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnFilterDelete");

		// Confirm there is delete of the filter
		TA_Base_App::AlarmFilter::FilterIndex fIndex = static_cast<TA_Base_App::AlarmFilter::FilterIndex> (wParam);
		if (fIndex == TA_Base_App::AlarmFilter::INDEX_LIST)
		{
			// Refresh the filter combo box
			// Always go back to Default when a filter is deleted
			populateNamedFilters(TA_Base_App::AlarmFilter::FILTER_DEFAULT);
		}

		// Otherwise do nothing
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionExit, "OnFilterDelete");
	}

	void AlarmsGuiDlg::OnFilterUpdate(TRA_WPARAM  wParam, TRA_LPARAM lParam)
	{
		LOG(SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry, "OnFilterUpdate");

		// Refresh the ComboBox
		std::string  strFilterName = ui.cboFilter->currentText().toStdString();
		populateNamedFilters(strFilterName);
	}

	void AlarmsGuiDlg::notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args /*= NULL*/)
	{
		if (_stricmp(updateType, EV_SET_ALARM_FILTER) == 0)
		{
			FilterEventArgs* evArgs = dynamic_cast<FilterEventArgs*>(args);
			m_alarmList->setFilter(*evArgs->getFilter(), evArgs->isDefault());
		}
		else if (_stricmp(updateType, EV_SYSTEM_STATUS) == 0 || _stricmp(updateType, EV_SESSION_CHANGED) == 0)
		{
			PrefilterChangeEventArgs* evArgs = dynamic_cast<PrefilterChangeEventArgs*>(args);
			m_alarmList->updatePreFilterLocationsAndSession(*evArgs->getPreFilter(), evArgs->getLocations(), evArgs->getSessionID());
		}
		else if (_stricmp(updateType, EV_SERVER_STATUS) == 0)
		{
			ServerStatusEventArgs* pEvent = dynamic_cast<ServerStatusEventArgs*>(args);
			if (pEvent->getServerState() == SERVER_DOWN)
			{
				DisableButtons();
				//m_alarmList->deleleteAllItems();
				TA_Base_Bus::TransActiveMessage userMsg;
				userMsg.showMsgBox(IDS_UI_040028);
			}
			else
			{
				TA_Base_Bus::TransActiveMessage userMsg;
				userMsg.showMsgBox(IDS_UI_040029);
			}
		}
		else if (_stricmp(updateType, EV_FILTER_ON_ASSET) == 0)
		{
			FilterEventArgs* pEvArgs = dynamic_cast<FilterEventArgs*>(pEvArgs);
			TA_Base_Bus::Filter filter = *pEvArgs->getFilter();
			m_alarmList->setFilter(filter, false);
			populateNamedFilters(RunParams::getInstance().get(RPARAM_FILTERONASSET.c_str()));
		}
		else if (_stricmp(updateType, EV_FILTER_ON_ENTITY) == 0)
		{
			FilterEventArgs* pEvArgs = dynamic_cast<FilterEventArgs*>(pEvArgs);
			TA_Base_Bus::Filter filter = *pEvArgs->getFilter();
			m_alarmList->setFilter(filter, false);
			populateNamedFilters(RunParams::getInstance().get(RPARAM_FILTERONENTITY.c_str()));
		}
	}

	void AlarmsGuiDlg::doNotifyMessage(unsigned long messageID, TRA_WPARAM wParam, TRA_LPARAM lParam)
	{
		FUNCTION_ENTRY("doNotifyMessage");
		switch (messageID)
		{
		case WM_SET_ALARM_FILTER:
			{
				FilterEventArgs* evArgs = reinterpret_cast<FilterEventArgs*>(wParam);
				m_alarmList->setFilter(*evArgs->getFilter(), evArgs->isDefault());
			}
			break;
		case WM_APPLY_FILTER:
			{
				OnFilterApply(wParam, lParam);
			}
			break;
		}

		FUNCTION_EXIT;
	}

	void AlarmsGuiDlg::OnUpdateAlarmCount(int totalUnAcked, int totalAlarms)
	{
		if (m_exceededMaxDisplayAlarm)
		{
			TAUtfString str = TAUtfString::number(totalAlarms) + TAUtfString("+");
			ui.txtTotalAlarms->setText(str.std_str());
		}
		else
			ui.txtTotalAlarms->setText(TAUtfString::number(totalAlarms).std_str());

		ui.txtLdNoAckAlarms->setText(TAUtfString::number(totalUnAcked).std_str());

	}
}
 