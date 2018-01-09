#include "TransActiveLoginDlgModel.h"

#include "bus/common_subsystem_facade/src/SecuritySubsystemFacade.h"

#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"

#include "../../src/resource.h"

#include "core/exceptions/src/DataException.h"
#include "core/exceptions/src/DatabaseException.h"
#include "core/exceptions/src/CannotUseSysControllerException.h"
#include "core/exceptions/src/EntityTypeException.h"
#include "core/exceptions/src/AuthenticationSecurityException.h"
#include "core/exceptions/src/AuthenticationAgentException.h"
#include "core/exceptions/src/ObjectResolutionException.h"
#include "core/exceptions/src/GenericGUIException.h"
#include "core/exceptions/src/CannotUseSysControllerException.h"


namespace TA_Base_App
{


	TransActiveLoginDlgModel::TransActiveLoginDlgModel(TA_Base_Bus::IGUIApplicationController* appController, const char * pmodelName)
		: TA_Base_Bus::AbstractPModel(appController, pmodelName)
        , m_selectedOperator(NULL)
        , m_selectedProfile(NULL)
	{
	}

	TransActiveLoginDlgModel::~TransActiveLoginDlgModel(void)
	{
		try
		{
			//
			// Delete all the operators
			//
			OperatorVector::iterator operatorIter = m_allOperators.begin();
			while (operatorIter != m_allOperators.end())
			{
				if (*operatorIter != NULL)
				{
					delete *operatorIter;
					*operatorIter = NULL;
				}
				++operatorIter;
			}
			m_allOperators.erase( std::remove(m_allOperators.begin(), m_allOperators.end(), static_cast<TA_Base_Core::IOperator*>(NULL)), m_allOperators.end() );

			//
			// Delete all the profiles
			//
			for (ProfileMap::iterator prof = m_profileMap.begin(); prof != m_profileMap.end(); prof++)
			{
				delete prof->second;
				prof->second = NULL;
			}

			//
			// Delete all the locationss
			//
			for (LocationMap::iterator loc = m_locationMap.begin(); loc != m_locationMap.end(); loc++)
			{
				delete loc->second;
				loc->second = NULL;
			}
		}
		catch (...)
		{
			LOG_EXCEPTION_CATCH( SourceInfo, "Unknown exception", "Some exception was caught in the destructor. Ignoring");
		}
	}

	void TransActiveLoginDlgModel::checkValidConsole()
	{
		std::string hostName = m_appController->getConsoleName();

		LOG1( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "This console is: %s",hostName.c_str() );

		try
		{
			std::auto_ptr<TA_Base_Core::IConsole> console( TA_Base_Core::ConsoleAccessFactory::getInstance().getConsoleFromAddress(hostName) );
			unsigned long locationKey = console->getLocation();
			m_consoleKey = console->getKey();

			std::auto_ptr<TA_Base_Core::ILocation> location( TA_Base_Core::LocationAccessFactory::getInstance().getLocationByKey(locationKey) );
			m_locationDescription = location->getDescription();

		}
		catch ( const TA_Base_Core::DataException& )
		{
			LOG( SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "DataException", "Error occurred while attempting to look up console in the database");
			TA_THROW( TA_Base_Core::CannotUseSysControllerException("Console has not been configured.") );
		}
		catch ( const TA_Base_Core::DatabaseException& )
		{
			LOG( SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "DatabaseException", "Error occurred while attempting to look up console in the database");
			TA_THROW( TA_Base_Core::CannotUseSysControllerException("Console has not been configured.") );
		}
		catch ( const TA_Base_Core::EntityTypeException& )
		{
			LOG( SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "EntityTypeException", "Error occurred while attempting to look up console in the database");
			TA_THROW( TA_Base_Core::CannotUseSysControllerException("Console has not been configured.") );
		}
	}

	TransActiveLoginDlgModel::ProfileVector TransActiveLoginDlgModel::getListOfProfiles(TA_Base_Core::IOperator* theOperator)
	{
		LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry,"getListOfProfiles");

		TA_ASSERT(theOperator != NULL, "The operator passed was equal to NULL");

		ProfileVector associatedProfiles;

