/**
  * The source code in this file is the property of
  * Ripple Systems and is not for redistribution
  * in any form.
  *
  * Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/event/event_viewer/pmodel/src/.h $
  * @author:  
  * @version: $Revision:  $
  *
  * Last modification: $DateTime:  $
  * Last modified by:  $Author:  $
  *
  * 
  *
  */

#ifndef COMBINED_EVENT_FILTER_DLG_PMODEL_1234567890_H
#define COMBINED_EVENT_FILTER_DLG_PMODEL_1234567890_H

#include "app/event/event_viewer/pmodel/src/CombinedEventFilter.h"
#include "app/event/event_viewer/pmodel/src/DatabaseAccess.h"
#include  "app/event/event_viewer/pmodel/src/ConstantItems.h"
#include "app/event/event_viewer/pmodel/src/EVDisplayStrings.h"
#include  "app/event/event_viewer/pmodel/src/IATSComWrapper.h"

#include "bus/generic_gui_pmod/src/transactiveviewmodel.h"


#include "core/data_access_interface/src/NamedFilter.h"

namespace TA_Base_App
{
    class CombinedEventFilterPModel : public TA_Base_Bus::AbstractPModel
    {
	public:
		CombinedEventFilterPModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName);
		virtual ~CombinedEventFilterPModel(void);

		void Close();
		void apply();
		void onOK();
		void intialSetup(std::vector<TA_Base_App::ATSSystemItem> ATSSystemsVector,std::vector<ISCSSubsystemItem> ISCSsubsystemsVector);
		
		std::string getFilterName();
		void setNamedFilter();

		/**
     * populateFilterCriteria
     *
     * set the whole FilterCiteria by the current filter.
	 * 
	 * @param filter: the filterCriteria which will be set.
     */
	void populateFilterCriteria(TA_Base_App::FilterCriteria & filterCriteria);


	SelectedItems getFilterSystemList();
	SelectedItems getSubSystemList();
	void setFilterTime(unsigned long toTime, unsigned long fromTime);

	void setFilterSystem(SelectedItems selection);
	void setFilterSubsystem( SelectedItems useSelection);
	void setFilterLocation(SelectedItems selection);
	void setFilterOperator(SelectedItems selection);

	void loadDefaultFilter(std::string oldFilterName, bool &enableApplybutton );
	void loadSelectedFilter(std::string filterName, bool& enableButtons); //TODO reduce to one funct
	void saveNamedFilter(std::string filterName, std::string old_filterName);
	void deleteNamedFilter();

	bool isDisableEvents();
	void setFilterName(std::string name);


	SelectedItems getNumericFilter(std::string columId);
	void setNumericFilter(std::string column, SelectedItems selection);
	void setFilterText(std::string column, std::string text);




	/**
	 * populateListStringFromSelection
	 *
	 * Returns the selected items as a comma separated list.
	 *
	 * @param selection The selected items to display in the edit control.
	 * @param items The list ofitems that will map the key to the text.
	 */
	std::string populateListStringFromSelection( const SelectedItems& selection, ListItems& items );

	// reset m_ISCSsubsystems/ m_ATSsubsystems/m_subsystems, reset m_substemEdit to "ALL", reset 3 NumericFilters
	void setSubsystemsBySystemkeys ( SelectedItems ISCSselection);

		/**
     * loadFilter
     *
     * Reads data from the filter object and popultes the dialogs controls with the information.
     */
    void loadFilter( bool importFromFilter );

	void setFilterFromAppController();


	
	// 	// These 3 list are the related subystem selected by system(in filter), which will show on the filter

	ListItems m_subsystems; //( m_subsystems = m_ISCSsubsystems + m_ATSsubsystems)
	ListItems m_ISCSsubsystems;
	ListItems m_ATSsubsystems;
	ListItems m_systems;   // m_system = ISCS's systems + m_atsSystems
	ListItems m_atsSystems; 
	ListItems m_locationKeyDisplayNameList;
	ListItems m_operators;
	ListItems m_alarmtypes;
	ListItems m_alarmState;
	std::vector<std::pair<unsigned long,std::string> > m_alarmSeverities;
	std::vector<TA_Base_App::ATSSystemItem> m_ATSSystemsVector;
	std::vector<ISCSSubsystemItem> m_ISCSsubsystemsVector;



