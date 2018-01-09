#include "StdAfx.h"
#include "bus/language/src/ToolBarResource.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    ToolBarResource::ToolBarResource( const std::map<int, CString>& buttonCaptionMap )
        : m_buttonCaptionMap( buttonCaptionMap )
    {
    }


    void ToolBarResource::applyToolBar( CToolBar* pToolBar )
    {
        if ( NULL == pToolBar )
        {
            return;
        }

        CToolBarCtrl &toolBarCtrl = pToolBar->GetToolBarCtrl();
        int count = toolBarCtrl.GetButtonCount();

        for ( std::map<int, CString>::iterator it = m_buttonCaptionMap.begin(); it != m_buttonCaptionMap.end(); ++it )
        {
            int index = it->first;
            CString& text = it->second;

            if ( index < count )
            {
                pToolBar->SetButtonText( index, text );
            }
        }

        pToolBar->Invalidate();

        // update tool tip
        CToolTipCtrl* toolTipCtrl = toolBarCtrl.GetToolTips();

        if ( NULL == toolTipCtrl )
        {
            return;
        }

        for ( int i = 0; i < pToolBar->GetCount(); ++i )
        {
            UINT id = pToolBar->GetItemID( i );
            const CString& str = TALoadString( id ).c_str();

            int pos = str.Find( '\n' );

            if ( pos != -1 )
            {
                CString tip = str.Right( str.GetLength() - pos - 1 );

                if ( tip.IsEmpty() != TRUE )
                {
                    toolTipCtrl->UpdateTipText( tip, pToolBar, id );
                }
            }
        }
    }

}
