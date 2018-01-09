#pragma once
#include "bus/language/src/AbstractResource.h"


namespace TA_Base_Bus
{

    class ToolBarResource : public AbstractResource
    {
    public:

        ToolBarResource( const std::map<int, CString>& buttonCaptionMap );

        virtual void applyToolBar( CToolBar* pToolBar );

    private:

        std::map<int, CString> m_buttonCaptionMap;
    };

    typedef boost::shared_ptr<ToolBarResource> ToolBarResourcePtr;

}
