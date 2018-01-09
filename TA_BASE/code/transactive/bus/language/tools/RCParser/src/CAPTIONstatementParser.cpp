#include "StdAfx.h"
#include "CAPTIONstatementParser.h"
#include "Statements.h"
#include "RegexFactory.h"


IStatementPtr CAPTIONstatementParser::parse()
{
    // CAPTION statement
    // CAPTION "captiontext"
    const char* caption_statement_regex_str =
        "(?x)"
        "\\A \\s* CAPTION \\s+"
        "(\"[^\"]*\")"          // $1 captiontext
        "";

    boost::smatch m;
    const boost::regex& caption_statement_regex = RegexFactory::instance().create_regex( caption_statement_regex_str );

    if ( boost::regex_search( m_string, m, caption_statement_regex ) )
    {
        CAPTIONstatement* caption_statement = new CAPTIONstatement;

        const std::string& captiontext = m.str(1);
        caption_statement->m_captiontext = captiontext;

        return IStatementPtr( caption_statement );
    }

    std::cout << "parse failed:\n" << m_string << std::endl;
    return IStatementPtr();
}
