#include "StdAfx.h"
#include "CHECKBOXcontrolParser.h"
#include "Controls.h"
#include "RegexFactory.h"


IControlPtr CHECKBOXcontrolParser::parse()
{
    // CHECKBOX control
    // CHECKBOX text, id, x, y, width, height [, style [, extended-style]]

    const char* checkbox_control_regex_str =
        "(?x)"
        "CHECKBOX \\s+"
        "(\"[^\"]*\"), \\s*"         // $1 text
        "(\\w+), \\s*"               // $2 id
        "(\\d+), \\s*"               // $3 x
        "(\\d+), \\s*"               // $4 y
        "(\\d+), \\s*"               // $5 width
        "(\\d+)  \\s*"               // $6 height
        "(.*?\\z)?"                  // $7 [, style [, extended-style]]
        "";

    boost::smatch m;
    const boost::regex& checkbox_control_regex = RegexFactory::instance().create_regex( checkbox_control_regex_str );

    if ( boost::regex_search( m_string, m, checkbox_control_regex ) )
    {
        CHECKBOXcontrol* checkbox_control = new CHECKBOXcontrol;

        checkbox_control->m_text = m.str(1);
        checkbox_control->m_id = m.str(2);
        checkbox_control->m_x = m.str(3);
        checkbox_control->m_y = m.str(4);
        checkbox_control->m_width = m.str(5);
        checkbox_control->m_height = m.str(6);
        checkbox_control->m_style = m.str(7);

        return IControlPtr( checkbox_control );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IControlPtr();
}
