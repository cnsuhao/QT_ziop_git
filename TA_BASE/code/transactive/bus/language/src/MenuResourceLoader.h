#pragma once
#include "bus/language/src/IResourceLoader.h"
#include "bus/language/src/IResourceContainer.h"
#include "bus/language/src/RawResourceSplitter.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    class MenuResourceLoader : public IResourceLoader
    {
        enum { APPLICATION_ID, RESOURCE_TYPE_ID, MENU_ID, MENU_NAME_ID };

    public:

        MenuResourceLoader( const std::string& rawResource );

        virtual IResourcePtr createResource();

        virtual void addResourceToContainer( IResourcePtr resource, IResourceContainerPtr containder );

        static IResourceLoaderPtr create( const std::string& rawResource ) { return IResourceLoaderPtr( new MenuResourceLoader(rawResource) ); }
        static UINT getResourceTypeID() { return RESOURCE_TYPE_ID_Menu; }

    private:

        RawResourceSplitter m_splitter;
    };

}
