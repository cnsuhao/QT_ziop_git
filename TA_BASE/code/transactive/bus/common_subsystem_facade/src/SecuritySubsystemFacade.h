#ifndef SECURITY_SUBSYSTEM_FACADA_H__
#define SECURITY_SUBSYSTEM_FACADA_H__

#include "bus/security/rights_library/src/RightsLibrary.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"
#include "bus/security/authentication_library/src/SessionInfo.h"
#include "core/synchronisation/src/NonReEntrantThreadLockable.h"
#include <string>
#include <vector>

namespace TA_Base_Bus
{
    class AuthenticationLibrary;
    class RightsLibrary;

	struct SessionDetails
	{
		std::string password;
		unsigned long operatorKey;
		unsigned long profileKey;
		unsigned long locationKey;
		unsigned long consoleKey;
	};

    class ISecuritySubsystemAccessor
    {
    public :

		virtual ~ISecuritySubsystemAccessor() {};

    public:

        virtual bool canIgnoreRightsCheck() = 0;

        // ----------------- Begin Rights Lib virtual method -----------------------------

		/**
		  * isActionPermittedOnResource
		  *
		  * Makes a decision as th wheter a *subject* can perform
		  * an *action* on a *resource*. 
		  * This library does not catch any exceptions.
		  *
		  * @return bool, true == permit, false == deny.
		  *
		  * @param 	sessionId,      used to look up profiles etc.
		  * 		resourceKey,    only resources with an associated key
		  * 		                are access controlled.
		  * 		actionKey,      only actions with an associated key
		  * 		                are access controlled.
		  * 		reasonString,   only populated when a request is denied.
		  * 		decisionModule, some clients of the library may want to
		  * 		                behave differently depending on why the
		  * 		                deny decision was made.
		  * 
		  * @exceptions RightsException, If either insufficient attributes
		  *                              were found by the rights processor or
		  *                              a policy applying to the request did not
		  *                              exits the exception string will contain
		  *                              Indeterminate OR Not Applicable respectively.
		  *                              Otherwise the sting will contian the message
		  *                              retrived from any third-party exceptions
		  *                              caught by this method.
		  */
		virtual bool isActionPermittedOnResource(
            const std::string& sessionId, unsigned long resourceKey,
            unsigned long actionKey, std::string& reasonString,
            EDecisionModule& decisionModule) = 0;

        virtual bool isActionPermittedOnResourceAndLocSub(
            const std::string& sessionId, unsigned long resourceKey, unsigned long locationKey, unsigned long subsystemKey,
            unsigned long actionKey, std::string& reasonString,
            EDecisionModule& decisionModule ) = 0;


        virtual void isActionPermittedOnResources(
            const std::string& sessionId, ResourceList& resourceKeys,ResourceList& returnResourceList,
            unsigned long actionKey, std::string& reasonString,
            EDecisionModule& decisionModule ) = 0;

        virtual void areActionsPermittedOnSubsystemAndLocSub(
            const std::string& sessionId,unsigned long resourceKey,  unsigned long locationKey, unsigned long subsystemKey,
            ActionList& actionKeys,ActionList& returnActionList, std::string& reasonString,
            EDecisionModule& decisionModule ) = 0;

        virtual bool isActionPermittedOnResourceAndLocSubWithoutDuty(
            const std::string& sessionId, unsigned long resourceKey, unsigned long locationKey, unsigned long subsystemKey,
            unsigned long actionKey, std::string& reasonString,
            EDecisionModule& decisionModule ) = 0;

        virtual void resetCache() = 0;

        // ----------------- End Rights Lib virtual method -----------------------------

        // ----------------- Begin Authentication Lib virtual method -----------------------------

        /**
         * requestSession
         *
	     * This method checks the supplied credentials and creates sessions if they are sufficient.
         *
	     * @param    userId         The key to the user id in the operation table
	     * @param    profileId      The key to the profile the user is logging in under
	     * @param    locationId     The key to the location the user wants to use the profile for
	     * @param    workstationId  The key to the workstation from which the user is attempting to gain access
	     * @param    password       The password with which the user is attempting to log in.
         *
         * @return The ID of the new session as a string
         *
         * @exception AuthenticationSecurityException Thrown if the log in attpemt fails for security reasons.
         * @exception AuthenticationAgentException Thrown if there is a general error with the
         * log in attempt.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
	     */
        virtual std::string requestSession(unsigned long userId, 
                                   unsigned long profileId, 
                                   unsigned long locationId, 
                                   unsigned long workstationId, 
                                   const std::string& password,
								   bool display = false) = 0;

