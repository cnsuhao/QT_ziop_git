#pragma once
#include "IControlParser.h"


class PUSHBOXcontrolParser : public IControlParser
{
public:

    PUSHBOXcontrolParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IControlPtr parse();

    static IControlParserPtr create( const std::string& s )
    {
        return IControlParserPtr( new PUSHBOXcontrolParser(s) );
    }

private:

    std::string m_string;
};
