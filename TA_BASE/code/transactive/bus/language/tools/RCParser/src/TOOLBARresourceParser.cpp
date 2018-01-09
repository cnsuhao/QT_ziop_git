#include "StdAfx.h"
#include "TOOLBARresourceParser.h"
#include "StatementParserFactory.h"
#include "Resources.h"
#include "RegexFactory.h"


IResourcePtr TOOLBARresourceParser::parse()
{
    // TOOLBAR resource
    // toolbarID TOOLBAR  x, y {button-statements}
    const char* toolbar_resource_regex_str =
        "(?x)"
        // $1                                                $2           $3
        // toolbarID   TOOLBAR                               x,           y,
        "^ (\\w+) \\s+ TOOLBAR \\s+ (?:DISCARDABLE \\s+)? (\\d+), \\s+ (\\d+)"
        " .*? "
        "^BEGIN$"
        "(.*?)"      // $4 {button-statements}
        "^END$"
        "";

    boost::smatch m;
    const boost::regex& toolbar_resource_regex = RegexFactory::instance().create_regex( toolbar_resource_regex_str );

    if ( boost::regex_match( m_string, m, toolbar_resource_regex ) )
    {
        TOOLBARresource* toolbar_resource = new TOOLBARresource;

        toolbar_resource->m_toolbarID     = m.str(1);  // $1 toolbarID
        toolbar_resource->m_x             = m.str(2) ; // $2 x
        toolbar_resource->m_y             = m.str(3) ; // $3 y

        // TODO: deal with SEPARATOR
        // button-statements
        const std::string& control_stements = m.str(4); // $4 {button-statements}
        StatementParserFactory statement_parser_factory( control_stements );
        toolbar_resource->m_buttons = statement_parser_factory.parse();

        return IResourcePtr( toolbar_resource );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IResourcePtr();
}