        /**
         * changePassword
         *
         * This method updates the password for the user of the specified session, and notifies all
         * other Authentication Agents of the change.
         *
         * @param sessionId     The session ID from which the password change request is being made.
         * @param newPassword   The new password to set for the user related to the specified session.
         *
         * @return true if the password change was successful, false otherwise.
         *
         * @exception AuthenticationSecurityException Thrown if the session is not valid.
         * @exception AuthenticationAgentException Thrown if there is a problem changing the password.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
         *
         * pre: The password CANNOT be the empty string.
         */
        virtual void changePassword(const std::string& sessionId, const std::string& oldPassword, const std::string& newPassword) = 0;


        /**
         * beginOverride
         *
         * Associates the profile the overriding user is logging into with the session of the base user.
         *
         * @param sessionId      The session that is being overriden
         * @param userId         The key to the user id in the operation table
	     * @param profileId      The key to the profile the user is logging in under
	     * @param locationId     The key to the location the user wants to use the profile for
	     * @param password       The password with which the user is attempting to log in.
         *
         * Note: The workstation ID is not a parameter, as it is taken from the passed in session ID.
         *
         * @return true if the override was successful
         *
         * @exception AuthenitcationSecurityException Thrown if the override is denied for seucrity reasons.
         * @exception AuthenticationAgentException Thrown if there is a probelm initaiting override.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
         */
	    virtual void beginOverride(const std::string& sessionId, 
                           unsigned long userId, 
                           unsigned long profileId, 
                           unsigned long locationId, 
                           const std::string& password) = 0;


        /**
         * endOverride
         *
         * This method removes any additional profiles associated with the specified session.
         *
         * @param sessionId     The session for which to remove overrides.
         *
         * @exception AuthenticationAgentException Thrown if there is a problem removing the overrides.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
         */
	    virtual void endOverride( const std::string& sessionId) = 0;


        /**
         * changeProfile
         *
         * This method moves the operator logged into the specified session to a new profile, with a new session ID. This method
         * creates the new session before ending the old one, to allow the operator to continue using their current session of the
         * log in is denied.
         *
         * @param sessionId     The session the operator is currently using.
         * @param profileId     The ID of the new profile the operator wishes to change to.
         * @param locationId    The ID of the location for the new profile
         * @param password      The password for the new user.
         *
         * @return The ID of the new session as a string
         *
         * @exception AuthenitcationSecurityException Thrown if the override is denied for seucrity reasons.
         * @exception AuthentiactionAgentException Thrown if there is a problem logging the operator into the new session.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
         */
        virtual std::string changeProfile(const std::string& sessionId, 
                                  unsigned long profileId, 
                                  unsigned long locationId, 
                                  const std::string& password,
								  bool display = false) = 0;


	    /**
         * getProfiles
         *
	     * Checks the supplied session Id to determine if it is a current session (that is, not yet expired), and if it is, 
         * will return the profiles associated with the session.
         *
	     * @param    sessionId    The session Id to check for validity.
         *
         * @return A vector of profileId's representing the profiles associated with the session. If the vector is empty
         * (i.e. vector.size() == 0), the session is not currently valid.
         *
         * @exception AuthenticationAgentException Thrown if there is a problem retrieving the profile IDs.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
	     */
        virtual void getProfiles(const std::string& sessionId, std::vector<unsigned long>& vtProfile ) = 0;

        /**
         * getSessionInfo
         *
         * This method retrieves information about the requested session. It should be used by applications wishing
         * to determine session information, rather than going directly to the security tables in the database.
         *
         * @param sessionId             The ID of the session for which to retrieve information
         * @param requestingSessionId   The ID of the session making the request.
         *
         * @return The information about this session in a SessionInfo struct.
         *
         * @exception AuthentiactionAgentException Thrown if there is a problem retrieving the session information.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
         */
        virtual SessionInfo getSessionInfo(const std::string& sessionId) = 0;


        /**
         * endSession
         *
         * Ends the specified session, by removing it from the session register. After this method has been called,
         * any calls to getProfiles() will return an empty vector.
         *
         * @param sessionId     The ID of the session to end.
         *
         * @exception AuthenticationSecurityExcetpion Thrown if the session is not valid.
         * @exception AuthenticationAgentExcetpion  Thrown if there is a problem ending the session.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
         */
        virtual void endSession( const std::string& sessionId) = 0;


