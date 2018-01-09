#pragma once
#include "IStatementParser.h"


class STYLEstatementParser : public IStatementParser
{
public:

    STYLEstatementParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IStatementPtr parse();

    static IStatementParserPtr create( const std::string& s)
    {
        return IStatementParserPtr( new STYLEstatementParser( s ) );
    }

private:

    std::string m_string;
};
