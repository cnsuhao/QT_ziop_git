#pragma once
#include "bus/language/src/IResourceLoader.h"
#include "bus/language/src/RawResourceSplitter.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    class StringResourceLoader : public IResourceLoader
    {
        enum { APPLICATION_ID, RESOURCE_TYPE_ID, STRING_ID, STRING_NAME_ID, STRING_CAPTION };

    public:

        StringResourceLoader( const std::string& rawResource );

        virtual IResourcePtr createResource();
        virtual void addResourceToContainer( IResourcePtr resource, IResourceContainerPtr containder );

        static IResourceLoaderPtr create( const std::string& rawResource ) { return IResourceLoaderPtr( new StringResourceLoader(rawResource) ); }
        static UINT getResourceTypeID() { return RESOURCE_TYPE_ID_String; }

    private:

        RawResourceSplitter m_splitter;
    };

}
