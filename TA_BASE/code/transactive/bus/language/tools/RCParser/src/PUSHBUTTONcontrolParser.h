#pragma once
#include "IControlParser.h"


class PUSHBUTTONcontrolParser : public IControlParser
{
public:

    PUSHBUTTONcontrolParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IControlPtr parse();

    static IControlParserPtr create( const std::string& s )
    {
        return IControlParserPtr( new PUSHBUTTONcontrolParser(s) );
    }

private:

    std::string m_string;
};
