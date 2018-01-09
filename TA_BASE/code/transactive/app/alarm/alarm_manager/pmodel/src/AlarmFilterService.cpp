#include "app\alarm\alarm_manager\pmodel\src\AlarmFilterService.h"



#include "core\data_access_interface\src\NamedFilterAccessFactory.h"
#include "core\data_access_interface\src\ISubsystem.h"
#include "core\data_access_interface\src\SubsystemAccessFactory.h"
#include "core\data_access_interface\src\ILocation.h"
#include "core\data_access_interface\src\LocationAccessFactory.h"
#include "core\data_access_interface\src\IOperator.h"
#include "core\data_access_interface\src\OperatorAccessFactory.h"
#include "core\data_access_interface\src\IAlarmSeverityData.h"
#include "core\data_access_interface\src\AlarmSeverityData.h"
#include "core\data_access_interface\src\AlarmSeverityAccessFactory.h"
#include "core\data_access_interface\src\IAlarmTypeData.h"
#include "core\data_access_interface\src\AlarmTypeAccessFactory.h"
#include "core/exceptions/src/DataException.h"
#include "core\exceptions\src\AlarmListCtrlException.h"
#include "core\exceptions\src\DatabaseException.h"
#include "core\synchronisation\src\ThreadGuard.h"

//#include "bus/generic_gui_view/src/TransActiveMessage.h"


using TA_Base_Core::NamedFilter;
using namespace TA_Base_Bus;


namespace TA_Base_App
{


	AlarmFilterService* AlarmFilterService::m_pInstance = NULL;
	TA_Base_Core::NonReEntrantThreadLockable AlarmFilterService::m_singletonLock;
	// GetInstance
	AlarmFilterService& AlarmFilterService::getInstance()
	{
		TA_Base_Core::ThreadGuard guard(m_singletonLock);
		if (NULL == m_pInstance)
		{
			m_pInstance = new AlarmFilterService();
		}

		return *m_pInstance;
	}

	// ReleaseInstance
	void AlarmFilterService::releaseInstance()
	{
		TA_Base_Core::ThreadGuard guard(m_singletonLock);
		if (NULL != m_pInstance)
		{
			delete m_pInstance;
			m_pInstance = NULL;
		}
	}

	AlarmFilterService::AlarmFilterService()
	{}

