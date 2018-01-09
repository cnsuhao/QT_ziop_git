#include "StdAfx.h"
#include "bus/language/src/RadioButtonResourceLoader.h"
#include "bus/language/src/RadioButtonResource.h"


namespace TA_Base_Bus
{

    RadioButtonResourceLoader::RadioButtonResourceLoader( const std::string& rawResource )
        : AbstractDialogItemResourceLoader( rawResource )
    {
    }


    IResourcePtr RadioButtonResourceLoader::createResource()
    {
        UINT id = m_splitter.getUnsignedInteger( RADIOBUTTON_ID );
        CString caption = m_splitter.getCString( RADIOBUTTON_CAPTION );

        RadioButtonResourcePtr radioButtonResource( new RadioButtonResource( id, caption ) );

        addOptionalPropertyToResource( radioButtonResource );

        return radioButtonResource;
    }

}
