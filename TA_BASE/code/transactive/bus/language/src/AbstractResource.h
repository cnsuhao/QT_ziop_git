#pragma once
#include "bus/language/src/IResource.h"


namespace TA_Base_Bus
{

    class AbstractResource : public IResource
    {
    public:

        virtual void applyDialog( CDialog* pDialog ) {}
        virtual void applyMenu( CMenu* pMenu ) {}
        virtual void applyToolBar( CToolBar* pToolBar ) {}
        virtual const CString& getCString() { static CString empty; return empty; }
        virtual void addOptionalProperty( IOptionalPropertyPtr optionalProperty ) {}
    };

}
