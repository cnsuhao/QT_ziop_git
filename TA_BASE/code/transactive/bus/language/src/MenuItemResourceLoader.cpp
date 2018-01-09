#include "StdAfx.h"
#include "bus/language/src/MenuItemResourceLoader.h"
#include "bus/language/src/MenuItemResource.h"
#include "bus/language/src/MenuResource.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    MenuItemResourceLoader::MenuItemResourceLoader( const std::string& rawResource )
        : m_splitter( rawResource )
    {
    }


    IResourcePtr MenuItemResourceLoader::createResource()
    {
        UINT id = m_splitter.getUnsignedInteger( MENUITEM_ID );
        CString caption = m_splitter.getCString( MENUITEM_CAPTION );

        return MenuItemResourcePtr( new MenuItemResource(id, caption) );
    }


    void MenuItemResourceLoader::addResourceToContainer( IResourcePtr menuItemResource, IResourceContainerPtr containder )
    {
        UINT id = m_splitter.getUnsignedInteger( MENUITEM_ID );
        UINT menuID = m_splitter.getUnsignedInteger( MENU_ID );

        IResourcePtr resource = containder->getResource( MenuResourceKey(menuID) );
        MenuItemResourcePtr menuItem = boost::dynamic_pointer_cast<MenuItemResource>( menuItemResource );
        MenuResourcePtr menu = boost::dynamic_pointer_cast<MenuResource>( resource );

        if ( menu && menuItem )
        {
            menu->addMenuItemResource( id, menuItem );
        }
    }

}
