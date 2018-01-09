#pragma once
#include "IStatementParser.h"


class FONTstatementParser : public IStatementParser
{
public:

    FONTstatementParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IStatementPtr parse();

    static IStatementParserPtr create( const std::string& s)
    {
        return IStatementParserPtr( new FONTstatementParser( s ) );
    }

private:

    std::string m_string;
};
