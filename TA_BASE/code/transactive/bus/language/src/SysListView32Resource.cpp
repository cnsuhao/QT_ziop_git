#include "StdAfx.h"
#include "bus/language/src/SysListView32Resource.h"


namespace TA_Base_Bus
{

    SysListView32Resource::SysListView32Resource( UINT id, const std::map<UINT, CString>& columnNameMap )
        : AbstractDialogItemResource( id ),
          m_columnNameMap( columnNameMap )
    {
    }


    void SysListView32Resource::applyDialog( CDialog* pDialog )
    {
        if ( NULL == pDialog )
        {
            return;
        }

        CListCtrl* pListCtrl = dynamic_cast<CListCtrl*>( pDialog->GetDlgItem( m_id ) );

        if ( NULL == pListCtrl )
        {
            return;
        }

        CHeaderCtrl* pHeaderCtrl = pListCtrl->GetHeaderCtrl();

        if ( NULL == pHeaderCtrl )
        {
            return;
        }

        for ( size_t i = 0; i < m_inheritedOptionalPropertyList.size(); ++i )
        {
            m_inheritedOptionalPropertyList[i]->applyWnd( pListCtrl );
        }

        for ( size_t i = 0; i < m_optionalPropertyList.size(); ++i )
        {
            m_optionalPropertyList[i]->applyWnd( pListCtrl );
        }

        for ( std::map<UINT, CString>::const_iterator it = m_columnNameMap.begin(); it != m_columnNameMap.end(); ++it )
        {
            HDITEM htItem = { 0 };
            htItem.mask = HDI_TEXT;
            htItem.pszText = const_cast<TCHAR*>( (const TCHAR*)it->second );

            if ( static_cast<int>(it->first) < pHeaderCtrl->GetItemCount() )
            {
                pHeaderCtrl-> SetItem( it->first, &htItem );
            }
        }
    }

}
