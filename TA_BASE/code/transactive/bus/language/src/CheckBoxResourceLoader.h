#pragma once
#include "bus/language/src/AbstractDialogItemResourceLoader.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    class CheckBoxResourceLoader : public AbstractDialogItemResourceLoader
    {
        enum { APPLICATION_ID, RESOURCE_TYPE_ID, CHECKBOX_ID, PARENT_ID, CHECKBOX_NAME_ID, CHECKBOX_CAPTION, OPTIONAL_PROPERTY_LIST };

    public:

        CheckBoxResourceLoader( const std::string& rawResource );

        virtual IResourcePtr createResource();

        static IResourceLoaderPtr create( const std::string& rawResource ) { return IResourceLoaderPtr( new CheckBoxResourceLoader(rawResource) ); }
        static UINT getResourceTypeID() { return RESOURCE_TYPE_ID_CheckBox; }

    private:

        virtual UINT getParentIDIndex() { return PARENT_ID; };
        virtual UINT getOptionalPropertyListIndex() { return OPTIONAL_PROPERTY_LIST; }
    };

}
