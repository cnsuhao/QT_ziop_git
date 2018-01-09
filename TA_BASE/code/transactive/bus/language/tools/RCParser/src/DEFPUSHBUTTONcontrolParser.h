#pragma once
#include "IControlParser.h"


class DEFPUSHBUTTONcontrolParser : public IControlParser
{
public:

    DEFPUSHBUTTONcontrolParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IControlPtr parse();

    static IControlParserPtr create( const std::string& s )
    {
        return IControlParserPtr( new DEFPUSHBUTTONcontrolParser(s) );
    }

private:

    std::string m_string;
};
