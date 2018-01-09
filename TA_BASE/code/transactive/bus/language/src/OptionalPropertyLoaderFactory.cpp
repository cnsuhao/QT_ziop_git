#include "StdAfx.h"
#include "bus/language/src/OptionalPropertyLoaderFactory.h"
#include "bus/language/src/FontOptionalPropertyLoader.h"


namespace TA_Base_Bus
{

    OptionalPropertyLoaderFactory& OptionalPropertyLoaderFactory::instance()
    {
        static OptionalPropertyLoaderFactory instance_;
        return instance_;
    }


    OptionalPropertyLoaderFactory::OptionalPropertyLoaderFactory()
    {
        m_operationPropertyLoaderCreatFuncMap.insert( OperationPropertyLoaderCreatFuncMap::value_type( FontOptionalPropertyLoader::getPrefixString(), FontOptionalPropertyLoader::create ) );
    }


    IOptionalPropertyPtr OptionalPropertyLoaderFactory::create( const std::string& rawOptionalProperty )
    {
        std::string prefixString;
        static boost::regex e( "(?x) ^ [[:word:]]+ " );
        boost::smatch m;

        if ( boost::regex_search( rawOptionalProperty, m, e ) )
        {
            prefixString = m.str();
        }

        for ( OperationPropertyLoaderCreatFuncMap::iterator it = m_operationPropertyLoaderCreatFuncMap.begin(); it != m_operationPropertyLoaderCreatFuncMap.end(); ++it )
        {
            if ( prefixString == it->first )
            {
                IOptionalPropertyLoaderPtr optionalPropertyLoader = it->second( rawOptionalProperty );
                return optionalPropertyLoader->createOptionalProperty();
            }
        }

        return IOptionalPropertyPtr();
    }

}
