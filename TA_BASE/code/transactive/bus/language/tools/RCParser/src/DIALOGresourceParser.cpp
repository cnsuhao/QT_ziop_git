#include "StdAfx.h"
#include "DIALOGresourceParser.h"
#include "StatementParserFactory.h"
#include "ControlParserFactory.h"
#include "Resources.h"
#include "RegexFactory.h"


IResourcePtr DIALOGresourceParser::parse()
{
    // DIALOG resource
    // nameID DIALOG x, y, width, height  [optional-statements] {control-statement  . . . }
    const char* dialog_resource_regex_str =
        "(?x)"
        // $1                                              $2           $3          $4          $5
        // nameID      DIALOG                              x,           y,          width,      height
        "^ (\\w+) \\s+ DIALOG \\s+ (?:DISCARDABLE \\s+)? (\\d+), \\s+ (\\d+), \\s+ (\\d+), \\s+ (\\d+)"
        "(.*?)"      // $6 [optional-statements]
        "^BEGIN$"
        "(.*?)"      // $7 {control-statement  . . . }
        "^END$"
        "";

    boost::smatch m;
    const boost::regex& dialog_resource_regex = RegexFactory::instance().create_regex( dialog_resource_regex_str );

    if ( boost::regex_match( m_string, m, dialog_resource_regex ) )
    {
        DIALOGresource* dialog_resource = new DIALOGresource;

        dialog_resource->m_nameID = m.str(1);  // $1 nameID
        dialog_resource->m_x = m.str(2) ;      // $2 x
        dialog_resource->m_y = m.str(3) ;      // $3 y
        dialog_resource->m_width = m.str(4) ;  // $4 width
        dialog_resource->m_height = m.str(5) ; // $5 height

        // [optional-statements]
        const std::string& optional_statements = m.str(6); // $6 [optional-statements]
        StatementParserFactory statement_parser_factory( optional_statements );
        dialog_resource->m_optional_statements = statement_parser_factory.parse();

        // control-statements
        const std::string& control_stements = m.str(7); // $7 {control-statement  . . . }

        const boost::regex& white_space_regex = RegexFactory::instance().create_regex( "\\s*" );

        if ( ! boost::regex_match( control_stements, white_space_regex ) )
        {
            ControlParserFactory control_parser_factory( control_stements );
            dialog_resource->m_controls = control_parser_factory.parse();
        }

        return IResourcePtr( dialog_resource );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IResourcePtr();
}
