#pragma once
#include "bus/language/src/AbstractResource.h"


namespace TA_Base_Bus
{

    class IDialogItemResource : public AbstractResource
    {
    public:

        virtual void addInhertiedOptionalProperty( IOptionalPropertyPtr optionalProperty ) = 0;
    };

    typedef boost::shared_ptr<IDialogItemResource> IDialogItemResourcePtr;

}
