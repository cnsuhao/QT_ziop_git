#pragma once
#include "IStatementParser.h"


class CAPTIONstatementParser : public IStatementParser
{
public:

    CAPTIONstatementParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IStatementPtr parse();

    static IStatementParserPtr create( const std::string& s)
    {
        return IStatementParserPtr( new CAPTIONstatementParser( s ) );
    }

private:

    std::string m_string;
};
