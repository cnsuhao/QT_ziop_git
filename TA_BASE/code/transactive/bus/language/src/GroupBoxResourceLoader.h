#pragma once
#include "bus/language/src/AbstractDialogItemResourceLoader.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    class GroupBoxResourceLoader : public AbstractDialogItemResourceLoader
    {
        enum { APPLICATION_ID, RESOURCE_TYPE_ID, GROUPBOX_ID, PARENT_ID, GROUPBOX_NAME_ID, GROUPBOX_CAPTION, OPTIONAL_PROPERTY_LIST };

    public:

        GroupBoxResourceLoader( const std::string& rawResource );

        virtual IResourcePtr createResource();

        static IResourceLoaderPtr create( const std::string& rawResource ) { return IResourceLoaderPtr( new GroupBoxResourceLoader(rawResource) ); }
        static UINT getResourceTypeID() { return RESOURCE_TYPE_ID_GroupBox; }

    private:

        virtual UINT getParentIDIndex() { return PARENT_ID; };
        virtual UINT getOptionalPropertyListIndex() { return OPTIONAL_PROPERTY_LIST; }
    };

}
