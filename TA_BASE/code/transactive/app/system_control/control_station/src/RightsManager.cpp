#include "StdAfx.h"
#include "RightsManager.h"
#include "DataCache.h"
#include "Signals.h"
#include "SessionDetails.h"
#include "DataCache.h"
#include "bus/security/access_control/actions/src/AccessControlledActions.h"
#include "core/threads/src/Thread.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/message/types/SystemControllerComms_MessageTypes.h"
#include "core/data_access_interface/entity_access/src/ControlStation.h"
#include "core/data_access_interface/src/IResource.h"
#include "core/configuration_updates/src/OnlineUpdateListener.h"

using namespace TA_Base_Core;
using namespace TA_Base_Bus;

RightsManager::RightsManager()
{
    setupSignalConnections();
}

void RightsManager::setupSignalConnections()
{
    Signals::operatorOverride.connect(boost::bind(&RightsManager::resetCache, this));
    Signals::removeOperatorOverride.connect(boost::bind(&RightsManager::resetCache, this));
}

void RightsManager::asyncInitialize()
{
    m_rightsLibrary.reset(RightsLibraryFactory::getInstance().buildRightsLibrary(TA_Base_Bus::DEFAULT_RL, TA_Base_Bus::RightsLibraryFactory::DEFAULT));
    m_retries = DataCache::getInstance().getControlStationEntity()->getCorbaRetries();
    m_resourceKey = DataCache::getInstance().getControlStationResource()->getKey();
    OnlineUpdateListener::getInstance().registerAllInterests(TA_Base_Core::SECURITY, *this);
    OnlineUpdateListener::getInstance().registerAllInterests(TA_Base_Core::ACTION_GROUP, *this);
}

bool RightsManager::isActionPermittedOnResource(const std::string& sessionId, unsigned long resourceKey,
                                                unsigned long actionKey, std::string& reasonString,
                                                EDecisionModule& decisionModule)
{
    std::pair<bool, bool> findResult = findCache(sessionId, actionKey, resourceKey);

    if (findResult.first)
    {
        return findResult.second;
    }

    bool result = m_rightsLibrary->isActionPermittedOnResource(sessionId, resourceKey, actionKey, reasonString, decisionModule);
    addCache(sessionId, actionKey, resourceKey, result);
    return result;
}

void RightsManager::isActionPermittedOnResources(const std::string& sessionId,
                                                 ResourceList& resourceKeys,
                                                 ResourceList& returnResourceList,
                                                 unsigned long actionKey,
                                                 std::string& reasonString,
                                                 EDecisionModule& decisionModule)
{
    std::pair<bool, ResourceList> findResult = findCache(sessionId, actionKey, resourceKeys);

    if (findResult.first)
    {
        returnResourceList = findResult.second;
        return;
    }

    m_rightsLibrary->isActionPermittedOnResources(sessionId, resourceKeys, returnResourceList, actionKey, reasonString, decisionModule);
    addCache(sessionId, actionKey, resourceKeys, returnResourceList);
}

TA_ControlStation::EPermission RightsManager::isActionPermitted(unsigned long action, unsigned long resourceKey)
{
    TA_ControlStation::EPermission permission;
    std::string reason;
    TA_Base_Bus::EDecisionModule decisionModule = TA_Base_Bus::UNDEFINED_DM;
    std::string sessionId = SessionDetails::getInstance().getSessionId();

    std::pair<bool, bool> findResult = findCache(sessionId, action, resourceKey);

    if (findResult.first)
    {
        return (findResult.second ? TA_ControlStation::PERMITTED : TA_ControlStation::DENIED);
    }

    try
    {
        bool allowed = m_rightsLibrary->isActionPermittedOnResource(sessionId, resourceKey, action, reason, decisionModule);

        if (allowed)
        {
            permission = TA_Base_App::TA_ControlStation::PERMITTED;
        }
        else
        {
            permission = TA_Base_App::TA_ControlStation::DENIED;
            LOG_INFO("Not permitted to perform action=%d on control station as %s", action, reason.c_str());
        }
    }
    catch (const RightsException&)
    {
        // The library throws this exception when the right is either
        // indeterminate or not applicable
        permission = TA_Base_App::TA_ControlStation::UNKNOWN;
    }
    catch (...)
    {
        // The library does not catch any exception, so any exception
        // other than the RightsException is error
        permission = TA_Base_App::TA_ControlStation::GENERAL_ERROR;
    }

    addCache(sessionId, action, resourceKey, permission);
    return permission;
}

TA_ControlStation::EPermission RightsManager::isControlPermitted(unsigned long action)
{
    TA_ASSERT(m_resourceKey != -1, "Resource key is not set");
    return isActionPermitted(action, m_resourceKey);
}

void RightsManager::areActionsPermitted(unsigned long action, ResourceList& resourceKeyList, ResourceList& permittedResourceKeyList)
{
    permittedResourceKeyList.clear();
    std::string reason;
    std::string sessionId = SessionDetails::getInstance().getSessionId();
    EDecisionModule decisionModule;
    isActionPermittedOnResources(sessionId, resourceKeyList, permittedResourceKeyList, action, reason, decisionModule);
}

void RightsManager::resetCache()
{
    m_rightsLibrary->resetCache();
    m_cache.clear();
    m_cache2.clear();
}

void RightsManager::processUpdate(const TA_Base_Core::ConfigUpdateDetails& updateEvent)
{
    if (TA_Base_Core::ACTION_GROUP == updateEvent.getType())
    {
        resetCache();
    }
}

std::pair<bool, bool> RightsManager::findCache(const std::string& sessionId, unsigned long action, unsigned long resourceKey)
{
    Session2PermittedMap::iterator it = m_cache.find(sessionId);

    if (it != m_cache.end())
    {
        ActionResource2PermittedMap& permittedMap = it->second;
        ActionResource2PermittedMap::iterator it2 = permittedMap.find(std::make_pair(action, resourceKey));

        if (it2 != permittedMap.end())
        {
            return std::make_pair(true, it2->second);
        }
    }

    return std::make_pair(false, false);
}

void RightsManager::addCache(const std::string& sessionId, unsigned long action, unsigned long resourceKey, bool isPermitted)
{
    m_cache[sessionId][std::make_pair(action, resourceKey)] = isPermitted;
}

std::pair<bool, ResourceList> RightsManager::findCache(const std::string& sessionId, unsigned long action, ResourceList resourceKeys)
{
    Session2PermittedMap2::iterator it = m_cache2.find(sessionId);

    if (it != m_cache2.end())
    {
        ActionResources2PermittedMap& permittedMap = it->second;
        ActionResources2PermittedMap::iterator it2 = permittedMap.find(std::make_pair(action, resourceKeys));

        if (it2 != permittedMap.end())
        {
            return std::make_pair(true, it2->second);
        }
    }

    return std::make_pair(false, ResourceList());
}

void RightsManager::addCache(const std::string& sessionId, unsigned long action, const ResourceList& resourceKeys, const ResourceList& resultResourceKeys)
{
    m_cache2[sessionId][std::make_pair(action, resourceKeys)] = resultResourceKeys;
}
