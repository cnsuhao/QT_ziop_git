/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/alarm/alarm_manager/src/AlarmsGuiDlg.h $
  * @author:  Ripple
  * @version: $Revision: #6 $
  *
  * Last modification: $DateTime: 2016/11/22 14:12:39 $
  * Last modified by:  $Author: Noel $
  *
  */
// AlarmsGuiDlg.h : header file
//

#if !defined(AFX_ALARMSGUIDLG_H__F9076B67_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_)
#define AFX_ALARMSGUIDLG_H__F9076B67_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_


#include <set>
 
#include "bus/generic_gui_pmod/src/PModelInterface.h"

//#include "bus\generic_gui_entity\src\IGUIAccess.h"
#include "bus\user_settings\src\SettingsMgr.h"
#include "bus/generic_gui_view/src/TransActiveDialog.h"
//#include "bus/generic_gui_view/src/ISystemStateUpdateObserver.h"

#include "core\utilities\src\RunParams.h"
#include "core\synchronisation\src\ReEntrantThreadLockable.h"

#include "core\alarm\IDL\src\AlarmAgentCorbaDef.h"
#include "core\alarm\IDL\src\AlarmMessageCorbaDef.h"
#include "bus/alarm/alarm_list_control/src/AlarmListCtrl.h"
#include "bus\alarm\alarm_list_control\src\IAlarmListObserver.h"
#include "bus\alarm\alarm_list_control\pmodel\src\Filter.h"
#include "app\alarm\alarm_manager\GeneratedFiles\ui_alarmGUIDlg.h"
#include "bus\unicode_common_lib\src\TAUnicodeString.h"

/////////////////////////////////////////////////////////////////////////////
// AlarmsGuiDlg dialog
namespace TA_Base_App
{
	class AlarmGUIDlgModel;
	class AlarmFilterDlg;

	class AlarmsGuiDlg :
		public TA_Base_Bus::AbstractTransActiveDialog, 
		public TA_Base_Bus::IAlarmListObserver

