#include "StdAfx.h"
#include "bus/language/src/StringResourceLoader.h"
#include "bus/language/src/StringResource.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    StringResourceLoader::StringResourceLoader( const std::string& rawResource )
        : m_splitter( rawResource )
    {
    }


    IResourcePtr StringResourceLoader::createResource()
    {
        CString caption = m_splitter.getCString( STRING_CAPTION );

        return IResourcePtr( new StringResource(caption) );
    }


    void StringResourceLoader::addResourceToContainer( IResourcePtr resource, IResourceContainerPtr containder )
    {
        UINT id = m_splitter.getUnsignedInteger( STRING_ID );

        containder->addResource( StringResourceKey(id), resource );
    }

}
