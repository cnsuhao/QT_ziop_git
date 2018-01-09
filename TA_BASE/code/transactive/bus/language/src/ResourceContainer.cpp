#include "StdAfx.h"
#include "bus/language/src/ResourceContainer.h"


namespace TA_Base_Bus
{

    void ResourceContainer::addResource( const ResourceKey& id, IResourcePtr resource )
    {
        if ( m_resourceMap.find( id ) != m_resourceMap.end() )
        {
            throw ResourceKeyDuplicateException(id);
        }

        m_resourceMap.insert( ResourceMap::value_type(id, resource) );
    }


    IResourcePtr ResourceContainer::getResource( const ResourceKey& id )
    {
        ResourceMap::iterator it = m_resourceMap.find( id );

        if ( it != m_resourceMap.end() )
        {
            return it->second;
        }

        return IResourcePtr();
    }


    bool ResourceContainer::isExist(const ResourceKey& id)
    {
        return m_resourceMap.find( id ) != m_resourceMap.end();
    }


    bool ResourceContainer::isExist( IResourcePtr resource )
    {
        for ( std::map<ResourceKey, IResourcePtr>::iterator it = m_resourceMap.begin(); it != m_resourceMap.end(); ++it )
        {
            if ( *it->second == *resource )
            {
                return true;
            }
        }

        return false;
    }

}
