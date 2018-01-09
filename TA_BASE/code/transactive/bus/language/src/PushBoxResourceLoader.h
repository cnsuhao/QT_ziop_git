#pragma once
#include "bus/language/src/AbstractDialogItemResourceLoader.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    class PushBoxResourceLoader : public AbstractDialogItemResourceLoader
    {
        enum { APPLICATION_ID, RESOURCE_TYPE_ID, PUSHBOX_ID, PARENT_ID, PUSHBOX_NAME_ID, PUSHBOX_CAPTION, OPTIONAL_PROPERTY_LIST };

    public:

        PushBoxResourceLoader( const std::string& rawResource );

        virtual IResourcePtr createResource();

        static IResourceLoaderPtr create( const std::string& rawResource ) { return IResourceLoaderPtr( new PushBoxResourceLoader(rawResource) ); }
        static UINT getResourceTypeID() { return RESOURCE_TYPE_ID_PushBox; }

    private:

        virtual UINT getParentIDIndex() { return PARENT_ID; };
        virtual UINT getOptionalPropertyListIndex() { return OPTIONAL_PROPERTY_LIST; }
    };

}
