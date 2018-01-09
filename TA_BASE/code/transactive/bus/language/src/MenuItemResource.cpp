#include "StdAfx.h"
#include "bus/language/src/MenuItemResource.h"


namespace TA_Base_Bus
{

    MenuItemResource::MenuItemResource( UINT id, const CString& caption )
        : m_id( id ),
          m_caption( caption )
    {
    }


    void MenuItemResource::applyMenu( CMenu* pMenu )
    {
        if ( pMenu != NULL && FALSE == m_caption.IsEmpty() )
        {
            pMenu->ModifyMenu( m_id, MF_BYCOMMAND, m_id, m_caption );
        }
    }

}
