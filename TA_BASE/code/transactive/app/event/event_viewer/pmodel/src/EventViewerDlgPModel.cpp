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
#include "app/event/event_viewer/pmodel/src/EventViewerDlgPModel.h"
#include "app/event/event_viewer/pmodel/src/ConstantItems.h"
#include "app/event/event_viewer/pmodel/src/DatabaseAccess.h"
#include "app/event/event_viewer/pmodel/src/NotifyGUIPostMessage.h"
#include "app/event/event_viewer/pmodel/src/ActionThread.h"

#include "bus/security/rights_library/src/RightsLibrary.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"
#include "bus/application_types/src/apptypes.h"
#include "bus/generic_gui_pmod/src/AppLauncher.h"
#include "bus/generic_gui_pmod/src/GenericGuiConstants.h"
#include "bus/TA_MessageResource/resource.h"


#include "core/data_access_interface/entity_access/src/EventViewer.h"
#include "core/data_access_interface/src/DbConnectionStrings.h"
#include "core/data_access_interface/src/DbConnection.h"
#include "core/data_access_interface/src/NamedFilter.h"
#include "core/database/src/SimpleDb.h"
#include "core/exceptions/src/ApplicationException.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/DbConnectionFailed.h"
#include "core/utilities/src/TA_String.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/DateFormat.h"

// Use boost tokenizer for splitting up command line arguments.
#include "boost/tokenizer.hpp"



using TA_Base_Core::NamedFilter;
using TA_Base_Core::NamedFilterAccessFactory;
using TA_Base_Core::DataException;
using TA_Base_Core::RunParams;
using TA_Base_Core::DebugUtil;
using namespace TA_Base_Core;
using namespace EventViewerConstants;
using namespace TA_Base_Bus::TA_GenericGui;

namespace TA_Base_App
{

	EventViewerDlgPModel::EventViewerDlgPModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		: TA_Base_Bus::AbstractPModel(appController, pmodelName),m_pPopulateEventLevelsThread(NULL),m_pApplyRightsThread(NULL)
	{
		m_colourCodingEnabled = false;
		NotifyGUIPostMessage::getInstance().setPModel(this);
	}

	EventViewerDlgPModel::~EventViewerDlgPModel(void)
	{
		// Remove all columns.
		m_columns.clear();

		if (m_pApplyRightsThread != NULL)
		{
			delete m_pApplyRightsThread;
			m_pApplyRightsThread = NULL;
		}
		if (m_pPopulateEventLevelsThread != NULL)
		{
			delete m_pPopulateEventLevelsThread;
			m_pPopulateEventLevelsThread = NULL;
		}

		if(m_filter!=NULL){
			delete m_filter;
			m_filter = NULL;
		}

		NotifyGUIPostMessage::releaseInstance();
	}

	CombinedEventFilter* EventViewerDlgPModel::getFilter()
	{
		return m_filter;
	}

	void EventViewerDlgPModel::init()
	{
		FUNCTION_ENTRY("EventViewerDlgPModel::init");//zhiqiang++
		// Construct the rights library.
		TA_Base_Bus::RightsLibraryFactory rlFactory;
		m_rightsLibrary = std::auto_ptr<TA_Base_Bus::RightsLibrary>( rlFactory.buildRightsLibrary(TA_Base_Bus::DEFAULT_RL, TA_Base_Bus::RightsLibraryFactory::MANAGER) ); 

		// Get the current connection string
		m_currentDbString = getCurrentDatabaseString();

		// Set the colour coding behaviour
		TA_Base_Core::EventViewer* guiEntity = dynamic_cast<TA_Base_Core::EventViewer*>(m_appController->getEntity());

		//bool colourCodingEnabled(false);
		unsigned long maxEvents = 0;
		unsigned long defaultEventLevel = 0;
		unsigned long refreshTime = 5000;
		try
		{
			m_colourCodingEnabled = guiEntity->areAlarmEventsColourCoded();
			maxEvents = guiEntity->getEventLimit();   //20000 
			refreshTime = guiEntity->getEventRefreshTime()*1000;  // 5*1000
			defaultEventLevel = guiEntity->getDefaultEventVisibilityLevel();
		}
		catch(...)
		{
			// If something goes wrong keep what was successful! Everything else
			// will have the defaults values set above.
		}     

		notifyViews(INIT_PAGER_CTRL.c_str());

		// preload the lists used in Filter Dialog.
		loadListDataToCache();

		// Set up the GUI components.
		notifyViews(SETUP_STATUS_BAR.c_str());
		notifyViews(SETUP_TOOLBAR.c_str());
		notifyViews(SETUP_MENU.c_str());


		// Add styles to the list view
		/**/

		//TD17143 - do not call showWindow here as it will be called right after the dialog is initialized
		// ShowWindow(SW_SHOW); 

		// Send a "Size" message to repaint the event list (invalidate doesn't work here for some reason)
		//CRect viewerRect;
		//GetWindowRect(&viewerRect);
		//ScreenToClient(&viewerRect);
		//this->PostMessage(WM_SIZE,viewerRect.Height()-1, viewerRect.Width()-1 );

		//++TD17143
		m_filter = new CombinedEventFilter();
		//m_filter->setFilterID(IDD_COMBINED_EVENT_FILTER);


		// Populate the filter based on command line parameters.
		setFilterFromCommandLine();

		m_filter->setEventLevel( defaultEventLevel );
		m_filter->initEventsCount(maxEvents,refreshTime);
		m_filter->setSortingPriority();
		loadColumns();
		notifyViews(INIT_EVENT_LIST_COMPONENT.c_str());

		// Get a list of the filters.
		m_FilterNameList = NamedFilterAccessFactory::getInstance().getAllFilterNames();


		//initialize 2 handler(for real time & historical) in EventHandlerManager.
		m_filter->initialHandler(); 
		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "The 2 EventHandler(for real-time and historical mode) have been initialized. ");


		//EventHandlerManager.setParameter(), will call EventHandler.startParameter() 
		//which will start a real time/historical searching task.
		FilterCriteria filterCriteria;
		m_filter->populateFilterCriteria(filterCriteria);
		filterCriteria.StartTimePeriodDefined = false;
		filterCriteria.EndTimePeriodDefined = false;
		filterCriteria.setPrevious1dayForStartTime();
		m_filter->startNewTask(filterCriteria,TA_Base_App::RealTimeMode);