	{
		Q_OBJECT
	// Construction
	public:
	 
	  /**
		* Constructor
		* 
		* This constructs an instance of AlarmsGuiDlg.
		* Preconditions  - ITransActiveApp pointer passed in is not null
		*
		* @param ITransActiveApp* controlClass - This is a pointer to a constructed TransActiveApp
		* 
		*/
		AlarmsGuiDlg(TA_Base_Bus::ITransActiveApp* app);
		~AlarmsGuiDlg();

	public:  // IAlarmListObserver interface
		/*
		* updateAlarmCounts
		*
		* This method is the concrete implementation for the IAlarmListComponentObserver. The
		* method updates the total and unacknowledged alarm counters.
		*
		* @param The total alarms count.
		*
		* @param The total unacknowledged alarms count.
		*/
		virtual void updateAlarmCounts(unsigned int totAlarms, unsigned int totUnacknowledgedAlarms, bool exceededMaxDisplayAlarm, bool isLoadingAlarms);

		/*
		* updateAlarmSound
		*
		* This method is the concrete implementation for the IAlarmListComponentObserver. The
		* method updates the alarm severity sound to be played. 
		*
		* @param The severity tone to update as an unsigned long.
		* @param True if new alarms were inserted, and false otherwise.
		*/
		virtual void updateAlarmSound(unsigned long severity, bool wereAlarmsAdded);

		/**
		* updateAvailableAlarmActions
		*
		* This will pass in the current availability of each alarm action. This will be called whenever
		* the alarm selection changes or the current alarms change.
		*
		* @param ItemAvailability - This should pass the availability of every possible alarm action currently
		*                           being used.
		*/
		virtual void updateAvailableAlarmActions(const ItemAvailability& availability);

		/**
		* setCurrentDescriptionText
		*
		* This will pass some appropriate text to be shown by the observer in a status bar or the like.
		* This string will hold the description of the selected alarm. If multiple alarms are selected it
		* will indicate how many alarms are selected
		*
		* @param CString - The text to be displayed
		*/
		virtual void setCurrentDescriptionText(const TAUtfString& description);

		/**
		* notifyNoMatchingAlarms
		* 
		* This will be called if no alarms match the current filter. 
		*/
		virtual void notifyNoMatchingAlarms();

		/**
		* notifyFilterIndexReset
		*
		* This will be called if alarmlist control is notified of by alarm store to unmap 
		* so filter will be set to default
		*/
		virtual void notifyFilterReset();

		//virtual bool init();

		
	public:  //ITransActiveView interface
	 std::string  getMyPModelName(void);
	 void setupPModel(void);	  
	 // For dynamic language switching
	 virtual void translateUI();
	 virtual void doNotifyMessage(unsigned long messageID, TRA_WPARAM wParam, TRA_LPARAM lParam);

	protected:
  
		/*
		 * Print
		 *
		 * Enables the alarm list to be printed. This laucnhes a standard MS 
		 * printing dialog. 
		 */
		void Print();

		/*
		 * Save
		 *
		 * Enables the alarm list to be saved as .CSV file. This laucnhes a standard MS 
		 * Save As dialog.
		 */
		void Save();

		/*
		 * DisableButtons
		 *
		 * This method disables menus, buttons on the dialog. 
		 */
		void DisableButtons();

		virtual void initUIControl();

	public slots:  //menu and toolbar
		void OnAppExit();
	    void OnPrint();
		void OnExport();
		void OnAppAbout();		
	    void OnHelp();		 
		void OnAlarmAcknowledge();
		void OnAlarmAllAcknowledge();
		void OnAlarmClose();
		void OnAlarmDisplayComment();
		void OnAlarmSchematic();
		void OnAlarmPlan();
		void OnAlarmInspector();
	    void OnAlarmTrend();
		void OnRelatedAvalancheEvents();
		void OnAlarmJobRequest();
		//TD 13825 Add postpone dss menu command
	    void OnAlarmCancelPlan(); //POSTPONE_DSS
		void OnAlarmEventLog(); //INCIDENT_LOG_SHEET
		void OnAlarmEvents();  //related event
		void OnAlarmPlanInstance();		 
		//TD 11783 Event Viewer button is always enabled via toolbar, and will launch EventViewer
		 void OnEventsViewer();
		 void OnViewStatusBar() {};
		 void OnViewToolbar() {};
		 void OnDefaultSort();
		 void OnViewInhibitAlarmByDescription();
		 void OnViewInhibitAlarmByType();
		 void OnViewInhibitAlarms();
		 void OnViewInhibitions();
		 void OnSelchangeNamedFilter(int index);
		 void OnViewShowFilters();

		 void OnUpdateAlarmCount(int totalUnAcked, int totalAlarms);

	signals:
		 void alarmCountUpdate(int totalUnAcked, int totalAlarms);

	private:
	   
		void initilization();
	 	
		void setUpConnectionPoints();

		/*
		 * initListControlActions
		 *
		 * Initialises the alarm list control actions.
		 *
		 * @return A vector of alarm actions.
		 */
		std::vector<TA_Base_Bus::IAlarmAction*> initListControlActions();

		/*
		 * initListControlColumns
		 *
		 * Initialises the alarm list control columns.
		 */
		void initListControlColumns();
	 

		/** 
		  * initPreFiltering
		  *
		  * Creates a Filter object and a vector of locations based on
		  * the active session, so that they can be passed to the alarm
		  * list control.
		  *
		  * @param preFilter  The pre-filter to be applied.
		  * @param locations  The locations we wish to display alarms from.
		  *
		  */
		void initPreFiltering(TA_Base_Bus::Filter& preFilter, 
			std::vector<unsigned long>& locations);

	    void initializeAlarmList();

		/**
		* populateNamedFilters
		*
		* Populates the named filter combo box with the current list of named filters.
		*
		* @param string - The current filter name used in the alarm control list and to be displayed
		*/
		void populateNamedFilters(std::string curName);

		void OnFilterApply(TRA_WPARAM  wParam, TRA_LPARAM lParam);
		void OnFilterDelete(TRA_WPARAM  wParam, TRA_LPARAM lParam);
		void OnFilterUpdate(TRA_WPARAM  wParam, TRA_LPARAM lParam);

		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args = NULL);

	private:

		Ui_AlarmGUIDlg ui;

		//Disable the copy constructor and assignment operator so the 
		//compiler won't create its own version. 
		/*AlarmsGuiDlg(const AlarmsGuiDlg& theAlarmsGuiDlg);
		AlarmsGuiDlg& operator=(const AlarmsGuiDlg &);*/

		TA_Base_Bus::AlarmListCtrl* m_alarmList;		 
		std::vector<TA_Base_Bus::IAlarmColumn*> m_alarmColumns;
		
		AlarmGUIDlgModel* m_realPModel;
		bool m_exceededMaxDisplayAlarm;
		bool m_isLoadingAlarms;
	};
 
}
#endif // !defined(AFX_ALARMSGUIDLG_H__F9076B67_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_)
