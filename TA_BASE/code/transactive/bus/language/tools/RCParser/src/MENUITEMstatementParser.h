#pragma once
#include "IStatementParser.h"


class MENUITEMstatementParser : public IStatementParser
{
public:

    MENUITEMstatementParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IStatementPtr parse();

    static IStatementParserPtr create( const std::string& s)
    {
        return IStatementParserPtr( new MENUITEMstatementParser( s ) );
    }

private:

    std::string m_string;
};
