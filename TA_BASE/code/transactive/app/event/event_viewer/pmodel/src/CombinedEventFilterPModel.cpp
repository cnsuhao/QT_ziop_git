/**
* The source code in this file is the property of
* Ripple Systems and is not for redistribution
* in any form.
*
* Source:   $File: //depot/3001_TIP_NEW/TA_BASE/transactive/app/event/event_viewer/pmodel/src/EventViewerDlgPModel.cpp $
* @author:  
* @version: $Revision:  $
*
* Last modification: $DateTime:  $
* Last modified by:  $Author:  $
*
*
*/
#include "app/event/event_viewer/pmodel/src/CombinedEventFilterPModel.h"
#include "app/event/event_viewer/pmodel/src/GenericEventArgs.h"
#include "app/event/event_viewer/pmodel/src/DatabaseAccess.h"
#include "app/event/event_viewer/pmodel/src/ConstantItems.h"
#include "app/event/event_viewer/pmodel/src/EventViewerDlgPModel.h"

#include "bus/TA_MessageResource/resource.h"

#include "core/exceptions/src/DataException.h"
#include "core/data_access_interface/src/NamedFilterAccessFactory.h"

using TA_Base_Core::DataException;
using TA_Base_Core::NamedFilterAccessFactory;
using TA_Base_Core::NamedFilter;

namespace TA_Base_App
{

	CombinedEventFilterPModel::CombinedEventFilterPModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		: TA_Base_Bus::AbstractPModel(appController, pmodelName),//m_filter(filter),
		m_disableEvents(true),
		m_originalFilterName("")
	{
		m_someFilterChanged = false;
		m_originalFilterChanged = false;		
	}

	CombinedEventFilterPModel::~CombinedEventFilterPModel(void)
	{

	}

	void CombinedEventFilterPModel:: setFilterFromAppController()
	{
		EventViewerDlgPModel * pMainPModel = dynamic_cast <EventViewerDlgPModel *> (m_appController->getPModel(EVENT_VIEWER_DLG_PMODEL));
		m_filter = pMainPModel->getFilter();
		m_originalFilterName = m_filter->getNamedFilter()->getName();
	}

	void CombinedEventFilterPModel::Close()
	{
		m_filter->resumeCurrentListDataLoad();

		if ( !m_someFilterChanged )
		{
			GenericEventArgs * args = new GenericEventArgs();
			unsigned int msg = WM_USER_FILTER_DLG_CANCEL;
			args->Param1 = &(msg);
			notifyViews(INVOKE_POSTMESSGAE_NOTIFICATION.c_str(),args );		
			delete args;
			args = NULL;
		}
		else if ( m_originalFilterChanged )
		{
			onOK(); // will re-apply the current filter because it's been updated. 

		}
		else
		{
			GenericEventArgs * args = new GenericEventArgs();
			unsigned int msg = WM_USER_FILTER_DLG_CLOSED;
			args->Param1 = &(msg);
			notifyViews(INVOKE_POSTMESSGAE_NOTIFICATION.c_str(),args );	
			delete args;
			args = NULL;
		}
	}

	std::string CombinedEventFilterPModel::getFilterName()
	{
		return m_namedFilter.getName();
	}

	void CombinedEventFilterPModel::setNamedFilter()
	{
		// Apply has been clicked. Need to apply the filters.
		m_filter->setNamedFilter( std::auto_ptr<NamedFilter>( m_namedFilter.clone() ) );
	}

	void CombinedEventFilterPModel::apply()
	{
		/// add for 955: 
		FilterCriteria filterCriteria;
		populateFilterCriteria(filterCriteria);  

		if( !filterCriteria.StartTimePeriodDefined && !filterCriteria.EndTimePeriodDefined  )
		{
			filterCriteria.setPrevious1dayForStartTime();
			m_filter->startNewTask(filterCriteria, RealTimeMode);
		}
		else
		{
			filterCriteria.searchDirection = TA_Base_App::Backward;  // this is just the default value. can be deleted
			m_filter->startNewTask(filterCriteria,HistoricalMode);
		}
		//m_filter->setFilterCriteria(filterCriteria); // to save the current FilterCriteria 

		// Then notify the main dialog the filters have changed.
		GenericEventArgs * args = new GenericEventArgs();
		unsigned int msg = WM_USER_FILTER_DLG_APPLY;
		args->Param1 = &(msg);
		notifyViews(INVOKE_POSTMESSGAE_NOTIFICATION.c_str(),args );	
		delete args;
		args = NULL;
	}

	void CombinedEventFilterPModel::onOK()
	{
		m_filter->setNamedFilter( std::auto_ptr<NamedFilter>( m_namedFilter.clone() ) );

		notifyViews(CLEAR_SORT_MASK.c_str());

		// This will atually reset the sort info in DisplayItem which will used in sorting the eventstore list
		DisplayItem::setSortInfo(EVDisplayStrRes::GetInstance()->COLUMN_DATETIME, false); 

		/// start a new task: 
		FilterCriteria filterCriteria;
		populateFilterCriteria(filterCriteria);
		if( !filterCriteria.StartTimePeriodDefined && !filterCriteria.EndTimePeriodDefined  )
		{
			filterCriteria.setPrevious1dayForStartTime();
			m_filter->startNewTask(filterCriteria, RealTimeMode);
		}
		else
		{
			filterCriteria.searchDirection = TA_Base_App::Backward;
			m_filter->startNewTask(filterCriteria,HistoricalMode);
		}
		
		// Then notify the main dialog the filters have changed.
		GenericEventArgs * args = new GenericEventArgs();
		unsigned int msg = WM_USER_FILTER_DLG_CLOSED;
		args->Param1 = &(msg);
		notifyViews(INVOKE_POSTMESSGAE_NOTIFICATION.c_str(),args );	
		delete args;
		args = NULL;
	}


