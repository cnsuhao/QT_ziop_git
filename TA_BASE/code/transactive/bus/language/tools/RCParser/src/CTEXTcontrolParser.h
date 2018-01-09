#pragma once
#include "IControlParser.h"


class CTEXTcontrolParser : public IControlParser
{
public:

    CTEXTcontrolParser( const std::string& s )
        : m_string( s )
    {
    }

    virtual IControlPtr parse();

    static IControlParserPtr create( const std::string& s )
    {
        return IControlParserPtr( new CTEXTcontrolParser(s) );
    }

private:

    std::string m_string;
};
