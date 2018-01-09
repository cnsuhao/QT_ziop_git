#include "StdAfx.h"
#include "RADIOBUTTONcontrolParser.h"
#include "Controls.h"
#include "RegexFactory.h"


IControlPtr RADIOBUTTONcontrolParser::parse()
{
    // RADIOBUTTON control
    // RADIOBUTTON text, id, x, y [, width, height, style [, extended-style]]

    const char* radiobutton_control_regex_str =
        "(?x)"
        "RADIOBUTTON \\s+"
        "(\"[^\"]*\"), \\s*"         // $1 text
        "(\\w+), \\s*"               // $2 id
        "(\\d+), \\s*"               // $3 x
        "(\\d+), \\s*"               // $4 y
        "(\\d+), \\s*"               // $5 width
        "(\\d+)  \\s*"               // $6 height
        "(.*?\\z)?"                  // $7 [, style [, extended-style]]
        "";

    boost::smatch m;
    const boost::regex& radiobutton_control_regex = RegexFactory::instance().create_regex( radiobutton_control_regex_str );

    if ( boost::regex_search( m_string, m, radiobutton_control_regex ) )
    {
        RADIOBUTTONcontrol* radiobutton_control = new RADIOBUTTONcontrol;

        radiobutton_control->m_text = m.str(1);
        radiobutton_control->m_id = m.str(2);
        radiobutton_control->m_x = m.str(3) ;
        radiobutton_control->m_y = m.str(4) ;
        radiobutton_control->m_width = m.str(5) ;
        radiobutton_control->m_height = m.str(6) ;
        radiobutton_control->m_style = m.str(7);

        return IControlPtr( radiobutton_control );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IControlPtr();
}
