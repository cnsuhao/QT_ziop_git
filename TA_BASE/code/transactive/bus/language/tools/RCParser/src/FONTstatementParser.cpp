#include "StdAfx.h"
#include "FONTstatementParser.h"
#include "Statements.h"
#include "RegexFactory.h"


IStatementPtr FONTstatementParser::parse()
{
    // FONT statement
    // FONT pointsize, "typeface", weight, italic, charset
    const char* font_statement_regex_str =
        "(?x)"
        "\\A \\s* FONT \\s+ "
        "(\\d+), \\s+ "             // $1 pointsize
        "(\"[^\"]+\") "             // $2 typeface
        "(, \\s* (\\d+), \\s*)? "   // $4 weight
        "((\\d+), \\s*)? "          // $6 italic
        "([0-9xX]+)? "              // $8 charset
        "";

    boost::smatch m;
    const boost::regex& font_statement_regex = RegexFactory::instance().create_regex( font_statement_regex_str );

    if ( boost::regex_search( m_string, m, font_statement_regex ) )
    {
        FONTstatement* font_statement = new FONTstatement;

        font_statement->m_pointsize = m.str(1) ;
        font_statement->m_typeface = m.str(2);

        const std::string& weight = m.str(4);
        const std::string& italic = m.str(6);
        const std::string& charset = m.str(8);

        if ( false == weight.empty() )
        {
            font_statement->m_weight = weight ;
            font_statement->m_italic = boost::lexical_cast<bool>( italic );
            // font_statement->m_charset = charset ; // TODO: hex
        }

        return IStatementPtr( font_statement );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IStatementPtr();
}
