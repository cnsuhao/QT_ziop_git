#include "StdAfx.h"
#include "bus/language/src/AbstractResourceLoader.h"
#include "bus/language/src/OptionalPropertyLoaderFactory.h"


namespace TA_Base_Bus
{

    AbstractResourceLoader::AbstractResourceLoader( const std::string& rawResource )
        : m_splitter( rawResource )
    {
    }


    void AbstractResourceLoader::addOptionalPropertyToResource( IResourcePtr resource )
    {
        for ( size_t i = getOptionalPropertyListIndex(); i < m_splitter.size(); ++i )
        {
            const std::string& rawOptionalProperty = m_splitter.getString(i);
            IOptionalPropertyPtr optionalProperty = OptionalPropertyLoaderFactory::instance().create( rawOptionalProperty );
            resource->addOptionalProperty( optionalProperty );
        }
    }

}
