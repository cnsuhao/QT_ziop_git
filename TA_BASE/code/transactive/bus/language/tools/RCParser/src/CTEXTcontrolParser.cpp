#include "StdAfx.h"
#include "CTEXTcontrolParser.h"
#include "Controls.h"
#include "RegexFactory.h"


IControlPtr CTEXTcontrolParser::parse()
{
    // CTEXT control
    // CTEXT text, id, x, y [, width, height, style [, extended-style]]

    const char* ctext_control_regex_str =
        "(?x)"
        "CTEXT \\s+"
        "(\"[^\"]*\"), \\s*"         // $1 text
        "(\\w+), \\s*"               // $2 id
        "(\\d+), \\s*"               // $3 x
        "(\\d+), \\s*"               // $4 y
        "(\\d+), \\s*"               // $5 width
        "(\\d+)  \\s*"               // $6 height
        "(.*?\\z)?"                  // $7 [, style [, extended-style]]
        "";

    boost::smatch m;
    const boost::regex& ctext_control_regex = RegexFactory::instance().create_regex( ctext_control_regex_str );

    if ( boost::regex_search( m_string, m, ctext_control_regex ) )
    {
        CTEXTcontrol* ctext_control = new CTEXTcontrol;

        ctext_control->m_text = m.str(1);
        ctext_control->m_id = m.str(2);
        ctext_control->m_x = m.str(3);
        ctext_control->m_y = m.str(4);
        ctext_control->m_width = m.str(5) ;
        ctext_control->m_height = m.str(6) ;
        ctext_control->m_style = m.str(7);

        return IControlPtr( ctext_control );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IControlPtr();
}
