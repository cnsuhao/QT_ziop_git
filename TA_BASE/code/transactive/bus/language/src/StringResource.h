#pragma once
#include "bus/language/src/AbstractResource.h"


namespace TA_Base_Bus
{

    class StringResource : public AbstractResource
    {
    public:

        StringResource( const CString& caption );

        const CString& getCString() { return m_caption; }

    private:

        CString m_caption;
    };

    typedef boost::shared_ptr<StringResource> StringResourcePtr;

}
