#pragma once
#include "bus/language/src/AbstractResource.h"


namespace TA_Base_Bus
{

    class PopupResource : public AbstractResource
    {
    public:

        PopupResource( const CString& caption, const std::vector<UINT>& popupNestedIndex );

        virtual void applyMenu( CMenu* pMenu );

        bool operator == ( const PopupResource& rhs ) const;

    private:

        CString m_caption;
        std::vector<UINT> m_popupNestedIndex;
    };

    typedef boost::shared_ptr<PopupResource> PopupResourcePtr;

}
