
/**
  * The source code in this file is the property of 
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/PRIV_GenericGUI_QT/TA_BASE/transactive/bus/alarm/alarm_list_control/src/alarmlistctrl.h $
  * @author:  Karen Graham
  * @version: $Revision: #6 $
  *
  * Last modification: $DateTime: 2016/11/23 18:37:10 $
  * Last modified by:  $Author: Noel $
  * 
  * This is a read-only list of alarms.
 */

#if !defined(AFX_ALARMLISTCTRL_H__CC35262F_8DDA_4C64_9CAB_E6CF217A2343__INCLUDED_)
#define AFX_ALARMLISTCTRL_H__CC35262F_8DDA_4C64_9CAB_E6CF217A2343__INCLUDED_

//#if _MSC_VER > 1000
//#pragma once
//#endif // _MSC_VER > 1000

#include <vector>
#include <QMenu> 
#include "core/utilities/src/RunParams.h" 
#include "core/exceptions/src/AlarmListCtrlException.h"
#include "core/synchronisation/src/ReEntrantThreadLockable.h" 
#include "bus/generic_gui_view/src/TransActiveLibraryApp.h"
#include "bus/generic_gui_view/src/AbstractTransActiveView.h"
#include "bus/common_control/src/itemview/CtlTableView.h"
#include "bus/alarm/alarm_list_control/src/AlarmRetriever.h"
#include "bus/alarm/alarm_list_control/src/ActionThread.h"
#include "bus\unicode_common_lib\src\TAUnicodeString.h"


using namespace TA_Base_Bus;
namespace TA_Base_Bus
{	
    class Filter;
    class Sort;
    class IAlarmListObserver;
    class AlarmListContextMenu;
    class AlarmActionFactory;
    class IAlarmAction;
    class IAlarmColumn;
    class AlarmListModel;
    class AlarmListTableModel;
 

