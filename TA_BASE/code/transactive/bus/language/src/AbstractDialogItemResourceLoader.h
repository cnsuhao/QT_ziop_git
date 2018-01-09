#pragma once
#include "bus/language/src/AbstractResourceLoader.h"
#include "bus/language/src/IDialogItemResource.h"


namespace TA_Base_Bus
{

    class AbstractDialogItemResourceLoader : public AbstractResourceLoader
    {
    public:

        virtual void addResourceToContainer( IResourcePtr resource, IResourceContainerPtr resourceContainer );
        virtual UINT getParentIDIndex() = 0;

    protected:

        AbstractDialogItemResourceLoader( const std::string& rawResource );

    protected:

        IDialogItemResourcePtr m_resource;
    };

}