	void CombinedEventFilterPModel::intialSetup(std::vector<TA_Base_App::ATSSystemItem> ATSSystemsVector,std::vector<ISCSSubsystemItem> ISCSsubsystemsVector)
	{
		m_ATSSystemsVector = ATSSystemsVector;
		m_ISCSsubsystemsVector = ISCSsubsystemsVector;
		// Load data required for multiple selections.
		m_namedFilter = *(m_filter->getNamedFilter());

		loadListDataFromCache();  // for subsystem: should be filtered by system selections.

		// Load the filters
		loadFilter( true );
	}

	void CombinedEventFilterPModel::loadListDataFromCache()
	{
		// to do:

		// get ISCS systems pkey/name from DataAccess cache and 
		// get ATS systems pkey/name from EventViewerDlg cache
		// and combined the 2 into (m_systems)
		loadSystemsListData();

		// get ISCS subsystems pkey/name from DataAccess cache and 
		// get ATS subsystems pkey/name from EventViewerDlg cache
		// and combined the 2 into (m_subsystems)
		SelectedItems emptySelected;
		retrieveSubsystemsBySeletedSystem(emptySelected);

		// Retrieve the locations and then populate the locations list.
		loadLocationKeyDisplayNameListData();

		// Retrieve the operators and then populate the operators list.
		loadOperatorListData();

		// load alarmStates to m_alarmState list
		loadAlarmStateListData();

		// Retrieve the alarm types and then populate the alarmtypes list.
		// Currently AlarmType is hidden in 3001 (it's only used in 4669), but for convenience, we still keep the code and prepare data for it.
		// If no requirement in the future, we might delete this part of code(including the code in DatabaseAccess->load4FilterDlg->loadAlarmTypes).
		loadAlarmTypeListData();

		// load alarmSeverities to m_alarmSeverities.
		m_alarmSeverities = DatabaseAccess::getInstance().getSeverities();

	}
	
