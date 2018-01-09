#include "StdAfx.h"
#include "bus/language/src/DialogResourceLoader.h"
#include "bus/language/src/DialogResource.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    DialogResourceLoader::DialogResourceLoader( const std::string& rawResource )
        : AbstractResourceLoader( rawResource )
    {
    }


    void DialogResourceLoader::addResourceToContainer( IResourcePtr resource, IResourceContainerPtr containder )
    {
        UINT id = m_splitter.getUnsignedInteger( DIALOG_ID );

        containder->addResource( DialogResourceKey(id), resource );
    }


    IResourcePtr DialogResourceLoader::createResource()
    {
        UINT id = m_splitter.getUnsignedInteger( DIALOG_ID );
        CString caption = m_splitter.getCString( DIALOG_CAPTION );

        DialogResourcePtr dialogResource( new DialogResource( id, caption ) );

        addOptionalPropertyToResource( dialogResource );

        return dialogResource;
    }

}