		// Start listening for RunParam changes.
		RunParams& runParams = RunParams::getInstance();
		runParams.registerRunParamUser( this, RPARAM_SESSIONID );
		runParams.registerRunParamUser( this, RPARAM_FILTER_SUBSYSTEM.c_str() );
		runParams.registerRunParamUser( this, RPARAM_FILTER_LOCATION.c_str() );
		runParams.registerRunParamUser( this, RPARAM_FILTER_OPERATOR.c_str() );
		runParams.registerRunParamUser( this, RPARAM_FILTER_SYSTEM.c_str() );
		runParams.registerRunParamUser( this, RPARAM_FILTER_ASSET.c_str() );
		runParams.registerRunParamUser( this, RPARAM_FILTER_DESCRIPTION.c_str() );
		runParams.registerRunParamUser( this, RPARAM_FILTER_FROM.c_str() );
		runParams.registerRunParamUser( this, RPARAM_FILTER_TO.c_str() );
		runParams.registerRunParamUser( this, RPARAM_FILTER_SEVERITY.c_str() );
		runParams.registerRunParamUser( this, RPARAM_FILTER_ALARMTYPE.c_str() );
		runParams.registerRunParamUser( this, RPARAM_FILTER_EVENTTYPE.c_str() );
		runParams.registerRunParamUser( this, RPARAM_FILTER_ALARMID.c_str() );
		runParams.registerRunParamUser( this, RPARAM_FILTER_ALARMCHILD.c_str() );
		runParams.registerRunParamUser( this, RPARAM_FILTER_DEFAULT.c_str() );

		// Load the db connection strings
		std::string dbConnectionFile = TA_Base_Core::RunParams::getInstance().get(RPARAM_DBCONNECTIONFILE);
		TA_Base_Core::DbConnectionStrings dbConnectionStrings(dbConnectionFile);

		try
		{
			std::vector<TA_Base_Core::DataConnection> dataConnections = dbConnectionStrings.getConnectionList(TA_Base_Core::Event_Ad, TA_Base_Core::Read);
			std::vector<TA_Base_Core::DataConnection>::iterator iter = dataConnections.begin();
			for(; iter != dataConnections.end(); ++iter)
			{
				std::string tempString = (*iter).first;

				std::ostringstream str;
				str << RPARAM_DBPREFIX << tempString;

				runParams.registerRunParamUser( this, str.str().c_str() );
			}
		}
		catch(const TA_Base_Core::DbConnectionFailed& ex)
		{
			// If there was a failure getting the connection strings, this isn't catastrophic, but it 
			// means that we won't be able to be notified when a database goes down or up.
			LOG_EXCEPTION_CATCH(SourceInfo, "DbConnectionFailed", ex.what());
			LOG_GENERIC(SourceInfo, DebugUtil::DebugWarn, "Cannot listen for database status changes");
		}

		//TD17143 - post update here once everything else is initialized already
		GenericEventArgs *args = new GenericEventArgs();
		UINT message = WM_USER_UPDATE_EVENTFILTERS_RIGHTS;
		int params = 0;
		args->Param1 = &(message);
		args->Param2 = &(params);
		args->Param3 = &(params);
		postMessageNotifyViews(args); 
		delete args;
		args = NULL;


