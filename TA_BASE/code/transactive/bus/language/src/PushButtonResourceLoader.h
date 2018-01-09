#pragma once
#include "bus/language/src/AbstractDialogItemResourceLoader.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    class PushButtonResourceLoader : public AbstractDialogItemResourceLoader
    {
        enum { APPLICATION_ID, RESOURCE_TYPE_ID, PUSHBUTTON_ID, PARENT_ID, PUSHBUTTON_NAME_ID, PUSHBUTTON_CAPTION, OPTIONAL_PROPERTY_LIST };

    public:

        PushButtonResourceLoader( const std::string& rawResource );

        virtual IResourcePtr createResource();

        static IResourceLoaderPtr create( const std::string& rawResource ) { return IResourceLoaderPtr( new PushButtonResourceLoader(rawResource) ); }
        static UINT getResourceTypeID() { return RESOURCE_TYPE_ID_PushButton; }

    private:

        virtual UINT getParentIDIndex() { return PARENT_ID; };
        virtual UINT getOptionalPropertyListIndex() { return OPTIONAL_PROPERTY_LIST; }
    };

}
