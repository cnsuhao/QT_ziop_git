#pragma once
#include "bus/language/src/IOptionalPropertyLoader.h"


namespace TA_Base_Bus
{

    class OptionalPropertyLoaderFactory
    {
    public:

        static OptionalPropertyLoaderFactory& instance();

        IOptionalPropertyPtr create( const std::string& rawOptionalProperty );

    private:

        OptionalPropertyLoaderFactory();

        typedef IOptionalPropertyLoaderPtr (*OptionalPropertyLoaderCreateFunc)( const std::string& rawResource );
        typedef std::map<std::string, OptionalPropertyLoaderCreateFunc> OperationPropertyLoaderCreatFuncMap;
        OperationPropertyLoaderCreatFuncMap m_operationPropertyLoaderCreatFuncMap;
    };

}
