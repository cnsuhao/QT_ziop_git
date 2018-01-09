#pragma once
#include "IStatementParser.h"


class EXSTYLEstatementParser : public IStatementParser
{
public:

    EXSTYLEstatementParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IStatementPtr parse();

    static IStatementParserPtr create( const std::string& s)
    {
        return IStatementParserPtr( new EXSTYLEstatementParser( s ) );
    }

private:

    std::string m_string;
};
