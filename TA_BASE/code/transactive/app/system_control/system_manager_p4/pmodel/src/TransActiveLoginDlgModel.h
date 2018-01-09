#pragma once

#include "bus/generic_gui_pmod/src/transactiveviewmodel.h"
#include "bus/common_subsystem_facade/src/SecuritySubsystemFacade.h"

#include "core/data_access_interface/src/IOperator.h"
#include "core/data_access_interface/src/OperatorAccessFactory.h"
#include "core/data_access_interface/src/ILocation.h"
#include "core/data_access_interface/src/LocationAccessFactory.h"
#include "core/data_access_interface/src/IProfile.h"
#include "core/data_access_interface/src/ProfileAccessFactory.h"
#include "core/data_access_interface/entity_access/src/IConsole.h"
#include "core/data_access_interface/entity_access/src/ConsoleAccessFactory.h"

namespace TA_Base_Core
{
	class AuthenticationSecurityException;
}

namespace TA_Base_App
{



	class TransActiveLoginDlgModel :
		public TA_Base_Bus::AbstractPModel
	{
	public:
		TransActiveLoginDlgModel(TA_Base_Bus::IApplicationController* appController, const char * pmodelName);
		virtual ~TransActiveLoginDlgModel(void);

		typedef std::map<unsigned long, TA_Base_Core::ILocation*> LocationMap;
		typedef std::map<unsigned long, TA_Base_Core::IProfile*> ProfileMap;

		typedef std::vector<TA_Base_Core::IOperator*> OperatorVector;
		typedef std::vector<TA_Base_Core::IProfile*> ProfileVector;
		typedef std::vector<TA_Base_Core::ILocation*> LocationVector;

	public:
		/**
		* checkValidConsole
		*
		* This will retrieve the console key and location description for this 
		* machine from the database.
		*
		* @exception CannotUseSysControllerException - This indicates that the configuration for the console
		*                                              could not be found in the database
		* @exception GenericGUIException - This indicates that the actual hostname of the machine could not
		*                                  be found. This exception is unlikely
		*/
		void checkValidConsole();

		/**
		* getListOfProfiles
		*
		* PreCondition: The operator passed is not NULL.
		*
		* @param IOperator* - The list of profiles is based on the chosen operator. This is the 
		*                     operator to load the profiles for.
		* @param onlyOverridable - If this is true, it will only return the ones that are overridable
		*                    by the current login profile.  Otherwise, it will return all profiles.
		*
		* @return ProfileVector - This vector contains all the profiles in the database available
		*                          for the specified operator.  Only returns the ones that are overridable
		*                          by the operator if the onlyOverridable is set to true.
		*/
		ProfileVector getListOfProfiles(TA_Base_Core::IOperator* theOperator);


		/**
		* getListOfLocations
		*
		* PreCondition: The profile passed is not NULL.
		*
		* @param IPorilfe* - The list of locations is based on the chosen profile. This is the 
		*                    profile to load the locations for.
		* @return ProfileVector - This vector contains all the locations in the database available
		*                          for the specified profile.
		*/
		LocationVector getListOfLocations(TA_Base_Core::IProfile* theProfile);

		/**
		* verifyDetailsWithDatabase
		*
		* Called when the authentication agent cannot be used. Verifies that
		* the user's password is correct directly with the database.
		*
		* @param details  The user's details to verify.
		*
		* @exception AuthenticationSecurityException  Thrown if the password is
		*                                             incorrect.
		* @exception DataException                    Thrown if the user is not found.
		* @exception DatabaseException                Thrown if there is a problem
		*                                             accessing the database.
		*/
		void verifyDetailsWithDatabase(const TA_Base_Bus::SessionDetails& details);


		/**
		* checkUserPermissions
		*
		* This will check if this user has permission to run this application. If they do not then this
		* will throw an exception
		*
		* @exception GenericGUIException - This indicates that either the user did not have permission to run
		*                                  the application or there was an error attempting to retrieve the rights.
		*/
		virtual void checkUserPermissions();


		/**
		* authenticate
		*
		* This attempts to authenticate the user with the TransActive system.
		*
		* @param SessionDetails - The details the user has entered for the session
		*
		* @exception AuthenticationSecurityException - The user name or password etc the user has entered were
		*                                              invalid
		* @exception AuthenticationAgentException - The authentication agent encountered strange errors
		* @exception ObjectResolutionException - The authentication agent could not be found
		*/
		virtual void authenticate(const TA_Base_Bus::SessionDetails& sessionDetails);

	public:
		virtual void allocViewResource();
		unsigned int TransActiveLoginDlgModel::getErrorIdFromException( const TA_Base_Core::AuthenticationSecurityException &ex );
	public:
		unsigned long m_consoleKey; // The key for this console. We retrieve this early as there is no
		// point us continuing if it is not in the database.

		std::string m_locationDescription; // The description of this console's location.


		// This is a list of all operators, profiles and locations. 
		OperatorVector      m_allOperators;
		ProfileMap          m_profileMap;
		LocationMap         m_locationMap;

        // These member variables hold the user's current selections from the dialog.
        TA_Base_Core::IOperator* m_selectedOperator;
        TA_Base_Core::IProfile*  m_selectedProfile;

	};
}