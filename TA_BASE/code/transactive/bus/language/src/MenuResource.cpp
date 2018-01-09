#include "StdAfx.h"
#include "bus/language/src/MenuResource.h"


namespace TA_Base_Bus
{

    MenuResource::MenuResource( UINT id )
        : m_id( id )
    {
    }


    void MenuResource::applyMenu( CMenu* pMenu )
    {
        if ( NULL ==  pMenu)
        {
            return;
        }

        for ( size_t i = 0; i < m_popupList.size(); ++i )
        {
            m_popupList[i]->applyMenu( pMenu );
        }

        for ( std::map<UINT, MenuItemResourcePtr>::const_iterator it = m_menuItemMap.begin(); it != m_menuItemMap.end(); ++it )
        {
            it->second->applyMenu( pMenu );
        }
    }


    void MenuResource::addPopupResource( PopupResourcePtr popupResource )
    {
        if ( popupResource )
        {
            for ( size_t i = 0; i < m_popupList.size(); ++i )
            {
                if ( *popupResource == *m_popupList[i] )
                {
                    return;
                }
            }

            m_popupList.push_back( popupResource );
        }
    }


    void MenuResource::addMenuItemResource( UINT id, MenuItemResourcePtr menuItemResource )
    {
        if ( menuItemResource )
        {
            if ( m_menuItemMap.find( id ) == m_menuItemMap.end() )
            {
                m_menuItemMap[ id ] = menuItemResource;
            }
        }
    }

}
