#pragma once
#include "IStatementParser.h"


class MENUstatementParser : public IStatementParser
{
public:

    MENUstatementParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IStatementPtr parse();

    static IStatementParserPtr create( const std::string& s)
    {
        return IStatementParserPtr( new MENUstatementParser( s ) );
    }

private:

    std::string m_string;
};
