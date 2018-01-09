#include "StdAfx.h"
#include "EDITTEXTcontrolParser.h"
#include "Controls.h"
#include "RegexFactory.h"


IControlPtr EDITTEXTcontrolParser::parse()
{
    // EDITTEXT control
    // EDITTEXT id, x, y, width, height [, style [, extended-style]]

    const char* edittext_control_regex_str =
        "(?x)"
        "EDITTEXT \\s+"
        "(\\w+), \\s*"               // $1 id
        "(\\d+), \\s*"               // $2 x
        "(\\d+), \\s*"               // $3 y
        "(\\d+), \\s*"               // $4 width
        "(\\d+)  \\s*"               // $5 height
        "(.*?\\z)?"                  // $6 [, style [, extended-style]]
        "";

    boost::smatch m;
    const boost::regex& edittext_control_regex = RegexFactory::instance().create_regex( edittext_control_regex_str );

    if ( boost::regex_search( m_string, m, edittext_control_regex ) )
    {
        EDITTEXTcontrol* edittext_control = new EDITTEXTcontrol;

        edittext_control->m_id = m.str(1);
        edittext_control->m_x = m.str(2) ;
        edittext_control->m_y = m.str(3) ;
        edittext_control->m_width = m.str(4) ;
        edittext_control->m_height = m.str(5) ;
        edittext_control->m_style = m.str(6);

        return IControlPtr( edittext_control );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IControlPtr();
}
