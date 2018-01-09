#pragma once
#include "bus/language/src/AbstractDialogItemResource.h"


namespace TA_Base_Bus
{

    class SysListView32Resource : public AbstractDialogItemResource
    {
    public:

        SysListView32Resource( UINT id, const std::map<UINT, CString>& columnNameMap );

        virtual void applyDialog( CDialog* pDialog );

    private:

        std::map<UINT, CString> m_columnNameMap;
    };

    typedef boost::shared_ptr<SysListView32Resource> SysListView32ResourcePtr;

}
