#pragma once
#include "bus/language/src/IResourceContainer.h"


namespace TA_Base_Bus
{

    class ResourceContainer : public IResourceContainer
    {
    public:

        virtual void addResource( const ResourceKey& id, IResourcePtr resource );
        virtual IResourcePtr getResource( const ResourceKey& id );
        virtual bool isExist(const ResourceKey& id);
        virtual bool isExist( IResourcePtr resource );

    private:

        typedef std::map<ResourceKey, IResourcePtr> ResourceMap;
        ResourceMap m_resourceMap;
    };

    typedef boost::shared_ptr<ResourceContainer> ResourceContainerPtr;


    struct ResourceKeyDuplicateException
    {
        ResourceKeyDuplicateException(const ResourceKey& k) : m_key( k ) {}
        ResourceKey m_key;
    };

}