		FUNCTION_EXIT; //zhiqiang++
	}
	void EventViewerDlgPModel::onRunParamChange(const std::string& name, const std::string& value)
	{
		LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "Received new RunParam (%s = %s)", name.c_str(), value.c_str());//TD14337 yezh++

		// We need to reload the list when either:
		// 1) The current database has gone down
		// 2) The primary database has come back up and we are currently using a secondary

		//     std::ostringstream str;
		//     str << RPARAM_DBPREFIX << m_currentDbString;
		//     CString currentStr(str.str().c_str());
		//     if ( currentStr.CompareNoCase(name.c_str()) == 0 )
		//     {
		//         // Want to reload the list when the database being used has gone offline
		//         if ( value == RPARAM_DBOFFLINE )
		//         {
		// 			// Run the Database Connection change in other thread in order to avoid deadlock 
		// 			// caused while registerRunparams in DBstatus class.
		// 			PostMessage(WM_USER_UPDATE_DATABASECONNECTION_STRING);
		// 			return;
		//         }
		//     }

		if( value == RPARAM_DBONLINE || value == RPARAM_DBOFFLINE)
		{
			// Get the current connection
			std::string currentConnection = getCurrentDatabaseString();

			if(currentConnection!="" && m_currentDbString != currentConnection)
			{
				// There is now a new database connection. Reload the list.
				reloadEvents();

				m_currentDbString = currentConnection;
			}
			return;
		}

		if ( RPARAM_SESSIONID == name )
		{
			// If the session changed reapply any right dependant settings
			// and return.
			applyRights();

			m_filter->sessionChanged();
			// liuyu++ TD12737
			reloadEvents();
			// ++liuyu TD12737
			return;
		}

		if ( RPARAM_FILTER_DEFAULT == name ) // set to default filter and return
		{
			try
			{
				// Trigger the filter load.
				std::auto_ptr<NamedFilter> loadedFilter = NamedFilterAccessFactory::getInstance().getNamedFilter( EVDisplayStrRes::GetInstance()->GetInstance()->NAMEDFILTER_DEFAULT );
				m_filter->setNamedFilter( loadedFilter );
			}
			catch ( DataException& ex )
			{
				// Log the exception.
				LOG_EXCEPTION_CATCH( SourceInfo, "DataException", ex.getWhichData() );

				// Display the load error message.
				/*TA_Base_Bus::TransActiveMessage userMsg;
				userMsg << ERROR_LOAD;
				userMsg << ERROR_LOADED;
				userMsg.showMsgBox(IDS_UE_040031);*/ //todo 
				showUserMessageNotify(IDS_UE_040031,EVDisplayStrRes::GetInstance()->GetInstance()->ERROR_LOAD,EVDisplayStrRes::GetInstance()->GetInstance()->ERROR_LOADED);
				return;
			}
		}
		else
		{
			// Get a pointer to the current named filter.
			NamedFilter* namedFilter = m_filter->getNamedFilter();

			// Create the vector used to store the keys.
			std::vector<unsigned long> keyList;

			// Change filter depending on what changed.
			if ( RPARAM_FILTER_SUBSYSTEM == name )
			{
				DatabaseAccess& dbAccess=DatabaseAccess::getInstance();
				std::vector<unsigned long> appSubsystems;
				std::vector<unsigned long> physSubsystems;
				std::vector<unsigned long> atsSubsystems;

				// Set the subsystem filters to the specified values.
				tokenizeKeyList( value, keyList );
				dbAccess.separateSubsystems( keyList, appSubsystems, physSubsystems, atsSubsystems);
				namedFilter->setNumericFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_APPLICATION_SUBSYSTEM, appSubsystems );
				namedFilter->setNumericFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_PHYSICAL_SUBSYSTEM, physSubsystems );

			}
			else if ( RPARAM_FILTER_LOCATION == name )
			{
				// Set the location filter to the specified values.
				tokenizeKeyList( value, keyList );
				namedFilter->setNumericFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_LOCATION, keyList );
			}
			else if ( RPARAM_FILTER_OPERATOR == name )
			{
				// Set the operator filter to the specified values.
				tokenizeKeyList( value, keyList );
				namedFilter->setNumericFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_OPERATOR, keyList );
			}
			else if ( RPARAM_FILTER_SYSTEM == name )
			{
				// Set the system filter to the specified values.
				tokenizeKeyList( value, keyList );
				namedFilter->setNumericFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_ISCS_SYSTEM, keyList );
			}
			else if ( RPARAM_FILTER_EVENTTYPE == name )
			{
				// Set the event type filter to the specified values.
				tokenizeKeyList( value, keyList );
				namedFilter->setNumericFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_EVENTTYPE, keyList );
			}
			else if ( RPARAM_FILTER_ASSET == name )
			{
				// Clear current asset filter and add new.
				namedFilter->removeStringFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_ASSET );
				namedFilter->addStringFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_ASSET, value );
			}
			else if ( RPARAM_FILTER_DESCRIPTION == name )
			{
				// Clear current description filter and add new.
				namedFilter->removeStringFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_DESCRIPTION );
				namedFilter->addStringFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_DESCRIPTION, value );
			}
			else if ( RPARAM_FILTER_FROM == name )
			{
				// Covert value to time_t.
				std::stringstream fromStream( value );
				time_t fromTime;
				if ( ! ( fromStream >> fromTime ).fail() )
				{
					// 0 is a special value.
					if ( 0 == fromTime )
					{
						m_filter->setFromTime( m_filter->getEarliestTime() );
					}
					else
					{
						m_filter->setFromTime( fromTime );
					}
				}
			}
			else if ( RPARAM_FILTER_TO == name )
			{
				// Covert value to time_t.
				std::stringstream toStream( value );
				time_t toTime;
				if ( ! ( toStream >> toTime ).fail() )
				{
					m_filter->setToTime( toTime );
				}
			}
			else if ( RPARAM_FILTER_SEVERITY == name )
			{
				// Set the severity filter to the specified values.
				tokenizeKeyList( value, keyList );
				namedFilter->setNumericFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_SEVERITY, keyList );
			}
			else if ( RPARAM_FILTER_ALARMTYPE == name )
			{
				// Set the alarm type filter to the specified values.
				tokenizeKeyList( value, keyList );
				namedFilter->setNumericFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_ALARMTYPE, keyList );
			}
			else if ( RPARAM_FILTER_ALARMID == name )
			{
				// Clear current alarm id filter and add new.
				namedFilter->removeStringFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_ALARMID );
				namedFilter->addStringFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_ALARMID, value );

				//marvin++
				//TD14666
				LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "Found alarm parameter %s",value.c_str());

				RunParams& runParams = RunParams::getInstance();

				// Check alarm child id
				std::string alarmchildParam( runParams.get( RPARAM_FILTER_ALARMCHILD.c_str() ) );
				if ( 0 < alarmchildParam.size() )
				{

					LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "Found alarm child parameter");

					// get each child id separated by comma
					// and add to the filter for display
					std::vector<std::string>alarmIds;
					gTokeniseCommaSeparatedString(alarmchildParam, alarmIds);
					for(int i=0;i<alarmIds.size();i++)
					{

						LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "Alarm child id %s",alarmIds[i].c_str());

						namedFilter->addStringFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_ALARMID, alarmIds[i] );
					}
					/*CString alarmChildren( alarmchildParam.c_str() );
					int index = 0;
					while (index >= 0)
					{
					std::string alarmchildId;
					index = alarmChildren.Find( _T(','), index);
					if (index > 0) // there is more child id
					{
					alarmchildId = alarmChildren.Left(index).GetBuffer(38);
					// take away the added child id if there are more child from the alarmChildren
					alarmChildren = alarmChildren.Right( alarmChildren.GetLength() - index - 1 );
					}
					else // there is no more child id
					{
					alarmchildId = alarmChildren.GetBuffer(38);
					}

					LOG_GENERIC(SourceInfo, DebugUtil::DebugDebug, "Alarm child id %s",alarmchildId.c_str());

					namedFilter->addStringFilter( CombinedEventFilter::FILTER_ALARMID, alarmchildId );
					}*/
					m_filter->setFromTime( m_filter->getEarliestTime() );
				}
				//++marvin
				//TD14666
			}

		}

		// Update the filter dialog if it exists.
		notifyViews(UPDATE_FILTER_DLG.c_str());
		// set the window title to the changed filter
		notifyViews(SHOW_WINDOW_TEXT.c_str());

		// Trigger a database load.
		reloadEvents();
	}

	void EventViewerDlgPModel::setFilterFromCommandLine()
	{
		RunParams& runParams = RunParams::getInstance();

		// Get a pointer to the current named filter.
		NamedFilter* namedFilter = m_filter->getNamedFilter();

		// Create the vector used to store the keys.
		std::vector<unsigned long> keyList;

		// Check default
		std::string defaultParam( runParams.get( RPARAM_FILTER_DEFAULT.c_str() ) );
		if ( 0 < defaultParam.size() )
		{
			try
			{
				// Trigger the filter load.
				std::auto_ptr<NamedFilter> loadedFilter = NamedFilterAccessFactory::getInstance().getNamedFilter( EVDisplayStrRes::GetInstance()->GetInstance()->NAMEDFILTER_DEFAULT );
				m_filter->setNamedFilter( loadedFilter );
			}
			catch ( DataException& ex )
			{
				// Log the exception.
				LOG_EXCEPTION_CATCH( SourceInfo, "DataException", ex.getWhichData() );

				// Display the load error message.
				showUserMessageNotify(IDS_UE_040031,EVDisplayStrRes::GetInstance()->GetInstance()->ERROR_LOAD,EVDisplayStrRes::GetInstance()->GetInstance()->ERROR_LOADED);
			}

			return;
		}

		// Check subsystems
		std::string subsystemParam( runParams.get( RPARAM_FILTER_SUBSYSTEM.c_str() ) );
		if ( 0 < subsystemParam.size() )
		{
			DatabaseAccess& dbAccess=DatabaseAccess::getInstance();
			std::vector<unsigned long> appSubsystems;
			std::vector<unsigned long> physSubsystems;
			std::vector<unsigned long> atsSubsystems;

			// Set the subsystem filters to the specified values.
			tokenizeKeyList( subsystemParam, keyList );
			dbAccess.separateSubsystems( keyList, appSubsystems, physSubsystems,atsSubsystems );
			namedFilter->setNumericFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_APPLICATION_SUBSYSTEM, appSubsystems );
			namedFilter->setNumericFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_PHYSICAL_SUBSYSTEM, physSubsystems );

		}

		// Check locations
		std::string locationParam( runParams.get( RPARAM_FILTER_LOCATION.c_str() ) );
		if ( 0 < locationParam.size() )
		{
			tokenizeKeyList( locationParam, keyList );
			namedFilter->setNumericFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_LOCATION, keyList );
		}

		// Check operators
		std::string operatorParam( runParams.get( RPARAM_FILTER_OPERATOR.c_str() ) );
		if ( 0 < operatorParam.size() )
		{
			tokenizeKeyList( operatorParam, keyList );
			namedFilter->setNumericFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_OPERATOR, keyList );
		}

		// Check systems
		std::string systemParam( runParams.get( RPARAM_FILTER_SYSTEM.c_str() ) );
		if ( 0 < systemParam.size() )
		{
			tokenizeKeyList( systemParam, keyList );
			namedFilter->setNumericFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_ISCS_SYSTEM, keyList );
		}

		// Check event types
		std::string eventtypeParam( runParams.get( RPARAM_FILTER_EVENTTYPE.c_str() ) );
		if ( 0 < eventtypeParam.size() )
		{
			tokenizeKeyList( eventtypeParam, keyList );
			namedFilter->setNumericFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_EVENTTYPE, keyList );
		}

		// Check asset name
		std::string assetParam( runParams.get( RPARAM_FILTER_ASSET.c_str() ) );
		if ( 0 < assetParam.size() )
		{
			namedFilter->addStringFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_ASSET, assetParam );
		}

		// Check description
		std::string descriptionParam( runParams.get( RPARAM_FILTER_DESCRIPTION.c_str() ) );
		if ( 0 < descriptionParam.size() )
		{
			namedFilter->addStringFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_DESCRIPTION, descriptionParam );
		}

		std::string fromParam( runParams.get( RPARAM_FILTER_FROM.c_str() ) );
		if ( 0 < fromParam.size() )
		{
			// Covert value to time_t.
			std::stringstream fromStream( fromParam );
			time_t fromTime;
			if ( ! ( fromStream >> fromTime ).fail() )
			{
				// 0 is a special value.
				if ( 0 == fromTime )
				{
					m_filter->setFromTime( m_filter->getEarliestTime() );
				}
				else
				{
					m_filter->setFromTime( fromTime );
				}
			}
		}

		std::string toParam( runParams.get( RPARAM_FILTER_TO.c_str() ) );
		if ( 0 < toParam.size() )
		{
			// Covert value to time_t.
			std::stringstream toStream( toParam );
			time_t toTime;
			if ( ! ( toStream >> toTime ).fail() )
			{
				m_filter->setToTime( toTime );
			}
		}

		// Check severities
		std::string severityParam( runParams.get( RPARAM_FILTER_SEVERITY.c_str() ) );
		if ( 0 < severityParam.size() )
		{
			tokenizeKeyList( severityParam, keyList );
			namedFilter->setNumericFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_SEVERITY, keyList );
		}

		// Check alarm types
		std::string alarmtypeParam( runParams.get( RPARAM_FILTER_ALARMTYPE.c_str() ) );
		if ( 0 < alarmtypeParam.size() )
		{
			tokenizeKeyList( alarmtypeParam, keyList );
			namedFilter->setNumericFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_ALARMTYPE, keyList );
		}

		// Check alarm id
		std::string alarmidParam( runParams.get( RPARAM_FILTER_ALARMID.c_str() ) );
		if ( 0 < alarmidParam.size() )
		{
			namedFilter->addStringFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_ALARMID, alarmidParam );
			m_filter->setFromTime( m_filter->getEarliestTime() );
		}
		// Check alarm child id
		std::string alarmchildParam( runParams.get( RPARAM_FILTER_ALARMCHILD.c_str() ) );
		if ( 0 < alarmchildParam.size() )
		{
			// get each child id separated by comma
			// and add to the filter for display
			std::vector<std::string> alarmIds;
			gTokeniseCommaSeparatedString(alarmchildParam, alarmIds);
			for(int ntimes =0; ntimes<alarmIds.size();ntimes++)
			{
				namedFilter->addStringFilter( EVDisplayStrRes::GetInstance()->GetInstance()->FILTER_ALARMID, alarmIds[ntimes] );
			}
			/*CString alarmChildren( alarmchildParam.c_str() );
			int index = 0;
			while (index >= 0)
			{
			std::string alarmchildId;
			index = alarmChildren.Find( _T(','), index);
			if (index > 0) // there is more child id
			{
			alarmchildId = alarmChildren.Left(index).GetBuffer(38);
			// take away the added child id if there are more child from the alarmChildren
			alarmChildren = alarmChildren.Right( alarmChildren.GetLength() - index - 1 );
			}
			else // there is no more child id
			{
			alarmchildId = alarmChildren.GetBuffer(38);
			}

			namedFilter->addStringFilter( CombinedEventFilter::FILTER_ALARMID, alarmchildId );
			}*/
			m_filter->setFromTime( m_filter->getEarliestTime() );
		}
	}


	//load data from DB to cache(cache is in DatabaseAccess) to prepair for Filter Dlg. 
	void EventViewerDlgPModel::loadListDataToCache()
	{
		FUNCTION_ENTRY("loadListDataToCache");

		DatabaseAccess::getInstance().load4FilterDlg(); // load all the list from DB to cache for EventViewer filter dialog.

		retrieveISCSSubsystemsToVector();  //retrieve ISCS subsystem data into m_ISCSsubsystemsVector

		loadAndRetrieveATSSystemInfo(); //Load ATS system&subsystem info to EventViewer::m_ATSSystem

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "load ATS system info finished.");

		FUNCTION_EXIT;
	}

	void EventViewerDlgPModel::loadAndRetrieveATSSystemInfo()
	{
		//load ATS system list Data
		if( IATSComWrapper::getInstanceRealTime()->loadATSSystemsInfo()==0)
		{
			m_ATSSystemsVector = IATSComWrapper::getInstanceRealTime()->getATSSystemStructureVector();
		} 

	}


	void EventViewerDlgPModel::retrieveISCSSubsystemsToVector ()
	{

		if(m_ISCSsubsystemsVector.size()==0)
		{
			m_ISCSsubsystemsVector = DatabaseAccess::getInstance().getSubsystems();
		}

		LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "reSaveISCSSubsystemsListData");
	}

	std::string EventViewerDlgPModel::getCurrentDatabaseString()
	{
		std::string connectStr;
		try 
		{
			TA_Base_Core::DbConnection::getInstance().getConnectionString(TA_Base_Core::Event_Ad, TA_Base_Core::Read, connectStr);
			//this will throw DbConnectionFailed("No working database found");

			std::vector<std::string> components;
			SimpleDb::ParseConnectStr(connectStr, components);
			return components[1];

		}
		catch(TA_Base_Core::DbConnectionFailed ex)
		{
			//No working DB: return ""
			return "";
		}  

	}


	void EventViewerDlgPModel::reloadEvents()
	{
		FUNCTION_ENTRY("loadEvents");
		// Need to load events from scratch based on the current filter.

		// This can take some time, so enable a wait cursor
		//CWaitCursor waitCursor; -- todo set waitcurosr

		// Set the load flag - this is a load and not a refresh
		//m_updateType = UPDATE_LOAD;

		// load Events again.
		m_filter->reloadEvents();

		FUNCTION_EXIT;
	}

	void EventViewerDlgPModel::applyRights()
	{
		FUNCTION_ENTRY("applyRights"); //zhiqiang++
		// Flags to set visibility - default to visible.
		bool eventLevelVisible = true;
		bool showAllEventsEnabled = false;

		try
		{
			std::string reason;
			TA_Base_Bus::EDecisionModule decisionModule = TA_Base_Bus::UNDEFINED_DM;

			// Determine the resource (subsystem) key.
			std::auto_ptr<TA_Base_Core::IResource> resource(
				TA_Base_Core::ResourceAccessFactory::getInstance().getResourceFromEntity(m_appController->getEntity()->getKey()) );

			//TD17143++ - change action permission request to something that works faster
			TA_Base_Core::IEntityData* entityData = TA_Base_Core::EntityAccessFactory::getInstance().getEntity( resource->getKey()); 
			unsigned long locationKey = entityData->getLocation();
			unsigned long subsystemKey = entityData->getSubsystem();
			delete entityData;
			std::string sessionId = m_appController->getSessionId();
			unsigned long resourceId = resource->getKey();

			// Now determine the rights.
			// eventLevelVisible = m_rightsLibrary->isActionPermittedOnResource( m_controlClass.getSessionId(), resource->getKey(),
			//    TA_Base_Bus::aca_CHANGE_EVENT_LEVEL, reason, decisionModule );
			eventLevelVisible = m_rightsLibrary->isActionPermittedOnResourceAndLocSub( sessionId, resourceId,
				locationKey, subsystemKey, TA_Base_Bus::aca_CHANGE_EVENT_LEVEL, reason, decisionModule );

			// showAllEventsEnabled = m_rightsLibrary->isActionPermittedOnResource( m_controlClass.getSessionId(), resource->getKey(),
			//    TA_Base_Bus::aca_SHOW_ALL_EVENTS, reason, decisionModule );
			showAllEventsEnabled = m_rightsLibrary->isActionPermittedOnResourceAndLocSub( sessionId, resourceId,
				locationKey, subsystemKey,  TA_Base_Bus::aca_SHOW_ALL_EVENTS, reason, decisionModule );
			//++TD17143
		}
		catch( ... )
		{
			LOG_EXCEPTION_CATCH( SourceInfo, "DataException", "Errors occurred while trying to get rights.");
		}

		// Now apply them.
		GenericEventArgs * args = new GenericEventArgs();
		args->Param1 = &(showAllEventsEnabled);
		notifyViews(SETUP_ALLEVENTS_MENU.c_str(),args );
		delete args;
		args = NULL;

		FUNCTION_EXIT;//zhiqiang++
	}

	void EventViewerDlgPModel::tokenizeKeyList( const std::string& param, std::vector<unsigned long>& keys )
	{
		typedef boost::tokenizer< boost::char_separator<char> > tokenizer;

		// Variables required for string->integer conversion.
		std::stringstream tempStream;
		unsigned long key;

		// Prepare the tokenizer and iterator.
		boost::char_separator<char> lineSep( "," );
		boost::tokenizer< boost::char_separator<char> > keyTokens( param, lineSep );
		boost::tokenizer< boost::char_separator<char> >::iterator it = keyTokens.begin();

		// Make sure the vector is empty.
		keys.clear();

		// For each key...
		for ( ; it != keyTokens.end(); it++ )
		{
			// Prepare the string stream for conversion.

			// td11982
			tempStream.clear();
			tempStream.str( *it );

			// Convert the key to an integer and add it to the filter if conversion succeeded.
			if ( ! ( tempStream >> key ).fail() )
			{
				keys.push_back( key );
			}
		}

	}

	void EventViewerDlgPModel::deleteFilter()
	{
		if (m_filter != NULL)
		{	
			delete m_filter;
			m_filter = NULL;
		}
	}

	TA_Base_Core::NamedFilterAccessFactory::FilterNameList EventViewerDlgPModel::getFilterNameList()
	{
		// Get a list of the filters.
		if(m_FilterNameList.size()==0)
		{
			m_FilterNameList = NamedFilterAccessFactory::getInstance().getAllFilterNames();
		}
		return m_FilterNameList;
	}

	std::string EventViewerDlgPModel::getFilterStatusMsg()
	{
		return m_filter->getStatusMsg();
	}

	bool EventViewerDlgPModel::IsCurrentTimeInFilter()
	{
		return Filter::CURRENT_TIME == m_filter->getToTime() ? true: false;
	}

	bool EventViewerDlgPModel::IsFilterTOTimeEmpty()
	{
		return 0 == m_filter->getToTime()?true:false;
	}

	bool EventViewerDlgPModel::IsSelectedAlarmEvent(int index)
	{
		bool isAlarm = false;
		if( m_filter != NULL)
		{
			// We know only one event has been selected, now need to check if the alarm
			// has an associated alarm id.
			std::string alarmId = m_filter->getItemData( index, EVDisplayStrRes::GetInstance()->GetInstance()->HIDDEN_COLUMN_ALARMID );
			if(!alarmId.empty())
			{
				isAlarm = true;
			}
		}
		return isAlarm;
	}


	void EventViewerDlgPModel::next()
	{
		bool changeToRealTime = false;
		// Get the old time boundary:
		time_t old_toTime = (time_t) m_filter->getToTime();  
		time_t old_fromTime = (time_t) m_filter->getFromTime();
		if( old_fromTime == Filter::PREVIOUS_DAY)
		{
			old_fromTime = old_toTime - TWENTY_FOUR_HOURS;
		}
		time_t difference = old_toTime - old_fromTime;
		if ( Filter::CURRENT_TIME == old_toTime )
		{
			// The "current" flag is set. This method should not have been called, so put up a message box and return
			showUserMessageNotify(IDS_UI_040021);
			return;
		}

		// Set the new time boundary:
		time_t new_fromTime;
		time_t new_toTime;
		//const time_t currentTime = (time_t)CTime::GetCurrentTime().GetTime();
		time_t currentTime; 
		time(&currentTime);
		if(!m_filter->currentListWasCutted() )
		{
			new_fromTime = old_toTime;
		}else
		{
			new_fromTime = m_filter->getLatestTimeFromList();
		}
		new_toTime = new_fromTime + difference;
		if( new_toTime > currentTime)
		{
			// The new toTime is in the "future" so set the filter back into "realtime" mode
			new_toTime = currentTime;
			new_fromTime = currentTime - difference;
			changeToRealTime = true;

		}
		m_filter->setFromTime(new_fromTime);
		m_filter->setToTime(new_toTime);

		// Disable the previous button if there are no more events earlier than the from date
		if(m_filter->getFromTime() > m_filter->getEarliestTime())
		{			
			GenericEventArgs * args = new GenericEventArgs();
			bool update = true;
			args->Param1 = &(update);
			notifyViews(UPDATE_PREVIOUS_TOOLBAR.c_str(),args );
			delete args;
			args = NULL;
		}

		notifyViews(UPDATE_FILTER_DLG.c_str());
		// This change forced the named filter to an unnamed state, so the combo box must be updated.
		notifyViews(POPULATE_NAMED_FILTERS.c_str());

		FilterCriteria filterCriteria ;
		filterCriteria = m_filter->getFilterCriteria();  // get the last filterCriteria, but we will ignore some info(starttime, endtime)
		m_filter->populateTimeAndSortingToFilterCriteria(filterCriteria); 
		if(!changeToRealTime)
		{
			// start a new historical search task:
			filterCriteria.StartTimePeriodDefined = true;
			filterCriteria.EndTimePeriodDefined = true;
			filterCriteria.searchDirection = TA_Base_App::Forward;

			m_filter->setBothPkeyRightBoundary(filterCriteria);
			m_filter->startNewTask(filterCriteria, HistoricalMode);
		}
		else
		{
			// In "current" mode, can't use next, so disable it, but can use "updating" so enable that.			
			GenericEventArgs * args = new GenericEventArgs();
			bool update = false;
			args->Param1 = &(update);
			notifyViews(UPDATE_NEXT_TOOLBAR.c_str(),args );
			delete args;
			args = NULL;

			filterCriteria.StartTimePeriodDefined = true;
			filterCriteria.EndTimePeriodDefined = false;
			m_filter->setToTime(Filter::CURRENT_TIME);
			m_filter->startNewTask(filterCriteria, RealTimeMode);

			// after start new real-time task, we set from time as 0. So that when user open filter dialog later,
			// he wont see a from time and then he won't be confused.
			m_filter->setFromTime(0);
		}

	}

	void EventViewerDlgPModel::previous()
	{
		// Check if the "to" time is current.
		time_t toTime = (time_t)m_filter->getToTime();
		if ( Filter::CURRENT_TIME == toTime )
		{
			// Previous can leagally be called when the "to" flag is set, so need to get the
			// current time
			//toTime = (time_t) CTime::GetCurrentTime().GetTime();
			time(&toTime);
		}

		// Check if the "from" time is 24 hours before "to" time.
		time_t fromTime = (time_t) m_filter->getFromTime();
		if ( Filter::PREVIOUS_DAY == fromTime )
		{
			// Previous can legally be called when the "from" flag is set, so need to get the
			// time 24 hours before "to" time.
			fromTime = toTime - TWENTY_FOUR_HOURS;
		}

		// Now determine the new "to" and "from" times.
		time_t difference = toTime - fromTime;

		if( !m_filter->currentListWasCutted() )
		{
			m_filter->setToTime(fromTime);
			m_filter->setFromTime(fromTime - difference);
		}
		else 
		{
			time_t new_Totime = m_filter->getEarliestTimeFromList();
			m_filter->setToTime( new_Totime );
			m_filter->setFromTime( new_Totime - difference);
		}
		// Disable the previous button if there are no more events earlier than the from date
		if(m_filter->getFromTime() <= m_filter->getEarliestTime())
		{
			GenericEventArgs * args = new GenericEventArgs();
			bool update = false;
			args->Param1 = &(update);
			notifyViews(UPDATE_PREVIOUS_TOOLBAR.c_str(),args );
			delete args;
			args = NULL;
		} 

		GenericEventArgs * args = new GenericEventArgs();
		bool update = true;
		args->Param1 = &(update);
		notifyViews(UPDATE_NEXT_TOOLBAR.c_str(),args );
		delete args;
		args = NULL;

		notifyViews(UPDATE_FILTER_DLG.c_str());
		// This change forced the named filter to an unnamed state, so the combo box must be updated.
		notifyViews(POPULATE_NAMED_FILTERS.c_str());

		// And now load the events matching the new filter
		/*loadEvents(false);*/
		// start a new historical search task:
		FilterCriteria filterCriteria ;

		filterCriteria = m_filter->getFilterCriteria();
		m_filter->populateTimeAndSortingToFilterCriteria(filterCriteria); 
		filterCriteria.StartTimePeriodDefined = true;
		filterCriteria.EndTimePeriodDefined = true;
		filterCriteria.searchDirection = TA_Base_App::Backward;
		m_filter->setBothPkeyLeftBoundary(filterCriteria);
		m_filter->startNewTask(filterCriteria, HistoricalMode);
	}

	void EventViewerDlgPModel::pauseLoad()
	{
		// TD11124: Disable current load and refreshing while exporting is taking place.
		/*    m_eventList.setRefreshPeriod(0);*/
		m_filter->pauseCurrentListDataLoad();
	}

	void EventViewerDlgPModel::resumeLoad()
	{
		// TD11124: Enable current load and refreshing now that exporting is complete.
		m_filter->resumeCurrentListDataLoad();
		/*    m_eventList.setRefreshPeriod(m_refreshTime);*/
	}

	void EventViewerDlgPModel::exportEvents(std::string filename, int itemCount)
	{
		try
		{
			// TD11124: Use an output file stream to write the events to file.
			std::ofstream outFile( filename.c_str() );
			if ( !outFile.is_open() )
			{
				throw std::exception( "Couldn't open file for writing." );
			}

			// Output the date as a heading
			//COleDateTime currTime = COleDateTime::GetCurrentTime();

			TA_Base_Core::DateFormat dateFormat; 
			std::string dateFormatStr = dateFormat.getDateFormat(TA_Base_Core::DateFormat::DateTimeFormatNoSec1a);
			time_t currenttime;
			struct tm * currentTimeInfo;
			char formatStr [50];

			time (&currenttime);
			currentTimeInfo = localtime (&currenttime);

			strftime (formatStr,50,dateFormatStr.c_str(),currentTimeInfo);

			outFile << std::string(formatStr) << "\n\n";  /////--- todo check


			// TD12474 ++
			//outFile << std::string(currTime.Format("%d %B %Y %H:%M")) << "\n\n";

			//outFile << std::string(currTime.Format(dateFormatStr.c_str())) << "\n\n";
			// ++ TD12474

			// Output the column headings to the file
			//std::vector<Filter::Column> columns = m_filter->getColumns();
			unsigned int i = 0;
			for (i = 0;i < m_columns.size();i++)
			{
				outFile << m_columns[i].m_name;

				if (i < m_columns.size() - 1)
				{
					outFile << ",";
				}
			}
			outFile << "\n";

			// CHeaderCtrl* header = GetHeaderCtrl();

			std::string text;
			for (int index = 0; index < itemCount; ++index)
			{
				for (i = 0;i < m_columns.size();i++)
				{
					text = m_filter->getItemData(index,m_columns[i].m_name);
					// TD 14418 Export the comment if there is comment in the item
					if (m_columns[i].m_name.compare(EVDisplayStrRes::GetInstance()->COLUMN_COMMENT) == 0 && text.compare("C") == 0) //todo
					{
						text = m_filter->getItemData( index, m_columns[i].m_name, false );
					}
					convertText(text);

					outFile << text;
					if (i < m_columns.size() - 1)
					{
						outFile << ",";
					}
				}
				outFile << "\n";
			}
			outFile.flush();
			outFile.close();

		}
		catch ( ... )
		{
			throw;
		}
	}

	void EventViewerDlgPModel::convertText(std::string& text)
	{
		// replace all carriage returns with spaces
		text = replaceNonPrintableCharactersWithSpace(text);

		// First check for comma. If there is none then the string can remain the same
		if (std::string::npos == text.find(','))
		{
			return;
		}

		// Ok now we have established there is a comma we must change any existing double quotes to "".
		unsigned int pos = text.find('\"');
		while (pos != std::string::npos)
		{
			text.insert(pos,"\"");
			pos = text.find('\"',pos+2);
		}

		// Now add quotes at either end
		//text.insert(0,'\"');
		text.insert(0,"\"");
		text.insert(text.size(),"\"");
	}

	std::string EventViewerDlgPModel::replaceNonPrintableCharactersWithSpace(const std::string& inputString)
	{
		std::string adjustedString = inputString;

		// 	if(adjustedString.length()>0)
		// 	{
		// 		LOG_GENERIC( SourceInfo, DebugUtil::DebugInfo, "Begin EventListComponent::replaceNonPrintableCharactersWithSpace,  %s", adjustedString.c_str());
		// 	}

		// Loop through checking for non-printable characters
		for (unsigned int i = 0; i < adjustedString.length(); ++i)
		{	
			char c = adjustedString[i];

			if( (unsigned)(c + 1) > 256 || isprint(adjustedString[i]) == 0 )
			{
				// Replace the non-printable character with a space
				std::replace(adjustedString.begin(), adjustedString.end(), adjustedString[i], ' ');
			}
		}

		return adjustedString.c_str();
	}


	void EventViewerDlgPModel::launchAlarmManager()
	{
		try
		{

			// Determine the screen that will have the Alarm Manager. 
			DWORD pos = GetMessagePos(); //TODO check Rong to use Windows lib
			POINTS pt = MAKEPOINTS(pos);
			RECT position = TA_Base_Bus::AppLauncher::getInstance().getRect( SCREEN_CURRENT,
				AREA_SCHEMATIC,
				pt.x );
			LOG_GENERIC(SourceInfo, DebugUtil::DebugInfo, "Launching Event Viewer at xPos: %d yPos: %d and it should resize to fit on a single screen.", position.left, position.top);

			// Launch the alarm manager.
			TA_Base_Bus::AppLauncher::getInstance().launchApplication(ALARM_GUI_APPTYPE,
				"",
				POS_BOUNDED|POS_RESIZABLE,
				ALIGN_FIT|ALIGN_LEFT|ALIGN_TOP,
				NULL,
				&position);
		}
		catch(TA_Base_Core::ApplicationException&)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "ApplicationException", "Could not launch event viewer");

			// Construct the error message to display.
			showUserMessageNotify(IDS_UE_010002,"Alarm Manager");
		}
	}

	void EventViewerDlgPModel::setNamedFilter(TA_Base_Core::NamedFilterAccessFactory::FilterNameList filterList)
	{
		m_FilterNameList = filterList;
	}

	bool EventViewerDlgPModel::IsNotSameFilterSelected(std::string filterName)
	{
		// Make sure the same filter wasn't selected.
		return  ( filterName != m_filter->getNamedFilter()->getName() ) ? true:false;
	}

	void EventViewerDlgPModel::loadSelectedFilter(std::string filterName)
	{
		std::auto_ptr<NamedFilter> loadedFilter;
		// Load the filter.
		loadedFilter = NamedFilterAccessFactory::getInstance().getNamedFilter( filterName );

		// Make a copy of it.
		m_filter->setNamedFilter( loadedFilter );

		FilterCriteria filterCriteria;
		m_filter->populateFilterCriteria(filterCriteria);
		if (m_filter->getToTime()==0 && m_filter->getFromTime()==0)
		{
			filterCriteria.setPrevious1dayForStartTime();
			m_filter->startNewTask(filterCriteria,TA_Base_App::RealTimeMode);
		}
		else
		{
			m_filter->startNewTask(filterCriteria,TA_Base_App::HistoricalMode);
		}		

	}

	std::string EventViewerDlgPModel::getNameFilterName()
	{
		return m_filter->getNamedFilter()->getName() ;
	}

	void EventViewerDlgPModel::startRightsAndEventLevelThreads()
	{
		m_pPopulateEventLevelsThread = new CPopulateEventLevelsThread();
		m_pPopulateEventLevelsThread->start();

		m_pApplyRightsThread = new CApplyRightsThread(m_appController);
		m_pApplyRightsThread->start();
	}

	std::string EventViewerDlgPModel::getCurrentFilterText()
	{
		  
			// Set the title bar to show what the current filters are
			//std::string windowText(RunParams::getInstance().get(RPARAM_APPNAME));			
			return m_filter->getCurrentFilterString();	

			//if (0 < filterText.length())
			//{
			//	windowText += " - Filtered on: " + filterText;
							
			//}
			//return filterText;
		
	}

	void EventViewerDlgPModel::postMessageNotifyViews(GenericEventArgs *args)
	{
		notifyViews(POST_MESSAGE_TO_UI.c_str(),args);
	}

	void EventViewerDlgPModel::showUserMessageNotify(int message, std::string param1, std::string param2)
	{
		GenericEventArgs * args = new GenericEventArgs();		
		args->Param1 = &(message);
		args->Param2 = &(param1);
		args->Param3 = &(param2);
		notifyViews(SHOW_USER_MESSAGE_NOTIFY.c_str(),args);
	}

	void EventViewerDlgPModel::preparaForClose()
	{
		notifyViews(INVOKE_READY_TO_CLOSE.c_str());
	}

	////////////////////////////////////////////////////////////////////////////////////
	/////////////EventListComponent////////////////////////////////////////////////

	void EventViewerDlgPModel::initializePrintColWidth()
	{
		if(m_printoutColWidths == NULL)
		{
			// TD 12763 since this is the first time, establish the default printout width for each columns
			//std::vector<Filter::Column> filterColumns = m_filter->getColumns();
			m_printoutColWidths = new int [m_columns.size() + 1];
			for (unsigned int i=0 ; i<m_columns.size(); ++i )
			{
				Filter::Column col = m_columns[i];
				m_printoutColWidths[i+1] = col.m_width;
			}
		}
	}

	void EventViewerDlgPModel::loadColumns()
	{
		m_columns.clear();
		m_columns.push_back( Filter::Column( EVDisplayStrRes::GetInstance()->COLUMN_DATETIME, 135, ACTION_NO_ACTION ) ); //TD15005
		m_columns.push_back( Filter::Column( EVDisplayStrRes::GetInstance()->COLUMN_SEVERITY, 40, ACTION_NO_ACTION ) );
		m_columns.push_back( Filter::Column( EVDisplayStrRes::GetInstance()->COLUMN_ASSET, 270, ACTION_NO_ACTION ) ); //TD15005  
		m_columns.push_back( Filter::Column( EVDisplayStrRes::GetInstance()->COLUMN_DESCRIPTION, 600, ACTION_NO_ACTION ) ); //TD15005 
		m_columns.push_back( Filter::Column( EVDisplayStrRes::GetInstance()->COLUMN_VALUE, 167, ACTION_NO_ACTION ) );
		m_columns.push_back( Filter::Column( EVDisplayStrRes::GetInstance()->COLUMN_ALARM_STATUS, 94, ACTION_NO_ACTION ) );    
		m_columns.push_back( Filter::Column( EVDisplayStrRes::GetInstance()->COLUMN_T, 34, ACTION_NO_ACTION ) );  //add column "T"
		m_columns.push_back( Filter::Column( EVDisplayStrRes::GetInstance()->COLUMN_MMS, 34, ACTION_NO_ACTION ) );
		m_columns.push_back( Filter::Column( EVDisplayStrRes::GetInstance()->COLUMN_DSS, 34, ACTION_NO_ACTION ) );
		m_columns.push_back( Filter::Column( EVDisplayStrRes::GetInstance()->COLUMN_AVL, 34, ACTION_NO_ACTION ) );
		m_columns.push_back( Filter::Column( EVDisplayStrRes::GetInstance()->COLUMN_OPERATOR, 200, ACTION_NO_ACTION ) );
		//m_columns.push_back( Filter::Column( COLUMN_COMMENT, 24, ACTION_SHOW_COMMENT ) ); 
	}

	std::vector<Filter::Column> EventViewerDlgPModel::getFilterColumns()
	{
		return m_columns;
	}


	std::string EventViewerDlgPModel::getFilterItemData(const unsigned long index,const std::string& columnName)
	{
		if(m_filter != NULL )
		{
			return m_filter->getItemData(index,columnName);
		}
		return "";
	}

	int EventViewerDlgPModel::getCurrentSortColumn()
	{
		return m_currentSortColumn;
	}

	bool EventViewerDlgPModel::getCurrentSortOrder()
	{
		return m_sortAscending;
	}


	void EventViewerDlgPModel::sort(const std::string& columnName, const int columnId)
	{
		// Toggle order or change to new column
		bool newSortAscending = true;
		if(m_currentSortColumn == columnId)
		{
			newSortAscending = !m_sortAscending;
		}
		m_sortAscending = newSortAscending;

		if( m_currentSortColumn == columnId )
		{
			m_filter->reverse(columnName, m_sortAscending );
		}
		else
		{
			// Save the column ID for processing future sorting.
			m_currentSortColumn = columnId;

			// Need to use the "headerNameStr" here, as the headerName char array has been replaced with the last one selected.
			m_filter->sort(columnName,m_sortAscending);
		}
	}

	void EventViewerDlgPModel::setSelectedItems( std::vector<unsigned long>& selectedItems )
	{
		m_filter->setSelectedItems( selectedItems );
	}

	const std::vector<unsigned long>& EventViewerDlgPModel::getSelectedItems()
	{
		std::vector<unsigned long> selectedItems;
		if(m_filter != NULL)
		{
			return  m_filter->getSelectedItems();
		}
		return selectedItems;
	}

	void EventViewerDlgPModel::executeColumnAction(int columnID, std::string columnName)
	{
		// Retrieve the list of columns from the filter.
		//const std::vector<Filter::Column>& columns = m_filter->getColumns();
		if ( columnID < 0 || columnID >= static_cast<int>(m_columns.size()) )
		{
			return;
		}

		// Now find its associated Column object.
		std::vector<Filter::Column>::const_iterator it;
		for ( it=m_columns.begin(); it!=m_columns.end(); it++ )
		{
			// Execute the action if there is an associated action and the name of
			// the column is correct.
			if ( ( 0 != it->m_action ) && ( it->m_name == columnName ) )
			{
				executeAction(it->m_action);
				break;
			}
		}
		return;
	}

	void EventViewerDlgPModel::executeAction( const unsigned int actionId )
	{
		// Find the associated action.
		IEventAction* action = m_filter->getActionWithId( actionId );
		if ( NULL != action )
		{
			// Execute it.
			ActionThread  *actionthread = new ActionThread(m_filter,action);			

			//todo delete this pointer in Actionthread Terminate method
		}
		else
		{
			LOG_GENERIC( SourceInfo, DebugUtil::DebugError, "Failed to execute action %i. No matching action found.", actionId);
		}
	}


	void EventViewerDlgPModel::getFilterRowColour( const unsigned long index, COLORREF& foreground, COLORREF& background )
	{
		return m_filter->getRowColour( index, foreground, background );
	}

	COLORREF EventViewerDlgPModel::getFilterItemColour(const unsigned long index, const std::string& columnName)
	{
		return m_filter->getItemColour(index,columnName);
	}

	COLORREF EventViewerDlgPModel::getBackgroundColour()
	{
		return m_filter->getBackgroundColour();
	}

	int* EventViewerDlgPModel::getPrintColWidth()
	{
		return m_printoutColWidths;
	}

	void EventViewerDlgPModel::clearSortMark()
	{
		m_currentSortColumn = -1;
		m_sortAscending = false;
	}

	void EventViewerDlgPModel::previousPage()
	{
		m_filter->previousPage();
	}

	void EventViewerDlgPModel::nextPage()
	{
		m_filter->nextPage();
	}
}

