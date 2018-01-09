#include "StdAfx.h"
#include "LISTBOXcontrolParser.h"
#include "Controls.h"
#include "RegexFactory.h"


IControlPtr LISTBOXcontrolParser::parse()
{
    // LISTBOX control
    // LISTBOX id, x, y, width, height [, style [, extended-style]]

    const char* listbox_control_regex_str =
        "(?x)"
        "LISTBOX \\s+"
        "(\\w+), \\s*"               // $1 id
        "(\\d+), \\s*"               // $2 x
        "(\\d+), \\s*"               // $3 y
        "(\\d+), \\s*"               // $4 width
        "(\\d+)  \\s*"               // $5 height
        "(.*?\\z)?"                  // $6 [, style [, extended-style]]
        "";

    boost::smatch m;
    const boost::regex& listbox_control_regex = RegexFactory::instance().create_regex( listbox_control_regex_str );

    if ( boost::regex_search( m_string, m, listbox_control_regex ) )
    {
        LISTBOXcontrol* listbox_control = new LISTBOXcontrol;

        listbox_control->m_id = m.str(1);
        listbox_control->m_x = m.str(2) ;
        listbox_control->m_y = m.str(3) ;
        listbox_control->m_width = m.str(4) ;
        listbox_control->m_height = m.str(5) ;
        listbox_control->m_style = m.str(6);

        return IControlPtr( listbox_control );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IControlPtr();
}
