#pragma once
#include "bus/language/src/IResourceLoader.h"
#include "bus/language/src/RawResourceSplitter.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    class MenuItemResourceLoader : public IResourceLoader
    {
        enum { APPLICATION_ID, RESOURCE_TYPE_ID, MENUITEM_ID, MENU_ID, MENUITEM_NAME_ID, MENUITEM_CAPTION };

    public:

        MenuItemResourceLoader( const std::string& rawResource );

        virtual IResourcePtr createResource();
        virtual void addResourceToContainer( IResourcePtr menuItemResource, IResourceContainerPtr containder );

        static IResourceLoaderPtr create( const std::string& rawResource ) { return IResourceLoaderPtr( new MenuItemResourceLoader(rawResource) ); }
        static UINT getResourceTypeID() { return RESOURCE_TYPE_ID_MenuItem; }

    private:

        RawResourceSplitter m_splitter;
    };

}
