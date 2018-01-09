#pragma once
#include "bus/language/src/IResourceLoader.h"
#include "bus/language/src/ResourceContainer.h"


namespace TA_Base_Bus
{

    class ResourceFileLoader
    {
    public:

        static ResourceFileLoader& instance();
        ResourceContainerPtr loadResource( const std::string& resourceFilePath, UINT applicationID );

    private:

        ResourceFileLoader();
        IResourceLoaderPtr createResourceLoader( UINT resourceTypeId, const std::string& line );
        typedef IResourceLoaderPtr (*ResourceLoaderCreator)( const std::string& );
        typedef std::map<UINT, ResourceLoaderCreator> ResourceLoaderCreatorMap;
        ResourceLoaderCreatorMap m_resourceLoaderCreatorMap;
    };

}
