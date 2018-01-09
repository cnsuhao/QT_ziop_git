#include "StdAfx.h"
#include "STRINGTABLEresourceParser.h"
#include "StatementParserFactory.h"
#include "ControlParserFactory.h"
#include "STRINGresourceParser.h"
#include "Resources.h"
#include "RegexFactory.h"


IResourcePtr STRINGTABLEresourceParser::parse()
{
    STRINGTABLEresource* stringtable_resource = new STRINGTABLEresource;

    // STRINGTABLE resource
    // STRINGTABLE  [optional-statements] {stringID string  ...}
    const char* stringtable_resource_regex_str =
        "(?x)"
        " ^ \\s+ "
        " (\\w+ \\s+ \"[^\"]*\") "      // $1 stringID string
        "";

    const boost::regex& stringtable_resource_regex = RegexFactory::instance().create_regex( stringtable_resource_regex_str );
    boost::sregex_iterator it( m_string.begin(), m_string.end(), stringtable_resource_regex );
    boost::sregex_iterator end;

    for ( ; it != end; ++it )
    {
        // TODO: factory?
        STRINGresourceParser parser( it->str(1) );
        IResourcePtr resource = parser.parse();

        stringtable_resource->m_strings.push_back( resource );
    }

    return IResourcePtr( stringtable_resource );
}
