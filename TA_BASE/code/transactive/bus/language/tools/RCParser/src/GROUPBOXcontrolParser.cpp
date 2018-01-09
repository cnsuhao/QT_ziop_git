#include "StdAfx.h"
#include "GROUPBOXcontrolParser.h"
#include "Controls.h"
#include "RegexFactory.h"


IControlPtr GROUPBOXcontrolParser::parse()
{
    // GROUPBOX control
    // GROUPBOX text, id, x, y, width, height [, style [, extended-style]]

    const char* groupbox_control_regex_str =
        "(?x)"
        "GROUPBOX \\s+"
        "(\"[^\"]*\"), \\s*"         // $1 text
        "(\\w+), \\s*"               // $2 id
        "(\\d+), \\s*"               // $3 x
        "(\\d+), \\s*"               // $4 y
        "(\\d+), \\s*"               // $5 width
        "(\\d+)  \\s*"               // $6 height
        "(.*?\\z)?"                  // $7 [, style [, extended-style]]
        "";

    boost::smatch m;
    const boost::regex& groupbox_control_regex = RegexFactory::instance().create_regex( groupbox_control_regex_str );

    if ( boost::regex_search( m_string, m, groupbox_control_regex ) )
    {
        GROUPBOXcontrol* groupbox_control = new GROUPBOXcontrol;

        groupbox_control->m_text = m.str(1);
        groupbox_control->m_id = m.str(2);
        groupbox_control->m_x = m.str(3) ;
        groupbox_control->m_y = m.str(4) ;
        groupbox_control->m_width = m.str(5) ;
        groupbox_control->m_height = m.str(6) ;
        groupbox_control->m_style = m.str(7);

        return IControlPtr( groupbox_control );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IControlPtr();
}
