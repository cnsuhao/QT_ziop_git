#include "StdAfx.h"
#include "StatementParserFactory.h"
#include "STYLEstatementParser.h"
#include "EXSTYLEstatementParser.h"
#include "CAPTIONstatementParser.h"
#include "FONTstatementParser.h"
#include "MENUstatementParser.h"
#include "MENUITEMstatementParser.h"
#include "BUTTONstatementParser.h"
#include "RegexFactory.h"


StatementParserFactory::StatementParserFactory( const std::string& s )
    : m_string( s )
{
    m_create_map[ "STYLE" ]     = STYLEstatementParser::create;
    m_create_map[ "EXSTYLE" ]   = EXSTYLEstatementParser::create;
    m_create_map[ "CAPTION" ]   = &CAPTIONstatementParser::create;
    m_create_map[ "MENU" ]      = MENUstatementParser::create;
    m_create_map[ "FONT" ]      = FONTstatementParser::create;
    m_create_map[ "MENUITEM" ]  = MENUITEMstatementParser::create;
    m_create_map[ "BUTTON" ]    = BUTTONstatementParser::create;
}


IStatementParserPtrList StatementParserFactory::create_statement_parsers()
{
    IStatementParserPtrList statement_parsers;

    #define statement_key_words "STYLE|EXSTYLE|CAPTION|MENU|FONT|MENUITEM|BUTTON"

    const char* statement_regex_str =
        "(?x)"
        "^ \\s* (" statement_key_words ") \\s+"
        "";

    const boost::regex& style_statement_regex = RegexFactory::instance().create_regex( statement_regex_str );
    boost::sregex_iterator it( m_string.begin(), m_string.end(), style_statement_regex );
    boost::sregex_iterator end;
    std::vector< std::pair<std::string, size_t> > statement_info_list; // type, position

    for ( ; it != end; ++it )
    {
        statement_info_list.push_back( std::make_pair( it->str(1), it->position() ) );
    }

    statement_info_list.push_back( std::make_pair( "", m_string.size() ) );

    for ( size_t i = 0; i < statement_info_list.size() - 1; ++i )
    {
        const std::string& statement_type = statement_info_list[i].first;
        const std::string& statement_string = m_string.substr( statement_info_list[i].second, statement_info_list[i + 1].second - statement_info_list[i].second );

        IStatementParserPtr parser = create_statement_parser( statement_type, statement_string );

        if ( parser )
        {
            statement_parsers.push_back( parser );
        }
    }

    return statement_parsers;
}


IStatementParserPtr StatementParserFactory::create_statement_parser( const std::string& statement_type, const std::string& statement_string )
{
    return m_create_map[ statement_type ]( statement_string );
}


IStatementPtrList StatementParserFactory::parse()
{
    IStatementPtrList statements;
    IStatementParserPtrList statement_parsers = create_statement_parsers();

    for ( size_t i = 0; i < statement_parsers.size(); ++i )
    {
        IStatementPtr statement = statement_parsers[i]->parse();

        if ( statement )
        {
            statements.push_back( statement );
        }
    }

    return statements;
}
