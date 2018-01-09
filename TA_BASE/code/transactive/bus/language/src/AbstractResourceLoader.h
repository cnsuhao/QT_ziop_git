#pragma once
#include "bus/language/src/IResourceLoader.h"
#include "bus/language/src/RawResourceSplitter.h"


namespace TA_Base_Bus
{

    class AbstractResourceLoader : public IResourceLoader
    {
    protected:

        AbstractResourceLoader( const std::string& rawResource );

        virtual UINT getOptionalPropertyListIndex() = 0;
        virtual void addOptionalPropertyToResource( IResourcePtr resource );

    protected:

        RawResourceSplitter m_splitter;
    };

    typedef boost::shared_ptr<AbstractResourceLoader> AbstractResourceLoaderPtr;

}
