#pragma once
#include "bus/language/src/IResourceContainer.h"


namespace TA_Base_Bus
{

    class IResourceLoader
    {
    public:

        virtual ~IResourceLoader() {}
        virtual IResourcePtr createResource() = 0;
        virtual void addResourceToContainer( IResourcePtr resource, IResourceContainerPtr resourceContainer ) = 0;
    };

    typedef boost::shared_ptr<IResourceLoader> IResourceLoaderPtr;

}