    class AlarmListCtrl :  public CtlTableView 
		                  ,public TransActiveLibraryApp 
						  ,public TA_Base_Bus::AbstractTransActiveView
    {
        Q_OBJECT

		friend class AlarmRetriever;

    public:
		// AbstractTransActiveView

		virtual std::string getMyPModelName();
		virtual void notifyUpdate(const char * updateType, TA_Base_Bus::EventArgs * args=NULL);
        virtual void setupPModel();
	   
 
		 virtual IApplicationController * createApplicationController();
        /**
         * Constructor
         *
         * @param EViewMode - This is a quick way to set up which columns should be displayed and their sizes.
         * @param canColumnsBeSorted - This indicates if the columns can be sorted by clicking on the column headers.
         *                             If this is set to false then no column sorting will be allowed.
         * @param canColumnsBeReordered - This indicates if the columns can be reordered by dragging the column headers.
         *                                If this is set to false then no column reordering will be allowed.
         */
        AlarmListCtrl(bool canColumnsBeSorted = true, bool canColumnsBeReordered = true);
		 
        /**
         * Destructor
         */
        virtual ~AlarmListCtrl();
		virtual void translateUI();

    signals:
		void refresh_signal();
		void restart_signal();
		void remap_signal();
		void unmap_signal();

	public slots:
		//Pop up menu
		void popContextMenu(const QPoint& pos);
		//Handle selected menu
		void triggeredAction(QAction * action);
		
		void onAlarmStoreRefresh();
		void onAlarmStoreRestart();
		void onAlarmStoreRemap();
		void onALarmStoreUnmap();

		void currentRowChanged(const QModelIndex & current, const QModelIndex & previous);

		// Click event
		void onClicked(const QModelIndex & index);

		// Double click event
		void onDoubleClicked(const QModelIndex & index);

		void valueChanged(int value);

		/**
		* updateObserver
		*
		* This updates the observer with the number of new alarms that have come in and the highest
		* severity so it is able to play a sound if necessary
		*
		* @param bool - This indicates if any alarms were modified. If alarms were modified then the
		*               observer needs to be told the new alarm severity to play a sound for.
		* @param bool - This indicates if any alarms were added. If new alarms came in, then the observer
		*               may need to unsilence the alarm sound.
		* @param IAlarmStoreCorbaDef::IndexInfo - The details retrieved from the alarm store on the number
		*                                         of alarms etc.
		* @param long - The number of alarms currently selected.
		* @param bool - The number of alarms stored is more than the max allowable for display i.e. display
		*               has been trimmed to the max_display_alarms
		*/
		void updateObserver(bool wereAlarmsModified, bool wereAlarmsAdded,
			TA_Base_Core::IAlarmStoreCorbaDef::IndexInfo& details,
			unsigned long selectedAlarmCount);

	public:   //CtlTableView

		//get header click event for sorting
		virtual void sectionClickedEvent(int column, Qt::SortOrder order);
 

       /**
         * startAlarmList (TD17081)
         *
         * This must be called after the alarm list has been constructed and initialized
         * and start updating the list with what it should be showing 
         *
         * @param AlarmActionFactory& - A factory holding all the valid alarm actions to appear in the list
         *                              context menu. The caller must tidy this object up.
         * @param Filter - This is the prefilter to apply. This must not be changed regularly and should just
         *                 filter out alarms that this list does not need to see. Eg filter on a particular
         *                 profile. This would only change when the user changed their login.
         * @param vector<unsigned long> - A vector of location keys. This lets the list know which alarms it
         *                                should be displaying. For all locations the vector should be empty.
         * @param Filter - This is any active filters the user wants applied. This filter can be changed regularly.
         * @param sessionId - If set, the session ID the alarm should use to determine if an alarm is
         *                    acknowledgeable (and therefore requires an alarm sound).
         *
		 * @param isHighestSeverityNeeded - if true, the alarm store will return the highest un-acknowledged alarm severity
		 *									to the alarm list ctrl when the alarm list ctrl retrieve the alarm index details
         * @exception AlarmListCtrlException - This is thrown if an unrecoverable error occurred that means
         *                                     the alarm list will not be able to show any alarms.
         */
	   virtual void startAlarmList( AlarmActionFactory& alarmActionFactory,
									const std::vector<unsigned long>& locations,
									const Filter& preFilter,                                  
									const Filter& filter,                                    
									long sortColumn,
									bool isAscendingSort,
									const std::string& sessionId = "",
									bool isHighestSeverityNeeded = false);

       /**
         * setupAlarmList (TD17081)
         *
         * This initializes the alarm list after it has been constructed, and before the alarms are loaded
		 * to the list. This sets the alarm columns, and sort 
         *
         * @param IAlarmListObserver& - This will be used by the list to let an application know when
         *                              to enable/disable options and to tell it the current status
         *                              of the list etc.
         * @param std::vector<IAlarmColumn*> - All the columns to insert in the order they should be inserted. The
         *                                     caller is responsible for deleting the column objects. If a column
         *                                     is disabled it can be added into any position in the list as it
         *                                     will be ignored until it is enabled.      
         * @param bool - Indicates whether the sort is enable or not        
         */
	   virtual void setupAlarmList( IAlarmListObserver& observer, 
									const std::vector<IAlarmColumn*> &columns,  
                                    bool canColumnsBeReordered);

        /**
         * updatePreFilterLocationsAndSession
         *
         * This will update the prefilter, locations and sessionID the alarm store contains. Note that setting
         * this affects all alarm store users on the same machine.
         *
         * @param Filter - This is the prefilter to apply. This must not be changed regularly and should just
         *                 filter out alarms that this list does not need to see. Eg filter on a particular
         *                 profile. This would only change when the user changed their login.
         * @param vector<unsigned long> - A vector of location keys. This lets the list know which alarms it
         *                                should be displaying. For all locations the vector should be empty.
         * @param sessionID - The session ID the alarm store should use to determine the ackability of alarms.
         */
        virtual void updatePreFilterLocationsAndSession( const Filter& preFilter,
                                                  const std::vector<unsigned long>& locations,
                                                  const std::string& sessionID);
     

        /**
         * enableColumn
         *
         * This will display or hide the specified column. It can be called before or after startAlarmList().
         *
         * @param long - The column to show or hide
         * @param bool - True to view the column, false to hide it
         * @param int - The position the column should be inserted in in the list control. This only needs to be
         *              set if the column is being enabled.
         */
        //virtual void enableColumn(long column, bool isEnabled, unsigned int position = 0);

 
        /**
         * setFilter
         *
         * This sets a new filter on the alarm store. This must be called after a successful
         * call to startAlarmList().
         *
         * @param Filter - The new filter to set
         *
         * @exception AlarmListCtrlException - Thrown if the filter is invalid
         */
        virtual void setFilter(const Filter& filter,bool isDefault);
 
        void defaultSort();
  
        /**
         * executeAction
         *
         * This will perform the action associated with the command id passed in. This method
         * will detect any errors and display an error message to the user.
         *
         * @param UINT - The command id of the action to perform 
         */
        virtual void executeAction(unsigned long  commandId);
  
		void doPreAction();

        unsigned long getEarliestAlarmTime();

        unsigned long getLatestAlarmTime();
		
		TAUtfString getAlarmComment(int item);

        /**
         * isDeterminateColumn
         *
         * Determines the column name and column ID is paired 
         */
		bool isDeterminateColumn(const long columnID, TAUtfString& strColumnCaption);

    private:
		AlarmListCtrl(const AlarmListCtrl&);		 		 
        
        /**
         * updateDisplay
         *
         * The alarm display is updated periodically. This method is called every UPDATE_SLEEP_IN_MILLISECONDS
         * milliseconds to update the display
         */
        void updateDisplay();

        /**
         * updateObserverWithActionAvailability
         *
         * This updates the observer with information about each action and whether it is currently 
         * available or not.
         */
        void updateObserverWithActionAvailability();

        /**
         * updateObserverWithDescription
         *
         * This updates the observer with an appropriate description to show in a status bar or the like.
         *
         * @param long - The number of alarms currently selected.
         */
        void updateObserverWithDescription(unsigned long selectedAlarmCount, bool valid);
	
		// Process the multiple selection using mouse event
		void multiselectAlarmItem(int nTargetIndex);
		// Process the multiple selection using keyboard events
		void multiselectAlarmItem(int nTargetIndex, unsigned long nKeyPressed);

		bool eventFilter(QObject* object, QEvent* event);

	protected:

		/**
		* getRightsChecker
		*
		* This will return the object that allows rights to be checked
		*
		* @return RightsChecker& - The object to use to determine if the user can perform a certain action
		*/
		virtual RightsChecker& getRightsChecker();

		/**
		* getDatabaseCache
		*
		* This will return the object that caches database information for faster performance
		*
		* @return DatabaseCache& - The object to use to retrieve database information
		*/
		virtual DatabaseCache& getDatabaseCache();

    private:
		TAUtfString NO_VALUE;
		TAUtfString NO_ALARM_STORE_MSG;    // UE-040038
		TAUtfString NO_ALARMS_MATCH_FILTER_MSG;

        // These are all the alarm actions available to the user such as acking, closing, running plans etc
        AlarmActionFactory* m_alarmActionFactory;
 
        // This is used by alarm actions to retrieve alarm details and to perform their tasks
        AlarmRetriever m_alarmRetriever;
       
           // This is the alarm list observer we can callback to tell when things change etc
        IAlarmListObserver* m_observer;
  
		AlarmListModel*  m_alarmListModel;
		AlarmListTableModel*  m_tableModel;

		ActionThread*  m_actionThread;

		//Menu
		QMenu* m_popupMenu;
		 
		// This flag will serve as a guard to determine if its the first time to use shift key
		bool m_bHasSelected;

		// This variable holds the current position
		long m_nCurSelPos;

		// This variable will hold the currently selected alarm Item
		std::string m_strSelectedAlarmID;

		struct rangeSelInfo{
			long lSelStart;
			long lSelEnd;

			rangeSelInfo() : lSelStart(0), lSelEnd(0) {}
		};

		// Holds the record of the current selection index
		// This is used to check the if the previous range selection is the same as the current selection
		rangeSelInfo m_rangeSelInfo;

		// This variable is a flag to check if the last operation was range selection.
		bool m_isRangeSel;

		// This variable will be used the current selection direction for arrow up and down to functionality
		int m_nSelDirection; //  -1 = UP | 1 = DOWN  | 0 = NEUTRAL

		rangeSelInfo m_rangePrevious;

		bool m_bIgnoreEvent;
    };    
	 
}
#endif // !defined(AFX_ALARMLISTCTRL_H__CC35262F_8DDA_4C64_9CAB_E6CF217A2343__INCLUDED_)
