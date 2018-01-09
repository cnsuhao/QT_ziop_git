#pragma once
#include "bus/language/src/AbstractDialogItemResource.h"


namespace TA_Base_Bus
{

    class TextResource : public AbstractDialogItemResource
    {
    public:

        TextResource( UINT id, const CString& caption );

        const CString& getCaption() { return m_caption; }

    private:

        CString m_caption;
    };

    typedef boost::shared_ptr<TextResource> TextResourcePtr;

}
