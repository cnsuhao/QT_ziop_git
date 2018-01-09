#pragma once
#include "bus/language/src/AbstractDialogItemResourceLoader.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    class TextResourceLoader : public AbstractDialogItemResourceLoader
    {
        enum { APPLICATION_ID, RESOURCE_TYPE_ID, TEXT_ID, PARENT_ID, TEXT_NAME_ID, TEXT_CAPTION, OPTIONAL_PROPERTY_LIST };

    public:

        TextResourceLoader( const std::string& rawResource );

        virtual IResourcePtr createResource();

        static IResourceLoaderPtr create( const std::string& rawResource ) { return IResourceLoaderPtr( new TextResourceLoader(rawResource) ); }
        static UINT getResourceTypeID() { return RESOURCE_TYPE_ID_Text; }

    private:

        virtual UINT getParentIDIndex() { return PARENT_ID; };
        virtual UINT getOptionalPropertyListIndex() { return OPTIONAL_PROPERTY_LIST; }
    };

    typedef boost::shared_ptr<TextResourceLoader> TextResourceLoaderPtr;

}
