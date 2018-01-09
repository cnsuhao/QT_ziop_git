#include "StdAfx.h"
#include "bus/language/src/GroupBoxResourceLoader.h"
#include "bus/language/src/GroupBoxResource.h"


namespace TA_Base_Bus
{

    GroupBoxResourceLoader::GroupBoxResourceLoader( const std::string& rawResource )
        : AbstractDialogItemResourceLoader( rawResource )
    {
    }


    IResourcePtr GroupBoxResourceLoader::createResource()
    {
        UINT id = m_splitter.getUnsignedInteger( GROUPBOX_ID );
        CString caption = m_splitter.getCString( GROUPBOX_CAPTION );

        GroupBoxResourcePtr groupBoxnResource( new GroupBoxResource( id, caption ) );

        addOptionalPropertyToResource( groupBoxnResource );

        return groupBoxnResource;
    }

}
