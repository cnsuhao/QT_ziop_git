#include "StdAfx.h"
#include "bus/language/src/DialogResource.h"
#include "bus/language/src/FontOptionalProperty.h"


namespace TA_Base_Bus
{

    DialogResource::DialogResource( UINT id, const CString& caption )
        : m_id( id ),
          m_caption( caption )
    {
    }


    void DialogResource::applyDialog( CDialog* pDialog )
    {
        if ( NULL == pDialog )
        {
            return;
        }

        for ( size_t i = 0; i < m_optionalPropertyList.size(); ++i )
        {
            m_optionalPropertyList[i]->applyWnd( pDialog );
        }

        if ( m_caption.IsEmpty() != TRUE )
        {
            CPropertyPage* pPropertyPage = dynamic_cast<CPropertyPage*>(pDialog);

            if ( pPropertyPage != NULL )
            {
                setWindowTextForPropertyPage( pPropertyPage );
            }
            else
            {
                pDialog->SetWindowText( m_caption );
            }
        }

        for ( size_t i = 0; i < m_dialogItemResourceList.size(); ++i )
        {
            m_dialogItemResourceList[i]->applyDialog( pDialog );
        }

        pDialog->Invalidate();
        pDialog->UpdateWindow();
    }


    void DialogResource::addOptionalProperty( IOptionalPropertyPtr optionalProperty )
    {
        if ( optionalProperty )
        {
            m_optionalPropertyList.push_back( optionalProperty );
        }
    }


    void DialogResource::addDialogItemResource( IDialogItemResourcePtr dialogItemResource )
    {
        if ( ! dialogItemResource )
        {
            return;
        }

        m_dialogItemResourceList.push_back( dialogItemResource );

        for ( size_t i = 0; i < m_optionalPropertyList.size(); ++i )
        {
            if ( typeid( *m_optionalPropertyList[i] ) == typeid( FontOptionalProperty ) )
            {
                dialogItemResource->addInhertiedOptionalProperty( m_optionalPropertyList[i] );
            }
        }
    }


    void DialogResource::setWindowTextForPropertyPage( CPropertyPage* pPropertyPage )
    {
        if ( NULL == pPropertyPage )
        {
            return;
        }

        CWnd* pParent = pPropertyPage->GetParent();

        if ( NULL == pParent )
        {
            return;
        }

        CPropertySheet* pSheet = dynamic_cast<CPropertySheet*>( pParent );

        if ( pSheet != NULL )
        {
            CTabCtrl* pTabCtrl = pSheet->GetTabControl();

            if ( pTabCtrl != NULL )
            {
                int index = pSheet->GetPageIndex(pPropertyPage);
                TCITEM tci;
                tci.mask = TCIF_TEXT;
                tci.pszText = const_cast<TCHAR*>( (const TCHAR*)m_caption );
                pTabCtrl->SetItem(index, &tci);
            }

            return;
        }

        CTabCtrl* pTab = dynamic_cast<CTabCtrl*>( pParent );

        if ( pTab != NULL )
        {
            // TODO
            return;
        }
    }

}