	private:

		
		void loadListDataFromCache();
		void loadLocationKeyDisplayNameListData();
		void loadAlarmStateListData();
		void loadOperatorListData();
		void loadAlarmTypeListData();
		void retrieveSubsystemsBySeletedSystem (SelectedItems selectedSystemKeys);
		void loadSubsystemsListDataAll ();
		void loadSystemsListData ();

		SelectedItems getATSSystemPkeysByNames(TA_Base_Core::NamedFilter::StringList atsSystemsStrList);
	SelectedItems getATSSubsystemPkeysByNames(TA_Base_Core::NamedFilter::StringList atsSubsystemsStrList);
	TA_Base_Core::NamedFilter::StringList getATSSubsystemNamesByPKeys(SelectedItems atsSubsystemPkeys);
	TA_Base_Core::NamedFilter::StringList getATSSystemNamesByPKeys(SelectedItems atsSystemPkeys);
	TA_Base_Core::NamedFilter::StringList getStrVectorFromStrList(list<std::string> strList);
	ListItems getListItmesFromVector(std::vector<std::pair<unsigned long,std::string> > keyNamePairs);

	
		void loadATSsubsystemsBySystemKeys( SelectedItems ISCSselection );

	void showUserMessageNotify(int message, std::string param1, std::string param2);


		
	/**
     * setStringlistToFilterCriteria
     *
     * set the list<string> in FilterCiteria by the current filter.
	 * 
	 * @param listToBeSet: the list to be set.	
	 * @param columnName: the numeric filter name used for retrieving the corresponding data from namedFilter
	 * @param items: the key&value pair list of the whole list for the specified column.
     */
	void setStringlistFromNumericNamedFilter(list<std::string> & listToBeSet, const std::string columnName, ListItems& items);


    /**
     * populateTMTypeFromFilter
     *
     * populate the operator/maintainence radio box from the m_namedFilter
     */
	void populateTMTypeFromFilter (); 

	/**
     * resetStartEndTimeDefined
     *
     * set the StartTimePeriodDefined and endTimePeriodDefined in FilterCriteria, by the check box status on the GUI . 
	 * 
	 * @param filter: the filterCriteria which will be set.
     */
	void resetStartEndTimeDefined( TA_Base_App::FilterCriteria & filter);

	/**
     * getTimeFromNamedFilter
     *
     * Extracts a time value from the named filter.
     *
     * @param timeIdentifier The time being requested.
     *
     * @return time_t The time value.
     */
    time_t getTimeFromNamedFilter( const std::string& timeIdentifier );


    /**
     * enableNamedFilterButtons
     *
     * Enables the 'Save' and 'Delete' buttons associated with the specified named filter as required.
     *
     * @param filterName The name of the filter to set the buttons for.
     */
    void enableNamedFilterButtons( const std::string& filterName );


	void getAllSubsystemKeys(SelectedItems &selection);

	

	
private:



	void SetEditText(std::string columnId, std::string displayText);

		/**
     * populateEditFromFilter
     *
     * Places the selected items into the provided edit control as a comma separated list.
     *
     * @param edit The edit control to place the list into.
     * @param columnId The column whose selection has changed.
     * @param items The list of items that will map the key to the text.
     */
    void populateEditFromFilter( const std::string& columnId, ListItems& items );

		
		// The filter object that contains the data represented by this dialog.
		CombinedEventFilter* m_filter;
		TA_Base_Core::NamedFilter m_namedFilter;



    // This member is used to disable events until after the dialog has been initialised.
    bool m_disableEvents;

	bool m_someFilterChanged; //this will be set to true when some namedfilter is saved or deleted.

	bool m_originalFilterChanged;

	//End-- TD14346
    // This string is used to determine if the filter in use has changed.
    std::string m_originalFilterName;


	};
}
#endif // COMBINED_EVENT_FILTER_DLG_PMODEL_1234567890_H
