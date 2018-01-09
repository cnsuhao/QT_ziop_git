#include "StdAfx.h"
#include "bus/language/src/PushButtonResourceLoader.h"
#include "bus/language/src/PushButtonResource.h"


namespace TA_Base_Bus
{

    PushButtonResourceLoader::PushButtonResourceLoader( const std::string& rawResource )
        : AbstractDialogItemResourceLoader( rawResource )
    {
    }


    IResourcePtr PushButtonResourceLoader::createResource()
    {
        UINT id = m_splitter.getUnsignedInteger( PUSHBUTTON_ID );
        CString caption = m_splitter.getCString( PUSHBUTTON_CAPTION );

        PushButtonResourcePtr pushButtonResource( new PushButtonResource( id, caption ) );

        addOptionalPropertyToResource( pushButtonResource );

        return pushButtonResource;
    }

}
