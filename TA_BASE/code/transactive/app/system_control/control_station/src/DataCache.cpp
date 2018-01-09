#include "StdAfx.h"
#include "DataCache.h"
#include "Utilities.h"
#include "ControlStationConstants.h"
#include "DatabaseStatusMonitor.h"
#include "ReturnSharedFunctionAdapter.h"
#include "core/utilities/src/Hostname.h"
#include "core/utilities/src/RunParamsAny.h"
#include "core/utilities/src/DebugUtil.h"
#include "core/utilities/src/RunParams.h"
#include "core/threads/src/ThreadPoolSingletonManager.h"
#include "core/data_access_interface/entity_access/src/ConsoleAccessFactory.h"
#include "core/data_access_interface/entity_access/src/IConsole.h"
#include "core/data_access_interface/entity_access/src/EntityAccessFactory.h"
#include "core/data_access_interface/entity_access/src/IEntityData.h"
#include "core/data_access_interface/system_controller/src/ISystemControllerData.h"
#include "core/data_access_interface/system_controller/src/SystemControllerAccessFactory.h"
#include "core/data_access_interface/src/OperatorAccessFactory.h"
#include "core/data_access_interface/src/ProfileAccessFactory.h"
#include "core/data_access_interface/src/LocationAccessFactory.h"
#include "core/data_access_interface/src/SecuritySettingAccessFactory.h"
#include "core/data_access_interface/src/ResourceAccessFactory.h"
#include "core/data_access_interface/src/IResource.h"
#include "core/data_access_interface/entity_access/src/ControlStation.h"
#include "core/data_access_interface/entity_access/src/Banner.h"

using namespace TA_Base_Core;
using namespace TA_Base_App;

typedef std::vector<ILocation*> ILocationVector;
typedef boost::shared_ptr<ILocationVector> ILocationVectorPtr;

typedef std::vector<IOperator*> IOperatorVector;
typedef boost::shared_ptr<IOperatorVector> IOperatorVectorPtr;

typedef std::vector<IProfile*> IProfileVector;
typedef boost::shared_ptr<IProfileVector> IProfileVectorPtr;

// if function F return T/T*/auto_ptr<T>/shared_ptr<T>, then make it return boost::shared_ptr<T>
namespace
{
    template <typename T>
    boost::shared_ptr<T> fromAny(boost::any& a)
    {
        typedef boost::shared_ptr<T> Ptr;
        typedef boost::shared_future<Ptr> Future;
        Future f = boost::any_cast<Future>(a);

        try
        {
            return f.get();
        }
        catch (...)
        {
            TA_ASSERT(false, "Database Error");
        }

        return f.get();
    }
}

DataCache::DataCache()
{
}

void DataCache::asyncInitialize()
{
    DatabaseStatusMonitor::getInstance().waitForAnyAvaliable(10000);
    m_map["AllOperators"] = sharedAsync(boost::bind(&OperatorAccessFactory::getAllNonSystemOperators, &OperatorAccessFactory::getInstanceNoInit(), false, true, false));
    sharedAsync(boost::bind(&EntityAccessFactory::getParameterDefaultValue, &EntityAccessFactory::getInstance(), "", 0));

    m_hostname = RunParams::getInstance().promiseGet("Localhost", 10000);
    m_entityName = RunParams::getInstance().get(RPARAM_ENTITYNAME);
    m_map["Console"] = sharedAsync(boost::bind(&ConsoleAccessFactory::getConsoleByName, &ConsoleAccessFactory::getInstance(), m_hostname));
    m_map["ControlStationEntity"] = sharedAsync(&DataCache::createControlStationEntity);

    const std::vector<std::string>& hostnames = RunParamsAny::getInstance().promiseGet<std::vector<std::string>>(TA_ControlStation::HOST_NAMES, 10000);
    TA_Assert(hostnames.size());

    for (const std::string& hostname : hostnames)
    {
        m_map[hostname] = sharedAsync(boost::bind(&EntityAccessFactory::getEntityByName, &EntityAccessFactory::getInstance(), hostname));
    }

    m_map[m_entityName] = sharedAsync(boost::bind(&EntityAccessFactory::getEntityByName, &EntityAccessFactory::getInstance(), m_entityName));
    m_map["ConsoleLocation"] = sharedAsync(&DataCache::createConsoleLocation);
    m_map["AllProfiles"] = sharedAsync(boost::bind(&ProfileAccessFactory::getAllNonSystemProfiles, &ProfileAccessFactory::getInstance(), false));
    m_map["AllLocations"] = sharedAsync(boost::bind(&LocationAccessFactory::getAllLocations, &LocationAccessFactory::getInstance(), false));
    m_map["SecuritySetting"] = sharedAsync(boost::bind(&SecuritySettingAccessFactory::getSetting, &SecuritySettingAccessFactory::getInstance()));
    m_map["Banners"] = sharedAsync(&DataCache::createBanners);
    m_map["DllHost"] = sharedAsync(boost::bind(&EntityAccessFactory::getEntityByName, &EntityAccessFactory::getInstance(), TA_ControlStation::DLLHOST_ENTITY_NAME));
    m_map["ControlStationResource"] = sharedAsync(&DataCache::createControlStationResource);
}

