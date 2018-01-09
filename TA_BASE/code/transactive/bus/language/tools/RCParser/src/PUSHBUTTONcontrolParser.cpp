#include "StdAfx.h"
#include "PUSHBUTTONcontrolParser.h"
#include "Controls.h"
#include "RegexFactory.h"


IControlPtr PUSHBUTTONcontrolParser::parse()
{
    // PUSHBUTTON control
    // PUSHBUTTON text, id, x, y [, width, height, style [, extended-style]]

    const char* pushbutton_control_regex_str =
        "(?x)"
        "PUSHBUTTON \\s+"
        "(\"[^\"]*\"), \\s*"         // $1 text
        "(\\w+), \\s*"               // $2 id
        "(\\d+), \\s*"               // $3 x
        "(\\d+), \\s*"               // $4 y
        "(\\d+), \\s*"               // $5 width
        "(\\d+)  \\s*"               // $6 height
        "(.*?\\z)?"                  // $7 [, style [, extended-style]]
        "";

    boost::smatch m;
    const boost::regex& pushbutton_control_regex = RegexFactory::instance().create_regex( pushbutton_control_regex_str );

    if ( boost::regex_search( m_string, m, pushbutton_control_regex ) )
    {
        PUSHBUTTONcontrol* pushbutton_control = new PUSHBUTTONcontrol;

        pushbutton_control->m_text = m.str(1);
        pushbutton_control->m_id = m.str(2);
        pushbutton_control->m_x = m.str(3) ;
        pushbutton_control->m_y = m.str(4) ;
        pushbutton_control->m_width = m.str(5) ;
        pushbutton_control->m_height = m.str(6) ;
        pushbutton_control->m_style = m.str(7);

        return IControlPtr( pushbutton_control );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IControlPtr();
}
