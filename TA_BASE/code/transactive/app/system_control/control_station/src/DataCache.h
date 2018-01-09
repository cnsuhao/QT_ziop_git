#ifndef DATA_CACHE_HEADER_INCLUDED
#define DATA_CACHE_HEADER_INCLUDED

#include "Singleton.h"

namespace TA_Base_Core
{
    class IEntityData;
    class IOperator;
    class IProfile;
    class ILocation;
    class IConsole;
    class IResource;
    class ControlStation;
    struct SecuritySetting;
    typedef boost::shared_ptr<IEntityData> IEntityDataPtr;
    typedef boost::shared_ptr<IOperator> IOperatorPtr;
    typedef boost::shared_ptr<IProfile> IProfilePtr;
    typedef boost::shared_ptr<ILocation> ILocationPtr;
    typedef boost::shared_ptr<IConsole> IConsolePtr;
    typedef boost::shared_ptr<IResource> IResourcePtr;
    typedef boost::shared_ptr<SecuritySetting> SecuritySettingPtr;
    typedef boost::shared_ptr<ControlStation> ControlStationPtr;
}

class DataCache : public Singleton<DataCache>
{
public:

    TA_Base_Core::IConsolePtr getConsole();
    TA_Base_Core::ILocationPtr getConsoleLocation();
    TA_Base_Core::IEntityDataPtr getEntity(const std::string& entityName);
    TA_Base_Core::ControlStationPtr getControlStationEntity();
    std::vector<TA_Base_Core::IOperator*>& getAllOperators();
    std::vector<TA_Base_Core::IProfile*>& getAllProfiles();
    std::vector<TA_Base_Core::ILocation*>& getAllLocations();
    TA_Base_Core::SecuritySettingPtr getSecuritySetting();
    std::vector<TA_Base_Core::IEntityData*>& getBanners();
    TA_Base_Core::IEntityDataPtr getDllHost();
    TA_Base_Core::IResourcePtr getControlStationResource();

    static TA_Base_Core::ControlStation* createControlStationEntity();
    static TA_Base_Core::ILocation* createConsoleLocation();
    static TA_Base_Core::IResource* createControlStationResource();
    static std::vector<TA_Base_Core::IEntityData*> createBanners();

protected:

    DataCache();
    virtual void asyncInitialize() override;

public:

    std::string m_hostname;
    std::string m_entityName;
    typedef std::map<std::string, boost::any> FutureMap;
    FutureMap m_map;
};

#endif
