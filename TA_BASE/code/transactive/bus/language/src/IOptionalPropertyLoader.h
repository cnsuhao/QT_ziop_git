#pragma once
#include "bus/language/src/IOptionalProperty.h"


namespace TA_Base_Bus
{

    class IOptionalPropertyLoader
    {
    public:
        ~IOptionalPropertyLoader() {}
        virtual IOptionalPropertyPtr createOptionalProperty() = 0;
    };

    typedef boost::shared_ptr<IOptionalPropertyLoader> IOptionalPropertyLoaderPtr;

}
