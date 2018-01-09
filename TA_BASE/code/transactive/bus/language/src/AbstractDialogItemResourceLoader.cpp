#include "StdAfx.h"
#include "bus/language/src/AbstractDialogItemResourceLoader.h"
#include "bus/language/src/IDialogResource.h"
#include "bus/language/src/TALanguage.h"


namespace TA_Base_Bus
{

    AbstractDialogItemResourceLoader::AbstractDialogItemResourceLoader( const std::string& rawResource )
        : AbstractResourceLoader( rawResource )
    {
    }


    void AbstractDialogItemResourceLoader::addResourceToContainer( IResourcePtr resource, IResourceContainerPtr containder )
    {
        UINT parentID = m_splitter.getInteger( getParentIDIndex() );
        IResourcePtr parentResource = containder->getResource( DialogResourceKey(parentID) );
        IDialogResourcePtr dialogResource = boost::dynamic_pointer_cast<IDialogResource>( parentResource );
        IDialogItemResourcePtr dialogItemResource = boost::dynamic_pointer_cast<IDialogItemResource>( resource );

        if ( dialogResource && dialogItemResource )
        {
            dialogResource->addDialogItemResource( dialogItemResource );
        }
    }

}
