#pragma once
#include "bus/language/src/IOptionalPropertyLoader.h"


namespace TA_Base_Bus
{

    class FontOptionalPropertyLoader : public IOptionalPropertyLoader
    {
    public:

        FontOptionalPropertyLoader( const std::string& rawFontResource );

        IOptionalPropertyPtr createOptionalProperty();

        static IOptionalPropertyLoaderPtr create( const std::string& rawFontResource );
        static const std::string getPrefixString() { return "FONT"; }

    private:

        std::string m_rawFontResource;
    };

}
