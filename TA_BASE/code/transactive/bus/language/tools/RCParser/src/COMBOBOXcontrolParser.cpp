#include "StdAfx.h"
#include "COMBOBOXcontrolParser.h"
#include "Controls.h"
#include "RegexFactory.h"


IControlPtr COMBOBOXcontrolParser::parse()
{
    // COMBOBOX control
    // COMBOBOX id, x, y, width, height [, style [, extended-style]]

    const char* combobox_control_regex_str =
        "(?x)"
        "COMBOBOX \\s+"
        "(\\w+), \\s*"               // $1 id
        "(\\d+), \\s*"               // $2 x
        "(\\d+), \\s*"               // $3 y
        "(\\d+), \\s*"               // $4 width
        "(\\d+)  \\s*"               // $5 height
        "(.*?\\z)?"                  // $6 [, style [, extended-style]]
        "";

    boost::smatch m;
    const boost::regex& combobox_control_regex = RegexFactory::instance().create_regex( combobox_control_regex_str );

    if ( boost::regex_search( m_string, m, combobox_control_regex ) )
    {
        COMBOBOXcontrol* combobox_control = new COMBOBOXcontrol;

        combobox_control->m_id = m.str(1);
        combobox_control->m_x = m.str(2);
        combobox_control->m_y = m.str(3);
        combobox_control->m_width = m.str(4);
        combobox_control->m_height = m.str(5);
        // TODO: [, style [, extended-style]]

        return IControlPtr( combobox_control );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IControlPtr();
}
