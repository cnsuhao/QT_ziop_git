#pragma once
#include "bus/language/src/AbstractResource.h"


namespace TA_Base_Bus
{

    class MenuItemResource : public AbstractResource
    {
    public:

        MenuItemResource( UINT id, const CString& caption );

        void applyMenu( CMenu* pMenu );

    private:

        UINT m_id;
        CString m_caption;
    };

    typedef boost::shared_ptr<MenuItemResource> MenuItemResourcePtr;

}
