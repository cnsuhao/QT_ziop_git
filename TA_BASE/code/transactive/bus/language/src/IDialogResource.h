#pragma once
#include "bus/language/src/IResource.h"
#include "bus/language/src/IDialogItemResource.h"


namespace TA_Base_Bus
{

    class IDialogResource : public AbstractResource
    {
    public:

        virtual void addDialogItemResource( IDialogItemResourcePtr dialogItemResource ) = 0;
        virtual const CString& getCaption() = 0;
    };

    typedef boost::shared_ptr<IDialogResource> IDialogResourcePtr;

}
