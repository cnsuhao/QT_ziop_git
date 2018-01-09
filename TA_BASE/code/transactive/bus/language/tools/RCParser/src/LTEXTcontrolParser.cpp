#include "StdAfx.h"
#include "LTEXTcontrolParser.h"
#include "Controls.h"
#include "RegexFactory.h"


IControlPtr LTEXTcontrolParser::parse()
{
    // LTEXT control
    // LTEXT text, id, x, y [, width, height, style [, extended-style]]

    const char* ltext_control_regex_str =
        "(?x)"
        "LTEXT \\s+"
        "(\"[^\"]*\"), \\s*"         // $1 text
        "(\\w+), \\s*"               // $2 id
        "-?(\\d+), \\s*"               // $3 x
        "-?(\\d+), \\s*"               // $4 y
        "-?(\\d+), \\s*"               // $5 width
        "-?(\\d+)  \\s*"               // $6 height
        "(.*?\\z)?"                  // $7 [, style [, extended-style]]
        "";

    boost::smatch m;
    const boost::regex& ltext_control_regex = RegexFactory::instance().create_regex( ltext_control_regex_str );

    if ( boost::regex_search( m_string, m, ltext_control_regex ) )
    {
        LTEXTcontrol* ltext_control = new LTEXTcontrol;

        ltext_control->m_text = m.str(1);
        ltext_control->m_id = m.str(2);
        ltext_control->m_x = m.str(3) ;
        ltext_control->m_y = m.str(4) ;
        ltext_control->m_width = m.str(5) ;
        ltext_control->m_height = m.str(6) ;
        ltext_control->m_style = m.str(7);

        return IControlPtr( ltext_control );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IControlPtr();
}
