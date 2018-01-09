#include "StdAfx.h"
#include "bus/language/src/CheckBoxResourceLoader.h"
#include "bus/language/src/CheckBoxResource.h"


namespace TA_Base_Bus
{

    CheckBoxResourceLoader::CheckBoxResourceLoader( const std::string& rawResource )
        : AbstractDialogItemResourceLoader( rawResource )
    {
    }


    IResourcePtr CheckBoxResourceLoader::createResource()
    {
        UINT id = m_splitter.getUnsignedInteger( CHECKBOX_ID );
        CString caption = m_splitter.getCString( CHECKBOX_CAPTION );

        CheckBoxResourcePtr checkBoxnResource( new CheckBoxResource( id, caption ) );

        addOptionalPropertyToResource( checkBoxnResource );

        return checkBoxnResource;
    }

}
