#pragma once
#include "IStatementParser.h"


// TODO: template
class StatementParserFactory
{
public:

    StatementParserFactory( const std::string& s );

    IStatementPtrList parse();

private:

    IStatementParserPtrList create_statement_parsers();
    IStatementParserPtr create_statement_parser( const std::string& statement_type, const std::string& statement_string );

private:

    std::string m_string;

    typedef IStatementParserPtr (* statement_parser_create_fun) ( const std::string& s );
    std::map<std::string, statement_parser_create_fun> m_create_map;
};
