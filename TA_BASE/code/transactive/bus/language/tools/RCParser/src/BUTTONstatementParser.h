#pragma once
#include "IStatementParser.h"


class BUTTONstatementParser : public IStatementParser
{
public:

    BUTTONstatementParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IStatementPtr parse();

    static IStatementParserPtr create( const std::string& s)
    {
        return IStatementParserPtr( new BUTTONstatementParser( s ) );
    }

private:

    std::string m_string;
};
