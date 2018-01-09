#pragma once
#include "bus/language/src/IResourceLoader.h"
#include "bus/language/src/RawResourceSplitter.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    class PopupResourceLoader : public IResourceLoader
    {
        enum { APPLICATION_ID, RESOURCE_TYPE_ID, MENU_ID, POPUP_NESTED_INDEX, POPUP_CAPTION };

    public:

        PopupResourceLoader( const std::string& rawResource );

        virtual IResourcePtr createResource();
        virtual void addResourceToContainer( IResourcePtr popupResource, IResourceContainerPtr containder );

        static IResourceLoaderPtr create( const std::string& rawResource ) { return IResourceLoaderPtr( new PopupResourceLoader(rawResource) ); }
        static UINT getResourceTypeID() { return RESOURCE_TYPE_ID_Popup; }

    private:

        RawResourceSplitter m_splitter;
    };

}
