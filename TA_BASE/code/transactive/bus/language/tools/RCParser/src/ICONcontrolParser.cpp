#include "StdAfx.h"
#include "ICONcontrolParser.h"
#include "Controls.h"
#include "RegexFactory.h"


IControlPtr ICONcontrolParser::parse()
{
    // ICON control
    // ICON text, id, x, y [, width, height, style [, extended-style]]

    const char* icon_control_regex_str =
        "(?x)"
        "ICON \\s+"
        "(\"[^\"]*\"), \\s*"         // $1 text
        "(\\w+), \\s*"               // $2 id
        "(\\d+), \\s*"               // $3 x
        "(\\d+), \\s*"               // $4 y
        "(\\d+), \\s*"               // $5 width
        "(\\d+)  \\s*"               // $6 height
        "(.*?\\z)?"                  // $7 [, style [, extended-style]]
        "";

    boost::smatch m;
    const boost::regex& icon_control_regex = RegexFactory::instance().create_regex( icon_control_regex_str );

    if ( boost::regex_search( m_string, m, icon_control_regex ) )
    {
        ICONcontrol* icon_control = new ICONcontrol;

        icon_control->m_text = m.str(1);
        icon_control->m_id = m.str(2);
        icon_control->m_x = m.str(3) ;
        icon_control->m_y = m.str(4) ;
        icon_control->m_width = m.str(5) ;
        icon_control->m_height = m.str(6) ;
        icon_control->m_style = m.str(7);

        return IControlPtr( icon_control );
    }

    //std::cout << "parse failed:\n" << m_string << std::endl;
    return IControlPtr();
}
