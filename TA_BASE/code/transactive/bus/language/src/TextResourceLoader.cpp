#include "StdAfx.h"
#include "bus/language/src/TextResourceLoader.h"
#include "bus/language/src/TextResource.h"


namespace TA_Base_Bus
{

    TextResourceLoader::TextResourceLoader( const std::string& rawResource )
        : AbstractDialogItemResourceLoader( rawResource )
    {
    }


    IResourcePtr TextResourceLoader::createResource()
    {
        UINT id = m_splitter.getUnsignedInteger( TEXT_ID );
        CString caption = m_splitter.getCString( TEXT_CAPTION );

        TextResourcePtr textResource( new TextResource(id, caption) );

        addOptionalPropertyToResource( textResource );

        return textResource;
    }

}
