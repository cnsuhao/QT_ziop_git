#pragma once
#include "bus/language/src/IResourceLoader.h"
#include "bus/language/src/RawResourceSplitter.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    class ToolBarResourceLoader : public IResourceLoader
    {
        enum { APPLICATION_ID, RESOURCE_TYPE_ID, TOOLBAR_ID, TOOLBAR_NAME_ID, TOOLBAR_BUTTON_CAPTION_LIST/*B0 "Button 0", B1 "Button 1", ...*/ };

    public:

        ToolBarResourceLoader( const std::string& rawResource );

        virtual IResourcePtr createResource();
        virtual void addResourceToContainer( IResourcePtr resource, IResourceContainerPtr containder );

        static IResourceLoaderPtr create( const std::string& rawResource ) { return IResourceLoaderPtr( new ToolBarResourceLoader(rawResource) ); }
        static UINT getResourceTypeID() { return RESOURCE_TYPE_ID_ToolBar; }

    private:

        RawResourceSplitter m_splitter;
    };

}