	void CombinedEventFilterPModel::loadSystemsListData ()
	{
		// clear m_system first:
		m_atsSystems.clear();
		m_systems.clear();

		//load ISCS system list Data
		std::vector< std::pair<unsigned long,std::string> > systems = DatabaseAccess::getInstance().getSystems();
		for ( int i=0; i<(int)systems.size(); i++ )
		{
			ListItem item( systems[i].first, systems[i].second );
			m_systems.push_back( item );
		}
		m_systems.sort();

		//load ATS system list Data from cache EventViewer::m_ATSSystemsVector:		
		std::vector<ATSSystemItem>::iterator itr;
		for(itr =m_ATSSystemsVector.begin(); itr !=m_ATSSystemsVector.end(); itr++)
		{
			ListItem item( itr->pkey, itr->systemName);   // to do: itr->pkey is a negative number, but ListItem is <unstigned long, string>
			m_atsSystems.push_back( item );	
		}

		//combine two list together:
		for (std::list<ListItem>::iterator itr = m_atsSystems.begin(); itr!=m_atsSystems.end(); itr++)
		{
			m_systems.push_back(*itr);
		}

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "getSystems");

	}
	
	void CombinedEventFilterPModel::retrieveSubsystemsBySeletedSystem (SelectedItems selectedSystemKeys)
	{
		// all system are selected:
		if( selectedSystemKeys.size() == 0 ||
			selectedSystemKeys.size() == m_systems.size())
		{
			loadSubsystemsListDataAll();
			return;
		}

		m_ISCSsubsystems.clear();
		m_ATSsubsystems.clear();
		m_subsystems.clear();

		if(selectedSystemKeys.size()>0)
		{
			std::vector<ISCSSubsystemItem>::iterator itr;
			// load ISCS subsystem by selected system key, from EventViewerDlg::m_ISCSsubsystemsVector
			// for each ISCS subsystem ( not filtered)
			for( itr=m_ISCSsubsystemsVector.begin() ; itr!=m_ISCSsubsystemsVector.end(); itr++)  
			{
				unsigned long currentSysKey = itr->system_key;

				for ( int i=0 ;i <(int)selectedSystemKeys.size(); i++) 
				{
					if (selectedSystemKeys[i]== currentSysKey)
					{
						//means this subsytem is under one of the selected system, so should be loaded
						ListItem tempItem(itr->pkey, itr->subsystemName);
						m_ISCSsubsystems.push_back(tempItem);
						break;
					}
				}
			}

			// load ATS subsystem by selected system key, from EventViewerDlg::m_ATSSystemsVector
			vector<ATSSystemItem>::iterator ATS_itr;
			for( ATS_itr=m_ATSSystemsVector.begin() ; ATS_itr!=m_ATSSystemsVector.end(); ATS_itr++)  
			{
				unsigned long currentSysKey = ATS_itr->pkey;

				for ( int i=0 ;i <(int)selectedSystemKeys.size(); i++) 
				{
					if (selectedSystemKeys[i]== currentSysKey)
					{
						//means this subsytem is under one of the selected system, so should be loaded
						std::vector<pair<int , std::string >> tempVector =  ATS_itr->subsystemVector;
						for( int j=0; j<(int)tempVector.size(); j++)
						{
							ListItem tempItem(tempVector[j].first,tempVector[j].second );
							m_ATSsubsystems.push_back(tempItem);
						}
						break;
					}
				}
			}
		}

		m_ISCSsubsystems.sort();

		//combined into m_subsystems
		ListItems::iterator ListItems_itr;
		for ( ListItems_itr=m_ISCSsubsystems.begin(); ListItems_itr!= m_ISCSsubsystems.end(); ListItems_itr++)
		{
			m_subsystems.push_back(*ListItems_itr);
		}

		for ( ListItems_itr=m_ATSsubsystems.begin(); ListItems_itr!= m_ATSsubsystems.end(); ListItems_itr++)
		{
			m_subsystems.push_back(*ListItems_itr);
		}

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "loadSubsystemsListDataBySeleted");
	}

	// should be called only when selectedSystems has no content.
	void CombinedEventFilterPModel::loadSubsystemsListDataAll ()
	{
		m_ISCSsubsystems.clear();
		m_ATSsubsystems.clear();
		m_subsystems.clear();
		std::vector<ISCSSubsystemItem>::iterator itr;
		// load ISCS subsystem by selected system key, from EventViewerDlg::m_ISCSsubsystemsVector
		// for each ISCS subsystem ( not filtered)
		for( itr=m_ISCSsubsystemsVector.begin() ; itr!=m_ISCSsubsystemsVector.end(); itr++)  
		{
			ListItem tempItem(itr->pkey, itr->subsystemName);
			m_ISCSsubsystems.push_back(tempItem);
		}
		// load ATS subsystem by selected system key, from EventViewerDlg::m_ATSSystemsVector
		vector<ATSSystemItem>::iterator ATS_itr;
		for( ATS_itr=m_ATSSystemsVector.begin() ; ATS_itr!=m_ATSSystemsVector.end(); ATS_itr++)  
		{
			std::vector<pair<int , std::string >> tempVector =  ATS_itr->subsystemVector;
			for( int j=0; j<(int)tempVector.size(); j++)
			{
				ListItem tempItem(tempVector[j].first,tempVector[j].second );
				m_ATSsubsystems.push_back(tempItem);
			}
		}
		m_ISCSsubsystems.sort();
		//combined into m_subsystems
		ListItems::iterator ListItems_itr;
		for ( ListItems_itr=m_ISCSsubsystems.begin(); ListItems_itr!= m_ISCSsubsystems.end(); ListItems_itr++)
		{
			m_subsystems.push_back(*ListItems_itr);
		}
		for ( ListItems_itr=m_ATSsubsystems.begin(); ListItems_itr!= m_ATSsubsystems.end(); ListItems_itr++)
		{
			m_subsystems.push_back(*ListItems_itr);
		}
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "loadSubsystemsListDataAll");
	}
	
	void CombinedEventFilterPModel::loadLocationKeyDisplayNameListData ()
	{
		std::vector< std::pair<unsigned long,std::string> > locations = DatabaseAccess::getInstance().getLocationKeyDispNamePairs();
		for (int i=0; i<(int)locations.size(); i++ )
		{
			ListItem item( locations[i].first, locations[i].second );
			m_locationKeyDisplayNameList.push_back( item );
		}
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "getLocations");
	}

	void CombinedEventFilterPModel::loadOperatorListData()
	{
		std::vector< std::pair<unsigned long,std::string> > operators = DatabaseAccess::getInstance().getOperators();
		for (int i=0; i<(int)operators.size(); i++ )
		{
			ListItem item( operators[i].first, operators[i].second );
			m_operators.push_back( item );
		}
		m_operators.sort();
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "getOperators");
	}	
	void CombinedEventFilterPModel::loadAlarmTypeListData()
	{
		std::vector< std::pair<unsigned long,std::string> > alarmtypes = DatabaseAccess::getInstance().getAlarmTypes();
		for (int i=0; i<(int)alarmtypes.size(); i++ )
		{
			ListItem item( alarmtypes[i].first, alarmtypes[i].second );
			m_alarmtypes.push_back( item );
		}
		m_alarmtypes.sort();
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "getAlarmTypes");
	}
	void CombinedEventFilterPModel::loadAlarmStateListData()
	{
		std::vector< std::pair<unsigned long,std::string> > alarmStates = DatabaseAccess::getInstance().getAlarmStates(); 
		for (int i=0; i<(int)alarmStates.size(); i++ )
		{
			ListItem item( alarmStates[i].first, alarmStates[i].second );
			m_alarmState.push_back( item );
		}
		m_alarmState.sort();
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "getAlarmStates");
	}

	
	time_t CombinedEventFilterPModel::getTimeFromNamedFilter( const std::string& timeIdentifier )
	{
		time_t time = 0;
		// Get the time.
		NamedFilter::NumericList timeList = m_namedFilter.getNumericFilter( timeIdentifier );
		if ( 0 < timeList.size() )
		{
			time = static_cast<time_t>(timeList[0]);
		}
		return time;
	}

	SelectedItems CombinedEventFilterPModel::getATSSystemPkeysByNames(NamedFilter::StringList atsSystemsStrList)
	{
		SelectedItems atsPkeyList;
		for(int i=0; i<(int)atsSystemsStrList.size(); i++)
		{
			for(std::list<ListItem>::iterator itr = m_atsSystems.begin(); itr!=m_atsSystems.end(); itr++)
			{
				if ( itr->str.compare(atsSystemsStrList[i]) ==0)
				{
					atsPkeyList.push_back(itr->pkey);
					break;
				}
			}
		}
		return atsPkeyList;
	}
	
	std::string CombinedEventFilterPModel::populateListStringFromSelection( const SelectedItems& selection, ListItems& items )
	{
		FUNCTION_ENTRY("populateEditFromSelection");

		std::string listStr("");

		if ( 0 == selection.size() )
		{
			// If nothing is selected ALL are allowed.
			listStr = EVDisplayStrRes::GetInstance()->NAMEDFILTER_DEFAULT;
		}
		else
		{
			// Otherwise allow only what is currently selected.
			SelectedItems::const_iterator selectionIt;
			for ( selectionIt=selection.begin(); selectionIt!=selection.end(); selectionIt++ )
			{
				// Get the string associated with the selected item.
				ListItem item( *selectionIt, "" );
				ListItems::iterator it = std::find( items.begin(), items.end(), item );

				if ( items.end() != it )
				{
					// Add it to the string.
					listStr += it->str;
				}
				else
				{
					// What should we do in this unlikely event? Just ignore it for now.
				}

				// Add a comma if there are still more remaining.
				if ( selection.end() != ( selectionIt+1 ) )
				{
					listStr += ", ";
				}
			}
		}

		return listStr;

		// Finally, set the text.
		//edit.SetWindowText(listStr.c_str());

		FUNCTION_EXIT;
	}
	
	void CombinedEventFilterPModel::populateEditFromFilter( const std::string& columnId, ListItems& items )
	{
		FUNCTION_ENTRY("populateEditFromFilter");

		SelectedItems selection = m_namedFilter.getNumericFilter( columnId );
		std::string displayStr = populateListStringFromSelection( selection, items );		
		SetEditText(columnId,displayStr);
		FUNCTION_EXIT;
	}

	void CombinedEventFilterPModel::SetEditText(std::string columnId, std::string displayText)
	{
		GenericEventArgs * args = new GenericEventArgs();
		std::string str1(columnId);
		args->Param1 = &(str1); //-------------TODO check deletion of this arg1
		args->Param2 = &(displayText);
		notifyViews(SET_WINDOW_TEXT.c_str(),args );
		delete args;
		args = NULL;
	}

	SelectedItems CombinedEventFilterPModel::getFilterSystemList()
	{
		SelectedItems systems = m_namedFilter.getNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_ISCS_SYSTEM );
		NamedFilter::StringList systemsStrListOld = m_namedFilter.getStringFilter( EVDisplayStrRes::GetInstance()->FILTER_SYSTEM_STR );
		SelectedItems atsSystemsPkey = getATSSystemPkeysByNames(systemsStrListOld);
		systems.insert(systems.end(), atsSystemsPkey.begin(), atsSystemsPkey.end());
		return systems;
	}

	SelectedItems CombinedEventFilterPModel::getATSSubsystemPkeysByNames(NamedFilter::StringList atsSubsystemsStrList)
	{
		SelectedItems atsPkeyList;
		for(int i=0; i<(int)atsSubsystemsStrList.size(); i++)
		{
			for(std::list<ListItem>::iterator itr = m_ATSsubsystems.begin(); itr!=m_ATSsubsystems.end(); itr++)
			{
				if ( itr->str.compare(atsSubsystemsStrList[i]) ==0)
				{
					atsPkeyList.push_back(itr->pkey);
					break;
				}
			}
		}

		return atsPkeyList;
	}


	SelectedItems CombinedEventFilterPModel::getSubSystemList()
	{
		// Get the subsystems ids
		NamedFilter::NumericList subsystemIDs = m_namedFilter.getNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_APPLICATION_SUBSYSTEM );
		NamedFilter::NumericList physicalSubsystems = m_namedFilter.getNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_PHYSICAL_SUBSYSTEM );
		NamedFilter::StringList atsSubsystemsStrList = m_namedFilter.getStringFilter( EVDisplayStrRes::GetInstance()->FILTER_SUBSYSTEM_STR );
		SelectedItems atsSubsystemsPkey = getATSSubsystemPkeysByNames(atsSubsystemsStrList);
		subsystemIDs.insert( subsystemIDs.end(), physicalSubsystems.begin(), physicalSubsystems.end() );
		subsystemIDs.insert( subsystemIDs.end(), atsSubsystemsPkey.begin(), atsSubsystemsPkey.end() );
		/*if(subsystemIDs.size()<m_subsystems.size())*/
		std::string subsystemDisplayStr;
		if(atsSubsystemsStrList.size() <= 0)
		{
			return subsystemIDs;			
		}
		else
		{
			NamedFilter::NumericList emptyList;
			return emptyList;
		}
	}

	void CombinedEventFilterPModel::loadFilter( bool importFromFilter )
	{
		FUNCTION_ENTRY("loadFilter");

		// If we need to load by importing the named filter from the filter object.
		if ( importFromFilter )
		{
			// Copy the current filter to our internal filter.
			m_namedFilter = *(m_filter->getNamedFilter());
		}

		// Disable events during load.
		m_disableEvents = true;

		// Now Get the to time.
		//CTime toTime( getTimeFromNamedFilter( Filter::FILTER_TOTIME ) );
		bool isToCheck = false;
		time_t toTime =  getTimeFromNamedFilter( Filter::FILTER_TOTIME );
		if ( Filter::CURRENT_TIME != toTime )
		{
			isToCheck = true;
		}
	
		bool isFromCheck = false;
		// Get the from time from the filter object and set the date and time selection boxes accordingly
		time_t fromTime =  getTimeFromNamedFilter( Filter::FILTER_FROMTIME );
		if ( Filter::PREVIOUS_DAY != fromTime )
		{
			isFromCheck = true;
		}

		GenericEventArgs * args = new GenericEventArgs();
		args->Param1 = &(isToCheck);
		args->Param2 = &(toTime);
		args->Param3 = &(isFromCheck);
		args->Param4 = &(fromTime);
		notifyViews(SET_TIME_CONTROLS.c_str(),args );	
		delete args;
		args = NULL;


		// populate systemEdit...
		NamedFilter::NumericList systemIDs = getFilterSystemList();

		std::string displayString = populateListStringFromSelection( systemIDs, m_systems );
		SetEditText(SYSTEM_EDIT,displayString);
		retrieveSubsystemsBySeletedSystem(systemIDs); 

		/*populateSubsystemEdit();*/
		SelectedItems subsystems = getSubSystemList();
		std::string subsystemDisplayStr = populateListStringFromSelection( subsystems,m_subsystems );
		SetEditText(SUB_SYSTEM_EDIT,subsystemDisplayStr);


		// And now do it all again for location...
		populateEditFromFilter( EVDisplayStrRes::GetInstance()->FILTER_LOCATION, m_locationKeyDisplayNameList );

		// Same for operator...
		populateEditFromFilter( EVDisplayStrRes::GetInstance()->FILTER_OPERATOR, m_operators );

		// Same for event type(alarm state)...
		populateEditFromFilter( EVDisplayStrRes::GetInstance()->FILTER_ALARMSTATE, m_alarmState );

		// TMTYPE : set radio box status by m_namedFilter
		std::string tmtype;
		if(m_namedFilter.getStringFilter( EVDisplayStrRes::GetInstance()->FILTER_TMTYPE ).size()> 0)
		{
			tmtype = m_namedFilter.getStringFilter( EVDisplayStrRes::GetInstance()->FILTER_TMTYPE )[0];
		}
		GenericEventArgs * args3= new GenericEventArgs();
		args3->Param1 = &(tmtype);
		notifyViews(POPULATE_TMTYPE.c_str(),args3 );	
		delete args;
		args = NULL;

		// And now the asset and description
		std::vector<std::string> names;
		names = m_namedFilter.getStringFilter( EVDisplayStrRes::GetInstance()->FILTER_ASSET );
		if ( 0 == names.size() )
		{
			names.push_back( "" );
		}
		//m_assetEdit.SetWindowText( names[0].c_str() );
		SetEditText(EVDisplayStrRes::GetInstance()->FILTER_ASSET,names[0]);

		names = m_namedFilter.getStringFilter( EVDisplayStrRes::GetInstance()->FILTER_DESCRIPTION );
		if ( 0 == names.size() )
		{
			names.push_back( "" );
		}
		//m_descriptionEdit.SetWindowText( names[0].c_str() );
		SetEditText(EVDisplayStrRes::GetInstance()->FILTER_DESCRIPTION,names[0]);

		//Severity
		notifyViews(SET_SEVERITY_COMBOX_BOX.c_str() );	

		// Alarm Type
		populateEditFromFilter( EVDisplayStrRes::GetInstance()->FILTER_ALARMTYPE, m_alarmtypes );

		// Alarm Id
		names = m_namedFilter.getStringFilter( EVDisplayStrRes::GetInstance()->FILTER_ALARMID );
		if (0 < names.size())
		{
			//m_alarmIdEdit.SetWindowText( names[0].c_str() );
			SetEditText(EVDisplayStrRes::GetInstance()->FILTER_ALARMID,names[0]);
		}
		else
		{
			SetEditText(EVDisplayStrRes::GetInstance()->FILTER_ALARMID,EVDisplayStrRes::GetInstance()->ALARM_FILTER_MESSAGE);
		}

		// Now that the data is loaded events can be enabled.
		m_disableEvents = false;

		//GetFilterData();   //TD14346
		notifyViews(GET_FILTER_DATA.c_str());
		FUNCTION_EXIT;
	}

	void CombinedEventFilterPModel::setFilterTime(unsigned long toTime, unsigned long fromTime)
	{
		// reset the 'to'/'from' time to the named filter.
		m_namedFilter.removeNumericFilter( Filter::FILTER_TOTIME );
		m_namedFilter.removeNumericFilter( Filter::FILTER_FROMTIME );

		m_namedFilter.addNumericFilter( Filter::FILTER_FROMTIME, fromTime);		
		m_namedFilter.addNumericFilter( Filter::FILTER_TOTIME, toTime);
	}

	void CombinedEventFilterPModel::getAllSubsystemKeys(SelectedItems &selection)
	{
		selection.clear();
		for(std::list<ListItem>::iterator itr=m_subsystems.begin(); itr!=m_subsystems.end(); itr++)
		{
			selection.push_back(itr->pkey);
		}
	}


	void CombinedEventFilterPModel::setFilterSubsystem( SelectedItems useSelection)
	{
		// Clear the filters.
        m_namedFilter.removeNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_APPLICATION_SUBSYSTEM );
        m_namedFilter.removeNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_PHYSICAL_SUBSYSTEM );
		m_namedFilter.removeStringFilter( EVDisplayStrRes::GetInstance()->FILTER_SUBSYSTEM_STR);

		// if selection.size==0
		SelectedItems selectedSystems = m_namedFilter.getNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_ISCS_SYSTEM ); 
		if( useSelection.size() == 0 && selectedSystems.size()>0)
		{
			SelectedItems allSubsystems,appSubsystems,physicalSubsystems, atsSubsystems;
			getAllSubsystemKeys(allSubsystems);
			DatabaseAccess::getInstance().separateSubsystems( allSubsystems, appSubsystems, physicalSubsystems, atsSubsystems );

			// Apply the filters.
			if ( !appSubsystems.empty() )
			{
				m_namedFilter.setNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_APPLICATION_SUBSYSTEM, appSubsystems );
			}
			if ( !physicalSubsystems.empty() )
			{
				m_namedFilter.setNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_PHYSICAL_SUBSYSTEM, physicalSubsystems );
			}
