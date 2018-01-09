#include "StdAfx.h"
#include "MENUstatementParser.h"
#include "Statements.h"
#include "RegexFactory.h"


IStatementPtr MENUstatementParser::parse()
{
    // MENU statement
    // MENU menuname
    const char* menu_statement_regex_str =
        "(?x)"
        "\\A \\s* MENU \\s+"
        "(\\w+)"          // $1 menuname
        "";

    boost::smatch m;
    const boost::regex& menu_statement_regex = RegexFactory::instance().create_regex( menu_statement_regex_str );

    if ( boost::regex_search( m_string, m, menu_statement_regex ) )
    {
        MENUstatement* menu_statement = new MENUstatement;

        menu_statement->m_menuname = m.str(1);;

        return IStatementPtr( menu_statement );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IStatementPtr();
}
