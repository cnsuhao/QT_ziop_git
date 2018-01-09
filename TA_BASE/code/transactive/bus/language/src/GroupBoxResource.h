#pragma once
#include "bus/language/src/AbstractDialogItemResource.h"


namespace TA_Base_Bus
{

    class GroupBoxResource : public AbstractDialogItemResource
    {
    public:

        GroupBoxResource( UINT id, const CString& caption );

        const CString& getCaption() { return m_caption; }

    private:

        CString m_caption;
    };

    typedef boost::shared_ptr<GroupBoxResource> GroupBoxResourcePtr;

}
