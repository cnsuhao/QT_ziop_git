#include "StdAfx.h"
#include "bus/language/src/PopupResource.h"


namespace TA_Base_Bus
{

    PopupResource::PopupResource( const CString& caption, const std::vector<UINT>& popupNestedIndex )
        : m_caption( caption ),
          m_popupNestedIndex( popupNestedIndex )
    {
    }


    void PopupResource::applyMenu( CMenu* pMenu )
    {
        if ( NULL == pMenu || TRUE == m_caption.IsEmpty() )
        {
            return;
        }

        for ( size_t i = 0; i < m_popupNestedIndex.size() - 1 && pMenu != NULL; ++i )
        {
            pMenu = pMenu->GetSubMenu( m_popupNestedIndex[i] );
        }

        if ( pMenu != NULL )
        {
            pMenu->ModifyMenu( m_popupNestedIndex.back(), MF_BYPOSITION, 0, m_caption );
        }
    }


    bool PopupResource::operator == ( const PopupResource& rhs ) const
    {
        return m_caption == rhs.m_caption && m_popupNestedIndex == rhs.m_popupNestedIndex;
    }

}
