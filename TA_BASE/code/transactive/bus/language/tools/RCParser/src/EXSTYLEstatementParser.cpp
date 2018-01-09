#include "StdAfx.h"
#include "EXSTYLEstatementParser.h"
#include "Statements.h"
#include "RegexFactory.h"


IStatementPtr EXSTYLEstatementParser::parse()
{
    // EXSTYLE statement
    // EXSTYLE extended-style
    const char* exstyle_statement_regex_str =
        "(?x)"
        "\\A \\s* EXSTYLE \\s+"
        "((\\w+) ( \\s* \\| \\s* \\w+)*) "          // $1 exstyle
        "";

    boost::smatch m;
    const boost::regex& exstyle_statement_regex = RegexFactory::instance().create_regex( exstyle_statement_regex_str );

    if ( boost::regex_search( m_string, m, exstyle_statement_regex ) )
    {
        EXSTYLEstatement* exstyle_statement = new EXSTYLEstatement;

        exstyle_statement->m_extended_style = m.str(1);;

        return IStatementPtr( exstyle_statement );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IStatementPtr();
}
