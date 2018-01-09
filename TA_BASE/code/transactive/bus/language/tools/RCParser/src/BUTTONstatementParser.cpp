#include "StdAfx.h"
#include "BUTTONstatementParser.h"
#include "Statements.h"
#include "RegexFactory.h"


IStatementPtr BUTTONstatementParser::parse()
{
    // BUTTON statement
    // BUTTON id
    const char* menu_statement_regex_str =
        "(?x)"
        "\\A \\s* BUTTON \\s+"
        "(\\w+)"          // $1 id
        "";

    boost::smatch m;
    const boost::regex& menu_statement_regex = RegexFactory::instance().create_regex( menu_statement_regex_str );

    if ( boost::regex_search( m_string, m, menu_statement_regex ) )
    {
        BUTTONstatement* menu_statement = new BUTTONstatement;

        menu_statement->m_id = m.str(1);;

        return IStatementPtr( menu_statement );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IStatementPtr();
}
