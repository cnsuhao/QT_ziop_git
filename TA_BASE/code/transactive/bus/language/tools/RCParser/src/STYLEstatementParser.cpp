#include "StdAfx.h"
#include "STYLEstatementParser.h"
#include "Statements.h"
#include "RegexFactory.h"


IStatementPtr STYLEstatementParser::parse()
{
    // STYLE statement
    // STYLE style
    const char* style_statement_regex_str =
        "(?x)"
        "\\A \\s* STYLE \\s+"
        "((\\w+) ( \\s* \\| \\s* \\w+)*) "          // $1 style
        "";

    boost::smatch m;
    const boost::regex& style_statement_regex = RegexFactory::instance().create_regex( style_statement_regex_str );

    if ( boost::regex_search( m_string, m, style_statement_regex ) )
    {
        STYLEstatement* style_statement = new STYLEstatement;

        style_statement->m_style = m.str(1);;

        return IStatementPtr( style_statement );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IStatementPtr();
}
