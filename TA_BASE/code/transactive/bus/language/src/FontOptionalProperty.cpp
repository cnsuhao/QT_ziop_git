#include "StdAfx.h"
#include "bus/language/src/FontOptionalProperty.h"


namespace TA_Base_Bus
{

    FontOptionalProperty::FontOptionalProperty( UINT pointsize, const CString& typeface )
    {
        m_font.CreatePointFont( pointsize * 10, typeface );
    }


    void FontOptionalProperty::applyWnd( CWnd* pwnd )
    {
        pwnd->SetFont( &m_font );
    }

}
