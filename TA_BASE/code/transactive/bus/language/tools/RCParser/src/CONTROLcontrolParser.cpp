#include "StdAfx.h"
#include "CONTROLcontrolParser.h"
#include "Controls.h"
#include "RegexFactory.h"


IControlPtr CONTROLcontrolParser::parse()
{
    // CONTROL control
    // CONTROL text, id, class, style, x, y, width, height [, extended-style]

    const char* control_control_regex_str =
        "(?x)"
        "CONTROL \\s+"
        "(\"[^\"]*\"), \\s*"                            // $1 text
        "(\\w+), \\s*"                                  // $2 id
        "\"([^\"]+)\", \\s*"                            // $3 class
        // "( (?:NOT\\s+)? \\w+ \\s*)((?:\\s* \\| \\s* (?:NOT\\s+)? \\w+)*), \\s*"    // $4,5 style
        "((.*?)), \\s*"                                 // $4,5 style // TODO: need test
        "(\\d+), \\s*"                                  // $6 x
        "(\\d+), \\s*"                                  // $7 y
        "(\\d+), \\s*"                                  // $8 width
        "(\\d+)  \\s*"                                  // $9 height
        "(.*?\\z)?"                                     // $10 [, style [, extended-style]]
        "";

    boost::smatch m;
    const boost::regex& control_control_regex = RegexFactory::instance().create_regex( control_control_regex_str );

    if ( boost::regex_search( m_string, m, control_control_regex ) )
    {
        CONTROLcontrol* control_control = new CONTROLcontrol;

        control_control->m_text = m.str(1);
        control_control->m_id = m.str(2);
        control_control->m_class = m.str(3);
        control_control->m_style = m.str(4);
        control_control->m_style += m.str(5);
        control_control->m_x = m.str(6);
        control_control->m_y = m.str(7);
        control_control->m_width = m.str(8);
        control_control->m_height = m.str(9);
        control_control->m_extended_style = m.str(10);

        return IControlPtr( control_control );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IControlPtr();
}
