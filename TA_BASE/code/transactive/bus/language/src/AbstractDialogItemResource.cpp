#include "StdAfx.h"
#include "bus/language/src/AbstractDialogItemResource.h"


namespace TA_Base_Bus
{

    AbstractDialogItemResource::AbstractDialogItemResource( UINT id )
        : m_id( id )
    {
    }


    void AbstractDialogItemResource::applyDialog( CDialog* pDialog )
    {
        if ( NULL == pDialog )
        {
            return;
        }

        CWnd* pwnd = pDialog->GetDlgItem( m_id );

        if ( NULL == pwnd )
        {
            return;
        }

        for ( size_t i = 0; i < m_inheritedOptionalPropertyList.size(); ++i )
        {
            m_inheritedOptionalPropertyList[i]->applyWnd( pwnd );
        }

        for ( size_t i = 0; i < m_optionalPropertyList.size(); ++i )
        {
            m_optionalPropertyList[i]->applyWnd( pwnd );
        }

        const CString& caption = getCaption();

        if ( caption.IsEmpty() != TRUE )
        {
            pwnd->SetWindowText( caption );
        }
    }


    void AbstractDialogItemResource::addOptionalProperty( IOptionalPropertyPtr optionalProperty )
    {
        if ( optionalProperty )
        {
            m_optionalPropertyList.push_back( optionalProperty );
        }
    }


    void AbstractDialogItemResource::addInhertiedOptionalProperty( IOptionalPropertyPtr optionalProperty )
    {
        if ( optionalProperty )
        {
            m_inheritedOptionalPropertyList.push_back( optionalProperty );
        }
    }

}
