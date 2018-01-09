/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/app/system_control/banner/pages/iscs_page/src/AlarmsGuiDlg.h $
  * @author:  Ripple
  * @version: $Revision: #2 $
  *
  * Last modification: $DateTime: 2016/12/14 14:57:33 $
  * Last modified by:  $Author: Noel $
  *
  */
// AlarmsGuiDlg.h : header file
//

#if !defined(AFX_ALARMSGUIDLG_H__F9076B67_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_)
#define AFX_ALARMSGUIDLG_H__F9076B67_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <string>
#include <vector>
#include "globals.h"

#include "app/system_control/banner/pages/iscs_page/src/IscsBannerPage_rc.h"
#include "bus/generic_gui_pmod/src/PModelDataType.h"
#include "bus/alarm/alarm_list_control/src/AlarmListCtrl.h"
#include "bus/alarm/alarm_list_control/src/IAlarmListObserver.h"
#include "bus/banner_framework/src/BannerPage.h"
#include "bus/banner_framework/src/SilenceButton.h"

//#include "bus/mfc_extensions/src/fixed_column_header_ctrl/FixedHeaderCtrl.h"
#include "bus/generic_gui_view/src/AbstractTransActiveView.h"
#include "bus\unicode_common_lib\src\TAUnicodeString.h"

#include "core/synchronisation/src/ReEntrantThreadLockable.h"
#include "core/utilities/src/RunParams.h"

#include "GeneratedFiles/ui_iscsalarmsguidlg.h"
#include <QWidget>
#include <QIcon>
#include <QFont>

/////////////////////////////////////////////////////////////////////////////
// CAlarmsGuiDlg dialog

namespace TA_Base_App
{
	//Forward Declaration
	class CAlarmsGuiDlgPModel;

	class ISCSPAGEDLL_EXPORT CAlarmsGuiDlg
		: public TA_Base_Bus::BannerPage
        , public TA_Base_Bus::IAlarmListObserver
    {
		Q_OBJECT
    public:
        /**
         * Constructor
         */
        CAlarmsGuiDlg::CAlarmsGuiDlg(const std::string& buttonName, int xPos, int yPos, int width, int height,
                                     TA_Base_Bus::BannerFrameworkDialog* parentDialog);

        /**
         * Destructor
         */
        CAlarmsGuiDlg::~CAlarmsGuiDlg();

        /**
         * onButtonDoubleClick
         *
         * This method handles the event when the ISCS button is double clicked. This will
         * launch the Alarm Manager.
         */
        virtual void onButtonDoubleClick();
        
        /**
         * addBannerPage
         *
         * Tells the banner page to add itself to the banner.
         */
        virtual void addBannerPage();
        // Dialog Data
        TA_Base_Bus::SilenceButton	m_silenceButton;
      
		virtual std::string getMyPModelName(){ return ""; };
		void setupPModel();
		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL);
		virtual void initUIControl();

    public slots:
		void OnSilence();
		void OnUpdateAlarmCount(int totalUnAcked, int totalAlarms);

		//afx_msg void OnGetMinMaxInfo(MINMAXINFO FAR* lpMMI);
        //afx_msg void OnClose();
        
	signals:
		void alarmCountUpdate(int totalUnAcked, int totalAlarms);

    private:
        /**
	     * initListControlActions
	     *
	     * Initialises the alarm list control actions.
	     *
	     * @return A vector of alarm actions.
	     */
	    std::vector<TA_Base_Bus::IAlarmAction*> initListControlActions();

	    /**
	     * initListControlColumns
	     *
	     * Initialises the alarm list control columns.
	     */
	    void initListControlColumns();

	    /**
         * updateAlarmCounts
         *
         * This method update the alarm counters on the container class.
	     * 
	     * @param The total number of alarms.
	     *
	     * @param The total number of unacknowledged alarms.
         */
	    virtual void updateAlarmCounts(unsigned int totAlarms, unsigned int totUnacknowledgedAlarms, bool exceededMaxDisplayAlarm, bool isLoadingAlarms);

        /**
        * updateAlarmSound
        * 
        * The virtual method that updates the container's alarm severity tones.
        *
        * @param The severity tone to update as an unsigned long.
        * @param True if new alarms were inserted, and false otherwise.
        */
        virtual void updateAlarmSound(unsigned long severity, bool wereAlarmsAdded);

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
         * setWindowTitle
         *
         * This method tells the alarm container class to update its window's title bar if any.
	     * 
	     * @param The text to be displayed.
         */
	    virtual void setWindowTitle(const std::string& windowText);
		
		/**
		* setUpConnectionPoints
		*/
		void setUpConnectionPoints();
    private:	    
	    static const std::string RPARAM_USE_ALARM_ICON; //TD15156++
        std::vector<TA_Base_Bus::IAlarmColumn*> m_alarmColumns;
        
        TA_Base_Bus::AlarmListCtrl* m_alarmList;
        
		bool m_exceededMaxDisplayAlarm;
		bool m_isLoadingAlarms;
		//UI Separation
		CAlarmsGuiDlgPModel* m_pRealModel;
		Ui::ISCSAlarmsGUIDlg ui;

    protected:
        QIcon m_hIcon;
        QFont m_boldFont;
        static const int ALARM_SOUND_TIMER;
        
       // HACCEL  m_hAccel;
		
    };
}

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_ALARMSGUIDLG_H__F9076B67_5412_11D5_B5EE_0080AD74DCEE__INCLUDED_)