        /**
         * getActiveSessions
         *
         * Retrieves a list of all the currently active sessions in the system.
         *
         * @param sessionId     The ID of the session that is retrieving all others.
         *
         * @return the list of all currently active sessions as a vector of SessionInfo objects
         *
         * @exception AuthenticationAgentExcetpion  Thrown if there is a problem ending the session.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
         */
        virtual void getActiveSessions(const std::string& sessionId, std::vector<SessionInfo>& vtSessionInfo) = 0;

        // ----------------- End Authentication Lib virtual method -----------------------------
    };

    class SecuritySubsystemAccessor : public ISecuritySubsystemAccessor
    {
    public:

        static ISecuritySubsystemAccessor* getInstance();

        static void  cleanup();

    public:

        virtual bool canIgnoreRightsCheck()
        {
            return m_ignoreRightsCheck;
        }

        // ----------------- Begin Rights Lib virtual method -----------------------------

        virtual bool isActionPermittedOnResource(
            const std::string& sessionId, unsigned long resourceKey,
            unsigned long actionKey, std::string& reasonString,
            EDecisionModule& decisionModule );

        virtual bool isActionPermittedOnResourceAndLocSub(
            const std::string& sessionId, unsigned long resourceKey, unsigned long locationKey, unsigned long subsystemKey,
            unsigned long actionKey, std::string& reasonString,
            EDecisionModule& decisionModule );

        virtual void isActionPermittedOnResources(
            const std::string& sessionId, ResourceList& resourceKeys,ResourceList& returnResourceList,
            unsigned long actionKey, std::string& reasonString,
            EDecisionModule& decisionModule );

        virtual void areActionsPermittedOnSubsystemAndLocSub(
            const std::string& sessionId,unsigned long resourceKey,  unsigned long locationKey, unsigned long subsystemKey,
            ActionList& actionKeys,ActionList& returnActionList, std::string& reasonString,
            EDecisionModule& decisionModule );

        virtual bool isActionPermittedOnResourceAndLocSubWithoutDuty(
            const std::string& sessionId, unsigned long resourceKey, unsigned long locationKey, unsigned long subsystemKey,
            unsigned long actionKey, std::string& reasonString,
            EDecisionModule& decisionModule );

		virtual void resetCache();		

        // ----------------- End Rights Lib virtual method -----------------------------

        // ----------------- Begin Authentication Lib virtual method -----------------------------

        /**
         * requestSession
         *
	     * This method checks the supplied credentials and creates sessions if they are sufficient.
         *
	     * @param    userId         The key to the user id in the operation table
	     * @param    profileId      The key to the profile the user is logging in under
	     * @param    locationId     The key to the location the user wants to use the profile for
	     * @param    workstationId  The key to the workstation from which the user is attempting to gain access
	     * @param    password       The password with which the user is attempting to log in.
         *
         * @return The ID of the new session as a string
         *
         * @exception AuthenticationSecurityException Thrown if the log in attpemt fails for security reasons.
         * @exception AuthenticationAgentException Thrown if there is a general error with the
         * log in attempt.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
	     */
        virtual std::string requestSession(unsigned long userId, 
            unsigned long profileId,
            unsigned long locationId,
            unsigned long workstationId,
            const std::string& password,
            bool display = false);

        /**
         * changePassword
         *
         * This method updates the password for the user of the specified session, and notifies all
         * other Authentication Agents of the change.
         *
         * @param sessionId     The session ID from which the password change request is being made.
         * @param newPassword   The new password to set for the user related to the specified session.
         *
         * @return true if the password change was successful, false otherwise.
         *
         * @exception AuthenticationSecurityException Thrown if the session is not valid.
         * @exception AuthenticationAgentException Thrown if there is a problem changing the password.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
         *
         * pre: The password CANNOT be the empty string.
         */
        virtual void changePassword(const std::string& sessionId, const std::string& oldPassword, const std::string& newPassword);

        /**
         * beginOverride
         *
         * Associates the profile the overriding user is logging into with the session of the base user.
         *
         * @param sessionId      The session that is being overriden
         * @param userId         The key to the user id in the operation table
	     * @param profileId      The key to the profile the user is logging in under
	     * @param locationId     The key to the location the user wants to use the profile for
	     * @param password       The password with which the user is attempting to log in.
         *
         * Note: The workstation ID is not a parameter, as it is taken from the passed in session ID.
         *
         * @return true if the override was successful
         *
         * @exception AuthenitcationSecurityException Thrown if the override is denied for seucrity reasons.
         * @exception AuthenticationAgentException Thrown if there is a probelm initaiting override.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
         */
	    virtual void beginOverride(const std::string& sessionId, 
            unsigned long userId,
            unsigned long profileId,
            unsigned long locationId,
            const std::string& password);

