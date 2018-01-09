#include "StdAfx.h"
#include "STRINGresourceParser.h"
#include "Resources.h"
#include "RegexFactory.h"


IResourcePtr STRINGresourceParser::parse()
{
    // STRING resource
    // stringID string
    const char* string_resource_regex_str =
        "(?x)"
        " (\\w+) \\s+ "         // $1 stringID
        " (\"[^\"]*\") "        // $2 string
        "";

    boost::smatch m;
    const boost::regex& string_resource_regex = RegexFactory::instance().create_regex( string_resource_regex_str );

    if ( boost::regex_search( m_string, m, string_resource_regex ) )
    {
        STRINGresource* string_resource = new STRINGresource;

        string_resource->m_stringID = m.str(1);
        string_resource->m_string   = m.str(2);

        return IResourcePtr( string_resource );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IResourcePtr();
}
