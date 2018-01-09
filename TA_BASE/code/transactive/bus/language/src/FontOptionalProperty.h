#pragma once
#include "bus/language/src/IOptionalProperty.h"


namespace TA_Base_Bus
{

    class FontOptionalProperty : public IOptionalProperty
    {
    public:

        FontOptionalProperty( UINT pointsize, const CString& typeface );

        virtual void applyWnd( CWnd* pwnd );

    private:

        CFont m_font;
    };

}
