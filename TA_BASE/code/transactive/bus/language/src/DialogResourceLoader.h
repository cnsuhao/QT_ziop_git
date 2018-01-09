#pragma once
#include "bus/language/src/AbstractResourceLoader.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    class DialogResourceLoader : public AbstractResourceLoader
    {
        enum { APPLICATION_ID, RESOURCE_TYPE_ID, DIALOG_ID, PARENT_NAME_ID, DIALOG_CAPTION, OPTIONAL_PROPERTY_LIST };

    public:

        DialogResourceLoader( const std::string& rawResource );

        virtual IResourcePtr createResource();
        virtual void addResourceToContainer( IResourcePtr resource, IResourceContainerPtr containder );

        static IResourceLoaderPtr create( const std::string& rawResource ) { return IResourceLoaderPtr( new DialogResourceLoader(rawResource) ); }
        static UINT getResourceTypeID() { return RESOURCE_TYPE_ID_Dialog; }

        virtual UINT getOptionalPropertyListIndex() { return OPTIONAL_PROPERTY_LIST; }
    };

}
