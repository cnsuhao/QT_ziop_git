#include "StdAfx.h"
#include "bus/language/src/PushBoxResourceLoader.h"
#include "bus/language/src/PushBoxResource.h"


namespace TA_Base_Bus
{

    PushBoxResourceLoader::PushBoxResourceLoader( const std::string& rawResource )
        : AbstractDialogItemResourceLoader( rawResource )
    {
    }


    IResourcePtr PushBoxResourceLoader::createResource()
    {
        UINT id = m_splitter.getUnsignedInteger( PUSHBOX_ID );
        CString caption = m_splitter.getCString( PUSHBOX_CAPTION );

        PushBoxResourcePtr pushBoxResource( new PushBoxResource( id, caption ) );

        addOptionalPropertyToResource( pushBoxResource );

        return pushBoxResource;
    }

}
