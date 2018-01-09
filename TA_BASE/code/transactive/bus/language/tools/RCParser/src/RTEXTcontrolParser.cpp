#include "StdAfx.h"
#include "RTEXTcontrolParser.h"
#include "Controls.h"
#include "RegexFactory.h"


IControlPtr RTEXTcontrolParser::parse()
{
    // RTEXT control
    // RTEXT text, id, x, y [, width, height, style [, extended-style]]

    const char* rtext_control_regex_str =
        "(?x)"
        "RTEXT \\s+"
        "(\"[^\"]*\"), \\s*"         // $1 text
        "(\\w+), \\s*"               // $2 id
        "(\\d+), \\s*"               // $3 x
        "(\\d+), \\s*"               // $4 y
        "(\\d+), \\s*"               // $5 width
        "(\\d+)  \\s*"               // $6 height
        "(.*?\\z)?"                  // $7 [, style [, extended-style]]
        "";

    boost::smatch m;
    const boost::regex& rtext_control_regex = RegexFactory::instance().create_regex( rtext_control_regex_str );

    if ( boost::regex_search( m_string, m, rtext_control_regex ) )
    {
        RTEXTcontrol* rtext_control = new RTEXTcontrol;

        rtext_control->m_text = m.str(1);
        rtext_control->m_id = m.str(2);
        rtext_control->m_x = m.str(3) ;
        rtext_control->m_y = m.str(4) ;
        rtext_control->m_width = m.str(5) ;
        rtext_control->m_height = m.str(6) ;
        rtext_control->m_style = m.str(7);

        return IControlPtr( rtext_control );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IControlPtr();
}
