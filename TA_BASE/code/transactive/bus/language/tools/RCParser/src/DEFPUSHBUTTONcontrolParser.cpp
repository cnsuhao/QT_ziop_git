#include "StdAfx.h"
#include "DEFPUSHBUTTONcontrolParser.h"
#include "Controls.h"
#include "RegexFactory.h"


IControlPtr DEFPUSHBUTTONcontrolParser::parse()
{
    // DEFPUSHBUTTON control
    // DEFPUSHBUTTON text, id, x, y, width, height [, style [, extended-style]]
    const char* defpushbutton_control_regex_str =
        "(?x)"
        "DEFPUSHBUTTON \\s+"
        "(\"[^\"]*\"), \\s*"         // $1 text
        "(\\w+), \\s*"               // $2 id
        "(\\d+), \\s*"               // $3 x
        "(\\d+), \\s*"               // $4 y
        "(\\d+), \\s*"               // $5 width
        "(\\d+)  \\s*"               // $6 height
        "(.*? \\z)?"                 // $7 [, style [, extended-style]]
        "";

    boost::smatch m;
    const boost::regex& defpushbutton_control_regex = RegexFactory::instance().create_regex( defpushbutton_control_regex_str );

    if ( boost::regex_search( m_string, m, defpushbutton_control_regex ) )
    {
        DEFPUSHBUTTONcontrol* defpushbutton_control = new DEFPUSHBUTTONcontrol;

        defpushbutton_control->m_text = m.str(1);
        defpushbutton_control->m_id = m.str(2);
        defpushbutton_control->m_x = m.str(3) ;
        defpushbutton_control->m_y = m.str(4) ;
        defpushbutton_control->m_width = m.str(5) ;
        defpushbutton_control->m_height = m.str(6) ;
        defpushbutton_control->m_style = m.str(7);

        return IControlPtr( defpushbutton_control );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IControlPtr();
}
