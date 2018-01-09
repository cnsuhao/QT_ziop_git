#include "SecuritySubsystemFacade.h"
#include "bus/security/authentication_library/src/AuthenticationLibrary.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"


namespace TA_Base_Bus
{
    // Initialise statics
    //
    TA_Base_Core::NonReEntrantThreadLockable SecuritySubsystemAccessor::m_singletonLock;
    ISecuritySubsystemAccessor* SecuritySubsystemAccessor::m_instance = NULL;


    SecuritySubsystemAccessor::SecuritySubsystemAccessor(void)
        : m_pRightsLib(NULL),
          m_pAuthencationLib(NULL),
          m_ignoreRightsCheck(false)
    {
        m_ignoreRightsCheck = TA_Base_Core::RunParams::getInstance().isSet(RPARAM_RIGHTCHECKING);
        RightsLibraryFactory libraryFactory;
        m_pRightsLib = libraryFactory.buildRightsLibrary();
    }
    
    SecuritySubsystemAccessor::~SecuritySubsystemAccessor(void)
    {
        delete m_pRightsLib;
        m_pRightsLib = NULL;

        delete m_pAuthencationLib;
        m_pAuthencationLib = NULL;
    }

    ISecuritySubsystemAccessor* SecuritySubsystemAccessor::getInstance()
    {
        TA_Base_Core::ThreadGuard guard(m_singletonLock);

        if (m_instance == NULL)
        {
            m_instance = new SecuritySubsystemAccessor();
        }

        return m_instance;
    }

    AuthenticationLibrary* SecuritySubsystemAccessor::getAuthenLib()
    {
        TA_Base_Core::ThreadGuard guard(m_singletonLock);

        if (m_pAuthencationLib == NULL)
        {
            m_pAuthencationLib = new AuthenticationLibrary();
        }

        return m_pAuthencationLib;
    }

    void SecuritySubsystemAccessor::cleanup()
    {
        TA_Base_Core::ThreadGuard guard(m_singletonLock);

        if (m_instance != NULL)
        {
            delete m_instance;
            m_instance = NULL;
        }
    }

    bool SecuritySubsystemAccessor::isActionPermittedOnResource( const std::string& sessionId, unsigned long resourceKey, unsigned long actionKey, std::string& reasonString, EDecisionModule& decisionModule )
    {
        return m_pRightsLib->isActionPermittedOnResource(sessionId, resourceKey, actionKey, reasonString, decisionModule);
    }

    bool SecuritySubsystemAccessor::isActionPermittedOnResourceAndLocSub( const std::string& sessionId, 
                    unsigned long resourceKey, unsigned long locationKey, unsigned long subsystemKey, 
                    unsigned long actionKey, std::string& reasonString, EDecisionModule& decisionModule )
    {
        return m_pRightsLib->isActionPermittedOnResourceAndLocSub(sessionId, resourceKey, locationKey, subsystemKey, actionKey, reasonString, decisionModule);
    }

    void SecuritySubsystemAccessor::isActionPermittedOnResources( const std::string& sessionId, ResourceList& resourceKeys,
                     ResourceList& returnResourceList, unsigned long actionKey, 
                     std::string& reasonString, EDecisionModule& decisionModule )
    {
        return m_pRightsLib->isActionPermittedOnResources(sessionId, resourceKeys, returnResourceList, actionKey, reasonString, decisionModule);
    }

    void SecuritySubsystemAccessor::areActionsPermittedOnSubsystemAndLocSub( const std::string& sessionId,unsigned long resourceKey, 
                    unsigned long locationKey, unsigned long subsystemKey, ActionList& actionKeys,
                    ActionList& returnActionList, std::string& reasonString, EDecisionModule& decisionModule )
    {
        return areActionsPermittedOnSubsystemAndLocSub(sessionId, resourceKey, locationKey, subsystemKey, actionKeys, returnActionList, reasonString, decisionModule);
    }

    bool SecuritySubsystemAccessor::isActionPermittedOnResourceAndLocSubWithoutDuty( const std::string& sessionId, unsigned long resourceKey, 
                    unsigned long locationKey, unsigned long subsystemKey, unsigned long actionKey, 
                    std::string& reasonString, EDecisionModule& decisionModule )
    {
        return m_pRightsLib->isActionPermittedOnResourceAndLocSubWithoutDuty(sessionId, resourceKey,
            locationKey, subsystemKey, actionKey, reasonString, decisionModule);
    }

    std::string SecuritySubsystemAccessor::requestSession( unsigned long userId, unsigned long profileId, unsigned long locationId, unsigned long workstationId, 
            const std::string& password, bool display /*= false*/ )
    {
        return getAuthenLib()->requestSession(userId, profileId, locationId, workstationId, password, display);
    }

    void SecuritySubsystemAccessor::changePassword( const std::string& sessionId, const std::string& oldPassword, const std::string& newPassword )
    {
        return getAuthenLib()->changePassword(sessionId, oldPassword, newPassword);
    }

    void SecuritySubsystemAccessor::beginOverride( const std::string& sessionId, unsigned long userId, unsigned long profileId, 
             unsigned long locationId, const std::string& password )
    {
        return getAuthenLib()->beginOverride(sessionId, userId, profileId, locationId, password);
    }

    void SecuritySubsystemAccessor::endOverride( const std::string& sessionId )
    {
        return getAuthenLib()->endOverride(sessionId);
    }

    std::string SecuritySubsystemAccessor::changeProfile( const std::string& sessionId, unsigned long profileId, unsigned long locationId, const std::string& password, bool display /*= false*/ )
    {
        return getAuthenLib()->changeProfile(sessionId, profileId, locationId, password, display);
    }

    void SecuritySubsystemAccessor::getProfiles( const std::string& sessionId, std::vector<unsigned long>& vtProfile )
    {
        return getAuthenLib()->getProfiles(sessionId, vtProfile);
    }

    TA_Base_Bus::SessionInfo SecuritySubsystemAccessor::getSessionInfo( const std::string& sessionId)
    {
        return getAuthenLib()->getSessionInfo(sessionId, sessionId);
    }

    void SecuritySubsystemAccessor::endSession( const std::string& sessionId )
    {
        return getAuthenLib()->endSession(sessionId);
    }

    void SecuritySubsystemAccessor::getActiveSessions( const std::string& sessionId, std::vector<SessionInfo>& vtSessionInfo )
    {
        return getAuthenLib()->getActiveSessions(sessionId, vtSessionInfo);
    }

	void SecuritySubsystemAccessor::resetCache()
	{
		return m_pRightsLib->resetCache();
	}
}