	AlarmFilterService::~AlarmFilterService()
	{}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterService::retrieveSubsystems(ListItems& subsystemList)
	{
		std::vector<TA_Base_Core::ISubsystem*> allSubsystemData;
		try
		{
			allSubsystemData = TA_Base_Core::SubsystemAccessFactory::getInstance().getAllSubsystems(false);
		}
		catch (TA_Base_Core::DataException&)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "DataException",
				"Errors occured while trying to retrieve subsystems matching the one passed in");
			for (unsigned int i = 0; i < allSubsystemData.size(); ++i)
			{
				delete allSubsystemData[i];
			}
			TA_THROW(TA_Base_Core::TransactiveException("Failed to retrieve subsystem information from database. Aborting."));
		}
		catch (TA_Base_Core::DatabaseException&)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "DatabaseException",
				"Errors occured while trying to retrieve the severity information from the database.");
			for (unsigned int i = 0; i < allSubsystemData.size(); ++i)
			{
				delete allSubsystemData[i];
			}
			TA_THROW(TA_Base_Core::TransactiveException("Failed to retrieve subsystem information from database. Aborting."));
		}
		catch (...)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "UnknownException",
				"Unknown error whilst retrieving subsystem information!");
			for (unsigned int i = 0; i < allSubsystemData.size(); ++i)
			{
				delete allSubsystemData[i];
			}
			TA_THROW(TA_Base_Core::TransactiveException("Unknown exception caught whilst retrieving subsystem information."));
		}

		// store the subsystems into the map and delete the temporary structure
		subsystemList.clear();
		for (unsigned int i = 0; i < allSubsystemData.size(); ++i)
		{
			ListItem subsystem(allSubsystemData[i]->getKey(), allSubsystemData[i]->getName());
			subsystemList.push_back(subsystem);
			delete allSubsystemData[i];
		}
		subsystemList.sort();
	}
	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterService::retrieveSubsystemTypes(ListItems& subsystemtypeList, const ListItems& systemList)
	{
		subsystemtypeList.clear();
		// get subsystems from systems
		subsystemtypeList = SubsystemDataCache::getInstance().getSubsystemsFormSystems(systemList);
		// sort the subsystems
		subsystemtypeList.sort();
	}
	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterService::retrieveSystems(ListItems& systemList)
	{
		// store the subsystems into the map and delete the temporary structure
		systemList.clear();
		SubsystemDataCache::getInstance().getAllSystems(systemList);
		// sort the subsystems
		systemList.sort();
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterService::retrieveLocations(ListItems& locationList)
	{
		std::vector<TA_Base_Core::ILocation*> allLocationData;
		try
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugDebug,
				"Retrieving operator locations.");
			allLocationData = TA_Base_Core::LocationAccessFactory::getInstance().getAllLocations();
		}
		catch (TA_Base_Core::DatabaseException&)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError,
				"DatabaseException caught whilst retrieving operator locations. There were problems communicating with the database");
			for (unsigned int i = 0; i < allLocationData.size(); ++i)
			{
				delete allLocationData[i];
			}
			TA_THROW(TA_Base_Core::TransactiveException("Failed to retrieve location information from database. Aborting."));
		}
		catch (TA_Base_Core::DataException&)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError,
				"DataException caught whilst retrieving operator locations. The data could not be converted to the required format");
			for (unsigned int i = 0; i < allLocationData.size(); ++i)
			{
				delete allLocationData[i];
			}
			TA_THROW(TA_Base_Core::TransactiveException("Failed to retrieve location information from database. Aborting."));
		}
		catch (...)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError,
				"Caught unknown exception whilst retrieving Operator locations.");
			for (unsigned int i = 0; i < allLocationData.size(); ++i)
			{
				delete allLocationData[i];
			}
			TA_THROW(TA_Base_Core::TransactiveException("Unknown exception caught whilst retrieving location information."));
		}

		// store the locations into the map and delete the temporary structure
		locationList.clear();
		for (unsigned int i = 0; i < allLocationData.size(); ++i)
		{
			if (allLocationData[i]->getKey() != 0)
			{
				ListItem location(allLocationData[i]->getKey(), allLocationData[i]->getDisplayName());
				locationList.push_back(location);
				delete allLocationData[i];
			}
		}
		// sort the locations
		//m_locations.sort();
	}

	/////////////////////////////////////////////////////////////////////////////
	// Note that the operators are sorted first by their type (non-system operators
	// take precedence over system operators) and then by their name.
	void AlarmFilterService::retrieveOperators(ListItems& operatorList)
	{
		std::vector<TA_Base_Core::IOperator*> allOperatorData;
		try
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugDebug,
				"Retrieving operator locations.");

			//As we only need operator key/name/isSystem information, we don't have to load se_operator_regions and se_operator_profile.
			//allOperatorData = TA_Base_Core::OperatorAccessFactory::getInstance().getAllOperators();  
			allOperatorData = TA_Base_Core::OperatorAccessFactory::getInstanceNoInit().getAllOperatorsInfo();
		}
		catch (TA_Base_Core::DatabaseException&)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError,
				"DatabaseException caught whilst retrieving operator information. There were problems communicating with the database");
			for (unsigned int i = 0; i < allOperatorData.size(); ++i)
			{
				delete allOperatorData[i];
			}
			TA_THROW(TA_Base_Core::TransactiveException("Failed to retrieve operator information from database. Aborting."));
		}
		catch (TA_Base_Core::DataException&)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError,
				"DataException caught whilst retrieving operator information. The data could not be converted to the required format");
			for (unsigned int i = 0; i < allOperatorData.size(); ++i)
			{
				delete allOperatorData[i];
			}
			TA_THROW(TA_Base_Core::TransactiveException("Failed to retrieve operator information from database. Aborting."));
		}
		catch (...)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError,
				"Caught unknown exception whilst retrieving Operator information.");
			for (unsigned int i = 0; i < allOperatorData.size(); ++i)
			{
				delete allOperatorData[i];
			}
			TA_THROW(TA_Base_Core::TransactiveException("Unknown exception caught whilst retrieving operator information."));
		}

		// store the non-system (real) operators into the structure
		operatorList.clear();
		unsigned int i = 0;
		for (i = 0; i < allOperatorData.size(); ++i)
		{
			if (!allOperatorData[i]->isSystemOperator())
			{
				ListItem op(allOperatorData[i]->getKey(), allOperatorData[i]->getName());
				operatorList.push_back(op);
			}
		}
		// Sort the operator list
		operatorList.sort();

		// store the system operators into a temporary structure
		ListItems systemOperators;
		for (i = 0; i < allOperatorData.size(); ++i)
		{
			if (allOperatorData[i]->isSystemOperator())
			{
				ListItem op(allOperatorData[i]->getKey(), allOperatorData[i]->getName());
				systemOperators.push_back(op);
			}
		}
		// Sort the system operators
		systemOperators.sort();

		// Add the system operators to the non-system operators (at the end)
		for (ListItems::iterator it = systemOperators.begin(); it != systemOperators.end(); ++it)
		{
			operatorList.push_back(*it);
		}

		// Cleanup
		for (i = 0; i < allOperatorData.size(); ++i)
		{
			delete allOperatorData[i];
		}
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterService::retrieveAlarmTypes(ListItems& alarmTypeList)
	{
		// Retrieve the alarmType information from the database.
		std::vector<TA_Base_Core::IAlarmTypeData*> allAlarmTypeData;
		try
		{
			allAlarmTypeData = TA_Base_Core::AlarmTypeAccessFactory::getInstance().getAllAlarmTypes(false);
		}
		catch (TA_Base_Core::DataException&)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "DataException",
				"Errors occured while trying to retrieve alarm type matching the one passed in");
			// Delete the contents of the allAlarmTypeData vector
			for (unsigned int i = 0; i < allAlarmTypeData.size(); ++i)
			{
				delete allAlarmTypeData[i];
			}
			TA_THROW(TA_Base_Core::TransactiveException("Failed to retrieve alarm type information from database. Aborting."));
		}
		catch (TA_Base_Core::DatabaseException&)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "DatabaseException",
				"Errors occured while trying to retrieve the alarm type information from the database.");
			// Delete the contents of the allAlarmTypeData vector
			for (unsigned int i = 0; i < allAlarmTypeData.size(); ++i)
			{
				delete allAlarmTypeData[i];
			}
			TA_THROW(TA_Base_Core::TransactiveException("Failed to retrieve alarm type information from database. Aborting."));
		}
		catch (...)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "UnknownException",
				"Unknown error whilst retrieving subsystem information!");
			// Delete the contents of the allAlarmTypeData vector
			for (unsigned int i = 0; i < allAlarmTypeData.size(); ++i)
			{
				delete allAlarmTypeData[i];
			}
			TA_THROW(TA_Base_Core::TransactiveException("Unknown exception caught whilst retrieving alarm type information."));
		}

		// store the alarm types into the map and delete the temporary structure
		alarmTypeList.clear();
		for (unsigned int i = 0; i < allAlarmTypeData.size(); ++i)
		{
			ListItem alarmType(allAlarmTypeData[i]->getKey(), allAlarmTypeData[i]->getName());
			alarmTypeList.push_back(alarmType);
			delete allAlarmTypeData[i];
		}
		// sort the alarm types
		alarmTypeList.sort();
	}

	/////////////////////////////////////////////////////////////////////////////
	void AlarmFilterService::retrieveSeverities(ListSeverityItems& severityList)
	{
		std::vector<TA_Base_Core::IAlarmSeverityData*> allSeverityData;
		try
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugDebug,
				"Retrieving alarm severity information from database.");
			allSeverityData = TA_Base_Core::AlarmSeverityAccessFactory::getInstance().getAllAlarmSeverities();
		}
		catch (TA_Base_Core::DatabaseException&)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError,
				"DatabaseException caught whilst retrieving severity information. There were problems communicating with the database");
			// Delete the contents of the allSeverityData vector
			for (unsigned int i = 0; i < allSeverityData.size(); ++i)
			{
				delete allSeverityData[i];
			}
			TA_THROW(TA_Base_Core::TransactiveException("Failed to retrieve severity information from database. Aborting."));
		}
		catch (TA_Base_Core::DataException&)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError,
				"DataException caught whilst retrieving severity information. The data could not be converted to the required format");
			// Delete the contents of the allSeverityData vector
			for (unsigned int i = 0; i < allSeverityData.size(); ++i)
			{
				delete allSeverityData[i];
			}
			TA_THROW(TA_Base_Core::TransactiveException("Failed to retrieve severity information from database. Aborting."));
		}
		catch (...)
		{
			LOG(SourceInfo, TA_Base_Core::DebugUtil::GenericLog, TA_Base_Core::DebugUtil::DebugError,
				"Caught unknown exception whilst retrieving severity information.");
			// Delete the contents of the allSeverityData vector
			for (unsigned int i = 0; i < allSeverityData.size(); ++i)
			{
				delete allSeverityData[i];
			}
			TA_THROW(TA_Base_Core::TransactiveException("Unknown exception caught whilst retrieving alarm severity information."));
		}

		// store the severities into the map and cleanup the temporary structure
		severityList.clear();
		for (unsigned int i = 0; i < allSeverityData.size(); ++i)
		{
			SeverityItem item(allSeverityData[i]->getKey(), allSeverityData[i]->getName(), getSeverityColour(allSeverityData[i]));
			severityList.push_back(item);
	
			delete allSeverityData[i];
		}
	}

	bool AlarmFilterService::deleteNamedFilter(TA_Base_Core::NamedFilter& nmFilter)
	{
		FUNCTION_ENTRY("deleteNamedFilter");

		try
		{
			// Delete the current named filter.
			nmFilter.deleteNamedFilter();
		}
		catch (TA_Base_Core::DataException&)
		{
			return false;
		}

		FUNCTION_EXIT;
		return true;
	}


	TA_Base_Bus::Filter AlarmFilterService::convertNamedFilterToFilter(TA_Base_Core::NamedFilter& nmFilter)
	{
		FUNCTION_ENTRY("convertNamedFilterToFilter");

		// Create a filter object
		TA_Base_Bus::Filter filter;
		std::vector<std::string> sVector;
		std::vector<std::string>::iterator its;
		std::vector<unsigned long> ulVector;
		std::vector<unsigned long>::iterator itl;

		// Filter on subsystem
		try
		{
			// add filter for subsystem
			ulVector = nmFilter.getNumericFilter(AlarmFilter::FILTER_SUBSYSTEM);
			if (ulVector.size() > 0)
			{
				for (itl = ulVector.begin(); itl != ulVector.end(); ++itl)
				{
					filter.addFilter(TA_Base_Bus::Filter::FILTER_SUBSYSTEM, TA_Base_Bus::Filter::COMPARE_EQUAL, *itl);
				}
			}
			// add filter for subsystemType
			sVector = nmFilter.getStringFilter(AlarmFilter::FILTER_SUBSYSTEMTYPE);
			if (sVector.size() > 0)
			{
				for (its = sVector.begin(); its != sVector.end(); ++its)
				{
					filter.addFilter(TA_Base_Bus::Filter::FILTER_SUBSYSTEMTYPE, TA_Base_Bus::Filter::COMPARE_EQUAL, *its);
				}
			}
			// add filter for system
			sVector.clear();
			sVector = nmFilter.getStringFilter(AlarmFilter::FILTER_SYSTEM);
			if (sVector.size() > 0)
			{
				for (its = sVector.begin(); its != sVector.end(); ++its)
				{
					filter.addFilter(TA_Base_Bus::Filter::FILTER_SYSTEMTYPE, TA_Base_Bus::Filter::COMPARE_EQUAL, *its);
				}
			}

			// add filter on location
			ulVector.clear();
			ulVector = nmFilter.getNumericFilter(AlarmFilter::FILTER_LOCATION);
			if (ulVector.size() > 0)
			{
				for (itl = ulVector.begin(); itl != ulVector.end(); ++itl)
				{
					filter.addFilter(TA_Base_Bus::Filter::FILTER_LOCATION, TA_Base_Bus::Filter::COMPARE_EQUAL, *itl);
				}
			}

			// add filter on operator
			sVector.clear();
			sVector = nmFilter.getStringFilter(AlarmFilter::FILTER_OPERATOR);
			if (sVector.size() > 0)
			{
				for (its = sVector.begin(); its != sVector.end(); ++its)
				{
					// Add a filter for this STRING
					filter.addFilter(TA_Base_Bus::Filter::FILTER_OPERATOR, TA_Base_Bus::Filter::COMPARE_EQUAL, *its);
				}
			}

			// add filter on alarm type
			ulVector.clear();
			ulVector = nmFilter.getNumericFilter(AlarmFilter::FILTER_ALARMTYPE);
			if (ulVector.size() > 0)
			{
				for (itl = ulVector.begin(); itl != ulVector.end(); ++itl)
				{
					// Add a filter for this STRING
					filter.addFilter(TA_Base_Bus::Filter::FILTER_ALARM_TYPE, TA_Base_Bus::Filter::COMPARE_EQUAL, *itl);
				}
			}

			// add filter on asset
			sVector.clear();
			sVector = nmFilter.getStringFilter(AlarmFilter::FILTER_EQUIPMENT_ID);
			if (sVector.size() > 0)
			{
				for (its = sVector.begin(); its != sVector.end(); ++its)
				{
					filter.addFilter(TA_Base_Bus::Filter::FILTER_ASSET_NAME, TA_Base_Bus::Filter::COMPARE_CONTAINS, *its);
				}
			}

			// add filter on description
			sVector.clear();
			sVector = nmFilter.getStringFilter(AlarmFilter::FILTER_DESCRIPTION);
			if (sVector.size() > 0)
			{
				for (its = sVector.begin(); its != sVector.end(); ++its)
				{
					filter.addFilter(TA_Base_Bus::Filter::FILTER_DESCRIPTION, TA_Base_Bus::Filter::COMPARE_CONTAINS, *its);
				}
			}

			// add filter on severity
			ulVector.clear();
			ulVector = nmFilter.getNumericFilter(AlarmFilter::FILTER_SEVERITY);
			if (ulVector.size() > 0)
			{
				for (itl = ulVector.begin(); itl != ulVector.end(); ++itl)
				{
					filter.addFilter(TA_Base_Bus::Filter::FILTER_SEVERITY, TA_Base_Bus::Filter::COMPARE_EQUAL, *itl);
				}
			}
			else
			{
				// Create a filter that filters out all severities
				// Severities cannot be negative, so the following filter will do
				filter.addFilter(TA_Base_Bus::Filter::FILTER_SEVERITY, TA_Base_Bus::Filter::COMPARE_LESS_THAN, 0);
			}
			//add filter on omtype
			std::string omtype;
			sVector.clear();
			sVector = nmFilter.getStringFilter(AlarmFilter::FILTER_OMTYPE);
			if (sVector.size() >0) omtype = sVector[0].c_str();
			if (omtype == AlarmFilter::FILTER_OMTYPE_OPERATION)
			{
				filter.addFilter(TA_Base_Bus::Filter::FILTER_OMTYPE,
					TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::OMTYPE_OPERATION);
			}
			else if (omtype == AlarmFilter::FILTER_OMTYPE_MAINTENANCE)
			{
				filter.addFilter(TA_Base_Bus::Filter::FILTER_OMTYPE,
					TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::OMTYPE_MAINTENANCE);
			}
			else if (omtype == AlarmFilter::FILTER_OMTYPE_BOTH)
			{
				filter.addFilter(TA_Base_Bus::Filter::FILTER_OMTYPE,
					TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::OMTYPE_BOTH);
			}
			else
			{
				filter.addFilter(TA_Base_Bus::Filter::FILTER_OMTYPE,
					TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::OMTYPE_ALL);
			}

			//add filter on state
			std::string ack;
			std::string state;
			sVector.clear();
			sVector = nmFilter.getStringFilter(AlarmFilter::FILTER_ACKSTATE);
			if (sVector.size() >0) ack = sVector[0];

			sVector.clear();
			sVector = nmFilter.getStringFilter(AlarmFilter::FILTER_STATE);
			if (sVector.size() >0) state = sVector[0];

			if (ack == AlarmFilter::FILTER_ACKED)
			{
				if (state == AlarmFilter::FILTER_OPEN)
				{
					filter.addFilter(TA_Base_Bus::Filter::FILTER_STATE,
						TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::STATE_OPEN_ACKED);
				}
				else if (state == AlarmFilter::FILTER_CLOSE)
				{
					// Should never get here.
					TA_ASSERT(false, "Cannot filter on alarms that are acked AND closed");
				}
				else // Default state is ALL
				{
					filter.addFilter(TA_Base_Bus::Filter::FILTER_STATE,
						TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::STATE_OPEN_ACKED);
				}
			}
			else if (ack == AlarmFilter::FILTER_UNACKED)
			{
				if (state == AlarmFilter::FILTER_OPEN)
				{
					filter.addFilter(TA_Base_Bus::Filter::FILTER_STATE,
						TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::STATE_OPEN_UNACKED);
				}
				else if (state == AlarmFilter::FILTER_CLOSE)
				{
					filter.addFilter(TA_Base_Bus::Filter::FILTER_STATE,
						TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::STATE_CLOSED);
				}
				else // Default the state is ALL
				{
					filter.addFilter(TA_Base_Bus::Filter::FILTER_STATE,
						TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::STATE_ALL_UNACKED);
				}
			}
			else // Default the acknowledge is ALL
			{
				if (state == AlarmFilter::FILTER_OPEN)
				{
					filter.addFilter(TA_Base_Bus::Filter::FILTER_STATE,
						TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::STATE_ALL_OPEN);
				}
				else if (state == AlarmFilter::FILTER_CLOSE)
				{
					filter.addFilter(TA_Base_Bus::Filter::FILTER_STATE,
						TA_Base_Bus::Filter::COMPARE_EQUAL, TA_Base_Bus::Filter::STATE_CLOSED);
				}
				else // Default the state is ALL
				{
					// Do nothing
				}
			}

			// add filter on time
			// Get the FROM time.
			time_t fromTime = 0;
			NamedFilter::NumericList ftimeList = nmFilter.getNumericFilter(AlarmFilter::FILTER_FROMTIME);
			if (0 < ftimeList.size()) fromTime = static_cast<time_t>(ftimeList[0]);

			// Get the TO time.
			time_t toTime = 0;
			NamedFilter::NumericList ttimeList = nmFilter.getNumericFilter(AlarmFilter::FILTER_TOTIME);
			if (0 < ttimeList.size()) toTime = static_cast<time_t>(ttimeList[0]);

			if (fromTime && !toTime)
			{
				// Filter on from time
				//QDateTime ftime(fromTime);
				filter.addFilter(TA_Base_Bus::Filter::FILTER_ALARM_TIME,
					TA_Base_Bus::Filter::COMPARE_GREATER_THAN_EQUAL, fromTime);
			}
			else if (!fromTime && toTime)
			{
				// Filter on to time
				//QDateTime ttime(toTime);
				filter.addFilter(TA_Base_Bus::Filter::FILTER_ALARM_TIME,
					TA_Base_Bus::Filter::COMPARE_LESS_THAN_EQUAL, toTime);
			}
			else if (fromTime && toTime)
			{
				// Filter on both from time and to time
				//QDateTime ftime(fromTime);
				//QDateTime ttime(toTime);
				TA_Base_Bus::Filter::SDateRange range;
				range.startTime = fromTime;
				range.endTime = toTime;

				filter.addFilter(TA_Base_Bus::Filter::FILTER_ALARM_TIME_RANGE,
					TA_Base_Bus::Filter::COMPARE_RANGE_INCLUSIVE, range);
			}

			filter.setFilterName(nmFilter.getName());
		}
		catch (...)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Failed to convert Named Filter to Filter object");
			TA_ASSERT(false, "Failed to convert Named Filter to Filter object");

		}

		FUNCTION_EXIT;
		return filter;
	}


	TA_Base_Core::NamedFilter AlarmFilterService::convertFilterToNamedFilter(TA_Base_Bus::Filter& filter)
	{
		FUNCTION_ENTRY("convertFilterToNamedFilter");

		TA_Base_Core::NamedFilter nmFilter;
		try
		{
			std::vector<TA_Base_Bus::Filter::FilterDetails> filterDetails = filter.getFilters();

			for (std::vector<TA_Base_Bus::Filter::FilterDetails>::iterator iter = filterDetails.begin(); iter != filterDetails.end(); ++iter)
			{
				TA_Base_Bus::Filter::FilterDetails fDetails = *iter;
				switch (fDetails.type)
				{
				case TA_Base_Bus::Filter::FILTER_SUBSYSTEM:
					if (fDetails.comparison == TA_Base_Bus::Filter::COMPARE_EQUAL)
						nmFilter.addNumericFilter(AlarmFilter::FILTER_SUBSYSTEM, fDetails.value.ulValue);
					break;
				case TA_Base_Bus::Filter::FILTER_SUBSYSTEMTYPE:
					if (fDetails.comparison == TA_Base_Bus::Filter::COMPARE_EQUAL)
						nmFilter.addStringFilter(AlarmFilter::FILTER_SUBSYSTEMTYPE, *fDetails.value.stringValue);
					break;

				case TA_Base_Bus::Filter::FILTER_SYSTEMTYPE:
					if (fDetails.comparison == TA_Base_Bus::Filter::COMPARE_EQUAL)
						nmFilter.addStringFilter(AlarmFilter::FILTER_SYSTEM, *fDetails.value.stringValue);
					break;

				case TA_Base_Bus::Filter::FILTER_LOCATION:
					if (fDetails.comparison == TA_Base_Bus::Filter::COMPARE_EQUAL)
						nmFilter.addNumericFilter(AlarmFilter::FILTER_LOCATION, fDetails.value.ulValue);
					break;

				case TA_Base_Bus::Filter::FILTER_OPERATOR:
					if (fDetails.comparison == TA_Base_Bus::Filter::COMPARE_EQUAL)
						nmFilter.addStringFilter(AlarmFilter::FILTER_OPERATOR, *fDetails.value.stringValue);
					break;
				case TA_Base_Bus::Filter::FILTER_OMTYPE:
					if (fDetails.value.alarmOMTypeValue == TA_Base_Bus::Filter::OMTYPE_OPERATION)
					{
						nmFilter.addStringFilter(AlarmFilter::FILTER_OMTYPE, AlarmFilter::FILTER_OMTYPE_OPERATION);
					}
					else if (fDetails.value.alarmOMTypeValue == TA_Base_Bus::Filter::OMTYPE_MAINTENANCE)
					{
						nmFilter.addStringFilter(AlarmFilter::FILTER_OMTYPE, AlarmFilter::FILTER_OMTYPE_MAINTENANCE);
					}
					else if (fDetails.value.alarmOMTypeValue == TA_Base_Bus::Filter::OMTYPE_BOTH)
					{
						nmFilter.addStringFilter(AlarmFilter::FILTER_OMTYPE, AlarmFilter::FILTER_OMTYPE_BOTH);
					}
					else if (fDetails.value.alarmOMTypeValue == TA_Base_Bus::Filter::OMTYPE_ALL)
					{
						nmFilter.addStringFilter(AlarmFilter::FILTER_OMTYPE, AlarmFilter::FILTER_OMTYPE_ALL);
					}
					break;

				case TA_Base_Bus::Filter::FILTER_ALARM_TYPE:
					if (fDetails.comparison == TA_Base_Bus::Filter::COMPARE_EQUAL)
						nmFilter.addNumericFilter(AlarmFilter::FILTER_ALARMTYPE, fDetails.value.ulValue);
					break;

				case TA_Base_Bus::Filter::FILTER_ASSET_NAME:
					if (fDetails.comparison == TA_Base_Bus::Filter::COMPARE_CONTAINS)
						nmFilter.addStringFilter(AlarmFilter::FILTER_EQUIPMENT_ID, *fDetails.value.stringValue);
					break;

				case TA_Base_Bus::Filter::FILTER_DESCRIPTION:
					if (fDetails.comparison == TA_Base_Bus::Filter::COMPARE_CONTAINS)
						nmFilter.addStringFilter(AlarmFilter::FILTER_DESCRIPTION, *fDetails.value.stringValue);
					break;

				case TA_Base_Bus::Filter::FILTER_SEVERITY:
					if (fDetails.comparison == TA_Base_Bus::Filter::COMPARE_EQUAL)
						nmFilter.addNumericFilter(AlarmFilter::FILTER_SEVERITY, fDetails.value.ulValue);
					break;

				case TA_Base_Bus::Filter::FILTER_STATE:
					if (fDetails.value.alarmStateValue == TA_Base_Bus::Filter::STATE_OPEN_ACKED)
					{
						nmFilter.addStringFilter(AlarmFilter::FILTER_ACKSTATE, AlarmFilter::FILTER_ACKED);
						nmFilter.addStringFilter(AlarmFilter::FILTER_STATE, AlarmFilter::FILTER_OPEN);
					}
					else if (fDetails.value.alarmStateValue == TA_Base_Bus::Filter::STATE_OPEN_UNACKED)
					{
						nmFilter.addStringFilter(AlarmFilter::FILTER_ACKSTATE, AlarmFilter::FILTER_UNACKED);
						nmFilter.addStringFilter(AlarmFilter::FILTER_STATE, AlarmFilter::FILTER_OPEN);
					}
					else if (fDetails.value.alarmStateValue == TA_Base_Bus::Filter::STATE_CLOSED)
					{
						nmFilter.addStringFilter(AlarmFilter::FILTER_ACKSTATE, AlarmFilter::FILTER_UNACKED);
						nmFilter.addStringFilter(AlarmFilter::FILTER_STATE, AlarmFilter::FILTER_CLOSE);
					}
					else if (fDetails.value.alarmStateValue == TA_Base_Bus::Filter::STATE_ALL_UNACKED)
					{
						nmFilter.addStringFilter(AlarmFilter::FILTER_ACKSTATE, AlarmFilter::FILTER_UNACKED);
						nmFilter.addStringFilter(AlarmFilter::FILTER_STATE, AlarmFilter::FILTER_ALL);
					}
					else if (fDetails.value.alarmStateValue == TA_Base_Bus::Filter::STATE_ALL_OPEN)
					{
						nmFilter.addStringFilter(AlarmFilter::FILTER_ACKSTATE, AlarmFilter::FILTER_ALL);
						nmFilter.addStringFilter(AlarmFilter::FILTER_STATE, AlarmFilter::FILTER_OPEN);
					}
					break;

				case TA_Base_Bus::Filter::FILTER_ALARM_TIME:
					if (fDetails.comparison == TA_Base_Bus::Filter::COMPARE_GREATER_THAN_EQUAL)
					{
						time_t ftime = fDetails.value.ulValue;
						nmFilter.addNumericFilter(AlarmFilter::FILTER_FROMTIME, ftime);
					}
					if (fDetails.comparison == TA_Base_Bus::Filter::COMPARE_LESS_THAN_EQUAL)
					{
						time_t ttime = fDetails.value.ulValue;
						nmFilter.addNumericFilter(AlarmFilter::FILTER_TOTIME, ttime);
					}
					break;

				case TA_Base_Bus::Filter::FILTER_ALARM_TIME_RANGE:
					if (fDetails.comparison == TA_Base_Bus::Filter::COMPARE_RANGE_INCLUSIVE)
					{
						time_t ftime = fDetails.value.dateRange.startTime;
						time_t ttime = fDetails.value.dateRange.endTime;
						nmFilter.addNumericFilter(AlarmFilter::FILTER_FROMTIME, ftime);
						nmFilter.addNumericFilter(AlarmFilter::FILTER_TOTIME, ttime);
					}
					break;

				default: // Do nothing
					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "No filter settings from named filter for filter");

				}
			}

			nmFilter.setName(filter.getFilterName());

		}

		catch (...)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Failed to convert Filter to Named Filter object");
			TA_ASSERT(false, "Failed to convert Filter to Named Filter object");

		}
		FUNCTION_EXIT;
		return nmFilter;
	}

	bool AlarmFilterService::checkInNamedFilterDatabase(std::string curName)
	{
		FUNCTION_ENTRY("checkInNamedFilterDatabase");

		try
		{
			// Get a list of the filters.
			TA_Base_Core::NamedFilterAccessFactory::FilterNameList filters = TA_Base_Core::NamedFilterAccessFactory::getInstance().getAllFilterNames();

			// Now add each to the combo box.
			TA_Base_Core::NamedFilterAccessFactory::FilterNameList::iterator namedFilterIt;
			for (namedFilterIt = filters.begin(); namedFilterIt != filters.end(); namedFilterIt++)
			{
				if (curName == namedFilterIt->first) return true;
			}

		}
		catch (TA_Base_Core::DataException&)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Could not get a list of named filters!");
		}

		FUNCTION_EXIT;
		return false;
	}

	// TD17081- modified the parameter to the severitydata instead of the item key to
	// limit database access
	unsigned long AlarmFilterService::getSeverityColour(TA_Base_Core::IAlarmSeverityData* severityData)
	{
		unsigned long severityColour =0;

		TA_ASSERT(severityData != NULL, "Invalid severity data.");
		try
		{
			severityColour = severityData->getSeverityColourKey(TA_Base_Core::IAlarmSeverityData::OPEN_UNACKED_BG1);

		}
		//TD17081 - modified exception handling
		catch (const TA_Base_Core::DataException& ex)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "DataException", ex.what());
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Could not retrieve severity colour. Setting to default.");
		}
		catch (...)
		{
			LOG_EXCEPTION_CATCH(SourceInfo, "Unknown", "Unknown");
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Could not retrieve severity colour. Setting to default.");
		}
		//++TD17081
		return severityColour;
	}

	std::list<std::string> AlarmFilterService::getFilterNameList()
	{
		FUNCTION_ENTRY("getFilterNameList");
		std::list<std::string> retVal;

		try
		{
			// Add the default filter to the ComboBox for default selection
			retVal.push_back(AlarmFilter::FILTER_DEFAULT.c_str());

			// Get a list of the filters.
			TA_Base_Core::NamedFilterAccessFactory::FilterNameList filters = TA_Base_Core::NamedFilterAccessFactory::getInstance().getAllFilterNames();

			// Now add each to the combo box.
			TA_Base_Core::NamedFilterAccessFactory::FilterNameList::iterator namedFilterIt;
			for (namedFilterIt = filters.begin(); namedFilterIt != filters.end(); namedFilterIt++)
			{
				retVal.push_back(namedFilterIt->first.c_str());
			}

		}
		catch (TA_Base_Core::DataException&)
		{
			LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError, "Could not get a list of named filters!");
		}

		FUNCTION_EXIT;
		return retVal;
	}
	
}
