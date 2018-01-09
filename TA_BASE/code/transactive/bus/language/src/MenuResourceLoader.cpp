#include "StdAfx.h"
#include "bus/language/src/MenuResourceLoader.h"
#include "bus/language/src/MenuResource.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    MenuResourceLoader::MenuResourceLoader( const std::string& rawResource )
        : m_splitter( rawResource )
    {
    }


    IResourcePtr MenuResourceLoader::createResource()
    {
        UINT id = m_splitter.getUnsignedInteger( MENU_ID );

        return MenuResourcePtr( new MenuResource(id) );
    }


    void MenuResourceLoader::addResourceToContainer( IResourcePtr resource, IResourceContainerPtr containder )
    {
        UINT id = m_splitter.getUnsignedInteger( MENU_ID );

        containder->addResource( MenuResourceKey(id), resource );
    }

}