// 			if( !atsSubsystems.empty() )
// 			{
// 				NamedFilter::StringList atsSubsystemStrList = getATSSubsystemNamesByPKeys(atsSubsystems);
// 				m_namedFilter.setStringFilter( EVDisplayStrRes::GetInstance()->FILTER_ATS_SUBSYSTEM_STR, atsSubsystemStrList );
// 			}
		}
		else
		{
			// Need to distinguish between application and physical subsystems.
			NamedFilter::NumericList subsystems,physicalSubsystems,atsSubsystemPkeys;
			DatabaseAccess::getInstance().separateSubsystems( useSelection, subsystems, physicalSubsystems, atsSubsystemPkeys );

			// Apply the filters.
			if ( !subsystems.empty() )
			{
				m_namedFilter.setNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_APPLICATION_SUBSYSTEM, subsystems );
			}
			if ( !physicalSubsystems.empty() )
			{
				m_namedFilter.setNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_PHYSICAL_SUBSYSTEM, physicalSubsystems );
			}
// 			if( !atsSubsystemPkeys.empty() )
// 			{
// 				NamedFilter::StringList atsSubsystemNames = getATSSubsystemNamesByPKeys(atsSubsystemPkeys);
// 				m_namedFilter.setStringFilter( EVDisplayStrRes::GetInstance()->FILTER_ATS_SUBSYSTEM_STR, atsSubsystemNames );
// 			}
			NamedFilter::StringList subsystemStrListNew;
			SelectedItems::const_iterator selectionIt;
			for ( selectionIt=useSelection.begin(); selectionIt!=useSelection.end(); selectionIt++ )
			{
				// Get the string associated with the selected item.
				ListItem item( *selectionIt, "" );
				ListItems::iterator it = std::find( m_subsystems.begin(), m_subsystems.end(), item );

				if ( m_subsystems.end() != it )
				{
					// Add it to the string.
					subsystemStrListNew.push_back(it->str);
				}

			}

			if(!subsystemStrListNew.empty())
			{
				m_namedFilter.setStringFilter(EVDisplayStrRes::GetInstance()->FILTER_SUBSYSTEM_STR,subsystemStrListNew);
			}
		}
	}

	SelectedItems CombinedEventFilterPModel::getNumericFilter(std::string columId)
	{
		return m_namedFilter.getNumericFilter( columId );
	}

	
	ListItems CombinedEventFilterPModel::getListItmesFromVector(std::vector<std::pair<unsigned long,std::string> > keyNamePairs)
	{
		ListItems listToRet;
		for (int i=0; i<(int)keyNamePairs.size(); i++ )
		{
			ListItem item( keyNamePairs[i].first, keyNamePairs[i].second );
			listToRet.push_back( item );
		}
		return listToRet;
	}

	//set list<string>: System Names, subsystem names, operator names, location names to Filter Criteria
	void CombinedEventFilterPModel::setStringlistFromNumericNamedFilter( list<std::string> & listToBeSet, const std::string columnName, ListItems & items)
	{
		// set system names to filterCriteria:
		SelectedItems selection = m_namedFilter.getNumericFilter( columnName );
		if ( 0 != selection.size() )
		{
			SelectedItems::const_iterator selectionIt;
			for ( selectionIt=selection.begin(); selectionIt!=selection.end(); selectionIt++ )
			{
				// Get the string associated with the selected item.
				ListItem item( *selectionIt, "" );
				ListItems::iterator it = std::find( items.begin(), items.end(), item );

				if ( items.end() != it )
				{
					// Add it to the string.
					listToBeSet.push_back(it->str);
				}
				else
				{
					// What should we do in this unlikely event? Just ignore it for now.
				}
			}
		}
	}

	NamedFilter::StringList CombinedEventFilterPModel::getStrVectorFromStrList(list<std::string> strList)
	{
		NamedFilter::StringList strVector;
		for(list<std::string>::iterator itr=strList.begin();itr!=strList.end(); itr++)
		{
			strVector.push_back( *itr);
		}
		return strVector;
	}

	void CombinedEventFilterPModel::setFilterLocation(SelectedItems selection)
	{
		// If selection changed, update selection and repopulate edit control.
		m_namedFilter.removeNumericFilter(EVDisplayStrRes::GetInstance()->FILTER_LOCATION);
		m_namedFilter.removeStringFilter(EVDisplayStrRes::GetInstance()->FILTER_LOCATION_STR);
		m_namedFilter.setNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_LOCATION, selection );
		list<std::string> locationNames;
		ListItems locationKeyNameList = getListItmesFromVector(DatabaseAccess::getInstance().getLocationsKeyNamePairs());
		setStringlistFromNumericNamedFilter( locationNames, EVDisplayStrRes::GetInstance()->FILTER_LOCATION, locationKeyNameList );
		NamedFilter::StringList locationNamesStrList = getStrVectorFromStrList(locationNames);
		m_namedFilter.setStringFilter(EVDisplayStrRes::GetInstance()->FILTER_LOCATION_STR, locationNamesStrList);
	}
	
	void CombinedEventFilterPModel::setFilterOperator(SelectedItems selection)
	{
		// If selection changed, update selection and repopulate edit control.
		m_namedFilter.removeNumericFilter(EVDisplayStrRes::GetInstance()->FILTER_OPERATOR);
		m_namedFilter.removeStringFilter(EVDisplayStrRes::GetInstance()->FILTER_OPERATOR_STR);
        m_namedFilter.setNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_OPERATOR, selection );
		list<std::string> operatorNames;
		setStringlistFromNumericNamedFilter( operatorNames, EVDisplayStrRes::GetInstance()->FILTER_OPERATOR,m_operators );
		NamedFilter::StringList operatorStrList = getStrVectorFromStrList(operatorNames);
		m_namedFilter.setStringFilter(EVDisplayStrRes::GetInstance()->FILTER_OPERATOR_STR, operatorStrList);
	}


	void CombinedEventFilterPModel::setFilterSystem(SelectedItems selection)
	{
				// Clear the filters.
		m_namedFilter.removeNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_ISCS_SYSTEM );
		m_namedFilter.removeStringFilter( EVDisplayStrRes::GetInstance()->FILTER_SYSTEM_STR );

		// set filter: FILTER_ISCS_SYSTEM
		SelectedItems ISCS_selection;
		for (int i=0;i<(int)selection.size(); i++)
		{
			// if the selection is not an ATS's pkey, then keep it to filter:FILTER_ISCS_SYSTEM
			ListItem item( selection[i], "");
			ListItems::iterator it = std::find( m_atsSystems.begin(), m_atsSystems.end(), item );
			if(m_atsSystems.end() == it)
			{
				ISCS_selection.push_back(selection[i]);
			}
		}
		if (ISCS_selection.size()>0)
		{
			m_namedFilter.setNumericFilter(EVDisplayStrRes::GetInstance()->FILTER_ISCS_SYSTEM, ISCS_selection);
		}

		// set filter: FILTER_SYSTEM_STR
	    NamedFilter::StringList systemStrListNew;
		SelectedItems::const_iterator selectionIt;
		for ( selectionIt=selection.begin(); selectionIt!=selection.end(); selectionIt++ )
		{
			// Get the string associated with the selected item.
			ListItem item( *selectionIt, "" );
			ListItems::iterator it = std::find( m_systems.begin(), m_systems.end(), item );

			if ( m_systems.end() != it )
			{
				// Add it to the string.
				systemStrListNew.push_back(it->str);
			}

		}
		if(!systemStrListNew.empty())
		{
			m_namedFilter.setStringFilter(EVDisplayStrRes::GetInstance()->FILTER_SYSTEM_STR,systemStrListNew);
		}
	}
	// reset m_ISCSsubsystems/ m_ATSsubsystems/m_subsystems, reset m_substemEdit to "ALL", reset 3 NumericFilters
	void CombinedEventFilterPModel::setSubsystemsBySystemkeys ( SelectedItems selectedSystemKeys)
	{
		//reload m_ISCSsubsytems, m_atsSubsystems, m_subsystem 
		retrieveSubsystemsBySeletedSystem(selectedSystemKeys);

		// Populate the edit control : show 'all' in subsystemEdit
		SelectedItems selection;
		//populateEditFromSelection( m_subsystemEdit, selection, m_subsystems);
		std::string subsystemDisplayStr = populateListStringFromSelection( selection,m_subsystems );
		SetEditText(SUB_SYSTEM_EDIT,subsystemDisplayStr);

		// Clear the filters:
		m_namedFilter.removeNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_APPLICATION_SUBSYSTEM );
		m_namedFilter.removeNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_PHYSICAL_SUBSYSTEM );
		m_namedFilter.removeStringFilter( EVDisplayStrRes::GetInstance()->FILTER_SUBSYSTEM_STR );

		// reset the namedFilter:
		if(selectedSystemKeys.size()>0 )  //some systems has been selected:
		{
			SelectedItems subsystems,appSubsystems,physicalSubsystems, atsSubsystems;
			getAllSubsystemKeys(subsystems);
			DatabaseAccess::getInstance().separateSubsystems( subsystems, appSubsystems, physicalSubsystems, atsSubsystems );

			// Apply the filters.
			if ( !appSubsystems.empty() )
			{
				m_namedFilter.setNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_APPLICATION_SUBSYSTEM, appSubsystems );
			}
			if ( !physicalSubsystems.empty() )
			{
				m_namedFilter.setNumericFilter( EVDisplayStrRes::GetInstance()->FILTER_PHYSICAL_SUBSYSTEM, physicalSubsystems );
			}
	// 		if( !atsSubsystems.empty() )
	// 		{
	// 			NamedFilter::StringList atsSubsystemsStrList = getATSSubsystemNamesByPKeys (atsSubsystems);
	// 			m_namedFilter.setStringFilter( EVDisplayStrRes::GetInstance()->FILTER_ATS_SUBSYSTEM_STR, atsSubsystemsStrList );
	// 		}

		}
	}

	void CombinedEventFilterPModel::setNumericFilter(std::string column, SelectedItems selection)
	{
		m_namedFilter.removeNumericFilter(column );
		m_namedFilter.setNumericFilter( column, selection );
	}

	void CombinedEventFilterPModel::loadDefaultFilter(std::string oldFilterName, bool &enableApplybutton )
	{		
		std::auto_ptr<NamedFilter> loadedFilter;

		// Trigger the filter load.
		loadedFilter = NamedFilterAccessFactory::getInstance().getNamedFilter( EVDisplayStrRes::GetInstance()->NAMEDFILTER_DEFAULT );

		//as per TD 14762
		//we don't want to enable Apply button
		//if Default filter was already selected
		//so redo part of what enableButtons() does
		//here before the combo box is 
		//populated with new values
		std::string defaultFilterName;		
		defaultFilterName = loadedFilter->getName ();	
		if (oldFilterName.compare (defaultFilterName) == 0)
		{
			enableApplybutton = false;
		}

		// Copy the new filter to our internal filter.
		m_namedFilter = *(loadedFilter.get());

		// Force the filter dialog to show the new filter.
		loadFilter( false);		
	}

	void CombinedEventFilterPModel::loadSelectedFilter(std::string filterName, bool& enableButtons)
	{
		 // Make sure the same filter wasn't selected.
        if ( filterName != m_namedFilter.getName() )
        {
            std::auto_ptr<NamedFilter> loadedFilter;
			// Load the filter.
			loadedFilter = NamedFilterAccessFactory::getInstance().getNamedFilter( filterName );

			// Copy the new filter to our internal filter.
            m_namedFilter = *(loadedFilter.get());

            // Force the filter dialog to show the new filter.
            loadFilter( false );

			enableButtons = true;
        }
	}

	bool CombinedEventFilterPModel::isDisableEvents()
	{
		return m_disableEvents;
	}

	void CombinedEventFilterPModel::setFilterText(std::string column, std::string text)
	{
		m_namedFilter.removeStringFilter( column );
		if ( text.size() != 0 ) ///-- TODO check
		{
			m_namedFilter.addStringFilter( column, std::string(text) );
		}
	}

	void CombinedEventFilterPModel::saveNamedFilter(std::string filterName, std::string old_filterName)
	{
		// Set the name of the current filter to the name specified.
		m_namedFilter.setName(filterName );

		// Save the filter.
		m_namedFilter.saveNamedFilter();

		m_someFilterChanged = true;
		if(old_filterName.compare(m_originalFilterName) == 0)
		{
			m_originalFilterChanged = true;
		}		
	}

	void CombinedEventFilterPModel::deleteNamedFilter()
	{		
		// Store the old filter name so it can be used for comparison later.
		std::string oldFilterName = m_namedFilter.getName();

		// Delete the current named filter.
		m_namedFilter.deleteNamedFilter();

		// If the filter was the active one it also needs to be renamed.
		if ( oldFilterName == m_filter->getNamedFilter()->getName() )
		{
			// TD12035: Cannot just rename, we must now revert to default filter.
			/*      m_filter->setNamedFilter( std::auto_ptr<NamedFilter>( m_namedFilter.clone() ) );*/
			m_filter->getNamedFilter()->setName(EVDisplayStrRes::GetInstance()->USER_DEFINED_NAMED_FILTER_NAME); //---- to do string resource
			m_originalFilterChanged = true;
		}
		
		m_someFilterChanged= true;		
	}


	void CombinedEventFilterPModel::setFilterName(std::string name)
	{
		m_namedFilter.setName( name );
	}
















	




	NamedFilter::StringList CombinedEventFilterPModel::getATSSystemNamesByPKeys(SelectedItems atsSystemPkeys)
	{
		NamedFilter::StringList atsNameList;
		for(int i=0; i<(int)atsSystemPkeys.size(); i++)
		{
			for(std::list<ListItem>::iterator itr = m_atsSystems.begin(); itr!=m_atsSystems.end(); itr++)
			{
				if ( itr->pkey == atsSystemPkeys[i] )
				{
					atsNameList.push_back(itr->str);
					break;
				}
			}
		}

		return atsNameList;
	}

	NamedFilter::StringList CombinedEventFilterPModel::getATSSubsystemNamesByPKeys(SelectedItems atsSubsystemPkeys)
	{
		NamedFilter::StringList atsNameList;
		for(int i=0; i<(int)atsSubsystemPkeys.size(); i++)
		{
			for(std::list<ListItem>::iterator itr = m_ATSsubsystems.begin(); itr!=m_ATSsubsystems.end(); itr++)
			{
				if ( itr->pkey == atsSubsystemPkeys[i] )
				{
					atsNameList.push_back(itr->str);
					break;
				}
			}
		}

		return atsNameList;
	}

	void CombinedEventFilterPModel::populateFilterCriteria(TA_Base_App::FilterCriteria & filterCriteria)
	{
		m_filter->populateFilterCriteria(filterCriteria);  // this will populate many criteria of filter

		GenericEventArgs * args = new GenericEventArgs();
		args->Param1 = &(filterCriteria);			///todo CHECK DELETION -- DONT DELETE IT AS IT IS USED BY APP
		notifyViews(RESET_FILTER_TIME.c_str(),args );	


		// set Alarm states to filterCriteria:
		if(filterCriteria.EventTypeEqualOr.size()>0)
		{
			filterCriteria.EventTypeEqualOr.clear();
		}
		setStringlistFromNumericNamedFilter( filterCriteria.EventTypeEqualOr, EVDisplayStrRes::GetInstance()->FILTER_ALARMSTATE, m_alarmState);

		// 	setStringlistFromNumericNamedFilter( filterCriteria.SubsystemNameEqualOr, EVDisplayStrRes::GetInstance()->FILTER_APPLICATION_SUBSYSTEM, m_subsystems);
		// 	setStringlistFromNumericNamedFilter( filterCriteria.SubsystemNameEqualOr, EVDisplayStrRes::GetInstance()->FILTER_PHYSICAL_SUBSYSTEM, m_subsystems);
		// 	//for ATS subsystem names ,just add them directley into filterCriteria.SubsystemNameEqualOr
		// 	NamedFilter::StringList subsystemNames = m_namedFilter.getStringFilter(EVDisplayStrRes::GetInstance()->FILTER_SUBSYSTEM_STR);
		// 	for(int i=0; i<subsystemNames.size(); i++)
		// 	{
		// 		filterCriteria.SubsystemNameEqualOr.push_back(subsystemNames[i]);
		// 	}
		// 
	}

	void CombinedEventFilterPModel::showUserMessageNotify(int message, std::string param1, std::string param2)
	{
		GenericEventArgs * args = new GenericEventArgs();		
		args->Param1 = &(message);
		args->Param1 = &(param1);
		args->Param1 = &(param2);
		notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),args);
		delete args;
		args = NULL;
	}

	








}