#pragma once
#include "bus/language/src/AbstractResource.h"
#include "bus/language/src/MenuItemResource.h"
#include "bus/language/src/PopupResource.h"


namespace TA_Base_Bus
{

    class MenuResource : public AbstractResource
    {
    public:

        MenuResource( UINT id );

        virtual void applyMenu( CMenu* pMenu );
        void addPopupResource( PopupResourcePtr popupResource );
        void addMenuItemResource( UINT id, MenuItemResourcePtr menuItemResource );

    private:

        UINT m_id;
        std::vector<PopupResourcePtr> m_popupList;
        std::map<UINT, MenuItemResourcePtr> m_menuItemMap;
    };

    typedef boost::shared_ptr<MenuResource> MenuResourcePtr;

}