		// Load the new profiles
		try
		{
			std::vector<unsigned long> profiles = theOperator->getAssociatedProfileKeys();
			for (unsigned int i = 0 ; i < profiles.size(); i++)
			{
				ProfileMap::iterator profIter = m_profileMap.find(profiles[i]);
				if (profIter == m_profileMap.end())
				{
					LOG_GENERIC(SourceInfo,TA_Base_Core::DebugUtil::DebugError,
						"Unable to find the profile, %d, that is associated with operator, %d.  Ignoring", profiles[i], theOperator->getKey());
				}
				else
				{
					associatedProfiles.push_back(profIter->second);
				}
			}
		}
		catch ( TA_Base_Core::DataException& )
		{
			LOG( SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "DataException", "Error occurred while attempting to retrieve a list of profiles from the database");
		}
		catch ( TA_Base_Core::DatabaseException& )
		{
			LOG( SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "DatabaseException", "Error occurred while attempting to retrieve a list of profiles from the database");
		}

		// returning an array which points to our internal copies
		return associatedProfiles;
	}


	TransActiveLoginDlgModel::LocationVector TransActiveLoginDlgModel::getListOfLocations(TA_Base_Core::IProfile* theProfile)
	{
		LOG( SourceInfo, TA_Base_Core::DebugUtil::FunctionEntry,"getListOfLocations");

		TA_ASSERT(theProfile != NULL, "The profile passed was equal to NULL");

		LocationVector associatedLocations;

		// Load the new locations
		try
		{
			std::vector<unsigned long> locations = theProfile->getAssociatedLocationKeys();
			for (unsigned int i = 0 ; i < locations.size(); i++)
			{
				LocationMap::iterator locIter = m_locationMap.find(locations[i]);
				if (locIter == m_locationMap.end())
				{
					LOG_GENERIC(SourceInfo, TA_Base_Core::DebugUtil::DebugError,
						"Unable to find the location, %d, that is associated with profile, %d.  Ignoring", locations[i], theProfile->getKey());
				}
				else
				{
					associatedLocations.push_back(locIter->second);
				}
			}
		}
		catch ( TA_Base_Core::DataException& )
		{
			LOG( SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "DataException", "Error occurred while attempting to retrieve a list of locations from the database");
		}
		catch ( TA_Base_Core::DatabaseException& )
		{
			LOG( SourceInfo, TA_Base_Core::DebugUtil::ExceptionCatch, "DatabaseException", "Error occurred while attempting to retrieve a list of locations from the database");
		}

		// returning an array which points to our internal copies
		return associatedLocations;
	}


	void TransActiveLoginDlgModel::verifyDetailsWithDatabase(const TA_Base_Bus::SessionDetails& details)
	{
		unsigned long operatorKey = details.operatorKey;
		std::string enteredPassword = details.password;

		// Get the real password.
		TA_Base_Core::IOperator* operatorData = TA_Base_Core::OperatorAccessFactory::
			getInstance().getOperator(operatorKey, false);
		std::string actualPassword = operatorData->getPassword();
		delete operatorData;

		// Check that the passwords match.
		if (enteredPassword.compare(actualPassword) != 0)
		{
			TA_THROW(TA_Base_Core::AuthenticationSecurityException("The password is incorrect", 
				TA_Base_Core::AuthenticationSecurityException::INVALID_PASSWORD));
		}
	}

	void TransActiveLoginDlgModel::allocViewResource()
	{
		//first check it is valid console or not;
		checkValidConsole();

		try
		{
			m_allOperators = TA_Base_Core::OperatorAccessFactory::getInstance().getAllNonSystemOperators(false, false); 

			ProfileVector profiles = TA_Base_Core::ProfileAccessFactory::getInstance().getAllNonSystemProfiles(false);
			unsigned int i = 0;
			for ( i = 0; i < profiles.size(); i++)
			{
				m_profileMap.insert(ProfileMap::value_type(profiles[i]->getKey(), profiles[i]));
			}

			LocationVector locations = TA_Base_Core::LocationAccessFactory::getInstance().getAllLocations(false);
			for (i = 0; i < locations.size(); i++)
			{
				m_locationMap.insert(LocationMap::value_type(locations[i]->getKey(), locations[i]));
			}
		}
		catch ( const TA_Base_Core::DataException& )
		{
			LOG_EXCEPTION_CATCH( SourceInfo, "DataException", "Could not retrieve the operators to add to the combo box");
		}
		catch ( const TA_Base_Core::DatabaseException& )
		{
			LOG_EXCEPTION_CATCH( SourceInfo, "DatabaseException", "Could not retrieve the operators to add to the combo box");
		}
	}

	//it will throw database exception and security exception
	void TransActiveLoginDlgModel::authenticate(const TA_Base_Bus::SessionDetails& sessionDetails)
	{
		FUNCTION_ENTRY("authenticate");

		std::auto_ptr<TA_Base_Core::IOperator> iOperator(TA_Base_Core::OperatorAccessFactory::getInstance().getOperator(sessionDetails.operatorKey, false));

		//xinsong test
		//std::string session = "1867cf5f0f1950485c087d51fae473f7ca97";

		std::string session = TA_Base_Bus::SecuritySubsystemAccessor::getInstance()->requestSession(sessionDetails.operatorKey, 
			sessionDetails.profileKey, 
			sessionDetails.locationKey, 
			sessionDetails.consoleKey,
			sessionDetails.password,
			false);

		TA_Base_Core::RunParams::getInstance().set(RPARAM_SESSIONID, session.c_str());

		FUNCTION_EXIT;
	}


	void TransActiveLoginDlgModel::checkUserPermissions()
	{
		FUNCTION_ENTRY("checkUserPermissions");
		
		//xinsong test
		//return;

		TA_Base_Core::IEntityData * guiEntity = m_appController->getEntity();
		TA_ASSERT( guiEntity != NULL, "The m_guiEntity member must be set up before this method is called");

		TA_Base_Core::IResource* resource = NULL;
		bool permitted(false);
		std::string reason("");

		try
		{
			resource = TA_Base_Core::ResourceAccessFactory::getInstance().getResourceFromEntity( guiEntity->getKey() );
			int resourceId = resource->getKey();

			TA_Base_Bus::EDecisionModule decisionModule = TA_Base_Bus::UNDEFINED_DM;

			permitted = TA_Base_Bus::SecuritySubsystemAccessor::getInstance()->isActionPermittedOnResource(
				TA_Base_Core::RunParams::getInstance().get(RPARAM_SESSIONID), 
				resourceId, 
				TA_Base_Bus::aca_RUN_APPLICATION, 
				reason, 
				decisionModule);

			// Tidy up
			delete resource;
			resource = NULL;
		}
		catch ( ... )
		{
			if (resource != NULL)
			{
				delete resource;
				resource = NULL;
			}
			LOG_EXCEPTION_CATCH( SourceInfo, "Unknown", "Converting into a GenericGui initialisation error");
			TA_THROW( TA_Base_Core::GenericGUIException( TA_Base_Core::GenericGUIException::UNKNOWN_ACCESS_RIGHTS ) );
		}


		if (!permitted)
		{
			LOG_GENERIC( SourceInfo, TA_Base_Core::DebugUtil::DebugInfo, "Cannot run this application because %s", reason.c_str());
			TA_THROW ( TA_Base_Core::GenericGUIException( TA_Base_Core::GenericGUIException::ACCESS_DENIED ) );
		}

		FUNCTION_EXIT;
	}

	unsigned int TransActiveLoginDlgModel::getErrorIdFromException( const TA_Base_Core::AuthenticationSecurityException &ex )
	{
		unsigned int errorId;

		switch( ex.getReason() )
		{
		case(TA_Base_Core::AuthenticationSecurityException::INVALID_USER):           // The supplied user is not known to the system
			errorId = IDS_INVALID_USER;
			break;

		case(TA_Base_Core::AuthenticationSecurityException::INVALID_PASSWORD):       // The supplied password is incorrect for the user
			errorId = IDS_INVALID_PASSWORD;
			break;

		case(TA_Base_Core::AuthenticationSecurityException::INVALID_PROFILE):        // The supplied profile is not known to the system
			errorId = IDS_INVALID_PROFILE;
			break;

		case(TA_Base_Core::AuthenticationSecurityException::INVALID_LOCATION):       // The supplied location is not known to the system
			errorId = IDS_INVALID_LOCATION;
			break;

		case(TA_Base_Core::AuthenticationSecurityException::INVALID_WORKSTATION):    // The supplied workstation is not known to the system
			errorId = IDS_INVALID_WORKSTATION;
			break;

		case(TA_Base_Core::AuthenticationSecurityException::PROFILE_NOT_PERMITTED):  // The profile is not allowed for the supplied user
			errorId = IDS_PROFILE_NOT_PERMITTED;
			break;

		case(TA_Base_Core::AuthenticationSecurityException::LOCATION_NOT_PERMITTED): // The location is not allowed for the supplied profile
			errorId = IDS_LOCATION_NOT_PERMITTED;
			break;

		default:
			errorId = IDS_UNKNOWN_ERROR;
			break;
		}	

		return errorId;
	}

}