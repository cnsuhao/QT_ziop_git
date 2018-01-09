#include "StdAfx.h"
#include "MENUITEMstatementParser.h"
#include "Statements.h"
#include "RegexFactory.h"


IStatementPtr MENUITEMstatementParser::parse()
{
    // MENUITEM text, result, [optionlist]
    // MENUITEM SEPARATOR
    #define option_list "(?:,\\s*(?:CHECKED|GRAYED|HELP|INACTIVE|MENUBARBREAK|MENUBREAK))*"
    const char* menuitem_statement_regex_str =
        "(?x)"
        "\\A \\s* MENUITEM \\s+ (\"[^\"]*\"), \\s* (\\w+) \\s* " option_list // $1 text, $2 result
        " | "
        "    \\s* MENUITEM \\s+ (SEPARATOR) \\s* "                          // $3 SEPARATOR
        "";

    boost::smatch m;
    const boost::regex& menuitem_statement_regex = RegexFactory::instance().create_regex( menuitem_statement_regex_str );

    if ( boost::regex_search( m_string, m, menuitem_statement_regex ) )
    {
        MENUITEMstatement* menuitem_statement = new MENUITEMstatement;

        menuitem_statement->m_text          = m.str(1);
        menuitem_statement->m_result        = m.str(2);
        menuitem_statement->m_is_separator  = ( false == m.str(3).empty() );

        return IStatementPtr( menuitem_statement );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IStatementPtr();
}
