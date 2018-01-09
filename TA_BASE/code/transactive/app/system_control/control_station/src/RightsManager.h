#pragma once
#include "ControlStationConstants.h"
#include "Promise.h"
#include "Singleton.h"
#include "core/configuration_updates/src/IOnlineUpdatable.h"
#include "bus/security/rights_library/src/RightsLibrary.h"

using namespace TA_Base_App;

typedef Promise<TA_ControlStation::EPermission> PromisePermission;
typedef boost::shared_ptr<PromisePermission> PromisePermissionPtr;

namespace TA_Base_Core
{
    class IEntityData;
    typedef boost::shared_ptr<IEntityData> IEntityDataPtr;
}

namespace TA_Base_Bus
{
    class RightsLibrary;
    typedef boost::shared_ptr<RightsLibrary> RightsLibraryPtr;
}

class RightsManager : public TA_Base_Core::IOnlineUpdatable, public Singleton<RightsManager>
{
public:

    TA_ControlStation::EPermission isActionPermitted(unsigned long action, unsigned long resourceKey);
    TA_ControlStation::EPermission isControlPermitted(unsigned long action);
    virtual bool isActionPermittedOnResource(const std::string& sessionId, unsigned long resourceKey,
                                             unsigned long actionKey, std::string& reasonString,
                                             TA_Base_Bus::EDecisionModule& decisionModule);

    virtual void isActionPermittedOnResources(const std::string& sessionId,
                                              TA_Base_Bus::ResourceList& resourceKeys,
                                              TA_Base_Bus::ResourceList& returnResourceList,
                                              unsigned long actionKey,
                                              std::string& reasonString,
                                              TA_Base_Bus::EDecisionModule& decisionModule);

    void areActionsPermitted(unsigned long action, TA_Base_Bus::ResourceList& resourceKeyList, TA_Base_Bus::ResourceList& permittedResourceKeyList);

protected:

    RightsManager();
    virtual void asyncInitialize() override;
    void setupSignalConnections();
    void resetCache();
    virtual void processUpdate(const TA_Base_Core::ConfigUpdateDetails& updateEvent);
    std::pair<bool, bool> findCache(const std::string& sessionId, unsigned long action, unsigned long resourceKey);
    std::pair<bool, TA_Base_Bus::ResourceList> findCache(const std::string& sessionId, unsigned long action, TA_Base_Bus::ResourceList resourceKeys);
    void addCache(const std::string& sessionId, unsigned long action, unsigned long resourceKey, bool isPermitted);
    void addCache(const std::string& sessionId, unsigned long action, const TA_Base_Bus::ResourceList& resourceKeys, const TA_Base_Bus::ResourceList& resultResourceKeys);

private:

    unsigned int m_retries;
    unsigned long m_resourceKey;
    TA_Base_Bus::RightsLibraryPtr m_rightsLibrary;

    typedef std::pair<unsigned long, unsigned long> ActionResource;
    typedef std::map<ActionResource, bool> ActionResource2PermittedMap;
    typedef std::map<std::string, ActionResource2PermittedMap> Session2PermittedMap;
    Session2PermittedMap m_cache;

    typedef std::pair<unsigned long, TA_Base_Bus::ResourceList> ActionResources;
    typedef std::map<ActionResources, TA_Base_Bus::ResourceList> ActionResources2PermittedMap;
    typedef std::map<std::string, ActionResources2PermittedMap> Session2PermittedMap2;
    Session2PermittedMap2 m_cache2;
};
