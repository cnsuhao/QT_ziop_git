#include "StdAfx.h"
#include "bus/language/src/FontOptionalPropertyLoader.h"
#include "bus/language/src/FontOptionalProperty.h"
#include "bus/language/src/CharacterCodeConverter.h"


namespace TA_Base_Bus
{

    FontOptionalPropertyLoader::FontOptionalPropertyLoader( const std::string& rawFontResource )
        : m_rawFontResource( rawFontResource )
    {
    }


    IOptionalPropertyPtr FontOptionalPropertyLoader::createOptionalProperty()
    {
        static const boost::regex e
        (
            "(?x)"
            " ^ FONT [[:space:]]+ ( [[:digit:]]+ ) [[:space:]]* , [[:space:]]* "  // $1 pointsize
            " \" ( [^\"]+ ) \" $ "                                                // $2 typeface
        );

        boost::smatch m;

        if ( boost::regex_match( m_rawFontResource, m, e ) )
        {
            UINT fontsize = boost::lexical_cast<UINT>( m.str(1) );
            CString typeface = CharacterCodeConverter::convertUTF8toCString( m.str(2).c_str() );

            return IOptionalPropertyPtr( new FontOptionalProperty(fontsize, typeface) );
        }

        return IOptionalPropertyPtr();
    }


    IOptionalPropertyLoaderPtr FontOptionalPropertyLoader::create( const std::string& rawFontResource )
    {
        return IOptionalPropertyLoaderPtr( new FontOptionalPropertyLoader(rawFontResource) );
    }

}
