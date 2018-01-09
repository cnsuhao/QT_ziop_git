#pragma once
#include "bus/language/src/IOptionalProperty.h"


namespace TA_Base_Bus
{

    class IResource
    {
    public:

        virtual ~IResource() {}

        virtual void applyDialog( CDialog* pDialog ) = 0;
        virtual void applyMenu( CMenu* pMenu ) = 0;
        virtual void applyToolBar( CToolBar* pToolBar ) = 0;
        virtual const CString& getCString() = 0;
        virtual void addOptionalProperty( IOptionalPropertyPtr optionalProperty ) = 0;
        virtual bool operator == ( const IResource& rhs ) const { return false; }
    };

    typedef boost::shared_ptr<IResource> IResourcePtr;

}
