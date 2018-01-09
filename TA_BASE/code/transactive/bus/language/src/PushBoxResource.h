#pragma once
#include "bus/language/src/AbstractDialogItemResource.h"


namespace TA_Base_Bus
{

    class PushBoxResource : public AbstractDialogItemResource
    {
    public:

        PushBoxResource( UINT id, const CString& caption );

        const CString& getCaption() { return m_caption; }

    private:

        CString m_caption;
    };

    typedef boost::shared_ptr<PushBoxResource> PushBoxResourcePtr;

}