        /**
         * endOverride
         *
         * This method removes any additional profiles associated with the specified session.
         *
         * @param sessionId     The session for which to remove overrides.
         *
         * @exception AuthenticationAgentException Thrown if there is a problem removing the overrides.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
         */
	    virtual void endOverride( const std::string& sessionId);

        /**
         * changeProfile
         *
         * This method moves the operator logged into the specified session to a new profile, with a new session ID. This method
         * creates the new session before ending the old one, to allow the operator to continue using their current session of the
         * log in is denied.
         *
         * @param sessionId     The session the operator is currently using.
         * @param profileId     The ID of the new profile the operator wishes to change to.
         * @param locationId    The ID of the location for the new profile
         * @param password      The password for the new user.
         *
         * @return The ID of the new session as a string
         *
         * @exception AuthenitcationSecurityException Thrown if the override is denied for seucrity reasons.
         * @exception AuthentiactionAgentException Thrown if there is a problem logging the operator into the new session.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
         */
        virtual std::string changeProfile(const std::string& sessionId, 
            unsigned long profileId,
            unsigned long locationId,
            const std::string& password,
            bool display = false);

	    /**
         * getProfiles
         *
	     * Checks the supplied session Id to determine if it is a current session (that is, not yet expired), and if it is, 
         * will return the profiles associated with the session.
         *
	     * @param    sessionId    The session Id to check for validity.
         *
         * @return A vector of profileId's representing the profiles associated with the session. If the vector is empty
         * (i.e. vector.size() == 0), the session is not currently valid.
         *
         * @exception AuthenticationAgentException Thrown if there is a problem retrieving the profile IDs.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
	     */
        virtual void getProfiles(const std::string& sessionId, std::vector<unsigned long>& vtProfile );

        /**
         * getSessionInfo
         *
         * This method retrieves information about the requested session. It should be used by applications wishing
         * to determine session information, rather than going directly to the security tables in the database.
         *
         * @param sessionId             The ID of the session for which to retrieve information
         * @param requestingSessionId   The ID of the session making the request.
         *
         * @return The information about this session in a SessionInfo struct.
         *
         * @exception AuthentiactionAgentException Thrown if there is a problem retrieving the session information.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
         */
        virtual SessionInfo getSessionInfo(const std::string& sessionId);

        /**
         * endSession
         *
         * Ends the specified session, by removing it from the session register. After this method has been called,
         * any calls to getProfiles() will return an empty vector.
         *
         * @param sessionId     The ID of the session to end.
         *
         * @exception AuthenticationSecurityExcetpion Thrown if the session is not valid.
         * @exception AuthenticationAgentExcetpion  Thrown if there is a problem ending the session.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
         */
        virtual void endSession( const std::string& sessionId);

        /**
         * getActiveSessions
         *
         * Retrieves a list of all the currently active sessions in the system.
         *
         * @param sessionId     The ID of the session that is retrieving all others.
         *
         * @return the list of all currently active sessions as a vector of SessionInfo objects
         *
         * @exception AuthenticationAgentExcetpion  Thrown if there is a problem ending the session.
         * @exception ObjectResolutionException Thrown if there is an resolving the authentication agent
         */
        virtual void getActiveSessions(const std::string& sessionId, std::vector<SessionInfo>& vtSessionInfo);

        // ----------------- End Authentication Lib virtual method -----------------------------

    private:

        SecuritySubsystemAccessor(void);
        virtual ~SecuritySubsystemAccessor(void);

        // Disable copy Constructor.
        SecuritySubsystemAccessor(const SecuritySubsystemAccessor& rhs);

        // Disable assignment operator
        const SecuritySubsystemAccessor& operator=(const SecuritySubsystemAccessor& rhs);
        AuthenticationLibrary * getAuthenLib();

    private:

        AuthenticationLibrary * m_pAuthencationLib;
        RightsLibrary * m_pRightsLib;
        bool m_ignoreRightsCheck;

        // Static variables used to implement the singleton pattern.
        static ISecuritySubsystemAccessor * m_instance;
        static TA_Base_Core::NonReEntrantThreadLockable m_singletonLock;
    };
}

#endif
