#pragma once
#include "bus/language/src/IDialogItemResource.h"


namespace TA_Base_Bus
{

    class AbstractDialogItemResource : public IDialogItemResource
    {
    protected:

        AbstractDialogItemResource( UINT id );

        virtual void applyDialog( CDialog* pDialog );
        virtual void addOptionalProperty( IOptionalPropertyPtr optionalProperty );
        virtual void addInhertiedOptionalProperty( IOptionalPropertyPtr optionalProperty );
        virtual const CString& getCaption() { static CString empty; return empty; }

    protected:

        UINT m_id;
        std::vector<IOptionalPropertyPtr> m_optionalPropertyList;
        std::vector<IOptionalPropertyPtr> m_inheritedOptionalPropertyList;
    };

    typedef boost::shared_ptr<AbstractDialogItemResource> AbstractDialogItemResourcePtr;

}
