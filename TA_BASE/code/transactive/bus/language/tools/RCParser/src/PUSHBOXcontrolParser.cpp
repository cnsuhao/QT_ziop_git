#include "StdAfx.h"
#include "PUSHBOXcontrolParser.h"
#include "Controls.h"
#include "RegexFactory.h"


IControlPtr PUSHBOXcontrolParser::parse()
{
    // PUSHBOX control
    // PUSHBOX text, id, x, y [, width, height, style [, extended-style]]

    const char* pushbox_control_regex_str =
        "(?x)"
        "PUSHBOX \\s+"
        "(\"[^\"]*\"), \\s*"         // $1 text
        "(\\w+), \\s*"               // $2 id
        "(\\d+), \\s*"               // $3 x
        "(\\d+), \\s*"               // $4 y
        "(\\d+), \\s*"               // $5 width
        "(\\d+)  \\s*"               // $6 height
        "(.*?\\z)?"                  // $7 [, style [, extended-style]]
        "";

    boost::smatch m;
    const boost::regex& pushbox_control_regex = RegexFactory::instance().create_regex( pushbox_control_regex_str );

    if ( boost::regex_search( m_string, m, pushbox_control_regex ) )
    {
        PUSHBOXcontrol* pushbox_control = new PUSHBOXcontrol;

        pushbox_control->m_text = m.str(1);
        pushbox_control->m_id = m.str(2);
        pushbox_control->m_x = m.str(3) ;
        pushbox_control->m_y = m.str(4) ;
        pushbox_control->m_width = m.str(5) ;
        pushbox_control->m_height = m.str(6) ;
        pushbox_control->m_style = m.str(7);

        return IControlPtr( pushbox_control );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IControlPtr();
}