ControlStationPtr DataCache::getControlStationEntity()
{
    return fromAny<ControlStation>(m_map["ControlStationEntity"]);
}

IEntityDataPtr DataCache::getEntity(const std::string& entityName)
{
    return fromAny<IEntityData>(m_map[entityName]);
}

IConsolePtr DataCache::getConsole()
{
    return fromAny<IConsole>(m_map["Console"]);
}

ILocationPtr DataCache::getConsoleLocation()
{
    return fromAny<ILocation>(m_map["ConsoleLocation"]);
}

std::vector<ILocation*>& DataCache::getAllLocations()
{
    return *fromAny<std::vector<ILocation*> >(m_map["AllLocations"]);
}

std::vector<IOperator*>& DataCache::getAllOperators()
{
    return *fromAny<std::vector<IOperator*> >(m_map["AllOperators"]);
}

std::vector<IProfile*>& DataCache::getAllProfiles()
{
    return *fromAny<std::vector<IProfile*> >(m_map["AllProfiles"]);
}

SecuritySettingPtr DataCache::getSecuritySetting()
{
    return fromAny<SecuritySetting>(m_map["SecuritySetting"]);
}

std::vector<IEntityData*>& DataCache::getBanners()
{
    return *fromAny<std::vector<IEntityData*> >(m_map["Banners"]);
}

IEntityDataPtr DataCache::getDllHost()
{
    return fromAny<IEntityData>(m_map["DllHost"]);
}

IResourcePtr DataCache::getControlStationResource()
{
    return fromAny<IResource>(m_map["ControlStationResource"]);
}

ILocation* DataCache::createConsoleLocation()
{
    IConsolePtr console = DataCache::getInstance().getConsole();

    if (console)
    {
        return LocationAccessFactory::getInstance().getLocationByKey(console->getLocation());
    }

    return NULL;
}

ControlStation* DataCache::createControlStationEntity()
{
    std::string entityName = RunParams::getInstance().get(RPARAM_ENTITYNAME);
    IEntityData* entityData = EntityAccessFactory::getInstance().getEntity(entityName);
    ControlStation* controlStationEntity = dynamic_cast<ControlStation*>(entityData);

    if (controlStationEntity)
    {
        controlStationEntity->getHelper()->getAllParameters();
        EntityAccessFactory::getInstance().getParameterDefaultValue("", 0);
    }

    return controlStationEntity;
}

IResource* DataCache::createControlStationResource()
{
    std::string entityName = RunParams::getInstance().get(RPARAM_ENTITYNAME);
    IEntityDataPtr entity = DataCache::getInstance().getEntity(entityName);

    if (entity)
    {
        return ResourceAccessFactory::getInstance().getResourceFromEntity(entity->getKey());
    }

    return NULL;
}

std::vector<IEntityData*> DataCache::createBanners()
{
    std::vector<IEntityData*> banners = EntityAccessFactory::getInstance().getEntitiesOfType(TA_ControlStation::BANNER_ENTITY_TYPE);

    for (IEntityData* banner : banners)
    {
        dynamic_cast<Banner*>(banner)->getHelper()->getAllParameters();
    }

    return banners;
}
