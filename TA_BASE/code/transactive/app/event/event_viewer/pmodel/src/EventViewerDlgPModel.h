/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/event/event_viewer/pmodel/src/EventViewerDlgPModel.h $
  * @author:  
  * @version: $Revision:  $
  *
  * Last modification: $DateTime:  $
  * Last modified by:  $Author:  $
  *
  * 
  *
  */

#ifndef EVENT_VIEWERDLG_PMODEL_1234567890_H
#define EVENT_VIEWERDLG_PMODEL_1234567890_H

#include "app/event/event_viewer/pmodel/src/CombinedEventFilter.h"
#include "app/event/event_viewer/pmodel/src/DatabaseAccess.h"
#include "app/event/event_viewer/pmodel/src/IATSComWrapper.h"
#include "app/event/event_viewer/pmodel/src/PopulateEventLevelsThread.h"
#include "app/event/event_viewer/pmodel/src/ApplyRightsThread.h"
#include "app/event/event_viewer/pmodel/src/GenericEventArgs.h"

#include "bus/generic_gui_pmod/src/transactiveviewmodel.h"

#include "core/utilities/src/RunParams.h"
#include "core/data_access_interface/src/NamedFilterAccessFactory.h"

#include <vector>

namespace TA_Base_Bus
{
    class RightsLibrary;
}

namespace TA_Base_App
{
    class EventViewerDlgPModel : public TA_Base_Bus::AbstractPModel, public TA_Base_Core::RunParamUser
    {
	public:
		EventViewerDlgPModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~EventViewerDlgPModel(void);

		void init();

		 //---------- Methods Inherited from RunParamUser --------//
		/**
		 * onRunParamChange
		 *
		 * Called whenever a RunParam is updated during run-time.
		 *
		 * @param name The name of the RunParam that has changed.
		 * @param value The new value of the RunParam.
		 */
		void onRunParamChange(const std::string& name, const std::string& value);
		
		CombinedEventFilter* getFilter();

		void deleteFilter();

		TA_Base_Core::NamedFilterAccessFactory::FilterNameList getFilterNameList();

		std::string getFilterStatusMsg();

		bool IsCurrentTimeInFilter();
		bool IsFilterTOTimeEmpty();
		bool IsSelectedAlarmEvent(int index);
		void next();
		void previous();
		void exportEvents(std::string filename, int itemCount);
		void convertText(std::string& text);
		std::string replaceNonPrintableCharactersWithSpace(const std::string& inputString);
		void launchAlarmManager();
		void pauseLoad();
		void resumeLoad();
		void changeNameFilter();
		void setNamedFilter(TA_Base_Core::NamedFilterAccessFactory::FilterNameList filterList);  //--------TODO check to remove this
		std::string getNameFilterName();
		
		bool IsNotSameFilterSelected(std::string filterName); //--- todo reduce to one
		void loadSelectedFilter(std::string filterName);//--- todo reduce to one

		void startRightsAndEventLevelThreads();

		std::string getCurrentFilterText();
		
		void reloadEvents();

		void setFilterDuty(bool enable)
		{
			  m_filter->setDutyEnabled( enable );
		}
		
		void postMessageNotifyViews(GenericEventArgs *args);
		void showUserMessageNotify(int message, std::string param1 = "", std::string param2 = "");
				
		/**
		* applyRights
		*
		* Applys the current rights settings on any controls that a right dependant.
		*/
		void applyRights();

		void preparaForClose();
		
		void initializePrintColWidth();

		std::string getFilterItemData(const unsigned long index,const std::string& columnName);

		int getCurrentSortColumn();

		bool getCurrentSortOrder();

		void sort(const std::string& columnName, const int columnId);

		void setSelectedItems( std::vector<unsigned long>& selectedItems );

		const std::vector<unsigned long>& getSelectedItems();

		void executeColumnAction(int columnID, std::string columnName);

		void getFilterRowColour( const unsigned long index, COLORREF& foreground, COLORREF& background );

		COLORREF getFilterItemColour(const unsigned long index, const std::string& columnName);

		std::vector<Filter::Column> getFilterColumns();

		COLORREF getBackgroundColour();

		int* getPrintColWidth();
		
		void clearSortMark();
		
		void nextPage();
		
		void previousPage();
		
		void executeAction( const unsigned int actionId );

		bool getColorCodingEnabled()
		{
			return m_colourCodingEnabled;
		}

		void loadColumns();

		// DB cache, will be used by filter dialog, these 2 vecotr has the info of relationship between system and subsystem
		std::vector<ISCSSubsystemItem> m_ISCSsubsystemsVector;
		std::vector<TA_Base_App::ATSSystemItem> m_ATSSystemsVector;

	private:

		/**
		* setFilterFromCommandLine
		*
		* Populates the filter with data provided on the command line.
		*/
		void setFilterFromCommandLine();

		/**
     * tokenizeKeyList
     *
     * Splits the provided list of comma separated keys into a vector of the keys.
     *
     * @param param The parameter to convert.
     * @param keys The list of keys.
     */
    void tokenizeKeyList( const std::string& param, std::vector<unsigned long>& keys );
	
	void loadListDataToCache();
	void loadAndRetrieveATSSystemInfo();
	void retrieveISCSSubsystemsToVector ();

	// Enumeration that lets the event viewer know whether a list update was from a load or a refresh
   /* enum
    {
        UPDATE_LOAD,
        UPDATE_REFRESH
    } m_updateType;*/ //// Not Used

		int m_currentSortColumn;
		bool m_sortAscending;
		int * m_printoutColWidths;	
		
		std::string getCurrentDatabaseString();
		
		CombinedEventFilter * m_filter;
		// The rights library.
		std::auto_ptr<TA_Base_Bus::RightsLibrary> m_rightsLibrary;
		TA_Base_Core::NamedFilterAccessFactory::FilterNameList m_FilterNameList;

		CPopulateEventLevelsThread *m_pPopulateEventLevelsThread;
		CApplyRightsThread  *m_pApplyRightsThread;

		// The current db connection string
		std::string m_currentDbString;

		bool m_colourCodingEnabled;

		// A vector containing the columns
		std::vector<Filter::Column> m_columns;
	};
}
#endif // EVENT_VIEWERDLG_PMODEL_1234567890_H
